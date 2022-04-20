import formats from 'stores/formats'
import utils from 'utils/utils'
import imagesStore from 'stores/images'
import {versions, cid} from 'stores/consts'
import router from 'router'
import {reactive, computed} from 'vue'

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
      total: 0
    })
  }

  get my_id() {
    return this._state.my_id
  }

  set _my_id (val) {
    this._state.my_id = val
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

  set _is_artist(value) {
    this._state.is_artist = value
  }

  get artist_tx () {
    return this._state.artist_tx
  }

  set _artist_tx (val) {
    this._state.artist_tx = val
  }

  get artists () {
    return this._state.artists
  }

  set _artists (val) {
    this._state.artists = val
  }

  async _loadKey () {
    let {res} = await utils.invokeContractAsync({
      role: 'artist',
      action: 'get_id',
      cid
    })

    utils.ensureField(res, 'id', 'string')
    this._my_id = res.id
  }

  async _loadSelf() {
    let self = await this._loadArtistAsync(this.my_id)
    
    if (self === null) {
      this._is_artist = false
      return
    }
    
    // We always fail if unable to load self
    if (self.error) {
      throw self.error
    }

    this._is_artist = true
  }

  async _loadTotals () {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_artists_stats',
      cid
    })
    
    utils.ensureField(res, 'total', 'number')
    this._total = res.total
  }

  async _loadArtistAsync (id) {
    try {
      let {res} = await utils.invokeContractAsync({
        role: 'manager',
        action: 'view_artists',
        ids: id,
        cid
      })

      utils.ensureField(res, 'artists', 'array')
      if (res.artists.length > 1) throw new Error('_loadArtistAsync: artists.length > 1')
      if (res.artists.length == 0) return null

      let artist = this._fromContract(res.artists[0])
      this._state.artists[id] = artist
    } 
    catch (err) {
      console.log(`_loadArtistAsync for id ${id}:`, err)
      this._state.artists[id] = {id, error: err}
    }

    return this._state.artists[id]
  }

  loadArtist(id) {
    if (!id) {
      throw new Error('loadArtist: id is required')
    }

    return computed(() => {
      let cached = this._state.artists[id]
      if (cached) {
        return cached
      }
      this._state.artists[id] = {id, loading: true}
      
      this._state.artists[id] = this._loadArtistAsync(id)
      if (this._state.artists[id] === null) {
        this._state.artists[id] = {id, error: new Error('Artist not found')}
      }

      return this._state.artists[id]
    })
  }

  async loadAsync() {
    await this._loadKey()
    await this._loadSelf()
    await this._loadTotals()
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

    this._artist_tx = txid
    let interval = setInterval(async () => {
      try {
        let {res} = await utils.callApiAsync('tx_status', {txId: this.artist_tx})
        if ([0, 1, 5].indexOf(res.status) == -1) {
          clearInterval(interval)
          this._artist_tx = ''
        }
      }
      catch(err) {
        this._artist_tx = ''
        this.global.setError(err)
      }
    }, 1000)
  }

  async _toContract (label, data) {
    data.avatar = await imagesStore.toContract(data.avatar)
    data.banner = await imagesStore.toContract(data.banner)
    
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.ARTIST_VERSION, data)
    }
  } 

  _fromContract (cartist) {
    let artist = Object.assign({}, cartist)

    if (!artist.label) {
      //throw new Error('ArtistsStore._fromContract : artist label is empty')
    }

    let [label] = formats.fromContract(artist.label)
    let [version, data] = formats.fromContract(artist.data)

    if (version != versions.ARTIST_VERSION) {
      throw new Error('Artist version mismatch: ' + version + ' != ' + versions.ARTIST_VERSION)
    }

    artist.label = label
    artist.version = version
    
    Object.assign(artist, data)
    artist.avatar = imagesStore.fromContract(artist.avatar)
    artist.banner = imagesStore.fromContract(artist.banner)
  
    return artist
  }

  toBecomeArtist() {
    if (this.is_artist) {
      throw new Error('Unexpected toBecomeArtist, already an artist')
    }
    router.push({
      name: 'artist'
    })
  }

  toEditArtist() {
    if (!this.is_artist) {
      throw new Error('Unexpected EditArtist, not an artist yet')
    }
    router.push({
      name: 'artist',
      params: {
        id: this.my_id
      }
    })
  }
}

let artistsStore = new ArtistsStore()
export default artistsStore