import {ACTIVE, NETWORKS} from '../networks'

// Network presets live in src/networks.js; webpack reads the same file to
// decide which wasm client to bundle.
const active = NETWORKS[ACTIVE]

if (!active) {
  throw new Error(`Unknown network '${ACTIVE}' in networks.js`)
}

export const networkName = ACTIVE
export const isMainnet = ACTIVE === 'mainnet'

export default active
