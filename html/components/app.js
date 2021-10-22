import html    from '../utils/html.js'
import utils   from '../utils/utils.js'
import assets  from './assets.js'
import loading from './loading.js'
import error   from './error.js'

export default {
    props: {
        beam: {
            type: Object,
            required: true
        }
    },

    created () {
        this.shader = undefined
        this.cid    = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
    },

    data () {
        return {
            loading: true,
            artist_key: "",
            changed_txs: [],
            error: undefined
        }
    },

    computed: {
        style () {
            return this.beam ? this.beam.style : undefined
        },
        api () {
            return this.beam ? this.beam.api : undefined
        }
    },

    render () {
        if (this.error) {
            return html`
                <${error} 
                    error=${this.error.error} 
                    context=${this.error.context}
                    onClearError=${this.clearError}
                />`
        }

        if (this.loading) {
            return html`<${loading}/>`
        }

        return html`
            <${assets} cid=${this.cid} artist_key=${this.artist_key} changed_txs=${this.changed_txs}/>
        `
    },

    methods: {
        setError (error, context) {
            this.error = {error, context}
            if (!!this.error) this.start()
        },     
        
        clearError() {
            this.setError(undefined, undefined)
        },

        start () {
            // adjust styles
            const style = document.createElement('style');
            style.innerHTML = `.error {color: ${this.style.validator_error};}`
            document.head.appendChild(style)
            document.body.style.color = this.style.content_main

            // Start real thing
            utils.download("./galleryManager.wasm", (err, bytes) => {
                if (err) return this.setError(err, "Shader download")
                this.shader = bytes
                //utils.invokeContract("", (...args) => this.onShowMethods(...args))
                utils.callApi("ev_subunsub", {ev_txs_changed: true}, (...args) => this.checkError(...args))
                utils.invokeContract("role=manager,action=view", (...args) => this.onCheckCID(...args), this.shader)
            })
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
                    this.changed_txs = changed
                }
                
                return
            }

            this.setError(full, "Unexpected API result")
        },
        
        onCheckCID (err, res) {
            if (err) {
                return this.setError(err, "Failed to verify cid")     
            }
 
            if (!res.contracts.some(el => el.cid == this.cid)) {
                throw `CID not found '${this.cid}'`
            }

            utils.invokeContract(
                `role=artist,action=get_key,cid=${this.cid}`, 
                (...args) => this.onGetArtistKey(...args), this.shader
            )
        },

        onGetArtistKey(err, res) {
            if (err) return this.setError(err, "Failed to get artist key")     
            
            utils.ensureField(res, "key", "string")
            this.artist_key = res.key
            this.loading = false
        },

        onShowMethods (err, res) {
            if (err) return this.setError(err)
            alert(utils.formatJSON(res))
        },

        checkError(err) {
            if (err) return this.setError(err)
        }
    }
}
