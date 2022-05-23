import imagesStore from 'stores/images'
import formats from 'stores/formats'
import {versions} from 'stores/consts'

export default class ArtistsCommon {
  static fromContract(cartist) {
    let artist = Object.assign({}, cartist)

    artist.version   = artist.data.version
    artist.about     = artist.data.about
    artist.website   = artist.data.website
    artist.twitter   = artist.data.twitter
    artist.instagram = artist.data.instagram
    artist.avatar    = imagesStore.fromContract(artist.data.avatar)  // TODO: if (artist.error) artist.avatar = {error: true}
    artist.banner    = imagesStore.fromContract(artist.data.banner)  // TODO: if (artist.error) artist.avatar = {error: true}
  
    return artist
  }

  static async _toContract (label, data) {
    data.avatar = await imagesStore.toContract(data.avatar)
    data.banner = await imagesStore.toContract(data.banner)
    
    return {
      label: formats.toContract(label),
      data: formats.toContract(data, versions.ARTIST_VERSION)
    }
  } 
}
