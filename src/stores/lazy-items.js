import utils from 'utils/utils'
import formats from 'stores/formats'
import router from 'router'
import {cid} from 'stores/consts'
import {common} from 'utils/consts'
import {computed, reactive} from 'vue'
import {liveQuery} from 'dexie'
import {Observable} from 'rxjs'

export default class ItemsStore {
  constructor({objname, versions, perPage, dbKeys}) {
    this._objname = objname
    this._extra_dbkeys = dbKeys
    this._store_name = `${objname}s`
    this._metastore_name = `${objname}s_meta`
    this._versions = versions
    this._per_page = perPage || common.ITEMS_PER_PAGE
    this._my_key = ''
    this._loading = false
  }

  reset (global, db, state) {
    this._global = global
    this._db = db
    this._state = reactive(state || {})

    // TODO: move nft-specific loaders to nfts store. For example all with likes
    this._allocMode('user', (store) => {
      console.log(`loader for user:${this._store_name}`)
      return store.where('status')
        .equals('approved')
    })

    this._allocMode('user:liked', (store) => {
      console.log(`loader for user:liked:${this._store_name}`)
      return store
        .where(['liked', 'status'])
        .equals([1, 'approved'])
    })

    this._allocMode('user:sale', (store) => {
      console.log(`loader for user:liked:${this._store_name}`)
      return store
        .where(['status', 'sale'])
        .equals(['approved', 1])
    })
    
    this._allocMode('moderator', (store) => {
      // TODO: sort by height in reverse order ?
      console.log(`loader for moderator:${this._store_name}`)
      return store.where('status')
        .equals('pending')
    })

    this._allocMode('artist', (store, mykey) => {
      console.log(`loader for artist:${this._store_name}`)
      return store
        .where({'author': mykey})
    })

    this._allocMode('owner', (store) => {
      console.log(`loader for owner:${this._store_name}`)
      return store
        .where({'owned': 1})
    })

    this._allocMode('owner:sale', (store) => {
      console.log(`loader for owner:sale:${this._store_name}`)
      return store
        .where(['owned', 'sale'])
        .equals([1, 1])
    })

    this._allocMode('artist:sold', (store, mykey) => {
      console.log(`loader for artist:sold:${this._store_name}`)
      return store
        .where(['author', 'owned'])
        .equals([mykey, 0])
    })

    this._allocMode('liker:liked', (store) => {
      console.log(`loader for liker:liked:${this._store_name}`)
      return store
        .where({'my_impression': 1})
    })
  }

  _allocMode(mode, loader) {
    let result = {
      loader,
      total: 0,
      page: 1,
      pages: computed(() => {
        let total = this._getMode(mode).total
        return total ? Math.ceil(total / this._per_page) : 1
      })
    }
    this._state[mode] = result
    return result
  }

  _getMode(mode) {
    return this._state[mode]
  }

  getDBStores() {
    let res = {} 
    let keys = 'id, status, author, owned, [author+owned]'
    if (this._extra_dbkeys) {
      keys = [keys, this._extra_dbkeys].join(', ')
    }
    res[this._store_name] = keys
    res[this._metastore_name] = 'name'
    return res
  }

  getPage(mode) {
    return this._getMode(mode).page
  }

  setPage(mode, page) {
    this._getMode(mode).page = page
  }

  getPages(mode) {
    return this._getMode(mode).pages
  }

  getAllItemsCount(mode) {
    return this._getMode(mode).total
  }

  getLazyAllItems(mode) {
    // TODO: convert to computed with error + loading flags
    let loader = this._getMode(mode).loader
    if (loader) {
      let qloader = () => loader(this._db[this._store_name], this._my_key)
        .toArray(items => items.map(item => this._fromContract(item)))
      return liveQuery(qloader)
    }
    return new Observable(subscriber => subscriber.next(null))
  }
  
  getLazyPageItems(mode) {
    // TODO: convert to computed with error + loading flags
    let loader = this._getMode(mode).loader

    if (loader) {
      let page = this.getPage(mode)
      let qloader = () => loader(this._db[this._store_name], this._my_key)
        .offset((page -1) * this._per_page)
        .limit(this._per_page)
        .toArray(items => items.map(item => this._fromContract(item)))
        
      return liveQuery(qloader)
    }

    return new Observable(subscriber => subscriber.next(null))
  }

  getLazyItem(mode, id) {
    // TODO: convert to computed with error + loading flags
    let loader = () => this._db[this._store_name]
      .where('id')
      .equals(id)
      .toArray(items => items.map(item => this._fromContract(item)))
    return liveQuery(loader)
  }

  getTotal(mode) {
    return this._getMode(mode).total
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

  async loadAsync() {
    if(this._loading) {
      return false
    }

    if (!this._my_key) {
      this._my_key = await this._loadKeyAsync()
    }

    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: `view_${this._objname}s`,
      cid
    })

    if (this._global.state.debug) {
      console.log(`view_${this._objname}s result`, res)
    }

    utils.ensureField(res, 'items', 'array')
    let approved = 0
    let approved_liked = 0
    let pending = 0
    let rejected = 0
    let artist = 0
    let owned = 0
    let liked = 0

    for(let item of res.items) {
      let original = item
      original.liked = item.impressions ? 1 : 0
      original.sale  = (item.price || {}).amount > 0 ? 1 : 0

      try {
        if (!item.label) {
          throw new Error('label cannot be empty')
        }

        if (!item.data) {
          throw new Error('empty data on item') 
        }
        
        item.label = formats.fromContract(item.label)
        item.data  = formats.fromContract(item.data, this._versions) 
        item = this._fromContract(item)

        if (this._global.debug) {
          console.log('item loaded with label', item.label)
        }
      }
      catch(err) {
        console.log(`ItemsStore._loadItems for ${this._objname}-${item.id}`, err)
        item.error = err
        item.label = `Failed to load ${this._objname}`
      }

      if (item.status === 'approved') {
        approved++

        if (item.impressions) {
          approved_liked++
          
        }
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

    let db = this._db
    await db.transaction('rw!', db[this._store_name], db[this._metastore_name], async () => {
      await db[this._metastore_name].put({name: 'approved', value: approved})
      await db[this._metastore_name].put({name: 'rejected', value: rejected})
      await db[this._metastore_name].put({name: 'pending', value: pending})
      await db[this._metastore_name].put({name: 'artist', value: artist})
      await db[this._metastore_name].put({name: 'owned', value: owned})
      await db[this._metastore_name].put({name: 'liked', value: liked})
      await db[this._store_name].bulkPut(res.items)
    })

    this._getMode('user').total = approved
    this._getMode('user:liked').total = approved_liked
    this._getMode('artist').total = artist
    this._getMode('owner').total = owned
    this._getMode('moderator').total = pending
    this._loading = false

    return true
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

  toDetails(id) {
    router.push({
      name: this._objname,
      params: {
        id
      }
    })
  }
}