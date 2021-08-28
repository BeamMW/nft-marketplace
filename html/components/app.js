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
        this.shader = undefined
        this.cid    = "0e982209bf4202075fa4c4acd5b43e7b559112e5b7ffe5b78f8ebd88e3c07609"
    },

    data () {
        return {
            error: ""
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
                    ${this.error}
                </div>
            `
        },

        start () {
            //
            // adjust styles
            //
            const style = document.createElement('style');
            style.innerHTML = `.error {color: ${this.style.validator_error};}`
            document.head.appendChild(style)
            document.body.style.color = this.style.content_main

            //
            // Start real thing
            //
            utils.download("./galleryManager.wasm", (err, bytes) => {
                if (err) return this.setError(err, "Shader download")
                this.shader = bytes
                utils.invokeContract("checkCID", "role=manager,action=view", this.shader)
            })
        }, 

        setError (err, context) {
            this.error = [context || "Error occured",  utils.formatJSON(err)].join(':\n')             
            this.restart()
        }
    },

    render (...args) {
        if (this.error) return this.renderError(...args)
        return this.renderApp(...args)
    },
}
