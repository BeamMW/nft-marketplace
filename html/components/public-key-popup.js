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
        my_artist_key () {
            return this.$state.my_artist_key
        },
    },

    render () {
        return html`
            <div class="popup" id="popup">
                ${this.popup_type === popups.KEY ? this.renderKeyContent() : ''}
            </div>
        `;
    },

    mounted: function () {
        this.applyStyles();
    },

    methods: {
        applyStyles() {
            const { background_main, background_popup } = utils.getStyles();
            
            document.getElementById('popup').style.backgroundColor = background_main + '40';
            document.getElementById('popup-content').style.backgroundColor = background_popup;

        },

        renderKeyContent() {
            return html`
                <div class="popup__content-key" id="popup-content">
                    <div class="popup__content-key__title">
                        Public key
                    </div>

                    <div class="popup__content-key__data">
                        <span>${this.my_artist_key}</span>
                        <img class="copy-icon" onclick=${this.onCopy} src="./assets/icon-copy.svg"/>
                    </div>

                    <div class="popup__content-key__controls">
                        <${artButton} 
                        data=${this.my_artist_key}
                        onclick=${this.onClose} 
                        type="close"/>

                        <${artButton} 
                        data=${this.my_artist_key}
                        onclick=${this.onCopy} 
                        type="copy"/>
                    </div>
                </div>
            `;
        },

        onClose() {
            this.$store.changePopupState(false);
        },

        onCopy() {
            var textArea = document.createElement("textarea");
            textArea.style.position = "fixed";
            textArea.value = this.my_artist_key;
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
