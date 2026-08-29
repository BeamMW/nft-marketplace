# BEAM NFT Gallery

NFT marketplace for [Beam](https://beam.mw). Vue 3 SPA, no backend: it talks to the
`gallery` shader through a Beam wallet, and images come from IPFS.

Runs three ways — as a DApp in the Beam Desktop Wallet, in the browser with the Beam Web
Wallet extension, or standalone in the browser using a headless WASM wallet.

## Setup

Needs Node 18+ and Yarn 4 (`corepack enable` is enough — the version is pinned in
`package.json`).

```bash
yarn install
yarn serve
```

Dev server runs on <http://localhost:13666>.

In a plain browser this starts the headless WASM wallet, which has to sync before anything
shows up. It's usually quicker to point the Desktop Wallet at the dev server instead, since
that uses the wallet's own node.

| | |
|---|---|
| `yarn serve` | dev server, port 13666 |
| `yarn watch` | rebuild into `html/` on change |
| `yarn build` | production build into `html/` |
| `yarn build:dapp` | package `nft-gallery.dapp` |

ESLint and stylelint run as webpack plugins, so lint errors break the build. No tests.

## Networks

Node, gateways, contract id and wasm client are in [`src/networks.js`](src/networks.js).
Change `ACTIVE` to switch. It's CommonJS because `webpack.config.js` reads it too, to pick
the wasm client to bundle — the build fails outright if that package isn't installed.

Mainnet and dappnet need different client packages (`beam-wasm-client` vs
`beam-wasm-client-dappnet`); they're separate chains with different rules.

In the desktop wallet only `cid` matters — that build uses the wallet's own node and
ignores everything else in the preset.

## Packaging

```bash
yarn build:dapp
```

Zips the build plus a manifest into `nft-gallery.dapp`, which the wallet can install. The
patch version is the commit count; override with `MANIFEST_VERSION=1.1.0 yarn build:dapp`.

## Architecture

The app never speaks to a node directly. Everything goes through a wallet:

```
components / controls    Vue views and widgets
        |
      stores             one per entity: nfts, collections, artists, assets
        |
  utils/utils.js         facade
        |
   core/wallet.js        shader arg encoding, output unwrapping
        |
core/BeamDappConnector   connection, timeouts, reconnect, events
        |
      wallet             desktop, extension or headless WASM
```

**Calling the contract.** `utils.invokeContractAsync(args)` for reads,
`invokeContractAsyncAndMakeTx(args)` for anything that costs a transaction. `args` is
`{role, action, cid, ...}` naming a shader method — `view_*` reads use `role: 'manager'`,
writes use `'user'`, `'artist'` or `'moderator'`.

**Reading in bulk.** `stores/lazy-loader.js` pages through `view_*` and caches results in
IndexedDB (Dexie), so a reload is instant and the UI reads from the local copy. Outside
the desktop wallet it fetches from the HTTP cache gateway instead of the websocket API,
which is much faster. Stores are plain classes over Vue's `reactive` — no Vuex, no Pinia.

**Staying current.** The wallet pushes an event on every new block. That bumps
`state.height`, which stores watch to refresh, so the UI follows the chain without
polling.

**The connector.** `core/BeamDappConnector.js` is vendored from
[dex-app](https://github.com/BeamMW/dex-app) and skipped by ESLint so it stays diffable
against upstream; local changes are marked `LOCAL MODIFICATION`. Don't edit it to add
gallery behaviour — that belongs in `core/wallet.js`, which subclasses it.

## Notes for contributors

Match the surrounding style: no semicolons, single quotes, 2-space indent, Stylus in `.vue`
files.

The deployed contract isn't in this repo, and the `gallery` shader in the `beam` repo is an
older version of it — don't use it to reason about what's live. Ask the gateway instead:

```bash
curl 'https://apps-dappnet.beam.mw/cache/view_collections?id0=0&count=1'
```

## License

Apache-2.0
