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
}
