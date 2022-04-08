import {reactive, computed} from 'vue'
import {versions, cid} from 'stores/consts'
import utils from 'utils/utils'
import formats from 'stores/formats'
import artistsStore from 'stores/artists'
import imagesStore from 'stores/images'

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
    res.collections = res.collections.slice(1)

    let colls = []
    for (let coll of res.collections) {
      coll = this._fromContract(coll)
      
      let author = artistsStore.artists[coll.author]
      coll.author_name = computed(() => author.label)
      coll.owned = computed(() => artistsStore.my_id == coll.author)
      coll.avatar = computed(() => author.avatar)
      coll.cover = imagesStore.fromContract(coll.cover)

      if (coll.default) {
        coll.description = computed(() => `This collection includes all artworks by ${author.label} that are not in other collections.`)
        coll.cover = computed(() => author.banner)
        coll.label = computed(() => `${author.label} collection`)
      }

      colls.push(coll)
    }

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

  async _toContract(label, data) {
    data.cover = await imagesStore.toContract(data.cover)
    alert(JSON.stringify(data))
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.COLLECTION_VERSION, data)
    }
  }

  async loadAsync() {
    await this._loadTotals('user')
    await this._loadCollections('user')
  }

  async setCollection(label, data) {
    ({label, data} = await this._toContract(label, data))

    return await utils.invokeContractAsyncAndMakeTx({
      role: 'artist',
      action: 'set_collection',
      label, data, cid
    })
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore