import utils from 'utils/utils'
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
    return !Validators.image_error(image)
  }

  static image_error(image, minw, minh) {
    if (!image) return 'Image cannot be empty'

    if (image.error) {
      return image.error.toString()
    }

    if (image.file) {
      if(image.file.size >= common.MAX_IMAGE_SIZE) {
        return `Image size must be less than ${utils.formatBytes(common.MAX_IMAGE_SIZE)}`
      }
    }

    if (minw && minh) {
      if (!image.object) {
        return 'Image is not loaded'
      }
      
      console.log(minw, minh, image.object)
      if (image.object.width < minw || image.object.height < minh) {
        return `Image size must be at least ${minw}x${minh}px`
      }
    } 

    if (!image.file && !image.object) {
      return 'Image doesn\'t contain data'
    }

    return ''
  }
}
