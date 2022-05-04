import {versions} from 'stores/consts'
import {Dexie} from 'dexie'

class Database extends Dexie {
  constructor(...args) {
    super(...args)
  }

  reset (global) {    
    super.close()
    this._global = global
    this._started = false
  }

  async loadAsync(stores) {
    if (this._started) {
      return
    }

    this._started = true
    Object.assign(stores, {
      'meta': 'name'
    })
    
    this.version(versions.DATABASE_VERSION).stores(stores)
    await this.open()
  }

  getDexie() {
    return this._db
  }
}

let databse = new Database('gallery')
export default databse