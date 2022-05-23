export default class Formats {
  static toContract(data, version) {
    let escape = (str) => {
      return str.replace(/"/g, '\\"')
    }

    if (typeof data == 'string') {
      if (version) {
        throw new Error('Formats.toContract version for plain string is not supported')
      }

      let escaped  = escape(data)
      return `"${escaped}"`
    }

    if (typeof data == 'object') {
      if (version) {
        data = Object.assign({version}, data)
      }

      let escaped  = escape(JSON.stringify(data))
      return `"${escaped}"`
    }

    throw new Error('Formats.toContract invalid data type: ' + typeof data)
  }

  static fromContract(data, versions) {
    if (versions) {
      let parsed = JSON.parse(data)

      if (typeof parsed !== 'object') { 
        throw new Error('Formats.fromContract invalid parsed data type: ' + typeof parsed)
      }

      if (typeof versions === 'number') {
        if (versions !== parsed.version) {
          throw new Error('Version mismatch in Formats.fromContract for: ' + data) 
        }
      }
      else if (!versions.includes(parsed.version)) {
        throw new Error('Version mismatch in Formats.fromContract for: ' + data) 
      }

      return parsed
    }

    if (typeof(data) !== 'string') {
      throw new Error('Formats.fromContract non-string input without version')
    }

    return data
  }
}
