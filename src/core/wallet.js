import BeamDappConnector, {ConnectionState, WalletEnvironment} from 'core/BeamDappConnector'
import network from 'core/network'

//
// Gallery-specific wallet connector. BeamDappConnector (vendored from
// BeamMW/dex-app) owns connection, timeouts, reconnect and the event bus; the
// shader argument encoding and output unwrapping live here so the vendored
// file stays diffable against upstream.
//
class GalleryWallet extends BeamDappConnector {
  // Shader args go over the wire as a flat "key=value,key=value" string.
  static argsToString (args) {
    if (!args || typeof args !== 'object') {
      return args
    }

    let assign = ''
    for (let key in args) {
      assign += (assign ? ',' : '') + key + '=' + args[key]
    }
    return assign
  }

  // Returns {res, full, request}: res is the parsed shader output, full the
  // raw api result, request what we sent - used for error reporting.
  async invokeContract (args, bytes) {
    let params = {
      'create_tx': false
    }

    if (args) {
      params['args'] = GalleryWallet.argsToString(args)
    }

    if (bytes) {
      params['contract'] = bytes
    }

    let request = {method: 'invoke_contract', params}
    let result = await this.callApi('invoke_contract', params)

    // shader results arrive as a JSON string in `output`; a failed shader
    // still returns success at the api level
    if (result && typeof result.output === 'string') {
      let shaderAnswer = JSON.parse(result.output)

      if (shaderAnswer.error) {
        let err = new Error(shaderAnswer.error)
        err.answer = result
        err.request = request
        throw err
      }

      return {res: shaderAnswer, full: result, request}
    }

    return {res: result, full: result, request}
  }

  // Public wrappers over the connector internals, used when handing the
  // headless wallet over to the browser extension.
  async stopHeadlessWallet () {
    return this._stopHeadlessWallet()
  }

  async adoptWebApi (api) {
    if (this.api && this.api.delete) {
      this.api.delete()
    }

    try {
      await this._stopHeadlessWallet()
    }
    catch (err) {
      console.log('failed to stop headless wallet', err)
    }

    await api.callWalletApiResult((json) => this._handleApiResult(json))

    this.wallet = {api}
    this.api = api
    this.environment = WalletEnvironment.WEB

    return api
  }

  async invokeContractAndMakeTx (args) {
    let {full} = await this.invokeContract(args)

    if (!Array.isArray(full.raw_data)) {
      throw new Error('invoke_contract returned no raw_data')
    }

    let res = await this.callApi('process_invoke_data', {data: full.raw_data})

    if (!res || typeof res.txid !== 'string') {
      throw new Error('process_invoke_data returned no txid')
    }

    return res.txid
  }
}

const wallet = new GalleryWallet({
  appName: 'BEAM Gallery',
  apiVersion: '7.3',
  minApiVersion: '7.3',
  headlessNode: network.node,
  network: network.name,
  // without this a dropped response hangs the UI forever
  callTimeout: 60000,
  connectionTimeout: 30000,
  autoReconnect: true,
  showLoader: false
})

export default wallet
export {GalleryWallet, ConnectionState, WalletEnvironment}
