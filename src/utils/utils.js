import wallet, {WalletEnvironment} from 'core/wallet'
import network from 'core/network'

const MIN_AMOUNT = 0.00000001
const MAX_AMOUNT = 254000000

let BEAM         = null
let APIResCB     = undefined
let ipfsGateway  = network.ipfsGateway
let webGateway   = network.webGateway
let InitParams   = undefined

export default class Utils {
  static is_desktop = undefined
  static is_mobile  = undefined
  static is_android = undefined
  static is_web     = undefined
  static is_chrome  = undefined

  static get ipfsGateway() {
    return ipfsGateway
  }

  static get webGateway() {
    return webGateway
  }

  static isMobile () {
    if (Utils.is_mobile === undefined) {
      const ua = navigator.userAgent
      Utils.is_mobile  = (/android/i.test(ua) || /iPad|iPhone|iPod/.test(ua))
    }
    return Utils.is_mobile
  }

  static isCompact () {
    return Utils.isMobile()
  }

  static isDesktop () {
    if (Utils.is_desktop === undefined) {
      const ua = navigator.userAgent
      Utils.is_desktop = (/QtWebEngine/i.test(ua))
    }
    return Utils.is_desktop
  }

  static isWeb () {
    if (Utils.is_web === undefined) {
      Utils.is_web = (!Utils.isDesktop() && !Utils.isMobile())
    }
    return Utils.is_web
  }
    
  static isAndroid () {
    if (Utils.is_android === undefined) {
      const ua = navigator.userAgent
      Utils.is_android = (/android/i.test(ua))
    }
    return Utils.is_android
  }

  static isChrome () {
    if (Utils.is_chrome === undefined) {
      const ua = navigator.userAgent
      Utils.is_chrome = (/chrome|chromium|crios/i.test(ua) && ua.indexOf('Edg') == -1)
    }
    return Utils.is_chrome
  }

  static isHeadless () {
    return wallet.getEnvironment() === WalletEnvironment.HEADLESS
  }

  static get wallet () {
    return wallet
  }

  static async stopHeadlessWallet() {
    return wallet.stopHeadlessWallet()
  }

  static async switchToWebAPI () {
    if (!Utils.isHeadless()) {
      throw new Error('Wallet must be opened in a headless mode')
    }

    let apiver    = InitParams['api_version'] || 'current'
    let apivermin = InitParams['min_api_version'] || ''
    let appname   = InitParams['appname']

    const newAPI = await new Promise((resolve) => {
      const listener = async (ev) => {
        if (ev.data === 'apiInjected') {
          Utils.hideLoading()
          resolve(window.BeamApi)
        }

        if (ev.data === 'rejected') {
          // TODO
        }
      }

      // TODO: add some delay before showing connecting message
      //       if extension is installed and app is allowed it would flick
      window.addEventListener('message', listener, false)
      Utils.showLoading({
        headless: true,
        connecting: true,
        onCancel: (res) => {
          Utils.hideLoading()
          window.removeEventListener('message', listener)
          //TODO: add cancel handling in wallet
          window.postMessage({type: 'cancel_beam_api', apiver, apivermin, appname}, window.origin)
          resolve(res)
        },
        onReconnect: () => {
          window.postMessage({type: 'retry_beam_api', apiver, apivermin, appname}, window.origin)
        }
      })
      window.postMessage({type: 'create_beam_api', apiver, apivermin, appname}, window.origin)
    })

    if (newAPI) {
      await wallet.adoptWebApi(newAPI)
      BEAM = wallet.wallet
    }

    return newAPI
  }

  static async callApiAsync(method, params) {    
    let res = await wallet.callApi(method, params)
    return {res, full: res}
  }

  static callApi(method, params, cback) {
    wallet.callApi(method, params).then(
      (res) => cback(null, res, res),
      (err) => cback(err)
    )
  }

  static async invokeContractAsync(args, bytes) {
    return wallet.invokeContract(args, bytes)
  }

  static async invokeContractAsyncAndMakeTx (args) {
    try {
      return await wallet.invokeContractAndMakeTx(args)
    }
    catch(err) {
      if (Utils.isUserCancelled(err)) {
        return undefined
      }
      throw err
    }
  }

  static invokeContract(args, cback, bytes) {
    wallet.invokeContract(args, bytes).then(
      ({res, full, request}) => cback(null, res, full, request),
      (err) => cback(err)
    )
  }

