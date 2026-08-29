//
// Network presets: node address, IPFS and cache gateways, deployed contract id
// and which wasm wallet client to ship. Switch networks by changing ACTIVE.
//
// CommonJS on purpose - webpack.config.js reads this file too, to decide which
// wasm client package to copy into the build.
//

const ACTIVE = 'mainnet'

const NETWORKS = {
  mainnet: {
    name: 'mainnet',
    // Verified in the desktop wallet. That build talks to the wallet's own
    // node and ignores the node/gateway fields - only cid matters there.
    // No mainnet cache gateway currently resolves, so a browser build cannot
    // list anything until one exists.
    node: 'eu-node01.mainnet.beam.mw:8200',
    ipfsGateway: 'https://gallery20.apps.beam.mw/ipfs/',
    webGateway: 'https://gallery20.apps.beam.mw/cache/',
    cid: '4390f75c95f60e6c069fb25a4c210d9b3b8a79804b1e5ddba431965ea8eb4cd9',
    wasmClient: 'beam-wasm-client'
  },

  dappnet: {
    name: 'dappnet',
    // A separate chain, well ahead of mainnet in height. Node and cache
    // gateway are alive; the cid below was reported as not deployed and needs
    // rediscovering before this preset works.
    node: 'eu-node02.dappnet.beam.mw:8200',
    ipfsGateway: 'https://apps-dappnet.beam.mw/ipfs/',
    webGateway: 'https://apps-dappnet.beam.mw/cache/',
    cid: '4f35ecda4e84eb8acb6d3991b8b7443e26e01353a6987c07c8d90b00edd8fc80',
    wasmClient: 'beam-wasm-client-dappnet'
  }
}

module.exports = {ACTIVE, NETWORKS}
