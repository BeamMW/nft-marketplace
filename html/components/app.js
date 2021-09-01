import html from '../utils/html.js'
import utils from '../utils/utils.js'

export default {
    props: {
        beam: {
            type: Object,
            required: true
        }
    },

    created () {
        // we do not need reactivity on data below
        this.shader  = undefined
        this.cid     = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
    },

    data () {
        return {
            error: "",
            errleft: 0
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

    methods: {
        renderApp () {
            return html`
                <div class="loading">
                    Loading...
                </div>
            `
        },

        renderError () {
            return html`
                <div class="error">
                    <div>
                        <pre>${this.error}</pre>
                        <span class="restart">Restarting in ${this.errleft}</span>
                    </div>
                </div>
            `
        },

        setError (err, context) {
            this.error = [context || "Error occured",  utils.formatJSON(err)].join(':\n')             
            this.restart()
        },

        restart (now) {
            if (this.timeout) {
                clearInterval(this.timeout)
            }
            
            this.errleft = now ? 0 : 3
            this.timeout = setInterval(() => {
                this.errleft -= 1
                if (this.errleft == 0) {
                    clearInterval(this.timeout)
                    this.start()
                }
            }, now ? 0 : 1000)
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
                utils.invokeContract("checkCID", "role=manager,action=view", this.shader)
            })
        }, 

        onApiResult(err, res, full) {
            if (err) {
                return this.setError(err,  "API handling error")
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

            utils.invokeContract("showMethods")
        },

        onShowMethods (err, res) {
            if (err) {
                return this.setError(err)
            }
            alert(utils.formatJSON(res))
        }
    },

    render (...args) {
        if (this.error) return this.renderError(...args)
        return this.renderApp(...args)
    },
}
