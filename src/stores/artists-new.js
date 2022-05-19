import LazyItems from 'stores/lazy-items'
import {versions, cid} from 'stores/consts'

class ArtistsStore extends LazyItems {
  constructor () {
    super({
      objname: 'artist', 
      versions: [versions.ARTIST_VERSION],
      dbKeys: 'collection, my_impression, [owned+sale], [liked+status], [status+sale]'
    })
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore