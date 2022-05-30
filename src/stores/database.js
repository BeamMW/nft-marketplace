import {versions} from 'stores/consts'
import {Dexie} from 'dexie'

export default class Database extends Dexie {
  constructor() {
    super(['gallery', versions.DATABASE_VERSION].join('-'))
  }

  async initAsync (stores) {     
    Object.assign(stores, {
      'meta': 'name'
    })
    this.version(versions.DATABASE_VERSION).stores(stores)
    await this.open()
  }
}
