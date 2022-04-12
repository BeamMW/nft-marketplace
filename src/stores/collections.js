import {reactive, computed} from 'vue'
import {versions, cid} from 'stores/consts'
import utils from 'utils/utils'
import formats from 'stores/formats'
import artistsStore from 'stores/artists'
import imagesStore from 'stores/images'
import router from 'router'

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

  user_coll(id) {
    return computed(() => this._state.user.collections.find(c => c.id === id))
  }

  async _loadTotals(mode) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_collections_stats',
      cid
    })

    utils.ensureField(res, 'total', 'number')
    this._state[mode].total = res.total
  }

  async _loadCollections(mode) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_collections',
      cid
    })
    
    utils.ensureField(res, 'collections', 'array')
    let colls = []

    for (let coll of res.collections) {
      coll = this._fromContract(coll)
      coll.owned = computed(() => artistsStore.my_id == coll.author)
      coll.cover = imagesStore.fromContract(coll.cover)
      
      let author = artistsStore.loadArtist(coll.author)
      coll.author_name = computed(() => author.value.loading ? 'Loading...' : author.value.label)
      coll.avatar = computed(() => author.value.avatar)

      if (coll.default) {
        coll.cover = computed(() => {
          return author.value.loading ? {loading: true} : author.value.banner
        })
        coll.label = computed(() => {
          if (author.value.loading) return 'Loading...'
          return `${author.value.label} collection`
        })
        coll.description = computed(() => {
          if (author.value.loading) return ''
          return `This collection includes all artworks by ${author.value.label} that are not in other collections.`
        })
        coll.author_name = computed(() => {
          return author.value.loading ? '' : author.value.label
        })        
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
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.COLLECTION_VERSION, data)
    }
  }

  async loadAsync() {
    await this._loadTotals('user')
    await this._loadCollections('user')
  }

  async setCollection(id, label, data) {
    ({label, data} = await this._toContract(label, data))
    
    let args = {
      role: 'artist',
      action: 'set_collection',
      label, data, cid
    }

    if (id) {
      args['collection_id'] = id
    }

    return await utils.invokeContractAsyncAndMakeTx(args)
  }

  toNewCollection() {
    router.push({
      name: 'new-collection'
    })
  }

  toEditCollection(id) {
    router.push({
      name: 'edit-collection',
      params: {
        id
      }
    })
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore