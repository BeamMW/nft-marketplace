import {types} from 'stores/consts'

export default class Formats {
  //
  // From javascript types to hex strings that can be stored in contract and back
  //
  static u8arrToHex (arr) {
    if (!(arr instanceof Uint8Array)) {
      throw new Error('u8arrToHex invalid input')
    }

    return arr.reduce((str, byte) => str + byte.toString(16).padStart(2, '0'), '')
  }

  static hexToU8arr(hexstr, len) {
    if (typeof hexstr !== 'string') {
      throw new Error('hexToU8arr invalid input')  
    }
    
    return new Uint8Array(hexstr.match(/.{1,2}/g).map(byte => parseInt(byte, 16)))
  }

  static byteToHex(byte) {
    if (typeof byte !== 'number') {
      throw new Error('byteToHex invalid input')
    }

    let u8arr = new Uint8Array([byte])
    return Formats.u8arrToHex(u8arr)
  }

  static hexToByte(hexstr) {
    if (typeof hexstr !== 'string') {
      throw new Error('hexToByte invalid input')  
    }

    let u8arr = Formats.hexToU8arr(hexstr)
    return u8arr[0]
  }

  static numberToHex(number) {
    if (typeof number !== 'number') {
      throw new Error('numberToHex invalid input')
    }
    
    let u8arr = new Uint8Array(new Uint32Array([number]).buffer)
    return Formats.u8arrToHex(u8arr)
  }

  static hexToNumber(hexstr) {  
    if (typeof hexstr !== 'string') {
      throw new Error('hexToNumber invalid input')  
    }

    let u8arr = Formats.hexToU8arr(hexstr)
    return new Uint32Array(u8arr.buffer)[0]
  }

  static stringToHex(str) {
    if (typeof str !== 'string') {
      throw new Error('stringToHex invalid input')
    }
    
    let u8arr  = (new TextEncoder()).encode(str)
    return Formats.u8arrToHex(u8arr)
  }

  static hexToString(hexstr) {
    if (typeof hexstr !== 'string') {
      throw new Error('hexToNumber invalid input')  
    }

    let u8arr = Formats.hexToU8arr(hexstr)
    return (new TextDecoder()).decode(u8arr)
  }

  //
  // Convert multiple values to hex string 
  // [type: byte in hex (2 chars)][data_size: uint32 in hex: (8 chars)][data value: data_size chars]
  //
  static toContract(...values) {
    let result = ''

    for (let value of values) {
      if (typeof value === 'number') {
        let hex = Formats.numberToHex(value) 
        result += Formats.byteToHex(types.NUMBER)
        result += Formats.numberToHex(hex.length)
        result += hex
        continue
      }

      if (typeof value === 'string') {
        let hex = Formats.stringToHex(value) 
        result += Formats.byteToHex(types.STRING)
        result += Formats.numberToHex(hex.length)
        result += hex
        continue
      }

      if (typeof value == 'object') {
        let hex = Formats.stringToHex(JSON.stringify(value)) 
        result += Formats.byteToHex(types.JSON)
        result += Formats.numberToHex(hex.length)
        result += hex
        continue
      }

      throw new Error('Invalid type of value in Formats.toContract: ' + typeof value)
    }

    return result
  }

  static __extractType(hexstr, idx) {
    let typeLen   = 2
    let htype     = hexstr.substr(idx, typeLen)
    let type      = Formats.hexToByte(htype)
    
    if (type < types.MIN || type > types.MAX) {
      throw new Error('Invalid type in Formats.__extractType: ' + type)
    }
    
    return {type, idx: idx + typeLen}
  }

  static __extractSize(hexstr, idx) {
    let sizeLen = 8
    let hsize   = hexstr.substr(idx, sizeLen)
    let size    = Formats.hexToNumber(hsize)

    if (size < 2 || idx + sizeLen + size > hexstr.length) {
      throw new Error('Invalid data size in Formats.__extractSize: ' + size)
    }

    return {size, idx: idx + sizeLen}
  }

  static __extractData(hexstr, idx, size) {
    let data = hexstr.substr(idx, size)
    return {data, idx: idx + size}
  }

  //
  // Convert hex string from contract to multiple values
  //
  static fromContract(hexstr) {
    let result = []
    let idx = 0

    while(idx < hexstr.length) {
      let type, size, data
      
      ({type, idx} = Formats.__extractType(hexstr, idx));
      ({size, idx} = Formats.__extractSize(hexstr, idx));
      ({data, idx} = Formats.__extractData(hexstr, idx, size))

      if (type == types.NUMBER) {
        result.push(Formats.hexToNumber(data))
        continue
      }

      if (type == types.STRING) {
        result.push(Formats.hexToString(data))
        continue
      }

      if (type == types.JSON) {
        result.push(JSON.parse(Formats.hexToString(data)))
        continue
      }

      throw new Error('Unknown data type in Formats.fromContract: ' + type)
    }

    if (idx != hexstr.length) {
      throw new Error('Idx is out of bounds in Formats.fromContract')
    }

    return result
  }
}
