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
        this.shader = undefined
        this.cid = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
    },

    data () {
        return {
            name: "value"
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
                utils.invokeContract("role=manager,action=view", (...args) => this.onCheckCID(...args), this.shader)
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

            this.$router.push({
                name: "assets", 
            })
        },

        onShowMethods (err, res) {
            if (err) {
                return this.setError(err)
            }
            alert(utils.formatJSON(res))
        }
    }
}
