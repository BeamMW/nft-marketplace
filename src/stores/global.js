import {tabs, common, contract, sort} from 'utils/consts.js'
import utils from 'utils/utils.js'
import {reactive, nextTick, computed} from 'vue'

import router from 'router'
import formats from 'stores/formats'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'

function defaultState() {
  let state = {  
    /// OLD
    loading: true,
    error: undefined,
    shader: undefined,
    cid: contract.cid,
    cid_checked: false,
    is_admin: false,
    artworks: [],
    all_artworks: {
      [tabs.ALL]:   [],
      [tabs.MINE]:  [],
      [tabs.SALE]:  [],
      [tabs.LIKED]: [],
      [tabs.SOLD]:  [],
    },
    balance_beam: 0,
    balance_reward: 0,
    selected_artist: undefined,
    active_tab: tabs.ALL,
    sort_by: sort.OLDEST_TO_NEWEST,
    filter_by_artist: 0,
    pending_artworks: 0,
    is_headless: false,
    use_ipfs: true,
    gallery_tab: tabs.COLLECTIONS,
    gallery_artworks_page: 1,
    gallery_collections_page: 1,
    collections: [],
    debug: false
  }

  /*
  let desc = 'Blah blah blah, blah blah blah. Blah blah blah, blah blah blah! Blah blah blah, blah blah blah? Blah blah blah, blah blah blah.'
  for(let i = 0; i < 30; ++i) {
    state.collections.push({
      id: i,
      label: `Collection ${i}`,
      author: `Artist ${i}`,
      author_image: '/assets/test-author.png',
      cover_image: '/assets/test-collection-cover.png',
      description: [desc, desc, desc].join(' '),
    })
  }*/
  
  return state
}

function appstate() {
  let state = reactive({
    ...defaultState(),
    total_pages: computed(() => {
      let total = state.artworks.length
      return total ? Math.ceil(total / common.ITEMS_PER_PAGE) : 1
    })
  })
  return state
}

