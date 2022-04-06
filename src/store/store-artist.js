import formats from './store-formats.js'
import {versions} from './store-consts.js'
import {router} from '../router.js'
import utils from '../utils/utils.js'
import StoreImage from './store-image'

export default class StoreArtists {
  static defaultState () {
    return {
      my_artist_key: '',
      is_artist: false,
      set_artist_tx: '',
      artists: {},
    }
  }

  static storeMethdos () {
    return {
      toBecomeArtist() {
        if (this.state.is_artist) {
          throw new Error('Unexpected toBecomeArtist, already an artist')
        }
        router.push({
          name: 'artist'
        })
      },

      async loadArtistsAsync() {
        let {res} = await utils.invokeContractAsync({
          role: 'manager',
          action: 'view_artists',
          cid: this.state.cid
        })

        // TODO: remove test code --> filter
        utils.ensureField(res, 'artists', 'array')
        let artists = res.artists.map(artist => StoreArtists.fromContract(artist))
          .filter(artist => artist.id === '27db54db176900d0bf933490b9ecf5c784ddd39c0af5112d2ce60bdc084336f701') 

        // TODO: move to admin                          
        artists.sort((a,b) => a.label > b.label ? 1 : -1)

        // TODO: keep only id
        if (!this.state.selected_artist && artists.length) {  
          this.state.selected_artist = {
            id: artists[0].id,
            label: artists[0].label
          }
        }
        
        for (let artist of artists) {
          if (this.state.my_artist_key === artist.id) {
            this.state.is_artist = true
          }
    
          let oldArtist = this.state.artists[artist.id]
          this.state.artists[artist.id] = artist
    
          if (oldArtist) {
            if (!StoreImage.copyImage(this.state.artists[artist.id].banner, oldArtist.banner)) {
              StoreImage.loadImageAsync(this.state.artists[artist.id].banner)
            }
            if (!StoreImage.copyImage(this.state.artists[artist.id].avatar, oldArtist.avatar)) {
              StoreImage.loadImageAsync(this.state.artists[artist.id].avatar)
            }
          }
        }
      }

    }
  }

  static toContract (label, data) {
    return {
      // TODO: remove test code
      label: label == '1' ? label : formats.toContract(label),
      data: formats.toContract(versions.ARTIST_VERSION, data)
    }
  } 

  // TODO: do not fail app on this?
  static fromContract (cartist) {
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
}
