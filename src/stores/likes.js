import LazyLoader from 'stores/lazy-loader'
import utils from 'utils/utils'
import {cid} from 'stores/consts'

class LikesStore extends LazyLoader {
  constructor () {
    super({objname: 'like'})
  }

  fromDB (like) {
    return like
  }

  fromContract (like) {
    return like
  }

  async getLikes(ids) {
    return await this._db[this._store_name].bulkGet(ids)
  }

  async _req_AC_LOAD(id0, count) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: `view_${this._objname}s`,
      artist_id: this._global.state.my_key,
      id0,
      count,
      cid
    })
    utils.ensureField(res, 'items', 'array')
    utils.ensureField(res, 'next_id')
    return res
  }

  async _req_AC_UPDATE(h0, count) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: `view_${this._objname}s`,
      artist_id: this._global.state.my_key,
      h0,
      count,
      cid
    })
    utils.ensureField(res, 'items', 'array')
    return res
  }
}

let likesStore = new LikesStore()
export default likesStore