const store = {
  state: appstate(),

  //
  // Errors
  //
  setError(error, context, reload) {
    this.state.error = {error, context, reload}
    router.push({name: 'gallery'})
  },

  checkError (err) {
    if (err) this.setError(err)
  },

  clearError() {
    if (this.state.error) {
      let reload = this.state.error.reload
      this.state.error = undefined
      reload ? utils.reload() : this.start()
    }
  },

  setSortBy(val) {
    this.state.sort_by = val
    this.state.gallery_artworks_page = 1
    this.applySortAndFilters()
    this.loadCurrentArtworks()
  },

  setFilterByArtist(val) {
    this.state.filter_by_artist = val
    this.state.gallery_artworks_page = 1
    this.applySortAndFilters()
    this.loadCurrentArtworks()
  },

  //
  // Shader, CID, debug helpers
  //
  start () {
    Object.assign(this.state, defaultState())
    this.state.is_headless = utils.isHeadless()
    collsStore.reset(this)
    artistsStore.reset(this)
    artsStore.reset(this)
    imagesStore.reset(this)
    router.push({name: 'gallery'})

    nextTick(() => {
      utils.download('./galleryManager.wasm', (err, bytes) => {
        if (err) return this.setError(err, 'Failed to download shader')
        this.state.shader = bytes

        //tils.invokeContract('', (...args) => this.onShowMethods(...args), this.state.shader)
        utils.callApi('ev_subunsub', {ev_system_state: true}, (err) => this.checkError(err))
        utils.invokeContract('role=manager,action=view', (...args) => this.onCheckCID(...args), this.state.shader)
      })
    })
  },

  onShowMethods (err, res) {
    if (err) return this.setError(err)
    alert(utils.formatJSON(res))
  },

  onApiResult(err, res, full) {
    if (err) {
      // TODO: wrap in new Error
      return this.setError(err,  'API handling error')
    }

    if (full.id == 'ev_system_state') {
      // we update our data on each block
      this.refreshAllData()
      return
    }

    this.setError(full, 'Unexpected API result')
  },

  onCheckCID (err, res) {
    if (err) {
      return this.setError(err, 'Failed to verify cid')     
    }

    if (!res.contracts.some(el => el.cid == this.state.cid)) {
      throw `CID not found '${this.state.cid}'`
    }

    this.state.cid_checked = true
    this.refreshAllData()
  },

  refreshAllData () {
    // TODO: move loading sequence to promises, now loadBalance initiates update chain
    utils.invokeContract(
      `role=manager,action=view_params,cid=${this.state.cid}`, 
      (...args) => this.onLoadParams(...args)
    )
  },

  onLoadParams (err, res) {
    if (err) {
      return this.setError(err, 'Failed to load contract params')
    }

    utils.ensureField(res, 'Admin', 'number')
    utils.ensureField(res, 'voteReward_balance', 'number')
    this.state.is_admin = !!res.Admin
    this.state.balance_reward = res.voteReward_balance
    utils.invokeContract(
      `role=user,action=view_balance,cid=${this.state.cid}`, 
      (...args) => this.onLoadBalance(...args)
    )
  },

  async onLoadBalance(err, res) {
    if (err) {
      return this.setError(err, 'Failed to load balance')
    }

    // totals can be missing if user has nothing at all
    // also there can be not only beam. We just interested
    // only in beam for now
    let newBeam = 0
    if (res.totals) {
      utils.ensureField(res, 'totals', 'array')
      for (let item of res.totals) {
        if (item.aid == 0) {
          newBeam = item.amount
          break
        }
      }
    }

    this.state.balance_beam = newBeam
    try {
      await artistsStore.loadAsync()
      await collsStore.loadAsync()
      //await artsStore.loadAsync()
      this.state.loading = false
    }
    catch(err) 
    {
      this.setError(err)
    }
  },

  withdrawBEAM () {
    // TODO: this will not happen in demo but all the amounts might not fit into one block
    //       need to set nMaxCount to 100 max and reflet that in UI
    //       AND
    //       withdrawal in bulk negates privacy. Need to ask user what to do - in bulk
    //       or for each piece separately
    utils.invokeContract(
      `role=user,action=withdraw,nMaxCount=10,cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  //
  // Artworks
  //
  onLoadArtworks (err, res) {
    if (err) {
      return this.setError(err, 'Failed to load artwork list')
    }
    
    utils.ensureField(res, 'artworks', 'array')

    let oldstart = 0, oartworks = this.state.all_artworks[tabs.ALL]
    let all = [], sale = [], liked = [], mine = [], sold = []
    let mykey = artistsStore.my_id

    for (let awork of res.artworks) {
      let oawork = null
      for (let idx = oldstart; idx < oartworks.length; ++idx) {
        if (oartworks[idx].id == awork.id) {
          oldstart = idx + 1
          oawork = oartworks[idx]
          break
        }
      }
        
      if (oawork) {
        awork.title     = oawork.title
        awork.bytes     = oawork.bytes
        awork.sales     = oawork.sales
        awork.author    = oawork.author
        awork.pk_author = oawork.pk_author
      } 
      else {
        awork.author = (artistsStore.artists[awork.pk_author] || {}).label
      }

      awork.pk_owner = awork.pk
      delete awork.pk

      if (awork['price.aid'] != undefined) {
        awork.price = {
          aid: awork['price.aid'],
          amount: awork['price.amount']
        }
      }

      all.push(awork)
      if (awork.owned) mine.push(awork) // MINE is what I own
      if (awork.owned && awork.price) sale.push(awork) // SALE is what OWN && what has price set
      if (awork.my_impression) liked.push(awork) // awork.my_impression
      if (awork.pk_author === mykey && !awork.owned) sold.push(awork) // SOLD - I'm an author but I do not own
    }    

    this.state.all_artworks = {
      [tabs.ALL]:   all,
      [tabs.SALE]:  sale,
      [tabs.LIKED]: liked,
      [tabs.MINE]:  mine,
      [tabs.SOLD]:  sold
    }
        
    this.state.artworks = this.state.all_artworks[this.state.active_tab]
    if (this.state.sort_by !== 0 || this.state.filter_by_artist != 0) {
      this.applySortAndFilters()
    } 

    let currPage = this.state.gallery_artworks_page > this.state.total_pages ? this.state.total_pages : this.state.gallery_artworks_page
    this.setGalleryArtworksPage(currPage)

    // Finally we have something to display
    this.state.loading = false
  },

  applySortAndFilters() {
    let active_tab = this.state.active_tab
    this.state.artworks = this.state.all_artworks[active_tab]
        
    if (this.state.filter_by_artist != 0 && active_tab != tabs.SOLD) {
      let artist_key = this.state.filter_by_artist
      this.state.artworks = this.state.artworks.filter(awork => {
        if (awork.pk_author == artist_key)  {
          return true
        }
        return false
      })
    }

    switch(this.state.sort_by) {
    case sort.OLDEST_TO_NEWEST:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        return a.id < b.id ? -1 : 1
      })
      break

    case sort.NEWEST_TO_OLDEST:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        return a.id < b.id ? 1 : -1
      })
      break

    case sort.PRICE_ASC:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        if(a.price === undefined || a.price.amount === undefined) {
          return 1
        }
        if(b.price === undefined || b.price.amount === undefined) {
          return -1
        }
        return a.price.amount > b.price.amount ? 1 : -1
      })
      break

    case sort.PRICE_DESC:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        if(a.price === undefined || a.price.amount === undefined) {
          return 1
        }
        if(b.price === undefined || b.price.amount === undefined) {
          return -1
        }
        return  a.price.amount < b.price.amount ? 1 : -1
      })
      break

    case sort.LIKES_ASC:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        return a.impressions > b.impressions? 1 : -1
      })
      break

    case sort.LIKES_DESC:
      this.state.artworks =  this.state.artworks.sort((a,b) => {
        return a.impressions < b.impressions? 1 : -1
      })
      break

    default:
      break
    }
  },

  loadArtwork(tab, idx, id) {
    console.log(`Load Artwork: idx ${idx}, id ${id}`)
    this.state.pending_artworks++
    utils.invokeContract(
      `role=user,action=download,cid=${this.state.cid},id=${id}`, 
      (err, res) => {
        this.state.pending_artworks--
        this.onLoadArtwork(err, res, id, idx)
      }
    )  
  },

  onLoadArtwork(err, res, id, idx) {
    // list may have been changed, so we check 
    // if artwork with this id is still present
    let artwork = this.state.artworks[idx]
    if (!artwork || artwork.id != id) {
      return
    }

    try {
      if (err) throw err

      utils.ensureField(res, 'artist', 'string')
      let pk_author = res.artist

      utils.ensureField(res, 'data', 'string')
      var data = formats.u8arrToHex(res.data)

      // check version
      let version = data[0]
      if (version == 1) {
        // parse title
        let tend = data.findIndex(val => val == 0)
        if (tend == -1 || tend + 1 == data.length) {
          throw 'Unable to parse artwork title'
        }

        let rawTittle = data.subarray(1, tend)
        let title = (new TextDecoder()).decode(rawTittle)
        let bytes = data.subarray(tend + 2)

        artwork.title     = title
        artwork.bytes     = bytes
        artwork.pk_author = pk_author
        artwork.loading   = false
      } 
      else if (version == 2 && this.state.use_ipfs) {   
        let rawMeta = data.subarray(1)
        let meta = JSON.parse((new TextDecoder()).decode(rawMeta))

        utils.ensureField(meta, 'title', 'string')
        utils.ensureField(meta, 'ipfs_hash', 'string')

        artwork.title = meta.title
        artwork.pk_author = pk_author
        artwork.ipfs_hash = meta.ipfs_hash
        artwork.mime_type = meta.mime_type

        /*meta.ipfs_hash = 'QmWPMTkzfvN5Ue55WAvFj9uFpN71PqGKUwcZgfDLBCpiTB'
                if (this.get1) return
                this.get1 = 1

                utils.callApi('ipfs_gc', {}, (err, res) => {
                    if (err) {
                        alert("ipfs_gc: " + JSON.stringify(err))
                        return 
                    }

                    alert("ipfs_gc res: " + JSON.stringify(res))
                    return 
                })

                return
                */
        utils.callApi('ipfs_get', {hash: meta.ipfs_hash}, (err, res) => {
          artwork.loading = false
                    
          try {
            if (err) {
              // TODO:IPFS if not found reschedule for the next update
              // alert(`IPFS GET error ${JSON.stringify(err)}`)
              throw err
            }

            utils.ensureField(res, 'data', 'array')
            artwork.bytes = new Uint8Array(res.data)
          }
          catch(err) {
            artwork.error = err
            console.log(err)
          }
        })
      } 
      else {
        throw `Unknown artwork version ${data[0]}`
      }

      // We receive author only now, so add what's missing to SOLD
      let mykey = artistsStore.my_id
      if (artwork.pk_author === mykey && !artwork.owned) {
        this.state.all_artworks[tabs.SOLD].push(artwork)
        if (this.state.active_tab == tabs.SOLD) {
          this.applySortAndFilters()
          this.setGalleryArtworksPage(this.state.gallery_artworks_page)
        }
      }
    } 
    catch(err) {
      console.log(err)
      artwork.loading = false
      artwork.error = err
    }
  },

  //
  // Artwork actions, Buy, Sell, Like &c.
  //
  likeArtwork (id) {
    utils.invokeContract(
      `role=user,action=vote,id=${id},val=1,cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  unlikeArtwork (id) {
    utils.invokeContract(
      `role=user,action=vote,id=${id},val=0,cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  deleteArtwork (id) {
    utils.invokeContract(
      `role=manager,action=admin_delete,id=${id},cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  onMakeTx (err, sres, full) {
    if (err) {
      return this.setError(err, 'Failed to generate transaction request')
    }

    utils.ensureField(full.result, 'raw_data', 'array')
    utils.callApi(
      'process_invoke_data', {data: full.result.raw_data}, 
      (...args) => this.onSendToChain(...args)
    )
  },

  onSendToChain(err, res) {
    if (err) {
      if (utils.isUserCancelled(err)) return
      return this.setError(err, 'Failed to create transaction')
    }
    utils.ensureField(res, 'txid', 'string')
  },

  //
  // Admin stuff
  //
  addRewards (amount) {
    utils.invokeContract(
      `role=manager,action=add_rewards,num=${amount},cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  addArtist (key, name) {
    utils.invokeContract(
      `role=manager,action=set_artist,pkArtist=${key},label=${name},bEnable=1,cid=${this.state.cid}`, 
      (...args) => this.onMakeTx(...args)
    )
  },

  uploadArtwork (file, artist_key) {
    if (this.use_ipfs) {
      return this.uploadArtworkIPFS(file, artist_key)
    }
    return this.uploadArtworkChain(file, artist_key)
  },

  uploadArtworkChain (file, artist_key) {
    let name = file.name.split('.')[0]
    name = [name[0].toUpperCase(), name.substring(1)].join('')
            
    try {
      let reader = new FileReader()
      reader.readAsArrayBuffer(file)

      reader.onload = ()=> {
        let aver  = Uint8Array.from([1])
        let aname = (new TextEncoder()).encode(name)
        let asep  = Uint8Array.from([0, 0])
        let aimg  = new Uint8Array(reader.result)
        let hex   = [formats.u8arrToHex(aver), 
          formats.u8arrToHex(aname), 
          formats.u8arrToHex(asep), 
          formats.u8arrToHex(aimg)
        ].join('')
                        
        utils.invokeContract(`role=manager,action=upload,cid=${this.state.cid},pkArtist=${artist_key},data=${hex}`, 
          (...args) => this.onMakeTx(...args)
        )
      }
    }
    catch(err) {
      this.setError(err, 'Failed to upload artwork')
    }
  },

  uploadArtworkIPFS (file, artist_key) {
    let name = file.name.split('.')[0]
    name = [name[0].toUpperCase(), name.substring(1)].join('')
            
    try {
      let reader = new FileReader()
      reader.readAsArrayBuffer(file)

      reader.onload = ()=> {
        let imgArray = Array.from(new Uint8Array(reader.result))
        utils.callApi('ipfs_add', {data: imgArray}, (err, res) => {
          if (err) {
            return this.setError(err)
          }

          utils.ensureField(res, 'hash', 'string')
          // alert(`new ipfs hash: ${res.hash}`)
                    
          // Form what we write on blockchain
          // 2 (version) then meta
          let aver = Uint8Array.from([2])
          let meta = JSON.stringify({
            title: name,
            ipfs_hash: res.hash,
            mime_type: file.type
          })
          let ameta = (new TextEncoder()).encode(meta)
          let hex = [formats.u8arrToHex(aver), formats.u8arrToHex(ameta)].join('')

          // Register our NFT
          utils.invokeContract(`role=manager,action=upload,cid=${this.state.cid},pkArtist=${artist_key},data=${hex}`, 
            (...args) => this.onMakeTx(...args)
          )
        })  
      }
    }
    catch(err) {
      this.setError(err, 'Failed to upload artwork')
    }
  },

  showStats() {
    let total = this.state.all_artworks[tabs.ALL].length
    let left  = total

    for (let idx = 0; idx < total; ++idx) {
      let curridx = idx
      let currid  = this.state.all_artworks[tabs.ALL][curridx].id
      utils.invokeContract(`role=user,action=view_item,cid=${this.state.cid},id=${currid}`, 
        (err, res) => {
          if (err) {
            return this.setError(err)
          }

          if (res.sales.length) {
            let artwork = this.state.all_artworks[tabs.ALL][curridx]
            artwork.sales = res.sales
          } 
                    
          if (--left == 0) {
            // this is the last artwork
            this.__showStats()
          }
        }
      )
    }
  },

  __showStats() {
    const cnt_stats = 12
    let artworks = this.state.all_artworks[tabs.ALL]
    let artists = artistsStore.artists

    let likes = [], likes_total = 0
    let sales = [], tvb = 0

    let likes_compare = (l1, l2) => l2.impressions - l1.impressions
    let sells_compare = (s1, s2) => s2.amount - s1.amount

    for (let idx = 0; idx < artworks.length; idx++) {
      let artwork = artworks[idx]
            
      likes.push({idx, impressions: artwork.impressions})
      likes_total += artwork.impressions

      likes.sort(likes_compare)
      if (likes.length > cnt_stats) {
        likes.pop()
      }

      if (artwork.sales) {
        let max_this = 0
        for (let sale of artwork.sales) {
          tvb += sale.amount
          if (sale.amount > max_this) max_this = sale.amount
        }

        sales.push({
          idx, 
          amount: max_this
        })

        sales.sort(sells_compare)
        if (sales.length > cnt_stats) {
          sales.pop()
        }
      }
    }
        
    let formatter = function (arr, title, extra) {
      let result = [title, '\n'].join('')
      let cntr = 1

      for (let data of arr) {
        let artwork = artworks[data.idx]
        let eres = extra ? extra(artwork, data) : []
        result = [
          result, '\n  ', cntr, '. "', 
          artwork.title, '" by "', (artists[artwork.pk_author] || {}).label,
          '" id ', data.idx + 1
        ].join('')

        if (eres.length) {
          result = [result, ', ', ...eres].join('')
        }         

        cntr++
      }

      return result
    }

    let sliked = formatter(likes, 'Most liked:', (artwork) => [artwork.impressions, 'likes'].join(' '))
    let slikes = ['Total likes: ', likes_total].join('')
    let ssold  = formatter(sales, 'Top sales (one per artwork):', (artwork, data) => {
      let price = data.amount
      return [price / common.GROTHS_IN_BEAM, ' BEAM'].join('')
    })
    let stvb   = ['Total value of sales: ', tvb / common.GROTHS_IN_BEAM, ' BEAM'].join('')
    let snum   = ['Total artworks: ', artworks.length].join('')

    let message = [sliked, slikes, ssold, stvb, snum].join('\n\n')
    alert(message)
  },

  async switchToHeaded () {
    try {
      if (await utils.switchToWebAPI()) {
        this.start()
      }
    }
    catch(err) {
      this.setError(err)
    }
  },

  setGalleryArtworksPage(page) {
    this.state.gallery_artworks_page = page
    this.loadCurrentArtworks()
  },

  loadCurrentArtworks () {
    let start = (this.state.gallery_artworks_page - 1) * common.ITEMS_PER_PAGE
    let end = Math.min(start + common.ITEMS_PER_PAGE, this.state.artworks.length)
    for (let idx = start; idx < end; ++idx) {
      let art = this.state.artworks[idx]
      if (!art.bytes) {
        this.loadArtwork(this.state.active_tab, idx, art.id)
      }
    }
  },

  setActiveTab(id) {
    this.state.active_tab = id
    this.applySortAndFilters()
    this.setGalleryArtworksPage(1)
  },

  setGalleryTab(id) {
    this.state.gallery_tab = id
  },

  setGalleryCollectionsPage (page) {
    this.state.gallery_collections_page = page
  },

  toBack () {
    router.go(-1)
  },

  toMyPage() {
    router.push({
      name: 'my-page'
    })
  },

  toBalance() {
    router.push({
      name: 'balance'
    })
  },

  toNewNFT() {
    router.push({
      name: 'add-nft'
    })
  },
}

export default store