import ItemsStore from 'stores/items'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import formats from 'stores/formats'
import {versions} from 'stores/consts'
import {computed} from 'vue'

class ArtworksStore extends ItemsStore{
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
    
    return awork
  }

  async _toContract(label, data) {
    data.image = await imagesStore.toContract(data.image)
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.ARTWORK_VERSION, data)
    }
  }
}

let artworksStore = new ArtworksStore()
export default artworksStore