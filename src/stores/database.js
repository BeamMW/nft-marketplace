import {versions} from 'stores/consts'
import {Dexie} from 'dexie'

export default class Database extends Dexie {
  constructor(cid, key) {
    // Database storage is per-gallery so we generate
    // DB name as 'gallery' + db version + key + cid
    // This allows to switch cids and wallets (wallet.db)
    // Inside a single Wallet GUI 
    super(['gallery', versions.DATABASE_VERSION, key, cid].join('-'))
  }

  async initAsync (stores) {     
    Object.assign(stores, {
      'meta': 'name'
    })
    this.version(versions.DATABASE_VERSION).stores(stores)
    await this.open()
  }
}
