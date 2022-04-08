import {reactive} from 'vue'

export default class GalleryImage {
  constructor () {
    this._state = reactive({
      file: undefined,
      ipfs_hash: undefined,
      mime_type: undefined
    })
  }
}
