import formats from 'stores/formats'
import router from 'router'
import {common} from 'utils/consts'
import {computed} from 'vue'
import {liveQuery} from 'dexie'
import {useObservable} from '@vueuse/rxjs'
import LazyLoader from 'stores/lazy-loader'

export default class ItemsStore extends LazyLoader {
  // Keys 
  //  for all items: id
  //
  // Artists modes: 
  //  moderator -> keys: status
  //
  // Collections modes:
  //  moderator -> keys: status
  //  user      -> keys: status
  //  artist    -> keys: author
  //
  // NFTS modes:
  //  moderator   -> keys: status
  //  user        -> keys: status
  //  user:sale   -> keys: [status+sale]
  //  user:liked  -> keys: [status+liked]
  //  owned       -> keys: owned
  //  owned:sale  -> keys: [owned+sale]
  //  artist:sold -> keys: [author+owned]
  //  liker:liked -> keys: [satus+my_like]
  //
  constructor({objname, versions, perPage, extraDBKeys, modes}) {
    super({objname, versions})
    this._per_page = perPage || common.ITEMS_PER_PAGE
    this._store_name = `${objname}s`
    this._modes = [...modes]
    
    //
    // Generate keys list based on extra keys and modes
    //
    let keyset = new Set()
    keyset.add('id')

    if (extraDBKeys) {
      for (let key of extraDBKeys) {
        keyset.add(key)
      }
    }

    for (let mode of modes) {
      if (mode === 'moderator') {
        keyset.add('status')
      }

      if (mode === 'user') {
        keyset.add('status')
      }

      if (mode === 'artist') {
        keyset.add('author')
      }
      
      if (mode == 'user:sale') {
        keyset.add('[status+sale]')
      }

      if (mode == 'user:liked') {
        keyset.add('[status+liked]')
      }

      if (mode == 'owned') {
        keyset.add('owned')
      }

      if (mode == 'owned:sale') {
        keyset.add('[owned+sale]')
      }

      if (mode == 'artist:sold') {
        keyset.add('[author+owned]')
      }

      if (mode == 'liker:liked') {
        keyset.add('[status+my_like]')
      }
    }

    this._db_keys = Array.from(keyset)
  }

  reset (global, db) {
    super.reset(global, db)

    this._allocMode({
      mode: 'user', 
      loader: (store) => {
        console.log(`loader for user:${this._store_name}`)
        return store
          .where('status')
          .equals('approved')
      }
    })

    this._allocMode({
      mode: 'user:liked', 
      loader: (store) => {
        console.log(`loader for user:liked:${this._store_name}`)
        return store
          .where(['status', 'liked'])
          .equals(['approved', 1])
      }
    })

    this._allocMode({
      mode: 'user:sale', 
      loader: (store) => {
        console.log(`loader for user:liked:${this._store_name}`)
        return store
          .where(['status', 'sale'])
          .equals(['approved', 1])
      }
    })
    
    this._allocMode({
      mode: 'moderator', 
      loader: (store) => {
        console.log(`loader for moderator:${this._store_name}`)
        return store
          .where('status')
          .equals('pending')
      }
    })

    this._allocMode({
      mode: 'artist', 
      loader: (store, mykey) => {
        console.log(`loader for artist:${this._store_name}`)
        return store
          .where('author')
          .equals(mykey)
      }
    })

    this._allocMode({
      mode: 'owned', 
      loader: (store) => {
        console.log(`loader for owned:${this._store_name}`)
        return store
          .where('owned')
          .equals(1)
      }
    })

    this._allocMode({
      mode: 'owned:sale', 
      loader: (store) => {
        console.log(`loader for owned:sale:${this._store_name}`)
        return store
          .where(['owned', 'sale'])
          .equals([1, 1])
      }
    })

    this._allocMode({
      mode: 'artist:sold', 
      loader: (store, mykey) => {
        console.log(`loader for artist:sold:${this._store_name}`)
        return store
          .where(['author', 'owned'])
          .equals([mykey, 0])
      }
    })

    this._allocMode({
      mode: 'liker:liked', 
      loader: (store) => {
        console.log(`loader for liker:liked:${this._store_name}`)
        return store
          .where(['status', 'my_like'])
          .equals(['approved', 1])
      }
    })
  }

  _allocMode({mode, loader, computed_total}) {
    let result = {
      loader,
      total: 0,
      page: 1,
      pages: computed(() => {
        let total = this._getMode(mode).total
        return total ? Math.ceil(total / this._per_page) : 1
      })
    }

    if (computed_total) {
      let observable = useObservable(liveQuery(() => loader(this._db[this._store_name]).count()))
      result.total = computed(() => {
        return observable.value
      })
    }

    this._state[mode] = result
    return result
  }

  _getMode(mode) {
    return this._state[mode]
  }

