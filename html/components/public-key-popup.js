import html from '../utils/html.js';
import { common, popups } from '../utils/consts.js';
import utils from '../utils/utils.js';
import artButton from './art-button.js';

export default {
    props: {

    },

    components: {
        artButton
    },

    computed: {
        in_tx () {
            return this.$state.in_tx;
        },
        popup_type () {
            return this.$state.popup_type;
        },
        display_artist_key () {
            return this.$state.my_artist_keys[0];
        },
        id_to_sell () {
            return this.$state.id_to_sell;
        }
    },

    render () {
        return html`
            <div class="popup" id="popup">
                <div class="popup__content-${this.popup_type === popups.KEY ? 'key' : 'sell'}" 
                id="popup-content">
                    ${this.popup_type === popups.KEY 
                        ? this.renderKeyContent() 
                        : this.renderSellContent()}
                </div>
            </div>
        `;
    },

    mounted: function () {
        this.applyStyles();

        const sellInput = document.getElementById('sell-input');
        if (sellInput) {
            document.getElementById('sell-input').addEventListener('keydown', (event) => {
                const specialKeys = [
                    'Backspace', 'Tab', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp',
                    'Control', 'Delete', 'F5'
                ];

                if (specialKeys.indexOf(event.key) !== -1) {
                    return;
                }

                const current = document.getElementById('sell-input').value;
                const next = current.concat(event.key);
            
                if (!utils.handleString(next)) {
                    event.preventDefault();
                }
            })

            document.getElementById('sell-input').addEventListener('paste', (event) => {
                if (event.clipboardData !== undefined) {
                    const text = event.clipboardData.getData('text');
                    if (!utils.handleString(text)) {
                        event.preventDefault();
                    }
                }
            });
        }
    },

    methods: {
        applyStyles() {
            const { background_main, background_popup } = utils.getStyles();
            
            document.getElementById('popup').style.backgroundColor = background_main + '40';
            document.getElementById('popup-content').style.backgroundColor = background_popup;

        },

        renderKeyContent() {
            return html`
                <div class="popup__content-key__title">
                    Public key
                </div>

                <div class="popup__content-key__data">
                    <span>${this.display_artist_key}</span>
                    <img class="copy-icon" onclick=${this.onCopy} src="./assets/icon-copy.svg"/>
                </div>

                <div class="popup__content-key__controls">
                    <${artButton} 
                    onclick=${this.onClose} 
                    type="close"/>

                    <${artButton} 
                    data=${this.display_artist_key}
                    onclick=${this.onCopy} 
                    type="copy"/>
                </div>
            `;
        },

        renderSellContent() {
            return html`
                <div class="popup__content-sell__title">
                    Set the price
                </div>

                <div class="popup__content-sell__input">
                    <input type="text"
                    id="sell-input"
                    class="popup__content-sell__input__elem" 
                    placeholder="0"/>
                    <span class="popup__content-sell__input__text">BEAM</span>
                </div>

                <div class="popup__content-sell__fee">
                    <span class="popup__content-sell__fee__title">Fee</span>
                    <span class="popup__content-sell__fee__value">0.011 BEAM</span>
                </div>

                <div class="popup__content-sell__info">Small transaction fee must be paid</div>

                <div class="popup__content-sell__controls">
                    <${artButton}
                    onclick=${this.onClose} 
                    type="cancel"/>

                    <${artButton} 
                    onclick=${this.onProceed}
                    type="proceed"/>
                </div>
            `;
        },

        onClose() {
            this.$store.changePopupState(false);
        },

        onProceed() {
            const current = document.getElementById('sell-input').value;
            const price = parseFloat(current) * common.GROTHS_IN_BEAM;
            this.$store.sellArtwork(this.id_to_sell, price);
            this.onClose();
        },

        onCopy() {
            var textArea = document.createElement("textarea");
            textArea.style.position = "fixed";
            textArea.value = this.display_artist_key;
            document.body.appendChild(textArea);
            textArea.focus();
            textArea.select();
            this.onClose();
            try {
                return document.execCommand("copy");
            } catch (ex) {
                return false;
            } finally {
                document.body.removeChild(textArea);
            }
        }
    }
}
