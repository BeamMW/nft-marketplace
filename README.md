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

## Layout

```
src/
  components/   pages and modals
  controls/     reusable widgets
  stores/       contract-backed data layer
  core/         wallet connection
  utils/        formatting, validation, helpers
  networks.js   network presets
  galleryManager.wasm
```

Import through the webpack aliases (`stores/nfts`, not `../stores/nfts`): `assets`,
`stores`, `utils`, `controls`, `components`, `core`, `router`.

## How it fits together

Stores are plain classes over Vue's `reactive` — no Vuex or Pinia. Contract calls go
through `utils.invokeContractAsync(args)` or `invokeContractAsyncAndMakeTx(args)`, where
`args` is `{role, action, cid, ...}` naming a shader method. `view_*` reads use
`role: 'manager'`; writes use `'user'`, `'artist'` or `'moderator'`.

Bulk reads are batched in `stores/lazy-loader.js` and cached in IndexedDB via Dexie.
Outside the desktop wallet they go over the HTTP cache gateway rather than the websocket
API, which is much faster.

`src/core/BeamDappConnector.js` is vendored from
[dex-app](https://github.com/BeamMW/dex-app) and skipped by ESLint so it stays diffable
against upstream; local edits are marked `LOCAL MODIFICATION`. `src/core/wallet.js`
subclasses it and exports a configured singleton. `src/utils/utils.js` wraps that.

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
