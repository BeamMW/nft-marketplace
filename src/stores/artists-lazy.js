import LazyItems from 'stores/lazy-items'
import {versions} from 'stores/consts'
import ArtistsCommon from 'stores/artists-common'

class ArtistsStore extends LazyItems {
  constructor () {
    super({
      objname: 'artist', 
      versions: [versions.ARTIST_VERSION],
      modes: ['moderator']
    })
  }

  fromDB(artist) {
    artist = ArtistsCommon.fromDB(artist)
    artist.store = this
    return artist
  }
  
  fromContract(artist) {
    return ArtistsCommon.fromContract(artist)
  }

  getFirstIDX() {
    return '000000000000000000000000000000000000000000000000000000000000000001'
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore