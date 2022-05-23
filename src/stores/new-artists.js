import LazyItems from 'stores/lazy-items'
import imagesStore from 'stores/images'
import formats from 'stores/formats'
import utils from 'utils/utils'
import router from 'router'
import {versions, cid} from 'stores/consts'

class ArtistsStore extends LazyItems {
  constructor () {
    super({
      objname: 'artist', 
      versions: [versions.ARTIST_VERSION]
    })
  }

  reset (global, db) {
    super._reset(global, db, {
      artist_tx: '',
      is_artist: false
    })
  }

  async loadAsync() {
    if (!super.loadAsync()) {
      return false
    }

    // load self
    // await this.loadSelf()
    return true
  }

  async _toContract(label, data) {
    data.avatar = await imagesStore.toContract(data.avatar)
    data.banner = await imagesStore.toContract(data.banner)
    return {
      label: formats.toContract(label),
      data: formats.toContract(data, versions.ARTIST_VERSION)
    }
  }

  _fromContract (cartist) {
    let artist = Object.assign({}, cartist)

    if (!artist.label) {
      throw new Error('ArtistsStore._fromContract : artist label is empty')
    }

    let label = formats.fromContract(artist.label)
    let data  = formats.fromContract(artist.data, versions.ARTIST_VERSION)

    artist.label   = label
    artist.version = data.version
    artist.about   = data.about
    artist.avatar  = imagesStore.fromContract(artist.data.avatar)
    artist.banner  = imagesStore.fromContract(artist.data.banner)
  
    return artist
  }

  async setArtist(label, data) {
    ({label, data} = await this._toContract(label, data))
    let txid = await utils.invokeContractAsyncAndMakeTx({
      role: 'artist',
      action: 'set_artist',
      label, data, cid
    })

    if (!txid) {
      return
    }

    this._state.artist_tx = txid
    let interval = setInterval(async () => {
      try {
        let {res} = await utils.callApiAsync('tx_status', {txId: this.artist_tx})
        if ([0, 1, 5].indexOf(res.status) == -1) {
          clearInterval(interval)
          this._state.artist_tx = ''
        }
      }
      catch(err) {
        this._state.artist_tx = ''
        this.global.setError(err)
      }
    }, 1000)
  }

  toBecomeArtist() {
    if (this.is_artist) {
      throw new Error('Unexpected toBecomeArtist, already an artist')
    }
    router.push({
      name: 'become-artist'
    })
  }

  toEditSelf() {
    if (!this.is_artist) {
      throw new Error('Unexpected EditArtist, not an artist yet')
    }
    router.push({
      name: 'edit-artist',
      params: {
        id: this._state.my_id
      }
    })
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore