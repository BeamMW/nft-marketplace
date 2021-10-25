import html from '../utils/html.js'

export default {
    computed: {
        balance_beam () {
            return this.$state.balance_beam
        },
        artist_key () {
            return this.$state.artist_key
        },
        in_tx () {
            return this.$state.in_tx
        }
    },

    render () {
        return html`
            <div class="balance-container">
                <div class="balance">
                    <div class="balance-value">
                        ${this.balance_beam} GROTH
                    </div>
                    ${this.renderWithdraw()}
                </div>    
                <a href="#" onclick=${this.onShowKey} class="show-key">
                    Show my public key
                </a>
            </div>
        `
    },

    methods: {
        renderWithdraw () {
            if (!this.balance_beam || this.in_tx) {
                return ""
            }
    
            return html`
                <a href="#" onclick=${this.onWithdraw} class="show-key">
                    withdraw
                </a>
            `
        },

        onShowKey () {
            alert(this.artist_key)
        },

        onWithdraw () {
            this.$store.withdrawBEAM()
        }
    }
}
