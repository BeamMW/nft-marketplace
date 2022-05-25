import {common} from 'utils/consts'

export default class Validators {
  static twitter_allowed () {
    return /^[a-zA-Z0-9_]{1,15}$/
  }

  static instagram_allowed () {
    return /^[a-zA-Z0-9_.]{1,30}$/
  }

  static twitter(value) {
    return Validators.twitter_allowed().test(value)
  }

  static instagram(value) {
    return Validators.instagram_allowed().test(value)
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
