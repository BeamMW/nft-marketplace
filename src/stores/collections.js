import {computed} from 'vue'
import {versions, cid} from 'stores/consts'
import utils from 'utils/utils'
import formats from 'stores/formats'
import artistsStore from 'stores/artists'
import imagesStore from 'stores/images'
import ItemsStore from 'stores/items'

class CollectionsStore extends ItemsStore {
  constructor () {
    super('collection', [versions.COLLECTION_VERSION])
  }

  _fromContract (coll) {
    coll = Object.assign({}, coll)
    coll.description = coll.data.description

    let author = artistsStore.loadArtist(coll.author)
    coll.owned = computed(() => artistsStore.my_id == coll.author)

    coll.author_error = computed(() => {
      return author.error
    })

    coll.author_name = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return author.label
    }) 

    coll.by_author = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return `by <span style="color:#00f6d2">${author.label}</span>`
    })
    
    coll.avatar = computed(() => {
      if(author.loading) return {loading: true}
      if(author.error) return {error: true}
      return author.avatar
    })

    if (coll.error) {
      coll.cover = {error: true}
    } 

    if (!coll.error) {  
      coll.cover = imagesStore.fromContract(coll.data.cover)
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

  async setCollection(id, label, data) {
    ({label, data} = await this._toContract(label, data))
    
    let args = {
      role: 'artist',
      action: 'set_collection',
      label, data, cid
    }

    if (id) {
      args['id'] = id
    }

    return await utils.invokeContractAsyncAndMakeTx(args)
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore