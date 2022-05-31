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

  _fromContract(cartist) {
    let result = ArtistsCommon.fromContract(cartist)
    result.store = this
    return result
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore