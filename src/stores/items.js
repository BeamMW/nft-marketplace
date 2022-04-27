import {computed, reactive} from 'vue'
import utils from 'utils/utils.js'
import {cid} from 'stores/consts'
import {common} from 'utils/consts'
import formats from 'stores/formats'
import router from 'router'

export default class ItemsStore {
  constructor(objname, versions, perPage) {
    this._objname = objname
    this._versions = versions
    this._modes = ['user', 'artist']
    this._per_page = perPage || common.ITEMS_PER_PAGE
    this.reset()
  }

  reset (global) {
    this._global = global
    this._state = reactive({
      user: {
        items: [],
        total: 0,
        page: 1,
        pages: computed(() => {
          let total = this._state.user.total
          return total ? Math.ceil(total / this._per_page) : 1
        })
      },
      artist: {
        items: [],
        total: 0,
        page: 1,
        pages: computed(() => {
          let total = this._state.artist.total
          return total ? Math.ceil(total / this._per_page) : 1
        })
      }
    })
  }

  getItem(id) {
    for (let mode of this._modes) {
      for (let item of this._state[mode].items) {
        if (item.id == id) {
          return item
        }
      }
    }
   
    throw new Error(`ItemsStore::getItem - item ${this._objname}-${id} not found`)
  }

  get user_items() {
    return this._state.user.items
  }

  get user_page() {
    return this._state.user.page
  }

  get artist_items() {
    return this._state.artist.items
  }

  getPage(mode) {
    return this._state[mode].page
  }

  setPage(mode, page) {
    this._state[mode].page = page
    this._loadItems(mode)
  }

  getPages(mode) {
    return this._state[mode].pages
  }

  getItems(mode) {
    return this._state[mode].items
  }

  getTotal(mode) {
    return this._state[mode].total
  }
  
  async _loadTotals(mode) {
    let {res} = await utils.invokeContractAsync({
      role: mode,
      action: `view_${this._objname}s_stats`,
      cid
    })

    utils.ensureField(res, 'total', 'number')
    this._state[mode].total = res.total
  }

  async _loadItems(mode) {
    let idx0 = (this._state[mode].page - 1) * this._per_page
    let count = this._per_page
    let {res} = await utils.invokeContractAsync({
      role: mode,
      action: `view_${this._objname}s`,
      idx0, count, cid
    })

    console.log('idx0', idx0, 'count', count)

    let field = `${this._objname}s`
    utils.ensureField(res,field, 'array')
    let items = []
    
    for (let item of res[field]) {
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
      }
      catch(err) {
        console.log(`ItemsStore._loadItems for ${this._objname}-${item.id}`, err)
        item.error = err
        item.label = `Failed to load ${this._objname}`
      }
    
      item = this._fromContract(item)
      items.push(item)
    }

    this._state[mode].items = items
  }

  async loadAsync() {
    await this._loadTotals('user')
    await this._loadItems('user')
    // TODO: don't load artist items if user is not an artist
    await this._loadTotals('artist')
    await this._loadItems('artist')
    console.log(`ItemsStore.loadAsync artist ${this._objname}: `, this._state.artist.total)
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