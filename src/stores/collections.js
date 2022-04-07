import {reactive} from 'vue'

class CollectionsStore {
  constructor () {
    this.reset()
  }

  reset (global) {
    this._global = global
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

let collectionsStore = new CollectionsStore()
export default collectionsStore