import utils from '../utils/utils.js'

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
}
