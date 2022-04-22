import utils from 'utils/utils'
import {reactive} from 'vue'

class ImagesStore {
  constructor() {
    this.reset()
  }

  reset () {
    this._state = reactive({
      images: {}
    })
  }

  fromContract(image) {
    if (!image) {
      return image
    }

    try {
      utils.ensureField(image, 'ipfs_hash', 'string')
      utils.ensureField(image, 'mime_type', 'string')
    }
    catch(err) {
      console.log('ImagesStore.from_contract', err)
      this._setError(image, err)
      return image
    }

    let cached = this._state.images[image.ipfs_hash]
    if (cached) {
      return cached
    }

    this._state.images[image.ipfs_hash] = image
    this._setLoading(image)
    this._ipfsLoad(image)

    return this._state.images[image.ipfs_hash]
  }

  _setImage(image) {
    let old = this._state.images[image.ipfs_hash]
    
    if (old) {
      utils.clearAssign(this._state.images[image.ipfs_hash], image)
    }

    if (!old) {
      this._state.images[image.ipfs_hash] = image
    }

    return this._state.images[image.ipfs_hash]
  }

  async _fread(file) {
    return new Promise((resolve, reject) => {
      let reader = new FileReader()
      reader.onload = () => {
        resolve(reader.result)
      }
      reader.onerror = reject
      reader.readAsArrayBuffer(file)
    })
  }

  async _setLoading(image) {
    delete image.error
    image.loading = true
  }

  async _setError(image, err) {
    delete image.loading
    image.error = err
  }

  async _clearLR(image) {
    delete image.loading
    delete image.error
  }

  async _ipfsLoad(image) {
    try {
      this._setLoading(image)
      
      let {res} = await utils.callApiAsync('ipfs_get', {hash: image.ipfs_hash})
      utils.ensureField(res, 'data', 'array')
      
      // TODO: can skip u8arr here?
      let u8arr = new Uint8Array(res.data)
      let blob = new Blob([u8arr], {type: image.mime_type})
      // TODO: revoke object
      // TODO: keep only N images & delete and release old ones
      image.object = URL.createObjectURL(blob, {oneTimeOnly: false})

      this._clearLR(image)
      return image
    }
    catch(err) {
      console.log(`ImagesStore._ipfsLoad for hash ${image.ipfs_hash}`, err)
      this._setError(image, err)
    }

    return image
  }

  async _ipfsStore (image) {
    let buffer = await this._fread(image.file)
    let u8arr = new Uint8Array(buffer)
    let array = Array.from(u8arr)
    let {res} = await utils.callApiAsync('ipfs_add', {data: array})

    let blob = new Blob([u8arr], {type: image.file.type})
    let object =  URL.createObjectURL(blob, {oneTimeOnly: false})
    
    image = {
      ipfs_hash: res.hash,
      mime_type: image.file.type,
      object: object
    }

    this._setImage(image)
    return image
  }

  async toContract(image) {
    if (!image) {
      return
    }

    if (image.file) {
      image = await this._ipfsStore(image)
    }

    return {
      ipfs_hash: image.ipfs_hash,
      mime_type: image.mime_type
    }
  }
}

let imagesStore = new ImagesStore()
export default imagesStore