  getDBStores() {
    let res = super.getDBStores()
    res[this._store_name] = this._db_keys.join(',')
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

  //
  // Do not call getLazyAllItems from inside a computed() because of useObservable
  //
  getLazyAllItems(modename) {
    let mode = this._getMode(modename)
    if (!mode.loader) {
      throw new Error(`No loader for mode ${modename}`)
    }
    let loader = () => mode.loader(this._db[this._store_name], this.my_key).toArray(items => items.map(item => this.fromDB(item)))
    return useObservable(liveQuery(loader)) // TODO: -> null, undefined, value
  }
  
  getLazyPageItems(modename) {
    let mode = this._getMode(modename)
    if (!mode.loader) {
      throw new Error(`No loader for mode ${modename}`)
    }

    let page = this.getPage(modename)
    let qloader = () => mode.loader(this._db[this._store_name], this.my_key)
      .offset((page -1) * this._per_page)
      .limit(this._per_page)
      .toArray(items => items.map(item => this.fromDB(item)))
        
    return liveQuery(qloader)
  }

  //
  // Do not call getLazyItem from inside a computed() because of useObservable
  //
  getLazyItem(id) {
    let loader = () => {
      console.log(`lazy loader for ${this._objname}:${id}`)
      return this._db[this._store_name]
        .where('id')
        .equals(id)
        .toArray(items => items.map(item => this.fromDB(item)))
    }
    let observable = useObservable(liveQuery(loader))
    return computed(() => {
      let result = observable.value
      if (!result) return undefined
      return result.length == 0 ? null : result[0]
    })
  }

  defaultStatus() {
    let status = super.defaultStatus()
    return Object.assign(status, {
      'approved': 0,
      'pending': 0,
      'rejected': 0,
      'artist': 0,
      'artist_sold': 0, 
      'owned': 0,
      'owned_sale': 0, 
      'approved_liked': 0, 
      'approved_i_liked': 0,
      'approved_sale': 0,
    })
  }

  async onItem(status, item) {
    //
    // item would be written to database
    // we add some calculated props for convenience
    //
    item.type     = this._objname
    item.sale     = (item.price || {}).amount > 0 ? 1 : 0
    item.approved = (item.status === 'approved') ? 1 : 0 
    item.pending  = (item.status === 'pending') ? 1 : 0
    item.rejected = (!item.approved && !item.pending) ? 1 : 0
    item.liked    = (item.likes > 0) ? 1 : 0

    try {
      if (!item.label) {
        throw new Error('label cannot be empty')
      }

      if (!item.data) {
        throw new Error('empty data on item') 
      }
      
      item.label = formats.fromContract(item.label)
      item.data  = formats.fromContract(item.data, this._versions) 

      // fromContract  
      // - changes some props in the item
      // - add props to the item
      // - all data would be stored in db
      // - must not create a copy
      let fcitem = this.fromContract(item)

      if (fcitem !== item) {
        throw new Error('fromContract copied the item')
      }

      if (this._global.debug) {
        console.log('item loaded with label', item.label)
      }
    }
    catch(err) {
      item.error = err
      item.label = `Failed to load ${this._objname}`
    }

    let db = this._db
    let old = await db[this._store_name].get(item.id)

    //
    // Status
    // 
    if (old !== undefined && old.status !== item.status) {
      if (old.approved) {
        status.approved--
      }
      if (old.pending) {
        status.pending--
      }
      if (old.rejected) {
        status.rejected--
      }
    }
    
    if (old === undefined || old.status !== item.status) {
      if (item.approved) {
        status.approved++
      }
      if (item.pending) {
        status.pending++
      }
      if (item.rejected) {
        status.rejected++
      }
    }

    //
    // Author can't be changed, so we check it only if old object doesn't exist
    //
    if (old === undefined && item.author === this.my_key) {
      status.artist++
    }

    //
    // Owned status
    //
    if (old != undefined && old.owned) {
      status.owned--
    }

    if (item.owned) {
      status.owned++
    }

    //
    // Owned & sale
    //
    if (old != undefined && old.owned && old.sale) {
      status.owned_sale--
    }

    if (item.owned && item.sale) {
      status.owned_sale++
    }

    //
    // Artist & sold
    //
    if (old != undefined && old.author == this.my_key && !!old.first_sale) {
      status.artist_sold--
    }

    if (item.author == this.my_key && !!item.first_sale) {
      status.artist_sold++
    }

    //
    // Approved and ...
    //
    if (old && old.approved) {
      //
      // ... and liked by someone
      //
      if (old.likes > 0) {
        status.approved_liked--
      }

      //
      // ... and liked by me
      //
      if (old.my_like) {
        status.approved_i_liked--
      }

      //
      // ... on sale
      //
      if (old.sale) {
        status.approved_sale--
      }
    }

    if (item.approved) {
      //
      // ... and liked by someone
      //
      if (item.likes > 0) {
        status.approved_liked++
      }

      //
      // ... and liked by me
      //
      if (item.my_like) {
        status.approved_i_liked++
      }

      //
      // ... on sale
      //
      if (item.sale) {
        status.approved_sale++
      }
    }
  }

  async onEnd(status) {
    this._getMode('moderator').total   = status.pending
    this._getMode('user').total        = status.approved
    this._getMode('user:sale').total   = status.approved_sale
    this._getMode('user:liked').total  = status.approved_liked
    this._getMode('artist').total      = status.artist
    this._getMode('artist:sold').total = status.artist_sold
    this._getMode('owned').total       = status.owned
    this._getMode('owned:sale').total  = status.owned_sale
    this._getMode('liker:liked').total = status.approved_i_liked
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

  toDetails(id, mode) {
    router.push({
      name: this._objname,
      params: {
        id
      },
      query: {
        mode
      }
    })
  }
}
