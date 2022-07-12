import LazyLoader from 'stores/lazy-loader'

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
}

let likesStore = new LikesStore()
export default likesStore
