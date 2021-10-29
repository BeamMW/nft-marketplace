import {router} from './router.js';
import utils from './utils/utils.js';
import { tabs } from './utils/consts.js';

const defaultState = () => {
    return {
        loading: true,
        error: undefined,
        shader: undefined,
        cid: "ea3a002da2b8f8d24c5f5e7056e4c06aad6309097588c6946d10ac00349a9f52",
        my_artist_key: "",
        is_artist: false,
        is_admin: false,
        artworks: {
            [tabs.ALL]: []
        },
        artists: {},
        artists_count: 0,
        balance_beam: 0,
        in_tx: false,
        selected_artist: undefined,
        active_tab: tabs.ALL,
        is_popup_visible: false,
        popup_type: null
    }
}

export const store = {
    state: Vue.reactive(defaultState()),

    //
    // Errors
    //
    setError(error, context) {
        this.state.error = {error, context}
        router.push({name: 'gallery'})
    },

    checkError (err) {
        if (err) this.setError(err)
    },

    clearError() {
        this.state.error = undefined
        this.start()
    },

    changePopupState(value) {
        this.state.is_popup_visible = value;
    },

    setPopupType(value) {
        this.state.popup_type = value;
    },

    //
    // Shader, CID, debug helpers
    //
    start () {
        Object.assign(this.state, defaultState())
        router.push({name: 'gallery'})

        Vue.nextTick(() => {
            utils.download("./galleryManager.wasm", (err, bytes) => {
                if (err) return this.setError(err, "Failed to download shader")
                this.state.shader = bytes

                //utils.invokeContract("", (...args) => this.onShowMethods(...args), this.state.shader)
                utils.callApi("ev_subunsub", {ev_txs_changed: true, ev_system_state: true}, (err) => this.checkError(err))
                utils.invokeContract("role=manager,action=view", (...args) => this.onCheckCID(...args), this.state.shader)
            })
        })
    },

    refreshAllData () {
        this.loadParams()
    },

    onCheckCID (err, res) {
        if (err) {
            return this.setError(err, "Failed to verify cid")     
        }

        if (!res.contracts.some(el => el.cid == this.state.cid)) {
            throw `CID not found '${this.state.cid}'`
        }

        utils.invokeContract(
            `role=artist,action=get_key,cid=${this.state.cid}`, 
            (...args) => this.onGetArtistKey(...args), this.shader
        )
    },

    onShowMethods (err, res) {
        if (err) return this.setError(err)
        alert(utils.formatJSON(res))
    },

    //
    // Transactions
    //
    onApiResult(err, res, full) {
        if (err) {
            return this.setError(err,  "API handling error")
        }

        if (full.id == 'ev_txs_changed') {   
            let inTx = false            
            let txs = full.result.txs
            
            for (let tx of txs) {
                if (tx.status == 0 || tx.status == 1 || tx.status == 5) {
                    inTx = true
                    break
                }
            }

            this.state.in_tx = inTx
            return
        }

        if (full.id == 'ev_system_state') {
            // we update our data on each block
            this.refreshAllData()
            return
        }

        this.setError(full, "Unexpected API result")
    },

    //
    // Self info, balance & stuff
    //
    onGetArtistKey(err, res) {
        if (err) {
            return this.setError(err, "Failed to get artist key")     
        }
        
        utils.ensureField(res, "key", "string")
        this.state.my_artist_key = res.key
        this.refreshAllData()
    },

    loadParams () {
        // TODO: move loading sequence to promises, now loadBalance initiates update chain
        //       do not chain functions, but execue promises arrayy
        //       cancel all pending promises in new ev_system_state
        utils.invokeContract(
            `role=manager,action=view_params,cid=${this.state.cid}`, 
            (...args) => this.onLoadParams(...args)
        )
    },

    onLoadParams (err, res) {
        if (err) {
            return this.setError(err, "Failed to load contract params")
        }

        utils.ensureField(res, "Admin", "number")
        this.state.is_admin = !!res.Admin
        this.loadBalance()
    },
    
    loadBalance() {
        utils.invokeContract(
            `role=user,action=view_balance,cid=${this.state.cid}`, 
            (...args) => this.onLoadBalance(...args)
        )

        //if (this.state.is_admin) {
        //    utils.invokeContract(
        //        `role=manager,action=view_balance,cid=${this.state.cid}`, 
        //        (...args) => this.onLoadVotingBalance(...args)
        //    )
        //}
    },

    onLoadRewardsBalance (err, res) {
        if (err) {
            return this.setError(err, "Failed to load voting balance")
        }
    },

    onLoadBalance(err, res) {
        if (err) {
            return this.setError(err, "Failed to load balance")
        }

        // totals can be missing if user has nothing at all
        // also there can be not only beam. We just interested
        // only in beam for now
        let newBeam = 0
        if (res.totals) {
            utils.ensureField(res, "totals", "array")
            for (let item of res.totals) {
                if (item.aid == 0) {
                    newBeam = item.amount
                    break
                }
            }
        }

        this.state.balance_beam = newBeam
        this.loadArtists()
    },

    withdrawBEAM () {
        // TODO: this will not happen in demo but all the amounts might not fit into one block
        //       need to set nMaxCount to 100 max and reflet that in UI
        //       AND
        //       withdrawal in bulk negates privacy. Need to ask user what to do - in bulk
        //       or for each piece separately
        utils.invokeContract(
            `role=user,action=withdraw,nMaxCount=0,cid=${this.state.cid}`, 
            (...args) => this.onMakeTx(...args)
        )
    },

    //
    // Artists
    //
    loadArtists () {
        utils.invokeContract(
            `role=manager,action=view_artists,cid=${this.state.cid}`, 
            (...args) => this.onLoadArtists(...args)
        )
    },

    onLoadArtists(err, res) {
        if (err) {
            return this.setError(err, "Failed to load artists list")
        }

        utils.ensureField(res, "artists", "array")
        //
        // OPTIMIZE: 
        //       code below is not optimized, need to ask Vlad  if it is possible for contract/app to
        //       return artists old artists before new. In this case we can skip artists_count in res.artists
        //
        //       for (let idx = this.state.artists_count; idx < res.artists.length; ++idx) {
        //          let artist = res.artists[idx]
        //          if (artist.key == this.state.my_artist_key) {
        //            this.state.is_artist = true
        //          }
        //          this.state.artists[artist.key] = artist
        //       }
        //
        if (this.state.artists_count != res.artists.length) {
            if (!this.state.selected_artist) {
                // choose first artist for admin if nobody is selected
                this.state.selected_artist = {
                    key: res.artists[0].key,
                    label: res.artists[0].label
                }
            }

            for (let artist of res.artists) {
                if (artist.key == this.state.my_artist_key) {
                    this.state.is_artist = true
                }
                this.state.artists[artist.key] = artist
            }
        } 
        // END OF NOT OPTIMIZED
        this.state.artists_count = res.artists.length
        this.loadArtworks()
    },

    //
    // Artworks
    //
    hexEncodeU8A (arr) {
        return arr.reduce((str, byte) => str + byte.toString(16).padStart(2, '0'), '')
    },

    hexDecodeU8A (str) {
        return new Uint8Array(str.match(/.{1,2}/g).map(byte => parseInt(byte, 16)));
    },

    loadArtworks () {
        utils.invokeContract(
            `role=user,action=view_all,cid=${this.state.cid}`, 
            (...args) => this.onLoadArtworks(...args)
        )    
    },

    onLoadArtworks (err, res) {
        if (err) {
            return this.setError(err, "Failed to load artwork list")
        }
    
        utils.ensureField(res, "items", "array")
        let all = [], sale = [], liked = [], mine = [], sold = []

        for (const artwork of res.items) {
            // TODO: remove if < 2, this is for test only
            if (artwork.id < 3) continue

            // just for convenience, to make more clear what pk is
            artwork.pk_owner = artwork.pk
            delete artwork.pk
        
            if (artwork["price.aid"] != undefined) {
                artwork.price = {
                    aid: artwork["price.aid"],
                    amount: artwork["price.amount"]
                }
            }

            let oldArtwork = this.state.artworks[tabs.ALL].find((item) => {
                return item.id == artwork.id;
            });

            if (oldArtwork) {
                artwork.title = oldArtwork.title;
                artwork.bytes = oldArtwork.bytes;
                artwork.pk_author = oldArtwork.pk_author;
            }

            all.push(artwork)
            let mykey = this.state.my_artist_key

            // MINE is what I own
            if (artwork.owned) {
                mine.push(artwork)
            }

            // SALE is what OWN && what has price set
            if (artwork.owned && artwork.price) {
                sale.push(artwork)
            }
            
            // LIKED is waht I've liked
            if (artwork.my_impression) {
                liked.push(artwork)
            }
            
            // SOLD - I'm an author but I do not own
            if (artwork.pk_author == mykey && !artwork.owned) {
                sold.push(artwork)
            }

            if (!oldArtwork) {
                this.loadArtwork(all.length - 1, artwork.id);
            }
        }
        
        // TODO: need to find a way to optimize this
        this.state.artworks[tabs.ALL]   = all
        this.state.artworks[tabs.SALE]  = sale
        this.state.artworks[tabs.LIKED] = liked
        this.state.artworks[tabs.MINE]  = mine
        this.state.artworks[tabs.SOLD]  = sold
        this.state.loading = false;
    },

    setActiveTab(id) {
        this.state.active_tab = id;
    },

    loadArtwork(idx, id) {
        utils.invokeContract(
            `role=user,action=download,cid=${this.state.cid},id=${id}`, 
            (err, res) => this.onLoadArtwork(err, res, idx)
        )  
    },

    onLoadArtwork(err, res, idx) {
        if (err) {
            return this.setError(err, "Failed to download an artwork")
        }

        utils.ensureField(res, "artist", "string")
        let pk_author = res.artist

        utils.ensureField(res, "data", "string")
        var data = this.hexDecodeU8A(res.data)

        // check version
        if (data[0] != 1) {
            throw `Invalid format version: ${data[0]}`
        }

        // parse name
        let nend = data.findIndex(val => val == 0)
        if (nend == -1 || nend + 1 == data.length) {
            throw "Unable to parse image name"
        }

        let rawName = data.subarray(1, nend)
        let name = (new TextDecoder()).decode(rawName)

        // parse bytes
        let bytes = data.subarray(nend + 2)

        // save parsed data
        // list may have been changed, so we check if artwork with this id is still present
        let artwork = this.state.artworks[tabs.ALL][idx]
        if (artwork && artwork.id) {
            artwork.title = name
            artwork.bytes = bytes
            artwork.pk_author = pk_author

            // We receive author only now, so add what's missing to SOLD
            let mykey = this.state.my_artist_key
            if (artwork.pk_author === mykey && !artwork.owned) {
                this.state.artworks[tabs.SOLD].push(artwork)
            }
        }
    },

    //
    // Artwork actions, Buy, Sell, Like &c.
    //
    buyArtwork (id) {
        utils.invokeContract(
            `role=user,action=buy,id=${id},cid=${this.state.cid}`, 
            (...args) => this.onMakeTx(...args)
        )
    },

    sellArtwork (id, price) {
        utils.invokeContract(
            `role=user,action=set_price,id=${id},amount=${price},aid=0,cid=${this.state.cid}`, 
            (...args) => this.onMakeTx(...args)
        )
    },

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

    onMakeTx (err, sres, full) {
        if (err) {
            return this.setError(err, "Failed to generate transaction request")
        }

        utils.ensureField(full.result, "raw_data", "array")
        utils.callApi(
            'process_invoke_data', {data: full.result.raw_data}, 
            (...args) => this.onSendToChain(...args)
        )
    },

    onSendToChain(err, res) {
        if (err) {
            if (utils.isUserCancelled(err)) return
            return this.setError(err, "Failed to create transaction")
        }
        utils.ensureField(res, "txid", "string")
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
                let hex   = [this.hexEncodeU8A(aver), 
                             this.hexEncodeU8A(aname), 
                             this.hexEncodeU8A(asep), 
                             this.hexEncodeU8A(aimg)
                            ].join('')
                        
                utils.invokeContract(`role=manager,action=upload,cid=${this.state.cid},pkArtist=${artist_key},data=${hex}`, 
                    (...args) => this.onMakeTx(...args)
                )
            }
        }
        catch(err) {
            this.setError(err, "Failed to upload artwork")
        }
    },
}
