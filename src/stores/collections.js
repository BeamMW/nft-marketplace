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
      let author = artistsStore.loadArtist(coll.author)

      try {
        coll = this._fromContract(coll)
        coll.cover = imagesStore.fromContract(coll.cover)
        if (coll.default) {
          coll.label = computed(() => {
            if (author.value.loading) return 'Loading...'
            if (author.value.error) return 'Failed to load author'
            return `${author.value.label} collection`
          })
        }
      } 
      catch (err) {
        console.log(`CollectionsStore._loadCollections for ${coll.id}`, err)
        coll.error = err
        coll.label = 'Failed to load collection'
        coll.cover = {error: true}
      }

      coll.owned = computed(() => artistsStore.my_id == coll.author)
      coll.author_name = computed(() => {
        if (author.value.loading) return 'Loading...'
        if (author.value.error) return 'Failed to load author'
        return author.value.label
      }) 
      
      coll.avatar = computed(() => {
        if(author.value.loading) return {loading: true}
        if(author.value.error) return {error: true}
        return author.value.avatar
      })

      if (coll.default) {
        coll.cover = computed(() => {
          if(author.value.loading) return {loading: true}
          if(author.value.error) return {error: true}
          return author.value.banner
        })
        coll.description = computed(() => {
          if (author.value.loading || author.value.error) return ''
          return `This collection includes all artworks by ${author.value.label} that are not in other collections.`
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

    if (!label) {
      throw new Error('label cannot be empty')
    }

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