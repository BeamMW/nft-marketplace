import utils from 'utils/utils'
import {reactive} from 'vue'

class ImagesStore {
  constructor() {
    this.reset()
  }

  reset () {
    this._cache_size = 100
    this._state = reactive({
      images: {},
      track: []
    })
  }

  _ensureFields(image) {
    try {
      utils.ensureField(image, 'ipfs_hash', 'string')
      utils.ensureField(image, 'mime_type', 'string')
    }
    catch(err) {
      console.log('ImagesStore._ensureFields', err)
      return err
    }
    return null
  }

  fromContract(image) {
    if (!image) {
      return image
    }

    let err = this._ensureFields(image)

    if (err)
      alert('image error, id ' + image.id + ', err ' + err)
      
    return {
      ipfs_hash: image.ipfs_hash,
      mime_type: image.mime_type,
      error: err
    }
  }

  fromDB(image) {
    if (!image || image.error) {
      return image
    }

    let err = this._ensureFields(image)
    if (err) {
      return this._setError(image, err)
    }
    
    let cached = this._state.images[image.ipfs_hash]
    if (cached) {
      return cached
    }

    this._loadBytes(image)
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
      if (old.object) URL.revokeObjectURL(old.object)
      utils.clearAssign(this._state.images[image.ipfs_hash], image)
    }

    if (!old) {
      this._state.images[image.ipfs_hash] = image
      this._state.track.push(image.ipfs_hash)
      while(this._state.track.length > this._cache_size) {
        let hash = this._state.track.shift()
        let clear = this._state.images[hash]
        if (clear.object) URL.revokeObjectURL(image.object)
        delete this._state.images[hash]
      }
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

  async _loadBytes(what) {
    try {
      this._setLoading(what)

      let data = undefined
      
      if (utils.isDesktop()) {
        let {res} = await utils.callApiAsync('ipfs_get', {hash: what.ipfs_hash})
        utils.ensureField(res, 'data', 'array')  
        data = res.data
      }
      else {
        let url = [utils.ipfsGateway, what.ipfs_hash].join('')
        data = await utils.downloadAsync(url)
      }

      let u8arr = new Uint8Array(data)
      let blob = new Blob([u8arr], {type: what.mime_type})
      let object = URL.createObjectURL(blob, {oneTimeOnly: false})

      return this._setImage({
        ipfs_hash: what.ipfs_hash, 
        mime_type: what.mime_type,
        object
      })
    }
    catch (err) {
      console.log(`ImagesStore._loadBytes failed for hash ${what.ipfs_hash}`, err)
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

  async loadAsync() {
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