import html from '../utils/html.js';
import { common, popups } from '../utils/consts.js';

export default {
    computed: {
        balance_beam () {
            return this.$state.balance_beam / common.GROTHS_IN_BEAM;
        },
        my_artist_name () {
            let artist = this.$state.artists[this.$state.my_artist_keys[0]] ||
                         this.$state.artists[this.$state.my_artist_keys[1]]
            return (artist || {}).label
        },
        in_tx () {
            return this.$state.in_tx
        },
        show_balance () {
            return this.$state.is_artist || this.$state.balance_beam
        }
    },

    render () {
        return html`
            <div class="balance-container">
                <div class="balance ${this.show_balance ? '' : 'hidden'}">
                    <img src="./assets/icon-beam.svg"/>
                    <div class="balance-value">
                        ${this.balance_beam} BEAM
                    </div>
                    ${this.renderWithdraw()}
                </div>
                <div class="user-opt">
                    ${this.renderUserName()}
                    <div class="user-opt__key" onclick=${this.onShowKey}>
                        <img class="user-opt__key__icon" src="./assets/icon-key.svg"/>
                        <span class="user-opt__key__text">Show my public key</span>
                    </div>
                </div>
            </div>
        `
    },

    methods: {
        renderWithdraw () {
            if (!this.balance_beam || this.in_tx) {
                return ""
            }
    
            return html`
                <div onclick=${this.onWithdraw} class="balance-withdraw">
                    <img class="balance-withdraw__icon" src="./assets/icon-receive.svg"/>
                    <span class="balance-withdraw__text">withdraw</span>
                </div>
            `
        },

        renderUserName () {
            if (this.my_artist_name) {
                return html`
                <div class="user-opt__user" onclick=${this.onShowKey}>
                    <img class="user-opt__user__icon" src="./assets/icon-user.svg"/>
                    <span class="user-opt__user__text">${this.my_artist_name}</span>
                </div>
                `;
            }
        },
        
        onShowKey () {
            this.$store.setPopupType(popups.KEY);
            this.$store.changePopupState(true);
        },

        onWithdraw () {
            this.$store.withdrawBEAM()
        }
    }
}
