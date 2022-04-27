import {common} from './consts'

export default class Validators {
  static twitter(value) {
    return /^[@][a-zA-Z0-9_]{1,15}$/.test(value)
  }

  static instagram(value) {
    return (/^(?!.*[..]{2})[@][a-zA-Z0-9_.]{1,30}$/.test(value))
  }

  static url(value) {
    let url 
    try {
      url = new URL(value)
    }
    catch(_) {
      return false
    }
    return value.length <= 250 && 
         (url.protocol === 'http:' || url.protocol === 'https:') &&
         (url.toString() === value || url.toString() === value + '/')
  }

  static image (image) {
    if (!image) return false

    if (image.file) {
      return image.file.size <= common.MAX_IMAGE_SIZE
    }

    return image.ipfs_hash
  }
}
