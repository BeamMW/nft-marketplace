import {computed, reactive} from 'vue'
import utils from 'utils/utils.js'
import {cid} from 'stores/consts'
import {common} from 'utils/consts'
import formats from 'stores/formats'
import router from 'router'

export default class ItemsStore {
  constructor(objname, versions, perPage) {
    this._objname = objname
    this._store_name = `${objname}s`
    this._metastore_name = `${objname}s_meta`
    this._versions = versions
    this._per_page = perPage || common.ITEMS_PER_PAGE
    this._modes = ['user', 'moderator', 'artist', 'owner']
    this._my_key = ''
    this._loading = false
  }

  reset (global) {
    this._global = global
    this._state = reactive({
      artist_key: undefined
    })

    let makeMode = (mode) => {
      return {
        all_items: [],
        page_items: [],
        total: 0,
        page: 1,
        pages: computed(() => {
          let total = this._state[mode].total
          return total ? Math.ceil(total / this._per_page) : 1
        })
      }
    }

    for (let mode of this._modes) {
      this._state[mode] = makeMode(mode)
    }

    this._state['artist'].loader = () => {
      let my_key = this._my_key
      return this._db[this._store_name]
        .where('author')
        .equals(my_key)
    }

    this._state['owner'].loader = () => {
      return this._db[this._store_name]
        .where('owned')
        .equals(1)
    }
  }

  getItem(id) {
    for (let mode of this._modes) {
      for (let item of this._state[mode].page_items) {
        if (item.id == id) {
          return item
        }
      }
    }
    throw new Error(`ItemsStore::getItem - item ${this._objname}-${id} not found`)
  }

  get artist_items() {
    return this._state.artist.all_items
  }

  getPage(mode) {
    return this._state[mode].page
  }

  setPage(mode, page) {
    this._state[mode].page = page;
    (async () => {
      try
      {
        await this._loadPageItemsAsync(mode)
      }
      catch(err) {
        this._global.setError(err)
      }
    })()
  }

  getPages(mode) {
    return this._state[mode].pages
  }

  getAllItems(mode) {
    return this._state[mode].all_items
  }

  getPageItems(mode) {
    return this._state[mode].page_items
  }

  getTotal(mode) {
    return this._state[mode].total
  }

  getDBStores() {
    let res = {} 
    res[this._store_name] = 'id, status, author, owned'
    res[this._metastore_name] = 'name'
    return res
  }

  async _loadPageItemsAsync(mode) {
    let loader = this._state[mode].loader
    if (loader) {
      let page = this._state[mode].page
      let promise = loader()
        .offset((page - 1) * this._per_page)
        .limit(this._per_page)
        .toArray(arr => arr.map(item => this._fromContract(item)))
      this._state[mode].page_items = await promise
    }
  }

  async _loadKeyAsync () {
    let {res} = await utils.invokeContractAsync({
      role: 'artist',
      action: 'get_id',
      cid
    })

    utils.ensureField(res, 'id', 'string')
    return res.id
  }

  async loadAsync(db) {
    if(this._loading) {
      return
    }

    if (!this._my_key) {
      this._my_key = await this._loadKeyAsync()
    }

    this._db = db
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: `view_${this._objname}s`,
      cid
    })

    utils.ensureField(res, 'items', 'array')

    let approved = 0
    let pending = 0
    let rejected = 0
    let artist = 0
    let owned = 0
    let liked = 0

    for(let item of res.items) {
      try {
        [item.label] = formats.fromContract(item.label)
        if (!item.label) {
          throw new Error('label cannot be empty')
        }

        if (!item.data) {
          throw new Error('empty data on item') 
        }

        [item.version, item.data] = formats.fromContract(item.data)
        if (!this._versions.includes(item.version)) {
          throw new Error(`item version mismatch:  ${item.version}`) 
        }

        item = this._fromContract(item)
        item = Object.assign({}, item)
        console.log('item loaded with label', item.label)
      }
      catch(err) {
        console.log(`ItemsStore._loadItems for ${this._objname}-${item.id}`, err)
        item.error = err
        item.label = `Failed to load ${this._objname}`
      }

      if (item.status === 'approved') {
        approved++
      }

      if (item.status === 'pending') {
        pending++
      }

      if (item.status === 'rejected') {
        rejected++
      }

      if (item.author == this._my_key) {
        artist++
      }

      if (item.owned) {
        owned++
      }

      if (item.my_impression) {
        liked++
      }
    }

    await db.transaction('rw!', db[this._store_name], db[this._metastore_name], async () => {
      await db[this._metastore_name].put({name: 'approved', value: approved})
      await db[this._metastore_name].put({name: 'rejected', value: rejected})
      await db[this._metastore_name].put({name: 'pending', value: pending})
      await db[this._metastore_name].put({name: 'artist', value: artist})
      await db[this._metastore_name].put({name: 'owned', value: owned})
      await db[this._metastore_name].put({name: 'liked', value: liked})
      await db[this._store_name].bulkPut(res.items)
    })

    this._state['user'].total = approved
    this._state['artist'].total = artist
    this._state['owner'].total = owned
    this._state['moderator'].total = pending + rejected

    for (let mode of this._modes) {
      await this._loadPageItemsAsync(mode)
    }

    this._loading = false
  }

  toNewItem() {
    router.push({
      name: `new-${this._objname}`
    })
  }

  toEditItem(id) {
    router.push({
      name: `edit-${this._objname}`,
      params: {
        id
      }
    })
  }
}
