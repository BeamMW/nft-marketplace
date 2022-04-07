import {reactive} from 'vue'

class StoreCollections {
  constructor () {
    this.reset()
  }

  reset () {
    this._state = reactive({
      total: 0
    })
  }

  get total() {
    return this._state.total
  }

  set _total (value) {
    this._state.total = value
  }

  async loadAsync() {
  }
}

let collectionsStore = new StoreCollections()
export default collectionsStore