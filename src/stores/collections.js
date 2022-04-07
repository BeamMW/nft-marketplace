import {reactive} from 'vue'
import {versions, cid} from 'stores/consts'
import utils from 'utils/utils'
import formats from 'stores/formats'

class CollectionsStore {
  constructor () {
    this.reset()
  }

  reset (global) {
    this._global = global
    this._state = reactive({
      user: {
        collections: [],
        total: 0
      }
    })
  }

  get user_colls() {
    return this._state.user.collections
  }

  async _loadTotals(mode) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_collections_stats',
      cid
    })

    utils.ensureField(res, 'collections_stats', 'object')
    utils.ensureField(res.collections_stats, 'total', 'number')
    this._state[mode].total = res.collections_stats.total
  }

  async _loadCollections(mode) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_collections',
      cid
    })
    
    utils.ensureField(res, 'collections', 'array')
    // TODO: remove test code slice(1)
    let colls = res.collections.slice(1).map(coll => this._fromContract(coll))
    this._state[mode].collections = colls
  }

  // TODO: do not fail app on this?
  _fromContract(coll) {
    coll = Object.assign({}, coll)

    let [label] = formats.fromContract(coll.label)
    coll.label = label

    if (!coll.default) {
      let [version, data] = formats.fromContract(coll.data)
      coll.version = version

      if (version != versions.COLLECTION_VERSION) {
        throw new Error('Collection version mismatch: ' + version + ' != ' + versions.COLLECTION_VERSION) 
      }
      Object.assign(coll, data)
    }
    
    return coll
  }

  async loadAsync() {
    await this._loadTotals('user')
    await this._loadCollections('user')
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore