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
    this._loading = false
  }

  reset (global) {
    this._global = global
    this._state = reactive({
      artist_key: undefined
    })

    let makeMode = (mode) => {
      return {
        items: [],
        all_items: [],
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
  }

  getItem(id) {
    for (let mode of this._modes) {
      for (let item of this._state[mode].all_items) {
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
    this._state[mode].page = page
    //this._loadItems(mode)
  }

  getPages(mode) {
    return this._state[mode].pages
  }

  getAllItems(mode) {
    return this._state[mode].all_items
  }

  getPageItems(mode) {
    let page = this.getPage(mode)
    return this._state[mode].all_items.slice((page-1) * this._per_page, page * this._per_page)  
  }

  getTotal(mode) {
    return this._state[mode].total
  }

  getDBStores() {
    let res = {} 
    res[this._store_name] = 'id, status'
    res[this._metastore_name] = 'name'
    return res
  }

  async _loadKey () {
    let {res} = await utils.invokeContractAsync({
      role: 'artist',
      action: 'get_id',
      cid
    })

    utils.ensureField(res, 'id', 'string')
    return res.id
  }

  async loadAsync(db) {
    if(this._loading) return

    this._db = db
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: `view_${this._objname}s`,
      cid
    })

    utils.ensureField(res, 'items', 'array')

    let mykey = await this._loadKey()
    let approved = []
    let pending = []
    let rejected = []
    let author = []
    let owned = []
    let liked = []

    for(let item of res.items) {
      item = Object.assign({}, item)
      try {
        [item.label] = formats.fromContract(item.label)
        if (!item.label) {
          throw new Error('label cannot be empty')
        }

        if (item.data) {
          let [version, data] = formats.fromContract(item.data)
          Object.assign(item, data)
          item.version = version
        
          if (!this._versions.includes(version)) {
            throw new Error(`item version mismatch:  ${version}`) 
          }
        } 

        if (!item.data) {
          item.version = this.version
          item.data = {}
        }

        item = this._fromContract(item)
        console.log('item loaded with label', item.label)
      }
      catch(err) {
        console.log(`ItemsStore._loadItems for ${this._objname}-${item.id}`, err)
        item.error = err
        item.label = `Failed to load ${this._objname}`
      }

      if (item.status === 'approved') {
        approved.push(item)
      }

      if (item.status === 'pending') {
        pending.push(item)
      }

      if (item.status === 'rejected') {
        rejected.push(item)
      }

      if (item.author == mykey) {
        author.push(item)
      }

      // TODO: this is until we have a proper owned flag
      if (this._objname == 'collection') {
        if (item.author == mykey) {
          owned.push(item)
        }
      }

      if (item.owner) {
        owned.push(item)
      }

      if (item.my_impression) {
        liked.push(item)
      }
    }

    await db.transaction('rw!', db[this._store_name], db[this._metastore_name], async () => {
      await db[this._metastore_name].put({name: 'approved', value: approved.length})
      await db[this._metastore_name].put({name: 'rejected', value: rejected.length})
      await db[this._metastore_name].put({name: 'pending', value: pending.length})
      await db[this._metastore_name].put({name: 'author', value: author.length})
      await db[this._metastore_name].put({name: 'owned', value: owned.length})
      await db[this._metastore_name].put({name: 'liked', value: liked.length})
      await db[this._store_name].bulkPut(res.items)
    })

    this._state['user'].all_items = approved
    this._state['user'].total = approved.length
    this._state['artist'].all_items = author
    this._state['artist'].total = author.length
    this._state['owner'].all_items = owned
    this._state['owner'].total = owned.length
    this._state['moderator'].all_items = [...pending, ...rejected]
    this._state['moderator'].total = pending.length + rejected.length
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