  static async initialize(params, initcback) {
    InitParams = params
    APIResCB = params['apiResultHandler']

    // wallet-pushed events (new block, tx changes) drive per-block refreshes
    wallet.on('apiEvent', (response) => {
      // A response whose call already timed out lands here too. Dropping it
      // avoids a spurious "Unexpected API result".
      let id = String((response || {}).id || '')
      if (!id.startsWith('ev_')) {
        console.log('dropping late/unmatched api response', id)
        return
      }

      if (APIResCB) APIResCB(null, response.result, response)
    })

    wallet.on('error', (err) => {
      if (APIResCB) APIResCB(err)
    })

    try 
    {
      if (Utils.isWeb() && !Utils.isChrome()) {
        Utils.showChromeDownload()
        return false
      }

      if (Utils.isWeb()) {
        Utils.showLoading({
          headless: !!params['headless'],
          connecting: !params['headless']
        })
      }

      await wallet.connect({headless: !!params['headless']})
      BEAM = wallet.wallet

      let styles = Utils.getStyles()
      Utils.applyStyles(styles)
      Utils.hideLoading()

      if (!BEAM) {
        return initcback('Failed to create BEAM API')
      }

      return initcback(null)
    }
    catch (err)
    {
      if (Utils.isMobile()) {
        Utils.showMobileStoresLinks()
        return false
      }
      return initcback(err)
    }
  }

  static getStyles () {
    if (wallet.styles) {
      // TODO: проборосить стили из мобайла и экстеншена
      return wallet.styles
    }

    if (BEAM && BEAM.styles) {
      // TODO: проборосить стили из мобайла и экстеншена
      return BEAM.styles
    }

    return {
      appsGradientOffset: -174,
      appsGradientTop: 56,
      content_main: '#ffffff',
      background_main_top: '#035b8f',
      background_main: '#042548',
      background_popup: '#00446c',
      validator_error: '#ff625c'
    }
  }

  static applyStyles(style) {
    if (Utils.isMobile()) {
      document.body.classList.add('mobile')
    }

    if (Utils.isWeb()) {
      document.body.classList.add('web')
    }

    if (Utils.isCompact()) {
      document.body.classList.add('compact')
    }
  }
    
  //
  // Convenience functions
  //
  static reload () {
    window.location.reload()
  }
    
  static async injectScript(url) {
    return new Promise((resolve, reject) => {
      let js = document.createElement('script')
      js.type = 'text/javascript'
      js.async = true
      js.src = url
      js.onload = () => resolve()
      js.onerror = (err) => reject(err)
      document.getElementsByTagName('head')[0].appendChild(js)
    })
  }

  static hex2rgba = (hex, alpha = 1) => {
    const [r, g, b] = hex.match(/\w\w/g).map(x => parseInt(x, 16))
    return `rgba(${r},${g},${b},${alpha})`
  }

  static getById = (id)  => {
    return document.getElementById(id)
  }
    
  static setText(id, text) {
    Utils.getById(id).innerText = text
  }

  static show(id) {
    Utils.getById(id).classList.remove('hidden')
  }
    
  static hide(id) {
    Utils.getById(id).classList.add('hidden')
  }

  static downloadAsync(url, type) {
    return new Promise((resolve, reject) => {
      var xhr = new XMLHttpRequest()
      xhr.onreadystatechange = () => {
        if (xhr.readyState !== XMLHttpRequest.DONE) {
          return
        }
        
        if (xhr.status === 200) {
          if (type) {
            return resolve(xhr.response)
          }

          let buffer    = xhr.response
          let byteArray = new Uint8Array(buffer)
          let array     = Array.from(byteArray)

          if (array && array.length) {
            return resolve(array)            
          }
            
          return reject(new Error(`Empty data for ${url}`))
        } 
        
        let errMsg = `Code ${xhr.status} for ${url}`
        reject(new Error(errMsg))
      }

      xhr.open('GET', url, true)
      xhr.responseType = type ? type : 'arraybuffer'
      xhr.send(null)
    })
  }

  static handleString(next) {
    let result = true
    // no leading '-' - a negative price reaches the contract as a huge uint
    const regex = new RegExp(/^\d+(\.\d*)?$/g)
    const floatValue = parseFloat(next)
    const afterDot = next.indexOf('.') > 0 ? next.substring(next.indexOf('.') + 1) : '0'
    if ((next && !String(next).match(regex)) ||
            (String(next).length > 1 && String(next)[0] === '0' && next.indexOf('.') < 0) ||
            (parseInt(afterDot, 10) === 0 && afterDot.length > 7) ||
            (afterDot.length > 8) ||
            (floatValue === 0 && next.length > 1 && next[1] !== '.') ||
            (floatValue < 1 && next.length > 10) ||
            (floatValue > 0 && (floatValue < MIN_AMOUNT || floatValue > MAX_AMOUNT))) {
      result = false
    }
    return result
  }

  static showLoading({headless, connecting, onCancel, onReconnect}) {
    const styles = Utils.getStyles()
    Utils.applyStyles(styles)

    const topColor =  [styles.appsGradientOffset, 'px,'].join('')
    const mainColor = [styles.appsGradientTop, 'px,'].join('')

    let bg = document.createElement('div')
    bg.style.width = '100%'
    bg.style.height = '100%'
    bg.style.color = '#fff'
    bg.id = 'dapp-loader'
    bg.style.position = 'absolute'
    if (headless && connecting) {
      bg.style.top = '0'
      bg.style.left = '0'
      bg.style.position = 'fixed'
      bg.addEventListener('click', (ev) => {
        ev.stopPropagation()
        if (ev.target.id === 'dapp-loader') {
          onCancel()
        }
      })
    } else {
      bg.style.backgroundImage = [
        'linear-gradient(to bottom,',
        styles.background_main_top, topColor,
        styles.background_main, mainColor,
        styles.background_main
      ].join(' ')
    }
    let loadContainer = document.createElement('div')
    loadContainer.id = 'dapp-loading'

    loadContainer.style.textAlign = 'center'
    loadContainer.style.margin = '50px auto 0 auto'
    loadContainer.style.width = '585px'
    loadContainer.style.padding = '5%'
    loadContainer.style.borderRadius = '10px'

    let titleElem = null
    let subtitle = null
        
    if (connecting) {
      titleElem = document.createElement('h3')
      titleElem.innerText = 'Connecting to BEAM Web Wallet.' 
      subtitle = document.createElement('p')
      subtitle.innerText = ['To use ', InitParams['appname'], ' you should have BEAM Web Wallet installed and allow connection.'].join('')

      if (headless) {
        loadContainer.style.backgroundColor = 'rgba(3, 91, 133, 0.95)'
        const container = document.getElementById('container')
        if (container) {
          container.style.filter = 'blur(3px)'
        }
      } else {
        loadContainer.style.backgroundColor = 'rgba(255, 255, 255, 0.05)'
      }
    } else {
      loadContainer.style.backgroundColor = 'transparent'

      titleElem = document.createElement('div')
      titleElem.style.fontSize = '25px'
      titleElem.style.fontWeight = '400'
      titleElem.innerText = [InitParams['appname'], 'is loading'].join(' ')
      subtitle = document.createElement('p')
      subtitle.innerText = 'Please wait...'
    }
        
    loadContainer.appendChild(titleElem)
    loadContainer.appendChild(subtitle)

    if (connecting) {
      let reconnectButton = document.createElement('button')
      reconnectButton.innerText = 'Try to connect again'
      reconnectButton.style.height = '44px'
      reconnectButton.style.padding = '13px 30px'
      reconnectButton.style.borderRadius = '50px'
      reconnectButton.style.border = 'none'
      reconnectButton.style.color = '#fff'
      reconnectButton.style.cursor = 'pointer'
      reconnectButton.style.fontWeight = 'bold'
      reconnectButton.style.fontSize = '14px'
      reconnectButton.style.backgroundColor = 'rgba(255, 255, 255, 0.1)'

      reconnectButton.addEventListener('mouseover', () => {
        reconnectButton.style.boxShadow = '0 0 8px white'
      }, false)

      reconnectButton.addEventListener('mouseout', () => {
        reconnectButton.style.boxShadow = 'none'
      }, false)

      reconnectButton.addEventListener('click', onReconnect)

      let installButton = document.createElement('button')
      installButton.innerText = 'Install BEAM Web Wallet'
      installButton.style.height = '44px'
      installButton.style.padding = '13px 30px'
      installButton.style.borderRadius = '50px'
      installButton.style.border = 'none'
      installButton.style.color = '#042548'
      installButton.style.cursor = 'pointer'
      installButton.style.fontWeight = 'bold'
      installButton.style.fontSize = '14px'
      installButton.style.backgroundColor = '#00f6d2'
      installButton.addEventListener('click', () => {
        window.open('https://chrome.google.com/webstore/detail/beam-web-wallet/ilhaljfiglknggcoegeknjghdgampffk', 
          '_blank')
      })

      installButton.addEventListener('mouseover', () => {
        installButton.style.boxShadow = '0 0 8px white'
      }, false)
      installButton.addEventListener('mouseout', () => {
        installButton.style.boxShadow = 'none'
      }, false)
      installButton.style.marginLeft = '30px'
            
      let controlsArea = document.createElement('div')
      controlsArea.style.marginTop = '50px'
            
      loadContainer.appendChild(controlsArea)
      controlsArea.appendChild(reconnectButton)
      controlsArea.appendChild(installButton)
    }

    bg.appendChild(loadContainer)

    document.body.appendChild(bg)
  }

