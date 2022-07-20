import router from 'router'
import imagesStore from 'stores/images'
import lazyArtistsStore from 'stores/artists-lazy'
import artistsStore from 'stores/artists'
import collsStore from 'stores/collections'
import likesStore from 'stores/likes'
import nftsStore from 'stores/nfts'
import Database from 'stores/database'
import utils from 'utils/utils'
import ErrorEx from 'utils/errorex'
import {reactive, nextTick, computed} from 'vue'
import {common, contract, my_tabs} from 'utils/consts'
import {admin_tabs} from '../utils/consts'

function defaultState() {
  let state = {  
    loading: 'BEAM Gallery is loading',
    first_refresh: true,
    error: undefined,
    shader: undefined,
    cid: contract.cid,
    height: 0,
    my_key: false,
    is_admin: false,
    is_moderator: false,
    moderators: [],
    my_active_tab: my_tabs.OWNED_NFTS,
    coll_active_tab: 0,
    admin_active_tab: 0,
    user_active_tab: 0,
    balance_beam: 0,
    balance_reward: 0,
    is_headless: false,
    is_desktop: true,
    debug: false
  }
  
  return state
}

function appstate() {
  let state = reactive({
    ...defaultState(),
    total_pages: computed(() => {
      let total = state.nfts.length
      return total ? Math.ceil(total / common.ITEMS_PER_PAGE) : 1
    })
  })
  return state
}

