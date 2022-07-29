import utils from 'utils/utils'
import {cid} from 'stores/consts'
import {reactive} from 'vue'

//
// Lazily loads objects  from contract
// Object musst support view_xxx interface
//
const AC_LOAD = 1
const AC_UPDATE = 2

export default class LazyLoader {
  constructor({objname}) {
    this._objname = objname
    this._store_name = `${objname}s`
    this._metastore_name = `${objname}s_meta`
  }

  defaultState() {
    return {
      loading: false,
      updating: false,
    }
  }

  reset(global, db) {
    this._global = global
    this._db = db

    let clean_state = this.defaultState()
    this._state = reactive(clean_state)
    this.my_key = global.state.my_key
  }

  getDBStores() {
    let res = {} 
    res[this._store_name] ='id'
    res[this._metastore_name] = 'name'
    return res
  }

  defaultStatus () {
    return {
      'iprocessed': 0,
      'inext': 0,
      'hprocessed': 0 
    }
  }

  async _loadStatus() {
    let db = this._db 
    let status = this.defaultStatus()

    let names = Object.keys(status)
    let values = await db[this._metastore_name].bulkGet(names)

    for(let idx in names) {
      let name = names[idx]
      let item = values[idx]
      if (item !== undefined) {
        status[name] = item.value
      }
    }

    return status
  }

  async _saveStatus(status) {
    let save = []
    for (let key in status) {
      save.push({name: key, value: status[key]})
    }
    let db = this._db 
    await db[this._metastore_name].bulkPut(save)
  }

  getFirstIDX () {
    return 1
  }

  getZeroIDX() {
    return 0
  }

  async loadAsync (autoContinue) {
    if (this._state.loading) {
      return false
    }

    this._state.loading = true
    return await this._loadAsyncInternal(AC_LOAD, 0, autoContinue)
  }

  async updateAsync (autoContinue) {
    if (this._state.updating) {
      return false
    }

    this._state.updating = true
    return await this._loadAsyncInternal(AC_UPDATE, 0, autoContinue)
  }

  async onStart(status, items) {
  }

  async onItem(status, item) {
  }

  async onEnd(status, items) {
  }

  async _req_AC_LOAD(id0, count) {
    let res = undefined 

    if (!utils.isDesktop()) {
      let request = `${utils.webGateway}view_${this._objname}s?id0=${id0}&count=${count}`
      res = await utils.downloadAsync(request, 'json')
    }

    if (utils.isDesktop()) {
      ({res} = await utils.invokeContractAsync({
        role: 'manager',
        action: `view_${this._objname}s`,
        id0,
        count,
        cid
      }))
    }
    
    utils.ensureField(res, 'items', 'array')
    utils.ensureField(res, 'next_id')
    return res
  }

  async _req_AC_UPDATE(h0, count) {
    let res = undefined 
    
    if (!utils.isDesktop()) {
      let request = `${utils.webGateway}view_${this._objname}s?h0=${h0}&count=${count}`
      res = await utils.downloadAsync(request, 'json')
    }

    if (utils.isDesktop()) {
      ({res} = await utils.invokeContractAsync({
        role: 'manager',
        action: `view_${this._objname}s`,
        h0,
        count,
        cid
      }))
    }

    utils.ensureField(res, 'items', 'array')
    return res
  }

  async _loadAsyncInternal(action, depth, autoContinue) {
    console.log(`_loadAsyncInternal for ${this._objname}s, ${action == AC_LOAD ? 'AC_LOAD' : 'AC_UPDATE'} with depth ${depth}`)
    let status = await this._loadStatus()

    if (action == AC_LOAD) {
      if (status.iprocessed && status.inext == this.getZeroIDX()) {
        // initial loading already completed
        // nothig left to do
        await this.onEnd(status)
        console.log(`_loadAsyncInternal for ${this._objname}s completed`)
        return false
      }

      if (status.hprocessed == 0) {
        // first AC_LOAD, tell to start sync from the current height
        status.hprocessed = this._global.state.height
        console.log(`${this._objname} - AC_LOAD set processed block for update to ${this._global.state.height}`)
        await this._saveStatus(status)
      }
    }

    if (action == AC_UPDATE) {
      if (status.hprocessed === 0) {
        // not yet properly initialized. This can happen if initial AC_LOAD happens
        // a few msecs before new block. Usually should not happen
        console.log(`_loadAsyncInternal for ${this._objname}s ignored/not initialized`)
        await this.onEnd(status)
        return false
      }
    }

    //
    // Items - array of what we need to process
    //
    let items = []
    let res = undefined
    let db = this._db
    let height = this._global.height

    if (action == AC_UPDATE) {
      let hnext = status.hprocessed + 1
      res = await this._req_AC_UPDATE(hnext, 20)
      items = res.items
      if (items === undefined) {
        // eslint-disable-next-line no-debugger
        debugger
      }
      console.log(`loading ${this._objname}s, depth ${depth}, h0 is ${hnext}, got ${items.length} items`)
    }

    if (action == AC_LOAD) {
      let inext = status.iprocessed ? status.inext : this.getFirstIDX()
      res = await this._req_AC_LOAD(inext, 20)
      items = res.items
      if (items === undefined) {
        // eslint-disable-next-line no-debugger
        debugger
      }
      console.log(`loading ${this._objname}s, depth ${depth}, id0 is ${inext}, got ${items.length} items`)
    }

    // TODO: do not load unmoderated items for an average user. Drop db after user becomes a modeator
    // TODO: delete unapproved items for an average user
    // TODO: import headless db when switching to headed
    {
      await this.onStart(status, items)
    
      for(let item of items) {
        await this.onItem(status, item)
        if (action === AC_UPDATE) {
          console.log(`${this._objname} ${item.id} processed`)
          status.hprocessed = Math.max(status.hprocessed, item.updated)
        }
      }
     
      await this.onEnd(status, items)
    }

    if (action === AC_LOAD) {
      status.iprocessed += items.length
      status.inext = res.next_id
    }

    await db.transaction('rw!', db[this._store_name], db[this._metastore_name], async () => {
      await db[this._store_name].bulkPut(items)
      await this._saveStatus(status)
    })

    await this.onEnd(status)

    if (items.length > 0 || (action == AC_UPDATE && height < this._global.state.height)) {
      let further = async () => {return await this._loadAsyncInternal(action, ++depth, autoContinue)}
      if (autoContinue) {
        setTimeout(further, 10)
      }
      return further
    }

    if (action == AC_LOAD) {
      console.log(`Finished loading ${this._objname}s, AC ${action}, with ${status.iprocessed} items`)
      this._state.loading = false
    }
    
    if (action == AC_UPDATE) {
      console.log(`Finished updating ${this._objname}s, AC ${action}, at ${status.hprocessed}`)
      this._state.updating = false
    }

    return false
  }
}