import {router} from './router.js'
import utils from './utils/utils.js'

/*
export const store = Vuex.createStore({

    

    
})

store.setError = function (error, context) {
    this.dispatch('setError', {error, context})
}

*/

const defaultState = () => {
    return {
        loading: true,
        error: undefined,
        shader: undefined,
        cid: "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609",
        changed_txs: undefined,
        artist_key: "",
    }
}

export const store = {
    state: Vue.reactive(defaultState()),

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

    start () {
        Object.assign(this.state, defaultState())
        router.push({name: 'main'})
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

    onGetArtistKey(err, res) {
        if (err) return this.setError(err, "Failed to get artist key")     
        
        utils.ensureField(res, "key", "string")
        this.state.artist_key = res.key
        this.state.loading = false
    },

    onShowMethods (err, res) {
        if (err) return this.setError(err)
        alert(utils.formatJSON(res))
    },

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
}
