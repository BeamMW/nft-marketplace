import utils from 'utils/utils.js'

export default class StoreImage {
  static copyImage(newimg, oldimg) {
    if (!newimg || !oldimg) {
      return false
    }

    if (oldimg.ipfs_hash != newimg.ipfs_hash) {
      return false
    }

    newimg.object = oldimg.object
    return !!newimg.object
  }

  // TODO: check that this doesn't block
  static async loadImageAsync (image, context) {
    if (!image) {
      return
    } 

    try {
      let {res} = await utils.callApiAsync('ipfs_get', {hash: image.ipfs_hash})
      utils.ensureField(res, 'data', 'array')
      
      // TODO: can skip u8arr here?
      let u8arr = new Uint8Array(res.data)
      let blob = new Blob([u8arr], {type: image.mime_type})
      // TODO: revoke object
      console.log('URLOBJ for', image.ipfs_hash)
      image.object = URL.createObjectURL(blob, {oneTimeOnly: false})
      
      return image
    }
    catch(err) {
      if (err) {
        context = context || 'loading image from IPFS, hash ' + image.ipfs_hash
        image.error = {err, context}
      }
    }
  }

  static async readFileAsync(file) {
    return new Promise((resolve, reject) => {
      let reader = new FileReader()
      reader.onload = () => {
        resolve(reader.result)
      }
      reader.onerror = reject
      reader.readAsArrayBuffer(file)
    })
  }

  static async uploadImageAsync (file) {
    let buffer = await StoreImage.readFileAsync(file)
    let array = Array.from(new Uint8Array(buffer))
    let {res} = await utils.callApiAsync('ipfs_add', {data: array})
    return {
      ipfs_hash: res.hash,
      mime_type: file.type
    }
  }
}
