import {versions} from 'stores/consts'
import {Dexie} from 'dexie'

export default class Database extends Dexie {
  constructor() {
    super('gallery')
  }

  async initAsync (stores) {     
    Object.assign(stores, {
      'meta': 'name'
    })
    this.version(versions.DATABASE_VERSION).stores(stores)
    await this.open()
  }
}
