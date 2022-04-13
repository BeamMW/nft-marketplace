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

    let author = artistsStore.loadArtist(coll.author)
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

    if (coll.error) {
      coll.cover = {error: true}
    } 
    
    if (!coll.error) {  
      if (!coll.default) {
        coll.cover = imagesStore.fromContract(coll.cover)
      }

      if (coll.default) {
        coll.cover = computed(() => {
          if(author.value.loading) return {loading: true}
          if(author.value.error) return {error: true}
          return author.value.banner
        })
        coll.label = computed(() => {
          if (author.value.loading) return 'Loading...'
          if (author.value.error) return 'Failed to load author'
          return `${author.value.label} collection`
        })
        coll.description = computed(() => {
          if (author.value.loading || author.value.error) return ''
          return `This collection includes all artworks by ${author.value.label} that are not in other collections.`
        })
      } 
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
      args['collection_id'] = id
    }

    return await utils.invokeContractAsyncAndMakeTx(args)
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore