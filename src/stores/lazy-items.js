import formats from 'stores/formats'
import router from 'router'
import {common, sort as sortOrder} from 'utils/consts'
import {computed, reactive} from 'vue'
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
  //  owned       -> keys: owned
  //  owned:sale  -> keys: [owned+sale]
  //  artist:sold -> keys: [author+owned]
  //
  constructor({objname, versions, perPage, extraDBKeys, modes}) {
    super({objname, extraDBKeys})
    this._versions = versions
    this._per_page = perPage || common.ITEMS_PER_PAGE
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

      if (mode == 'owned') {
        keyset.add('owned')
      }

      if (mode == 'owned:sale') {
        keyset.add('[owned+sale]')
      }

      if (mode == 'artist:sold') {
        keyset.add('[author+owned]')
      }
    }

    this._db_keys = Array.from(keyset)
  }

  reset (global, db) {
    super.reset(global, db)

    this._query = reactive({
      search: '',
      // ids of artists whose name matches the search, comma separated so a
      // plain !== comparison in setQuery still works
      search_authors: '',
      author: '',
      sort: sortOrder.NEWEST_TO_OLDEST
    })

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
      mode: 'user:sale', 
      loader: (store) => {
        console.log(`loader for user:sale:${this._store_name}`)
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
  }

  _allocMode({mode, loader, computed_total}) {
    let result = {
      loader,
      total: 0,
      page: 1,
      // number of items left after search/filter, only meaningful while a
      // query is active - see _queryActive()
      filtered_total: 0,
      pages: computed(() => {
        let self = this._getMode(mode)
        let total = this._queryActive() ? self.filtered_total : self.total
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
  
  // Search, author filter and sort order, shared by every mode of the store
  // so switching tabs keeps the selection.
  get query () {
    return this._query
  }

  setQuery (patch) {
    let changed = false

    for (let key in patch) {
      if (this._query[key] !== patch[key]) {
        this._query[key] = patch[key]
        changed = true
      }
    }

    if (changed) {
      // a narrower result set can leave us past the last page
      for (let mode in this._state) {
        let entry = this._state[mode]
        if (entry && entry.page > 1) entry.page = 1
      }
    }
  }

  resetQuery () {
    this.setQuery({search: '', search_authors: '', author: '', sort: sortOrder.NEWEST_TO_OLDEST})
  }

  _queryActive () {
    return !!this._query.search ||
           !!this._query.author ||
           this._query.sort !== sortOrder.NEWEST_TO_OLDEST
  }

  _applyQuery (items) {
    let {search, author, sort} = this._query
    let result = items

    if (author) {
      result = result.filter(item => item.author === author)
    }

    if (search) {
      let needle = search.trim().toLowerCase()
      if (needle) {
        // searching an artist's name should surface their work, not nothing -
        // the page resolves matching artists and passes their ids down
        let authors = this._query.search_authors
          ? new Set(this._query.search_authors.split(','))
          : undefined

        result = result.filter(item => {
          if (String(item.label || '').toLowerCase().indexOf(needle) !== -1) {
            return true
          }
          return !!authors && authors.has(String(item.author))
        })
      }
    }

    const amount = (item) => Number((item.price || {}).amount || 0)
    const likes  = (item) => Number(item.likes || 0)

    // nft/collection ids are numbers, artist ids are 66-char hex strings -
    // subtracting those gives NaN and leaves the list unsorted
    const byId = (a, b) => {
      if (typeof a.id === 'number' && typeof b.id === 'number') {
        return a.id - b.id
      }
      return String(a.id).localeCompare(String(b.id))
    }

    // Unpriced items are not free - they belong at the end either way.
    const byPrice = (dir) => (a, b) => {
      let pa = amount(a)
      let pb = amount(b)
      if (!pa && !pb) return byId(b, a)
      if (!pa) return 1
      if (!pb) return -1
      return dir * (pa - pb)
    }

    switch (sort) {
    case sortOrder.OLDEST_TO_NEWEST:
      result = result.slice().sort(byId)
      break
    case sortOrder.PRICE_ASC:
      result = result.slice().sort(byPrice(1))
      break
    case sortOrder.PRICE_DESC:
      result = result.slice().sort(byPrice(-1))
      break
    case sortOrder.LIKES_ASC:
      result = result.slice().sort((a, b) => likes(a) - likes(b))
      break
    case sortOrder.LIKES_DESC:
      result = result.slice().sort((a, b) => likes(b) - likes(a))
      break
    default:
      // NEWEST_TO_OLDEST - ids grow monotonically with mint order
      result = result.slice().sort((a, b) => byId(b, a))
      break
    }

    return result
  }

  getLazyPageItems(modename) {
    let mode = this._getMode(modename)
    if (!mode.loader) {
      throw new Error(`No loader for mode ${modename}`)
    }

    let page = this.getPage(modename)

    // Fast path: no query, let Dexie do the paging.
    if (!this._queryActive()) {
      let qloader = () => mode.loader(this._db[this._store_name], this.my_key)
        .offset((page -1) * this._per_page)
        .limit(this._per_page)
        .toArray(items => items.map(item => this.fromDB(item)))

      return liveQuery(qloader)
    }

    // Query path: search, author filter and sort keys (price, likes) are not
    // all indexed, so the matching set is materialised and paged in memory.
    let qloader = () => mode.loader(this._db[this._store_name], this.my_key)
      .toArray(items => {
        let filtered = this._applyQuery(items)

        if (mode.filtered_total !== filtered.length) {
          mode.filtered_total = filtered.length
        }

        return filtered
          .slice((page - 1) * this._per_page, page * this._per_page)
          .map(item => this.fromDB(item))
      })

    return liveQuery(qloader)
  }

  // one-shot local lookup, no observable
  async getItemFromDB(id) {
    if (!this._db || !this._db[this._store_name]) {
      return undefined
    }

    let items = await this._db[this._store_name].where('id').equals(id).toArray()
    return items.length ? items[0] : undefined
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
      'artist_sold': 0, // TODO: move to NFTs store same as done with likes
      'owned': 0,
      'owned_sale': 0, // TODO: move to NFTs store same as with likes
      'approved_sale': 0, // TODO: move to NFTs store same as with likes
    })
  }

  async detectOwned(item) {
    return item.owned
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
    
    let owned = await this.detectOwned(item)
    if (owned !== undefined) {
      item.owned = owned
    }

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
      // ... on sale
      //
      if (old.sale) {
        status.approved_sale--
      }
    }

    if (item.approved) {
      //
      // ... on sale
      //
      if (item.sale) {
        status.approved_sale++
      }
    }

    return old
  }

  async onEnd(status) {
    this._getMode('moderator').total   = status.pending
    this._getMode('user').total        = status.approved
    this._getMode('user:sale').total   = status.approved_sale
    this._getMode('artist').total      = status.artist
    this._getMode('artist:sold').total = status.artist_sold
    this._getMode('owned').total       = status.owned
    this._getMode('owned:sale').total  = status.owned_sale
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
