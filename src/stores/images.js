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
      return this._setError(image, err)
    }

    let cached = this._state.images[image.ipfs_hash]
    if (cached) {
      return cached
    }

    this._ipfsLoad(image)
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

  _setLoading(image) {
    return this._setImage({
      ipfs_hash: image.ipfs_hash,
      mime_type: image.mime_type,
      loading: true
    })
  }

  _setError(image, error) {
    return this._setImage({
      ipfs_hash: image.ipfs_hash,
      mime_type: image.mime_type,
      error
    })
  }

  async _ipfsLoad(what) {
    try {
      console.log('load image', what.ipfs_hash)
      this._setLoading(what)
  
      let {res} = await utils.callApiAsync('ipfs_get', {hash: what.ipfs_hash})
      utils.ensureField(res, 'data', 'array')
      
      // TODO: can skip u8arr here?
      let u8arr = new Uint8Array(res.data)
      let blob = new Blob([u8arr], {type: what.mime_type})
      // TODO: revoke object
      // TODO: keep only N images & delete and release old ones
      let object = URL.createObjectURL(blob, {oneTimeOnly: false})

      //this._clearLR(image)
      return this._setImage({
        ipfs_hash: what.ipfs_hash, 
        mime_type: what.mime_type,
        object
      })
    }
    catch(err) {
      console.log(`ImagesStore._ipfsLoad for hash ${what.ipfs_hash}`, err)
      return this._setError(what, err)
    }
  }

  async _ipfsStore (image) {
    let buffer = await this._fread(image.file)
    let u8arr = new Uint8Array(buffer)
    let array = Array.from(u8arr)
    let {res} = await utils.callApiAsync('ipfs_add', {data: array})

    let blob = new Blob([u8arr], {type: image.file.type})
    let object =  URL.createObjectURL(blob, {oneTimeOnly: false})
    
    return this._setImage({
      ipfs_hash: res.hash,
      mime_type: image.file.type,
      object: object
    })
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