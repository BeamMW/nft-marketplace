import ItemsStore from 'stores/items'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import formats from 'stores/formats'
import {versions, cid} from 'stores/consts'
import {computed} from 'vue'
import utils from 'utils/utils'
import router from 'router'

class ArtworksStore extends ItemsStore {
  constructor () {
    super('artwork', [versions.ARTWORK_VERSION])
  }

  _fromContract(awork) {
    awork = Object.assign({}, awork)
    let author = artistsStore.loadArtist(awork.author)
    
    awork.author_error = computed(() => {
      return author.value.error
    })

    awork.author_name = computed(() => {
      if (author.value.loading) return 'Loading...'
      if (author.value.error) return 'Failed to load author'
      return author.value.label
    }) 

    awork.by_author = computed(() => {
      if (author.value.loading) return 'Loading...'
      if (author.value.error) return 'Failed to load author'
      return `by ${author.value.label}`
    })

    if (awork.error) {
      awork.image = {error: true}
    }

    if (!awork.error) {
      awork.image = imagesStore.fromContract(awork.image)
    }

    // TODO: convert to true js object
    if (awork['price.aid'] != undefined) {
      awork.price = {
        aid: awork['price.aid'],
        amount: awork['price.amount']
      }
    }
    
    return awork
  }

  async _toContract(label, data) {
    data.image = await imagesStore.toContract(data.image)
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.ARTWORK_VERSION, data)
    }
  }

  async createNFT(collid, label, data) {
    ({label, data} = await this._toContract(label, data))
    
    let args = {
      role: 'artist',
      action: 'set_artwork',
      collection_id: collid,
      label, data, cid
    }

    return await utils.invokeContractAsyncAndMakeTx(args)
  }

  toDetails(id) {
    router.push({
      name: 'artwork',
      params: {
        id
      }
    })
  }

  async setPrice (id, amount) {
    try {
      return await utils.invokeContractAsyncAndMakeTx({
        role: 'user',
        action: 'set_price',
        aid: 0,
        amount, id, cid
      })
    }
    catch(err) {
      this._global.setError(err)
    }
  }

  async buyArtwork (id) {
    try {
      return await utils.invokeContractAsyncAndMakeTx({
        role: 'user',
        action: 'buy',
        id, cid
      })
    }
    catch(err) {
      this._global.setError(err)
    }
  }

  async getSales(id) {
    try {
      let {res} = await utils.invokeContractAsync({
        role: 'manager',
        action: 'view_artwork_sales',
        id, cid
      })
      return res.sales
    }
    catch(err) {
      this._global.setError(err)
    }
  }
}

let artworksStore = new ArtworksStore()
export default artworksStore