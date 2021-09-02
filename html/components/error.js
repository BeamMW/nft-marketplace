import html from '../utils/html.js'
import utils from '../utils/utils.js'

export default {
    props: {
        error: {
            type: String,
            default: ""
        },
        context: {
            type: String,
            default: "Error occured"
        }
    },

    data () {
        return {
            errleft: 0
        }
    },

    created () {
        this.errleft = 5
        this.timeout = setInterval(() => {
            this.errleft -= 1
            if (this.errleft == 0) {
                clearInterval(this.timeout)
                this.$router.push({
                    name: "root", 
                })
                this.$root.start()
            }
        }, 1000)
    },

    computed: {
        errorText () {
            return [this.context, this.error].join(':\n')
        }
    },

    render (...args) {
        return html`
            <div class="error">
                <div>
                    <pre>${this.errorText}</pre>
                    <span class="restart">Restarting in ${this.errleft}</span>
                </div>
            </div>
        `
    }
}
