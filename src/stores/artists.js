import formats from 'stores/formats'
import utils from 'utils/utils'
import imagesStore from 'stores/images'
import {versions, cid} from 'stores/consts'
import router from 'router'
import {reactive} from 'vue'

class ArtistsStore {
  constructor () {
    this.reset()
  }

  reset (global) {
    this._global = global
    this._state = reactive({
      my_id: '',
      is_artist: false,
      artist_tx: '',
      artists: {},
      total: 0,
    })
  }

  get total() {
    return this._state.total
  }

  set _total (value) {
    this._state.total = value
  }

  get is_artist () {
    return this._state.is_artist
  }

  get artist_tx () {
    return this._state.artist_tx
  }

  get artists () {
    return this._state.artists
  }

  get my_id() {
    return this._state.my_id
  }

  get self () {
    return this._state.artists[this._state.my_id]
  }

  _setArtist(id, artist) {
    let old = this._state.artists[id]
    
    if (old) {
      utils.clearAssign(this._state.artists[id], artist)
    }

    if (!old) {
      this._state.artists[id] = artist
    }

    return this._state.artists[id]
  }

  async _loadKey () {
    let {res} = await utils.invokeContractAsync({
      role: 'artist',
      action: 'get_id',
      cid
    })

    utils.ensureField(res, 'id', 'string')
    this._state.my_id = res.id
  }

  async _loadSelf() {
    if (!this._state.my_id) {
      return
    }

    let self = await this._loadArtistAsync(this._state.my_id, false)
    if (self === null) {
      this._state.is_artist = false
      return
    }
    
    // We always fail if unable to load self
    if (self.error) {
      throw self.error
    }

    this._state.is_artist = true
  }

  async _loadTotals () {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_artists_stats',
      cid
    })
    
    utils.ensureField(res, 'total', 'number')
    this._state.total = res.total
  }

  async _loadArtistAsync (id, fail) {
    try {
      let {res} = await utils.invokeContractAsync({
        role: 'manager',
        action: 'view_artists',
        ids: id,
        cid
      })

      utils.ensureField(res, 'items', 'array')
      if (res.items.length > 1) {
        throw new Error('artists.length > 1')
      }

      if (res.items.length == 0) {
        if (fail) {
          throw new Error('artist not found')
        }
        return null
      }

      let artist = this._fromContract(res.items[0])
      this._setArtist(id, artist)
    } 
    catch (err) {
      console.log(`_loadArtistAsync for id ${id}:`, err)
      this._setArtist(id, {id, error: err})
    }

    return this._state.artists[id]
  }

  loadArtist(id) {
    if (!id) {
      throw new Error('loadArtist: id is required')
    }

    let cached = this._state.artists[id]
    if (cached) {
      return cached
    }
    
    this._setArtist(id, {id, loading: true})
    this._loadArtistAsync(id)
    return this._state.artists[id]
  }

  async loadAsync() {
    await this._loadKey()
    await this._loadSelf()
    await this._loadTotals()
    // TODO: check that after reload artists loaded with loadArtist would reload themselves
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

  async _toContract (label, data) {
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

    artist.label     = formats.fromContract(artist.label)
    artist.data      = formats.fromContract(artist.data, versions.ARTIST_VERSION)
    artist.version   = artist.data.version
    artist.about     = artist.data.about
    artist.website   = artist.data.website
    artist.twitter   = artist.data.twitter
    artist.instagram = artist.data.instagram
    artist.avatar    = imagesStore.fromContract(artist.data.avatar)
    artist.banner    = imagesStore.fromContract(artist.data.banner)
  
    return artist
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