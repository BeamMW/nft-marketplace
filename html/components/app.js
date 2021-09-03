import html from '../utils/html.js'
import utils from '../utils/utils.js'

export default {
    props: {
        beam: {
            type: Object,
            required: true
        },
        changed_tx: {
            type: String,
            required: false,
            default: "defval"
        }
    },

    created () {
        this.shader = undefined
        this.cid = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
        this.check_txs = false
    },

    data () {
        return {
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

    template: `
        <router-view></router-view>
    `,

    methods: {
        setError (error, context) {
            this.$router.push({
                name: "error", 
                params: {
                    error: utils.formatJSON(error),
                    context
                }
            })
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
                if (!this.check_txs) {
                    return
                }
                
                let txs = full.result.txs
                let changed = []

                for (let tx of txs) {
                    if (tx.status == 2 || tx.status == 3 || tx.status == 4) {
                        changed.push(tx.txId)
                    }
                }

                if (changed.length) {
                    this.$router.push({
                        name: "assets", 
                        params: {
                            changed_txs: changed
                        }
                    })
                }

                return
            }

            this.setError(full, "Unexpected API result")
        },
        
        onCheckCID (err, res) {
            if (err) {
                return this.setError(err)     
            }
 
            if (!res.contracts.some(el => el.cid == this.cid)) {
                throw `Failed to verify cid '${this.cid}'`
            }

            this.$router.push({
                name: "assets"
            })
            this.check_txs = true
        },

        onShowMethods (err, res) {
            if (err) {
                return this.setError(err)
            }
            alert(utils.formatJSON(res))
        },

        checkError(err) {
            if (err) return this.setError(err)
        }
    }
}
