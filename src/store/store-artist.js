import formats from './store-formats.js'
import {versions} from './store-consts.js'

export default class {
  static toContract (label, data) {
    //alert('write label: ' + formats.toContract(label))
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
      //alert('read label: ' + artist.label)
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
