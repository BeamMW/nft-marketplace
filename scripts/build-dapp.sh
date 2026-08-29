#!/usr/bin/env bash
set -euo pipefail

# Ported from BeamMW/dex-app (scripts/build-dapp.sh) and adapted for the gallery.
# Produces a .dapp bundle the Beam wallet can install.

cd "$(dirname "$0")/.."

DAPP_NAME="nft-gallery"
MANIFEST_NAME="BEAM NFT Gallery"
MANIFEST_DESCRIPTION="Buy and sell confidential NFTs"
MANIFEST_VERSION_PREFIX="1.1"
MANIFEST_ICON="localapp/app/logo.svg"
MANIFEST_URL="localapp/app/index.html"

# Keep these in step with the connector config in src/core/wallet.js - the
# wallet creates the app API from the manifest, the app asks for the same.
MANIFEST_API_VERSION="7.3"
MANIFEST_MIN_API_VERSION="7.3"

MANIFEST_GUID="ffbec734a0bb4f88a7104357a2680d20"

# Like the DEX, the patch number is the commit count so every build is
# monotonic and the wallet can tell versions apart. Override for a fixed
# version: MANIFEST_VERSION=1.0.0 ./scripts/build-dapp.sh
COMMIT_COUNT="$(git rev-list --count HEAD)"
VERSION="${MANIFEST_VERSION:-${MANIFEST_VERSION_PREFIX}.${COMMIT_COUNT}}"

ACTIVE_NETWORK="$(node -e "process.stdout.write(require('./src/networks').ACTIVE)")"

echo "Building ${MANIFEST_NAME} ${VERSION} for network '${ACTIVE_NETWORK}'"

# CI installs with --immutable before calling this
if [ -z "${SKIP_INSTALL:-}" ]; then
  yarn install
fi

yarn build

test -f html/index.html
test -f html/main.bundle.js
test -f html/main.css
test -f html/galleryManager.wasm
test -f src/appicon.svg

rm -rf "${DAPP_NAME}" "${DAPP_NAME}.dapp"
mkdir -p "${DAPP_NAME}/app"
cp -r html/* "${DAPP_NAME}/app/"
cp src/appicon.svg "${DAPP_NAME}/app/logo.svg"

cat > "${DAPP_NAME}/manifest.json" <<EOF
{
  "name": "${MANIFEST_NAME}",
  "description": "${MANIFEST_DESCRIPTION}",
  "icon": "${MANIFEST_ICON}",
  "url": "${MANIFEST_URL}",
  "version": "${VERSION}",
  "api_version": "${MANIFEST_API_VERSION}",
  "min_api_version": "${MANIFEST_MIN_API_VERSION}",
  "guid": "${MANIFEST_GUID}"
}
EOF

(
  cd "${DAPP_NAME}"
  zip -r -q "../${DAPP_NAME}.dapp" ./*
)

echo "Created ${DAPP_NAME}.dapp with version ${VERSION} (network: ${ACTIVE_NETWORK})"
