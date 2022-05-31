import {computed} from 'vue'
import {versions, cid} from 'stores/consts'
import utils from 'utils/utils'
import formats from 'stores/formats'
import artistsStore from 'stores/artists'
import imagesStore from 'stores/images'
import LazyItems from 'stores/lazy-items'

class CollectionsStore extends LazyItems {
  constructor () {
    super({
      objname: 'collection', 
      versions: [versions.COLLECTION_VERSION],
      modes: ['moderator', 'user', 'artist']
    })
  }

  _fromContract (coll) {
    coll = Object.assign({}, coll)
    
    coll.store = this
    coll.description = coll.data.description
    coll.instagram = coll.data.instagram
    coll.twitter = coll.data.twitter
    coll.website = coll.data.website

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

    // TODO: review all author props, may be there is a better way to do this
    // 1. move to author{props}
    coll.author_about = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return author.about
    }) 

    coll.author_instagram = computed(() => {
      if (author.loading || author.error) return undefined
      return author.instagram
    }) 

    coll.author_twitter = computed(() => {
      if (author.loading || author.error) return undefined
      return author.twitter
    }) 

    coll.author_website = computed(() => {
      if (author.loading || author.error) return undefined
      return author.website
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
      data: formats.toContract(data, versions.COLLECTION_VERSION)
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