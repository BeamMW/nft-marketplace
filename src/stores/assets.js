import utils from 'utils/utils'
import {reactive} from 'vue'

//
// Confidential asset metadata cache: resolves an aid into something
// displayable. Beam assets are always denominated in groths (8 decimals),
// same as BEAM, so there is no per-asset decimals handling here.
//
const BEAM_ASSET = {
  aid: 0,
  name: 'BEAM',
  unit_name: 'BEAM',
  known: true
}

class AssetsStore {
  constructor () {
    this.reset()
  }

  reset () {
    this._state = reactive({
      assets: {0: BEAM_ASSET}
    })
    this._pending = {}
  }

  get assets () {
    return this._state.assets
  }

  // Assets to offer in the picker: BEAM, anything resolved, and anything seen
  // on a price in this gallery.
  get known () {
    let list = Object.values(this._state.assets)
    list.sort((a, b) => a.aid - b.aid)
    return list
  }

  noteAid (aid) {
    aid = Number(aid || 0)
    if (!this._state.assets[aid]) {
      this.loadAsync(aid)
    }
  }

  // Synchronous accessor for templates: returns a placeholder and starts a
  // load, so bindings resolve on the next tick.
  get (aid) {
    aid = Number(aid || 0)

    if (this._state.assets[aid]) {
      return this._state.assets[aid]
    }

    this.loadAsync(aid)

    return {
      aid,
      name: `Asset #${aid}`,
      unit_name: `Asset #${aid}`,
      known: false
    }
  }

  name (aid) {
    return this.get(aid).unit_name
  }

  async loadAsync (aid) {
    aid = Number(aid || 0)

    if (this._state.assets[aid]) {
      return this._state.assets[aid]
    }

    // de-duplicate concurrent lookups for the same asset
    if (this._pending[aid]) {
      return this._pending[aid]
    }

    this._pending[aid] = (async () => {
      try {
        let {res} = await utils.callApiAsync('get_asset_info', {asset_id: aid})
        let asset = AssetsStore.fromApi(aid, res)
        this._state.assets[aid] = asset
        return asset
      }
      catch (err) {
        // get_asset_info throws when the asset is not in the local wallet db;
        // cache a placeholder so re-renders do not hammer the api
        console.log(`AssetsStore failed to load asset ${aid}`, err)
        let asset = {
          aid,
          name: `Asset #${aid}`,
          unit_name: `Asset #${aid}`,
          known: false
        }
        this._state.assets[aid] = asset
        return asset
      }
      finally {
        delete this._pending[aid]
      }
    })()

    return this._pending[aid]
  }

  //
  // Standard CA metadata is "STD:SCH_VER=1;N=Name;SN=Short;UN=Unit;NTHUN=..."
  // The wallet already parses it into metadata_pairs for us on api >= 6.1,
  // otherwise we parse the raw string ourselves.
  //
  static fromApi (aid, res) {
    let pairs = (res || {}).metadata_pairs || AssetsStore.parseMetadata((res || {}).metadata)
    let name = pairs.N || pairs.SN || `Asset #${aid}`
    let unit = pairs.UN || pairs.SN || name

    return {
      aid,
      name,
      unit_name: unit,
      known: true
    }
  }

  static parseMetadata (meta) {
    let pairs = {}

    if (typeof meta !== 'string') {
      return pairs
    }

    for (let chunk of meta.split(';')) {
      let eq = chunk.indexOf('=')
      if (eq === -1) continue
      let key = chunk.substring(0, eq).trim()
      // strip the "STD:" prefix that leads the first pair
      let colon = key.lastIndexOf(':')
      if (colon !== -1) key = key.substring(colon + 1)
      pairs[key] = chunk.substring(eq + 1).trim()
    }

    return pairs
  }
}

let assetsStore = new AssetsStore()
export default assetsStore
