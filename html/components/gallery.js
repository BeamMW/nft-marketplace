import html from '../utils/html.js';
import { common } from '../utils/consts.js';

export default {
    computed: {
        my_key () {
            return this.$state.my_artist_key;
        }
    },

    components: {
    },

    render() { 
        return html`<div class="coming-soon">
            <div class="coming-soon__title">
                NFT Gallery is coming soon…
            </div>
            <img class="coming-soon__icon" src="./assets/coming-soon.svg"/>
            <div class="coming-soon__text"> 
                <div>Your public key:</div>
                <div class="cp-div">${this.my_key} <img onclick=${this.onCopy} class="cp" src="./assets/icon-copy.svg"/></div> 
            </div>
        </div>`;
    },

    methods: {
        onCopy() {
            var textArea = document.createElement("textarea");
            textArea.style.position = "fixed";
            textArea.value = this.my_key;
            document.body.appendChild(textArea);
            textArea.focus();
            textArea.select();
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
