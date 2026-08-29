import network from 'core/network'

export const common = { 
  GROTHS_IN_BEAM: 100000000,
  ITEMS_PER_PAGE: 40,
  MAX_IMAGE_SIZE: 500 * 1024
}

export const user_tabs = {
  NFTS: 0,
  COLLECTIONS: 1,
  SALE_NFTS: 2,
  LIKED_NFTS: 3
}

export const admin_tabs = {
  NFTS: 0,
  COLLECTIONS: 1,
  ARTISTS: 2,
  ADMIN: 3
}

export const my_tabs = {
  COLLECTIONS: 0,
  OWNED_NFTS: 1,
  SALE_NFTS: 2,
  SOLD_NFTS: 3,
  LIKED_NFTS: 4,
}

export const coll_tabs = {
  ALL_NFTS: 0,
  SALE_NFTS: 1,
  LIKED_NFTS: 2,
}

export const sort = {
  OLDEST_TO_NEWEST: 0,
  NEWEST_TO_OLDEST: 1,
  PRICE_ASC: 2,
  PRICE_DESC: 3,
  LIKES_ASC: 4,
  LIKES_DESC: 5
}

// TESTING OVERRIDE - makes the admin panel reachable with any wallet.
// Keep false outside local testing.
export const force_admin_ui = false

export const contract = {
  cid: network.cid
}

// Mirrors the DEX palette so an asset gets the same colour in both apps.
export const pallete_assets = [
  '#72fdff', '#2acf1d', '#ffbb54', '#d885ff', '#008eff',
  '#ff746b', '#91e300', '#ffe75a', '#9643ff', '#395bff',
  '#ff3b3b', '#73ff7c', '#ffa86c', '#ff3abe', '#0aaee1',
  '#ff5200', '#6464ff', '#ff7a21', '#63afff', '#c81f68'
]

export function assetColor (aid) {
  aid = Number(aid || 0)
  return pallete_assets[aid] || pallete_assets[aid % pallete_assets.length]
}

export const def_images = {
  artist_avatar: require('assets/artist-default-avatar.svg'),
  artist_banner: require('assets/artist-default-banner.svg'),
  nft: require('assets/nft-default.svg')
}

let uniqueID = 0
export function genUniqueID () {
  uniqueID++
  return `id-${uniqueID}`
}