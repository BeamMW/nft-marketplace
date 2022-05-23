import LazyItems from 'stores/lazy-items'
import {versions} from 'stores/consts'
import ArtistsCommon from 'stores/artists-common'

class ArtistsStore extends LazyItems {
  constructor () {
    super({
      objname: 'artist', 
      versions: [versions.ARTIST_VERSION]
    })
  }

  _fromContract(cartist) {
    let artist = ArtistsCommon.fromContract(cartist)
    artist.store = this
    return artist
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore