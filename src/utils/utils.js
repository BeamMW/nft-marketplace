const MIN_AMOUNT = 0.00000001
const MAX_AMOUNT = 254000000

let BEAM         = null
let CallID       = 0
let Calls        = {}
let APIResCB     = undefined
let headlessNode = 'eu-node02.dappnet.beam.mw:8200'
let ipfsGateway  = 'https://apps-dappnet.beam.mw/ipfs/'
let webGateway   = 'https://apps-dappnet.beam.mw/cache/'
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
    return BEAM && BEAM.headless
  }

  static async createMobileAPI(apirescback) {
    return new Promise((resolve, reject) => {
      if (!window.BEAM) {
        return reject()
      }
      if (Utils.isAndroid()) { 
        document.addEventListener('onCallWalletApiResult', res => apirescback(res.detail))
      }
      else {
        window.BEAM.callWalletApiResult(apirescback)
      }
      resolve({
        api: window.BEAM
      })
    })
  }

  static async createDesktopAPI(apirescback) {
    await Utils.injectScript('qrc:///qtwebchannel/qwebchannel.js')
    return new Promise((resolve) => {
      new QWebChannel(qt.webChannelTransport, (channel) => { // eslint-disable-line no-undef
        channel.objects.BEAM.api.callWalletApiResult.connect(apirescback)
        resolve({
          api: channel.objects.BEAM.api,
          styles: channel.objects.BEAM.style
        })
      })
    })  
  }

  static async createWebAPI(apiver, apivermin, appname, apirescback) {
    return new Promise((resolve) => {
      window.addEventListener('message', async (ev) => {
        if (ev.data === 'apiInjected') {
          await window.BeamApi.callWalletApiResult(apirescback)
          resolve({
            api: window.BeamApi
          })
        }
      }, false)
      window.postMessage({type: 'create_beam_api', apiver, apivermin, appname}, window.origin)
    })
  }

  static async createHeadlessAPI(apiver, apivermin, appname, apirescback) {
    await Utils.injectScript('wasm-client.js')
        
    let WasmModule = await BeamModule()  // eslint-disable-line no-undef
    let WasmWalletClient = WasmModule.WasmWalletClient
    let client = new WasmWalletClient(headlessNode)
    client.startWallet()

    client.subscribe((response) => {
      throw new Error(`Unexpected wasm wallet client response call: ${response}`)
    })

    client.setApproveContractInfoHandler((info) => {
      throw new Error(`Unexpected wasm wallet client transaction in headless wallet ${info}`)
    })
        
    return new Promise((resolve, reject) => {
      let appid = WasmWalletClient.GenerateAppID(appname, window.location.href)
      client.createAppAPI(apiver, apivermin, appid, appname, (err, api) => {
        if (err) {
          reject(err)
        }
                
        api.setHandler(apirescback)
        resolve({
          headless: true,
          module: WasmModule,
          factory: WasmWalletClient,
          client,
          appid,
          api
        })
      })
    })
  }

  static async stopHeadlessWallet() {
    return new Promise((resolve, reject) => {
      BEAM.client.stopWallet((data) => {
        const running = BEAM.client.isRunning()
        console.log(`is running: ${BEAM.client.isRunning()}`)
        console.log('wallet stopped:', data)

        if (running) {
          reject(false)
        } else {
          resolve(true)
        }
      })
    })
  }

  static async switchToWebAPI () {
    if (!Utils.isHeadless()) {
      throw new Error('Wallet must be opened in a headless mode')
    }

    let apiver    = InitParams['api_version'] || 'current'
    let apivermin = InitParams['min_api_version'] || ''
    let appname   = InitParams['appname']
    let apirescb  = (...args) => Utils.handleApiResult(...args)

    const newAPI = await new Promise((resolve) => {
      const listener = async (ev) => {
        if (ev.data === 'apiInjected') {
          await window.BeamApi.callWalletApiResult(apirescb)
          Utils.hideLoading()
          resolve(window.BeamApi)
        }

        if (ev.data === 'rejected') {
          // TODO
        }
      }

      // TODO: add some delay before showing connecting message
      //       if extension is installed and app is allowed it would filck
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
      BEAM.api.delete()
      await Utils.stopHeadlessWallet()
      BEAM = {
        api: newAPI
      }
    }

    return newAPI
  }

  static async callApiAsync(method, params) {    
    return new Promise((resolve, reject) => {
      Utils.callApi(method, params, (err, res, full) => {
        if (err) return reject(err)
        return resolve({res, full})
      })
    })
  }

  static callApi(method, params, cback) {
    let callid = ['call', CallID++, method].join('-')
    let request = {
      'jsonrpc': '2.0',
      'id':      callid,
      'method':  method,
      'params':  params
    }
    Calls[callid] = {cback, request}

    if (Utils.isHeadless()) {
      return BEAM.api.callWalletApi(JSON.stringify(request))
    }

    if (Utils.isWeb()) {
      return BEAM.api.callWalletApi(callid, method, params)
    } 

    if (Utils.isMobile()) {
      return BEAM.api.callWalletApi(JSON.stringify(request))
    }
        
    if (Utils.isDesktop()) {
      return BEAM.api.callWalletApi(JSON.stringify(request))
    }
  }

  static async invokeContractAsync(args, bytes) {
    return new Promise((resolve, reject) => {
      Utils.invokeContract(args, (err, res, full, request) => {
        if (err) return reject(err)
        return resolve({res, full, request})
      },
      bytes)
    })
  }

  static async invokeContractAsyncAndMakeTx (args) {
    let {full} = await Utils.invokeContractAsync(args)
    Utils.ensureField(full.result, 'raw_data', 'array')

    try {
      let {res} = await Utils.callApiAsync('process_invoke_data', {data: full.result.raw_data})
      Utils.ensureField(res, 'txid', 'string')
      return res.txid
    }
    catch(err) {
      if (Utils.isUserCancelled(err)) {
        return undefined
      }
      throw err
    }
  }

  static invokeContract(args, cback, bytes) {
    let params = {
      'create_tx': false
    }

    if (args) {
      let assign = args

      if (typeof args === 'object') {
        assign = ''
        for (let key in args) {
          assign += (assign ? ',' : '') + key + '=' + args[key] 
        }
      }

      params = Object.assign({
        'args': assign
      }, params)
    }

    if (bytes) {
      params = Object.assign({
        'contract': bytes
      }, params)
    }

    // console.log('invoke contract', params)
    return Utils.callApi('invoke_contract', params, cback)
  }

  static handleApiResult (json) {
    let answer = undefined
    
    try
    {
      answer = JSON.parse(json)
      const id = answer.id
      const call = Calls[id] || {}
      const cback = call.cback || APIResCB
      const request = call.request
      delete Calls[id]
            
      if (answer.error) {
        return cback(answer)
      }

      if (typeof answer.result == 'undefined') {
        return cback({
          error: 'no valid api call result', 
          answer
        })
      }

      if (typeof answer.result.output == 'string') {
        // this is shader result
        let shaderAnswer = JSON.parse(answer.result.output)
        if (shaderAnswer.error) {
          return cback({
            error: shaderAnswer.error,
            answer,
            request
          })
        }
        return cback(null, shaderAnswer, answer, request)
      }
      else
      {
        return cback(null, answer.result, answer, request)
      }
    }
    catch (err)
    {
      APIResCB({
        error: err.toString(),
        answer: answer || json
      })
    }
  }

  static async initialize(params, initcback) {
    InitParams = params
    APIResCB = params['apiResultHandler']
    let headless = params['headless']
        
    try 
    {
      if (Utils.isDesktop()) {
        BEAM = await Utils.createDesktopAPI((...args) => Utils.handleApiResult(...args))
      } 
            
      if (Utils.isWeb()) {
        let apiver    = params['api_version'] || 'current'
        let apivermin = params['min_api_version'] || ''
        let appname   = params['appname']

        if (!Utils.isChrome()) {
          Utils.showChromeDownload()
          return false
        }
                
        if (headless) {
          Utils.showLoading({
            headless: true,
            connecting: false
          })
          BEAM = await Utils.createHeadlessAPI(
            apiver, apivermin, appname, 
            (...args) => Utils.handleApiResult(...args)
          )        
        } else {
          Utils.showLoading({
            headless: false,
            connecting: true
          })
          BEAM = await Utils.createWebAPI(
            apiver, apivermin, appname, 
            (...args) => Utils.handleApiResult(...args)
          )
        }
      }

      if (Utils.isMobile()) {
        try {
          BEAM = await Utils.createMobileAPI((...args) => Utils.handleApiResult(...args))
        } catch (e) {
          Utils.showMobileStoresLinks()
          return false
        }
      }

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
      return initcback(err)
    }
  }

  static getStyles () {
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
    const regex = new RegExp(/^-?\d+(\.\d*)?$/g)
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
    return err.error && err.error.code == -32021
  }

  static formatJSON(obj) {
    let res = JSON.stringify(obj, null, 2)
    return res == '{}' ? obj.toString() : res
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
    return str.replace(/([0-9]+(\.[0-9]+[1-9])?)(\.?0+$)/,'$1')
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
