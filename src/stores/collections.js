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

  fromDB(coll) {
    coll = Object.assign({}, coll)
    coll.store = this

    let author = artistsStore.loadArtist(coll.author)
    coll.author_error = computed(() => {
      return author.error
    })

    coll.author_approved = computed(() => {
      return author.approved
    })
    
    coll.author_name = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return author.label
    }) 

    coll.safe_author_name = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      // Author name cannot be changed, so if already ever approved (approved_cnt > 0) it is safe to display
      if (author.approved_cnt) return author.label
      return '[author is in moderation]' 
    }) 

    coll.by_author = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return `by <span style="color:#00f6d2">${author.label}</span>`
    })

    coll.safe_by_author = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      // Author name cannot be changed, so if already ever approved (approved_cnt > 0) it is safe to display
      if (author.approved_cnt) return  `by <span style="color:#00f6d2">${author.label}</span>`
      return '<span style="opacity:0.5">[author is in moderation]</span>'
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

    coll.safe_avatar = computed(() => {
      if(author.loading) return {loading: true}
      if(author.error) return {error: true}
      if(author.approved) return author.avatar
      return require('assets/artist-default-avatar.svg')
    })

    coll.safe_cover = computed(() => {
      if (coll.approved) return coll.cover
      return require('assets/artist-default-banner.svg')
    })

    coll.safe_label = computed(() => {
      if (coll.status === 'approved') return coll.label
      return '[collection is in moderation]'
    })

    coll.safe_description = computed(() => {
      if (coll.approved) return coll.description
      return '[collection is in moderation]'
    })

    if (coll.error) {
      coll.cover = {error: true}
    } 

    if (!coll.error) {  
      coll.cover = imagesStore.fromDB(coll.cover)
    }
    
    return coll
  }

  fromContract (coll) {
    coll.description = coll.data.description
    coll.instagram = coll.data.instagram
    coll.twitter = coll.data.twitter
    coll.website = coll.data.website
    coll.cover = coll.data.cover
    return coll
  }

  async toContract(label, data) {
    data.cover = await imagesStore.toContract(data.cover)
    return {
      label: formats.toContract(label),
      data: formats.toContract(data, versions.COLLECTION_VERSION)
    }
  }

  async setCollection(id, label, data) {
    ({label, data} = await this.toContract(label, data))
    
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

  async detectOwned(item) {
    if (utils.isDesktop()) {
      return item.owned
    }
    
    utils.ensureField(item, 'author', 'string')
    let {res} = await utils.invokeContractAsync({
      role: 'user',
      action: 'is_my_key',
      key: item.author,
      nft_id: 0,
      cid
    })
    
    utils.ensureField(res, 'key', 'number')
    return res.key
  }
}

let collectionsStore = new CollectionsStore()
export default collectionsStore