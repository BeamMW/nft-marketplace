import formats from 'stores/formats'
import utils from 'utils/utils'
import imagesStore from 'stores/images'
import {versions, cid} from 'stores/consts'
import {router} from 'router'
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
    // _loadArtist trhows on error, for self we want to fail
    this._is_artist = !!await this._loadArtist(this.my_id)
  }

  async _loadTotals () {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_artists_stats',
      cid
    })
    this._total = res.artists_stats.total
  }

  async _loadArtist (id) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_artists',
      ids: id,
      cid
    })

    utils.ensureField(res, 'artists', 'array')
    if (res.artists.length != 1) return undefined

    // TODO: throw and ensure reload ?
    let artist = this._fromContract(res.artists[0])
    let oldArtist = this.artists[artist.id]
    this.artists[artist.id] = artist

    if (oldArtist) {
      if (!imagesStore.copyImage(this.artists[artist.id].banner, oldArtist.banner)) {
        imagesStore.loadImageAsync(this.artists[artist.id].banner)
      }
      if (!imagesStore.copyImage(this.artists[artist.id].avatar, oldArtist.avatar)) {
        imagesStore.loadImageAsync(this.artists[artist.id].avatar)
      }
    }

    return this.artists[artist.id]
  }

  async loadAsync() {
    await this._loadKey()
    await this._loadSelf()
    await this._loadTotals()
    
    /*
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_artists',
      cid
    })

    // TODO: remove test code --> filter
    utils.ensureField(res, 'artists', 'array')
    let artists = res.artists.filter(artist => artist.id !== '5990c629ab78082a0e18a92b897468c7b04cf3c0a39314d951dfc31331ab6bc000') 
      .map(artist => this.__fromContract(artist))

    // TODO: move to admin                          
    artists.sort((a,b) => a.label > b.label ? 1 : -1)

    // TODO: keep only id
    if (!state.selected_artist && artists.length) {  
      state.selected_artist = {
        id: artists[0].id,
        label: artists[0].label
      }
    }
    
    for (let artist of artists) {
      if (this.my_id === artist.id) {
        this._is_artist = true
      }

      let oldArtist = this.artists[artist.id]
      this.artists[artist.id] = artist

      if (oldArtist) {
        if (!imagesStore.copyImage(this.artists[artist.id].banner, oldArtist.banner)) {
          imagesStore.loadImageAsync(this.artists[artist.id].banner)
        }
        if (!imagesStore.copyImage(this.artists[artist.id].avatar, oldArtist.avatar)) {
          imagesStore.loadImageAsync(this.artists[artist.id].avatar)
        }
      }
    }
    */
  }

  async setArtist(label, data, global) {
    if(data.avatar instanceof File) {
      data.avatar = await imagesStore.uploadImageAsync(data.avatar)  
    }

    if (data.banner instanceof File) {
      data.banner = await imagesStore.uploadImageAsync(data.banner)
    }

    ({label, data} = artistsStore.toContract(label, data))
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
        global.setError(err)
      }
    }, 1000)
  }

  toContract (label, data) {
    return {
      label: formats.toContract(label),
      data: formats.toContract(versions.ARTIST_VERSION, data)
    }
  } 

  // TODO: do not fail app on this?
  _fromContract (cartist) {
    let artist = Object.assign({}, cartist)

    if (artist.label == '31') 
    {
      // TODO:remove test code
      artist.label = '1'
      artist.data = {}
    } 
    else 
    {
      let [label] = formats.fromContract(artist.label)
      let [version, data] = formats.fromContract(artist.data)

      if (version != versions.ARTIST_VERSION) {
        throw new Error('Artist version mismatch: ' + version + ' != ' + versions.ARTIST_VERSION)
      }

      artist.label = label
      Object.assign(artist, data)
    }
  
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