
import ArtistsCommon from 'stores/artists-common'
import formats from 'stores/formats'
import utils from 'utils/utils'
import router from 'router'
import ErrorEx from 'utils/errorex'
import {cid, versions} from 'stores/consts'
import {reactive} from 'vue'

// TODO: direct artists store works slow in browser
//      use lazy store where possible or remove at all
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

  get my_key() {
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

    let self = await this._loadArtistAsync(this._state.my_id)
    if (self === null) {
      this._state.is_artist = false
      return
    }
    
    // We always fail if unable to load self
    if (self.error) {
      throw new ErrorEx('Failed to load self', self.error)
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

  // TODO: check if we need fail flag
  async _loadArtistAsync (id) {
    try {
      let {res} = await utils.invokeContractAsync({
        role: 'manager',
        action: 'view_artists',
        ids: id,
        cid
      })

      console.log('_loadArtistAsync', res)
      utils.ensureField(res, 'items', 'array')

      if (res.items.length > 1) {
        throw new Error('artists.length > 1')
      }

      if (res.items.length == 0) {
        console.log(`artist ${id} not found`)
        delete this._state.artists[id]
        return null
      }

      let item = res.items[0]
      
      if (!item.label) {
        throw new Error('label cannot be empty')
      }
      
      if (!item.data) {
        throw new Error('empty data on item') 
      }

      item.label = formats.fromContract(item.label)
      item.data = formats.fromContract(item.data, versions.ARTIST_VERSION)
      item = this.fromContract(item)
      item = this.fromDB(item)

      item.approved = (item.status === 'approved') ? 1 : 0 
      item.pending  = (item.status === 'pending') ? 1 : 0
      item.rejected = (!item.approved && !item.pending) ? 1 : 0
      
      this._setArtist(id, item)

      if (this._global.debug) {
        console.log('item loaded with label', item.label)
      }
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
    ({label, data} = await this.toContract(label, data))
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
      let {res} = await utils.callApiAsync('tx_status', {txId: this.artist_tx})
      if ([0, 1, 5].indexOf(res.status) == -1) {
        clearInterval(interval)
        this._state.artist_tx = ''
      }
    }, 1000)

    return txid
  }

  async toContract (label, data) {
    return await ArtistsCommon.toContract(label, data)
  } 

  fromContract (artist) {
    return ArtistsCommon.fromContract(artist)
  }

  fromDB(artist) {
    artist = ArtistsCommon.fromDB(artist)
    artist.store = this
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