  static showChromeDownload() {
    const styles = Utils.getStyles()
    Utils.applyStyles(styles)
    const topColor =  [styles.appsGradientOffset, 'px,'].join('')
    const mainColor = [styles.appsGradientTop, 'px,'].join('')

    let bg = document.createElement('div')
    bg.style.width = '100%'
    bg.style.height = '100%'
    bg.style.color = '#fff'
    bg.id = 'chrome-download'
    bg.style.position = 'absolute'
    bg.style.textAlign = 'center'
    bg.style.backgroundImage = [
      'linear-gradient(to bottom,',
      styles.background_main_top, topColor,
      styles.background_main, mainColor,
      styles.background_main
    ].join(' ')

    let notSupp = document.createElement('p')
    notSupp.innerText = 'Your browser is not supported'
    notSupp.style.color = '#fff'
    notSupp.style.fontWeight = 'bold'
    notSupp.style.fontSize = '18px'
    notSupp.style.marginTop = '200px'
    let download = document.createElement('p')
    download.innerText = 'Download any Chromium-based browser'
    download.style.cursor = 'pointer'
    download.style.color = '#00f6d2'

    download.addEventListener('click', () => {
      window.open('https://download-chromium.appspot.com/', 
        '_blank')
    })

    bg.appendChild(notSupp)
    bg.appendChild(download)

    document.body.appendChild(bg)
  }

  static showMobileStoresLinks() {
    const styles = Utils.getStyles()
    Utils.applyStyles(styles)
    const topColor =  [styles.appsGradientOffset, 'px,'].join('')
    const mainColor = [styles.appsGradientTop, 'px,'].join('')

    let bg = document.createElement('div')
    bg.style.width = '100%'
    bg.style.height = '100%'
    bg.style.color = '#fff'
    bg.id = 'chrome-download'
    bg.style.position = 'absolute'
    bg.style.textAlign = 'center'
    bg.style.backgroundImage = [
      'linear-gradient(to bottom,',
      styles.background_main_top, topColor,
      styles.background_main, mainColor,
      styles.background_main
    ].join(' ')

    let downloadLink = document.createElement('p')
    downloadLink.innerHTML = `To use ${InitParams.appname}<br>please download BEAM wallet`
    downloadLink.style.marginTop = '100px'
    downloadLink.style.fontSize = '20px'
    downloadLink.style.color = '#00f6d2'
    downloadLink.addEventListener('click', () => {
      Utils.isAndroid() 
        ? window.open('https://play.google.com/store/apps/details?id=com.mw.beam.beamwallet.mainnet', 
          '_blank')
        : window.open('https://apps.apple.com/us/app/beam-privacy-wallet/id1459842353?ls=1', 
          '_blank')
    })

    bg.appendChild(downloadLink)
    document.body.appendChild(bg)
  }

  static hideLoading() {
    const loader = document.getElementById('dapp-loader')
    if (loader) {
      loader.parentNode.removeChild(loader)
    }

    const container = document.getElementById('container')
    if (container) {
      container.style.filter = 'none'
    }
  }

  static formateValue(value) {
    if (value > 0) {
      return parseFloat(value.toFixed(2)).toString()
    } else {
      return value
    }
  }

  static numberWithCommas(x) {
    if (x > 0) {
      return x.replace(/\B(?=(\d{3})+(?!\d))/g, ',')
    } else {
      return x
    }
  }

  /*
  static getRateStr(value, rate) {
    const rateVal = Utils.formateValue(new Big(value).times(rate))
    return (rate > 0 && value > 0
      ? (rateVal > 0.1 ? (Utils.numberWithCommas(rateVal) + ' USD') : '< 1 cent')
      : '0 USD')
  }
  */

