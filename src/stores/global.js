import {tabs, common, contract, sort, my_tabs} from 'utils/consts.js'
import utils from 'utils/utils.js'
import {reactive, nextTick, computed} from 'vue'

import router from 'router'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'
import Database from 'stores/database'

function defaultState() {
  let state = {  
    /// OLD
    loading: 'BEAM Gallery is loading',
    error: undefined,
    shader: undefined,
    cid: contract.cid,
    is_admin: false,
    is_moderator: false,
    is_artist: false,
    my_active_tab: my_tabs.OWNED_NFTS,
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
    debug: true
  }
  
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
      /// TODO: start is async, handle error
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
  async start () {
    //
    // Reset state
    //
    Object.assign(this.state, defaultState())
    this.state.is_headless = utils.isHeadless()

    //
    // Reset database
    //
    if (this._database) {
      this._database.close()
      this._database = undefined
    }

    this._database = new Database()
    await this._database.initAsync({
      ...collsStore.getDBStores(),
      ...artsStore.getDBStores()
    })

    //
    // Reset stores
    //
    artsStore.reset(this, this._database)
    collsStore.reset(this, this._database)
    artistsStore.reset(this, this._database)
    imagesStore.reset(this, this._database)

    //
    // navigate to gallery
    //
    router.push({name: 'gallery'})
  
    //
    // Download shader & start working with contract
    // nextTick to not to block UI while downloading
    //
    nextTick(async () => {
      await this.checkCID()
      await this.refreshAllData()
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
      if (!this.state.loading) {
        console.log('new block')
        this.refreshAllData()
      }
      return
    }

    this.setError(full, 'Unexpected API result')
  },

  async checkCID () {
    this.state.shader = await utils.downloadAsync('./galleryManager.wasm')
    
    // utils.invokeContract('', (...args) => this.onShowMethods(...args), this.state.shader)
    await utils.callApiAsync('ev_subunsub', {ev_system_state: true})
    let {res} = await utils.invokeContractAsync(
      {role: 'manager', action: 'view'}, 
      this.state.shader
    )
    
    if(!res.contracts.some(el => el.cid == this.state.cid)) {
      throw new Error(`CID ${this.state.cid} not found `)
    }
  },

  async refreshAllData () {
    let {res} = await utils.invokeContractAsync({
      role: 'manager', 
      action: 'view_params', 
      'cid': this.state.cid}
    )
    
    utils.ensureField(res, 'is_admin', 'number')
    utils.ensureField(res, 'is_moderator', 'number')  
    utils.ensureField(res, 'vote_reward', 'object')
    utils.ensureField(res.vote_reward, 'balance', 'number')
   
    // TODO:TEST
    // this.state.is_admin = !!res.is_admin
    this.state.is_admin = true
    this.state.is_moderator = !!res.is_moderator
    this.state.balance_reward = res.vote_reward.balance;
    
    ({res} = await utils.invokeContractAsync({
      role: 'user', 
      action: 'view_balance', 
      'cid': this.state.cid
    }))

    // totals can be missing if user has nothing at all
    // also there can be not only beam. We just interested
    // only in beam for now
    if (res.totals) {
      utils.ensureField(res, 'totals', 'array')
      for (let item of res.totals) {
        if (item.aid == 0) {
          this.state.balance_beam = item.amount
          break
        }
      }
    }

    //
    // Reload stores
    //
    await imagesStore.loadAsync()
  
    if (this.state.loading) {
      this.state.loading = 'Loading Artists'
    }
    await artistsStore.loadAsync()
    
    if (this.state.loading) {
      this.state.loading = 'Loading Collections'
    }
    await collsStore.loadAsync()
  
    if (this.state.loading) {
      this.state.loading = 'Loading Artworks'
    }

    await artsStore.loadAsync()

    if (this.state.loading) { 
      this.state.loading = false
      if (artistsStore.is_artist) {
        this.state.my_active_tab = my_tabs.COLLECTIONS
      }
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
  // Artwork actions, Buy, Sell, Like &c.
  //
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
        await this.start()
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

  setMyTab(id) {
    this.state.my_active_tab = id
  },

  setGalleryTab(id) {
    this.state.gallery_tab = id
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
  }
}

export default store