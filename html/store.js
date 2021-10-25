import {router} from './router.js'
import utils from './utils/utils.js'

const defaultState = () => {
    return {
        loading: true,
        error: undefined,
        shader: undefined,
        cid: "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609",
        changed_txs: undefined,
        artist_key: "",
        is_artist: false,
        artworks: [], // TODO: Refresh artworks & artists by timer, 1min should be OK
        artists: {} 
    }
}

export const store = {
    state: Vue.reactive(defaultState()),

    //
    // Errors
    //
    setError(error, context) {
        this.state.error = {error, context}
    },

    checkError (err) {
        if (err) this.setError(err)
    },

    clearError() {
        this.state.error = undefined
        this.start()
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

                //utils.invokeContract("", (...args) => this.onShowMethods(...args))
                utils.callApi("ev_subunsub", {ev_txs_changed: true}, (err) => this.checkError(err))
                utils.invokeContract("role=manager,action=view", (...args) => this.onCheckCID(...args), this.state.shader)
            })
        })
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
            let txs = full.result.txs
            let changed = []

            for (let tx of txs) {
                if (tx.status == 2 || tx.status == 3 || tx.status == 4) {
                    changed.push(tx.txId)
                }
            }

            if (changed.length) {
                this.state.changed_txs = changed
            }
            
            return
        }

        this.setError(full, "Unexpected API result")
    },

    //
    // Artists
    //
    onGetArtistKey(err, res) {
        if (err) return this.setError(err, "Failed to get artist key")     
        
        utils.ensureField(res, "key", "string")
        this.state.artist_key = res.key
        this.loadArtists()
    },

    loadArtists () {
        utils.invokeContract(
            `role=manager,action=view_artists,cid=${this.state.cid}`, 
            (...args) => this.onLoadArtists(...args)
        )
    },

    onLoadArtists(err, res) {
        if (err) return this.setError(err, "Failed to load artists list")

        utils.ensureField(res, "artists", "array")

        let artists = {}
        for (let artist of res.artists) {
            if (artist.key == this.state.artist_key) {
                this.state.is_artist = true
            }
            artists[artist.key] = artist
        }

        this.state.artists = artists
        this.loadArtworks()
    },

    
    //
    // Artworks
    //
    loadArtworks () {
        utils.invokeContract(
            // TODO: do we need id=1 here?
            `role=user,action=view_all,cid=${this.state.cid},id=1`, 
            (...args) => this.onLoadArtworks(...args)
        )    
    },

    onLoadArtworks (err, res) {
        if (err) {
            return this.setError(err, "Failed to load artwork list")
        }
    
        // TODO: load transactions and check if any item is in_tx
        utils.ensureField(res, "items", "array")
    
        let artworks = []
        for (const artwork of res.items) {
            // TODO: remove if, this is for test only
            if (artwork.id < 12) continue
        
            if (artwork["price.aid"] != undefined) {
                artwork.price = {
                    aid: artwork["price.aid"],
                    amount: artwork["price.amount"]
                }
            }

            // TODO: optimize
            let found = false
            for (let idx = 0; idx < this.state.artworks.length; ++idx) {
                let old = this.state.artworks[idx]
                if (old.id == artwork.id) {
                    artwork.title = old.title
                    artwork.bytes = old.bytes
                    found = true
                }
            }

            // TODO: load transactions and check if any item is in_tx
            artwork.in_tx = false
            artworks.push(artwork)

            if (!found) {
                this.loadArtwork(artworks.length - 1, artwork.id)
            }   
        }
        
        // TODO: update existing artworks, not replace
        this.state.artworks = artworks
        this.state.loading = false 
    },

    loadArtwork(idx, id) {
        utils.invokeContract(
            `role=user,action=download,cid=${this.state.cid},id=${id}`, 
            (err, res) => this.onLoadArtwork(err, res, idx, id)
        )  
    },

    // TODO: check if we need to pass id
    onLoadArtwork(err, res, idx, id) {
        if (err) {
            return this.setError(err, "Failed to download an artwork")
        }

        utils.ensureField(res, "data", "string")
        var data = utils.hexDecodeU8A(res.data)

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
        let artwork = this.state.artworks[idx]
        if (artwork && artwork.id) {
            artwork.title = name
            artwork.bytes = bytes
        }
    },

    //
    // Buy & Sell
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

    onMakeTx (err, sres, full) {
        if (err) {
            return this.$root.setError(err, "Failed to generate transaction request")
        }

        utils.ensureField(full.result, "raw_data", "array")
        //utils.callApi(
        //    'process_invoke_data', {data: full.result.raw_data}, 
        //     (...args) => this.onSendToChain(...args)
        //)
    },

    onSendToChain(err, res) {
        if (err) {
            if (utils.isUserCancelled(err)) return
            return this.$root.setError(err, "Failed to create transaction")
        }

        // TODO: check that artwork becomes in_tx
        //for (let item of this.items) {
        //    if (item.id == id) {
        //        item.in_tx = true
        //    }
        //}
    }
}