  static ensureField(obj, name, type) {
    if (obj[name] == undefined) {
      throw new Error(`No '${name}' field on object`)
    }

    if (type == 'array') {
      if (!Array.isArray(obj[name])) {
        throw new Error(`${name} is expected to be an array`)
      }
      return
    }

    if (type) {
      let tof = typeof obj[name]
      if (tof !== type) {
        throw new Error(`Bad type '${tof}' for '${name}'. '${type}' expected.`)
      }
      return
    }
  }

  static isUserCancelled (err) {
    if (!err) return false
    if (err.error && err.error.code == -32021) return true
    return err.code == -32021
  }

  static formatJSON(obj) {
    let res = JSON.stringify(obj, null, 2)
    return res == '{}' ? obj.toString() : res
  }

  // 306998 -> 306,998; decimals are left alone
  static groupThousands(str) {
    let [int, frac] = String(str).split('.')
    let sign = ''

    if (int.startsWith('-')) {
      sign = '-'
      int = int.substring(1)
    }

    int = int.replace(/\B(?=(\d{3})+(?!\d))/g, ',')
    return frac === undefined ? sign + int : `${sign}${int}.${frac}`
  }

  static formatAmountFixed(amount, fixed) {
    if (amount == 0) return '0'
    let str = (amount / 100000000).toFixed(fixed)
    if (parseFloat(str) == 0) {
      let res = '< 0.'
      for (let i = 0; i < fixed - 1; ++i) {
        res += '0'
      }
      res += '1'
      return res
    }
    // strip trailing zeros
    if (str.indexOf('.') !== -1) {
      str = str.replace(/0+$/, '').replace(/\.$/, '')
    }

    return Utils.groupThousands(str)
  }

  static formatAmount3(amount) {
    amount = amount / 100000000
    const fixedNum = amount.toFixed(3)
  
    if (amount == 0) {
      return '0'
    }
    
    if (amount < 0.01) {
      return '< 0.01'
    }

    if (amount >= 0.01 && fixedNum < 1000) {
      return +amount.toFixed(2)
    }

    if (fixedNum >= 1000 && fixedNum < 1000000) {
      const head = fixedNum.slice(0, -7)
      const tail = fixedNum.slice(-7, -5)
      const n = +(head + '.' + tail)
      return n + ' k'
    }

    if (fixedNum >= 1000000 && fixedNum < 1000000000) {
      const head = fixedNum.slice(0, -10)
      const tail = fixedNum.slice(-10, -8)
      const n = +(head + '.' + tail)
      return n + ' m'
    }

    if (fixedNum >= 1000000000) {
      const head = fixedNum.slice(0, -13)
      const tail = fixedNum.slice(-13, -11)
      const n = +(head + '.' + tail)
      return n + ' b'
    }

    return 'error'
  }

  static formatHeight(height) {
    return new Intl.NumberFormat().format(height)
  }

  static formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 Bytes'

    const k = 1024
    const dm = decimals < 0 ? 0 : decimals
    const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB']
    const i = Math.floor(Math.log(bytes) / Math.log(k))

    return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i]
  }

  static clearAssign(oldobj, newobj) {
    for (let key in oldobj) {
      // eslint-disable-next-line no-prototype-builtins
      if (oldobj.hasOwnProperty(key)) {
        delete oldobj[key]
      }
    }
    for (let key in newobj) {
      // eslint-disable-next-line no-prototype-builtins
      if (newobj.hasOwnProperty(key)) {
        oldobj[key] = newobj[key]
      }
    }
  }

  // execCommand('paste') is blocked everywhere, so the async API is the only
  // option and there is no fallback when it is unavailable
  static async pasteText() {
    try {
      if (navigator.clipboard && navigator.clipboard.readText) {
        return await navigator.clipboard.readText()
      }
    }
    catch (err) {
      console.log('clipboard read failed', err)
    }
    return ''
  }

  static copyText(text) {
    var textArea = document.createElement('textarea')
    textArea.style.position = 'fixed'
    textArea.value = text
    document.body.appendChild(textArea)
    textArea.focus()
    textArea.select()
            
    try {
      return document.execCommand('copy')
    } 
    finally {
      document.body.removeChild(textArea)
    }
  }

  static waitAsync(msecs) {
    return new Promise(resolve => setTimeout(() => resolve(), msecs))
  }
}