const store = {
  state: appstate(),

  toggleDebug() {
    this.state.debug = !this.state.debug
  },

  //
  // Errors
  //
  setError(error, reload) {
    this.state.error = {error, reload}
    router.push({name: 'gallery'})
  },

  clearError() {
    if (this.state.error) {
      let reload = this.state.error.reload
      this.state.error = undefined
      reload ? utils.reload() : this.start()
    }
  },

  //
  // App entry point
  //
  async start () {
    //
    // Download shader & start working with contract
    // nextTick to not to block UI while downloading
    //
    nextTick(async () => {
      Object.assign(this.state, defaultState())
      this.state.is_headless = utils.isHeadless()
      this.state.is_desktop = utils.isDesktop()
      router.push({name: 'gallery'})
      
      await this.checkCID()
      await this.initStuff()      

      // state event would trigger data refresh
      await utils.callApiAsync('ev_subunsub', {ev_system_state: true})
    })
  },

  async initStuff() {
    //
    // Reset database
    //
    if (this._database) {
      this._database.close()
      this._database = undefined
    }

    //
    // get key
    //
    let {res} = await utils.invokeContractAsync({
      role: 'artist',
      action: 'get_id',
      cid: this.state.cid
    })
    utils.ensureField(res, 'id', 'string')
    this.state.my_key = res.id

    //
    // Initialize database
    //
    let db_id = utils.isHeadless() ? 'headless' : this.state.my_key
    this._database = new Database(this.state.cid, db_id)
    await this._database.initAsync({
      ...collsStore.getDBStores(),
      ...lazyArtistsStore.getDBStores(),
      ...likesStore.getDBStores(),
      ...nftsStore.getDBStores()
    })

    //
    // Reset stores
    //
    nftsStore.reset(this, this._database)
    likesStore.reset(this, this._database)
    collsStore.reset(this, this._database)
    artistsStore.reset(this, this._database)
    lazyArtistsStore.reset(this, this._database)
    imagesStore.reset(this, this._database)
  },

  onApiResult(err, res, full) {
    if (err) {
      return this.setError(new ErrorEx('API handling error', err))
    }

    // We update state on each block
    // TODO: save tip and check that it is really changed
    if (full.id == 'ev_system_state') {
      // if still not in sync doesn't have any sense to update
      // all shader calls would be delayed by platform until in sync
      if (!res.is_in_sync) {
        return
      }

      this.state.height = res.current_height
      console.log('new block reached, updating app state')
      this.refreshAllData()

      return
    }

    this.setError(new ErrorEx('Unexpected API result', full))
  },

  async checkCID () {
    let href = window.location.href
    let ipos = href.indexOf('index.html')
    let wasmPath = [href.substring(0, ipos), 'galleryManager.wasm'].join('')
    this.state.shader = await utils.downloadAsync(wasmPath)

    /*
    utils.invokeContract('', 
      (err, res) => {
        if (err) return this.setError(new ErrorEx('Failed to list methids', err))
        alert(utils.formatJSON(res))
      },
      this.state.shader
    )*/
    let {res} = await utils.invokeContractAsync(
      {role: 'manager', action: 'view'}, 
      this.state.shader
    )
    
    if(!res.contracts.some(el => el.cid == this.state.cid)) {
      throw new Error(`CID ${this.state.cid} not found `)
    }
  },

  async refreshAllData () {
    if (!this.state.first_refresh) {
      // do not refresh anything until loaded
      if (this.state.loading) {
        return
      }
    }

    this.state.first_refresh = false
    let {res} = await utils.invokeContractAsync({
      role: 'manager', 
      action: 'view_params', 
      'cid': this.state.cid}
    )

    if (this.state.debug) {
      console.log('view_params result:', res)
    }
    
    utils.ensureField(res, 'is_admin', 'number')
    utils.ensureField(res, 'is_moderator', 'number')  
    utils.ensureField(res, 'vote_reward', 'object')
    utils.ensureField(res.vote_reward, 'balance', 'number')
   
    this.state.is_admin = !!res.is_admin
    this.state.is_moderator = !!res.is_moderator
    this.state.balance_reward = res.vote_reward.balance
    if (this.state.is_admin) this.loadModerators()

    if (this.state.is_admin && !this.state.is_moderator) {
      this.state.admin_active_tab = admin_tabs.ADMIN
    }
    
    ({res} = await utils.invokeContractAsync({
      role: 'user', 
      action: 'view_balance', 
      'cid': this.state.cid
    }))

    // totals can be missing if user has nothing at all
    // also there can be not only beam. We just interested
    // only in beam for now
    let balance_beam = 0
    if (res.totals) {
      utils.ensureField(res, 'totals', 'array')
      for (let item of res.totals) {
        if (item.aid == 0) {
          balance_beam = item.amount
          break
        }
      }
    }
    this.state.balance_beam = balance_beam

    //
    // Reload stores
    //
    await imagesStore.loadAsync()
    await artistsStore.loadAsync()

    if (this.state.loading) {
      this.state.loading = 'Loading Artists'
      await lazyArtistsStore.loadAsync(true)

      this.state.loading = 'Loading Collections'
      await collsStore.loadAsync(true)

      this.state.loading = 'Loading NFTs'
      
      let loader = undefined
      loader = async(loadLikes, loadNfts) => {
        if (loadLikes) loadLikes = await loadLikes()
        if (loadNfts) loadNfts  = await loadNfts()
        if (loadLikes || loadNfts) setTimeout(() => loader(loadLikes, loadNfts), 10)
      }

      //
      // We make nfts store depend on likes store and do not 
      // allow them to load in parallel. Likes iteration should
      // always complete before nft iteration. This ensures that
      // NFT would always load updated likes information
      //
      await loader(
        async () => await likesStore.loadAsync(false), 
        async () => await nftsStore.loadAsync(false)
      )

      this.state.loading = false
      if (artistsStore.is_artist) {
        this.state.my_active_tab = my_tabs.COLLECTIONS
      }
    } 
    
    await lazyArtistsStore.updateAsync(true)
    await collsStore.updateAsync(true)

    let storesLoader = async () => {
      //
      // We make nfts store depend on likes store and do not 
      // allow them to load in parallel. Likes iteration should
      // always complete before nft iteration. This ensures that
      // NFT would always load updated likes information
      //
      let updater = undefined
      updater = async(updateLikes, updateNfts) => {
        if (updateLikes) updateLikes = await updateLikes()
        if (updateNfts) updateNfts  = await updateNfts()
        if (updateLikes || updateNfts) setTimeout(() => updater(updateLikes, updateNfts), 10)
      }

      await updater(
        async () => await likesStore.updateAsync(false), 
        async () => await nftsStore.updateAsync(false)
      )
    }

    // In web mode we let cache server time to be updates
    setTimeout(storesLoader, utils.isWeb() ? 5 : 0)

    if (this.state.my_active_tab == my_tabs.COLLECTIONS && !artistsStore.is_artist) {
      this.state.my_active_tab = my_tabs.OWNED_NFTS
    }
  },

  async loadModerators() {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_moderators',
      cid: this.state.cid
    })
    utils.ensureField(res, 'items', 'array')
    this.state.moderators = res.items
  },

  async setModerator(id, enable) {
    await utils.invokeContractAsyncAndMakeTx({
      role: 'manager',
      action: 'set_moderator',
      id,
      enable,
      cid: this.state.cid
    })
  },

  async withdrawBEAM () {
    // FUTURE: this will not happen in demo but all the amounts might not fit into one block
    //         need to set nMaxCount to 100 max and reflet that in UI
    //         AND
    //         withdrawal in bulk negates privacy. Need to ask user what to do - in bulk
    //         or for each piece separately
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'withdraw',
      nMaxCount: 10,
      cid: this.state.cid
    })
  },

  async approveNFTs(ids, approve) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: 'set_nft_status',
      ids: '"' + ids.join(';') + '"',
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  async approveCollections(ids, approve) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: 'set_collection_status',
      ids: '"' + ids.join(';') + '"',
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  async approveArtists(ids, approve) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: 'set_artist_status',
      ids: '"' + ids.join(';') + '"',
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  // TODO: use this more general method in moderator page
  //       refactor moderator page to be more general
  async setItemsStatus(type, ids, status) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: `set_${type}_status`,
      ids: '"' + ids.join(';') + '"',
      status,
      cid: this.state.cid
    })
  },

  //
  // Admin stuff
  //
  async addRewards (amount) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'add_rewards',
      aid: 0,
      amount,
      cid: this.state.cid
    })
  },

  async addArtist (key, name) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'manager',
      action: 'set_artist',
      pkArtist: key,
      label: name,
      bEnable: 1,
      cid: this.state.cid
    })
  },

  async switchToHeaded () {
    if (await utils.switchToWebAPI()) {
      await this.start()
    }
  },

  setMyTab(id) {
    this.state.my_active_tab = id
  },

  setCollTab(id) {
    this.state.coll_active_tab = id
  },

  setUserTab(id) {
    this.state.user_active_tab = id
  },

  setAdminTab(id) {
    this.state.admin_active_tab = id
  },

  openUrl(url) {
    window.open(url, '_blank')
  },

  openTwitter(nick) {
    let link = `https://www.twitter.com/@${nick}`
    window.open(link, '_blank')
  },

  openInstagram(nick) {
    let link = `https://www.instagram.com/@${nick}`
    window.open(link, '_blank')
  },

  toBack () {
    router.go(-1)
  },

  toMyPage() {
    router.push({
      name: 'my'
    })
  },

  toBalance() {
    router.push({
      name: 'balance'
    })
  },

  toAdmin() {
    router.push({
      name: 'moderator'
    })
  },

  getPrevPage() {
    return router.options.history.state.back
  }
}

export default store