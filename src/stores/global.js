import router from 'router'
import imagesStore from 'stores/images'
import lazyArtistsStore from 'stores/artists-lazy'
import artistsStore from 'stores/artists'
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'
import Database from 'stores/database'
import utils from 'utils/utils'
import ErrorEx from 'utils/errorex'
import {reactive, nextTick, computed} from 'vue'
import {common, contract, my_tabs} from 'utils/consts'

function defaultState() {
  let state = {  
    loading: 'BEAM Gallery is loading',
    error: undefined,
    shader: undefined,
    cid: contract.cid,
    is_admin: false,
    is_moderator: false,
    is_artist: false,
    my_active_tab: my_tabs.OWNED_NFTS,
    coll_active_tab: 0,
    admin_active_tab: 0,
    user_active_tab: 0,
    balance_beam: 0,
    balance_reward: 0,
    is_headless: false,
    debug: true
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
      ...nftsStore.getDBStores(),
      ...lazyArtistsStore.getDBStores()
    })

    //
    // Reset stores
    //
    nftsStore.reset(this, this._database)
    collsStore.reset(this, this._database)
    artistsStore.reset(this, this._database)
    lazyArtistsStore.reset(this, this._database)
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

  onApiResult(err, res, full) {
    if (err) {
      return this.setError(new ErrorEx('API handling error', err))
    }

    if (full.id == 'ev_system_state') {
      // we update our data on each block
      if (!this.state.loading) {
        console.log('new block')
        this.refreshAllData()
      }
      return
    }

    this.setError(new ErrorEx('Unexpected API result', full))
  },

  async checkCID () {
    this.state.shader = await utils.downloadAsync('galleryManager.wasm')
    
    /*
    utils.invokeContract('', 
      (err, res) => {
        if (err) return this.setError(new ErrorEx('Failed to list methids', err))
        alert(utils.formatJSON(res))
      },
      this.state.shader
    )*/

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

    if (this.state.debug) {
      console.log('view_params result:', res)
    }
    
    utils.ensureField(res, 'is_admin', 'number')
    utils.ensureField(res, 'is_moderator', 'number')  
    utils.ensureField(res, 'vote_reward', 'object')
    utils.ensureField(res.vote_reward, 'balance', 'number')
   
    this.state.is_admin = !!res.is_admin
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
    await lazyArtistsStore.loadAsync()
    
    if (this.state.loading) {
      this.state.loading = 'Loading Collections'
    }
    await collsStore.loadAsync()
  
    if (this.state.loading) {
      this.state.loading = 'Loading NFTs'
    }
    await nftsStore.loadAsync()

    if (this.state.loading) { 
      this.state.loading = false
      if (artistsStore.is_artist) {
        this.state.my_active_tab = my_tabs.COLLECTIONS
      }
    }
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
      action: 'set_artwork_status',
      ids: ids.join(','),
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  async approveCollections(ids, approve) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: 'set_collection_status',
      ids: ids.join(','),
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  async approveArtists(ids, approve) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'moderator',
      action: 'set_artist_status',
      ids: ids.join(','),
      status: approve ? 'approved' : 'rejected',
      cid: this.state.cid
    })
  },

  //
  // Admin stuff
  //
  async addRewards (amount) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'manager',
      action: 'add_rewards',
      num: amount,
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
  }
}

export default store