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

  async onItem(status, item) {
  }
}

let likesStore = new LikesStore()
export default likesStore
