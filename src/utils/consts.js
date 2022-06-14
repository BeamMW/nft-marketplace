export const common = { 
  GROTHS_IN_BEAM: 100000000,
  ITEMS_PER_PAGE: 2,
  MAX_IMAGE_SIZE: 500 * 1024
}

export const user_tabs = {
  COLLECTIONS: 0,
  NFTS: 1,
  SALE_NFTS: 2,
  LIKED_NFTS: 3,
}

export const admin_tabs = {
  NFTS: 0,
  COLLECTIONS: 1,
  ARTISTS: 2
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

export const contract = {
  cid: '6736a6f47af610283ac8c4e73ae1d0e6a3c6bdbe456d949bc555dfea7fb20262'
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