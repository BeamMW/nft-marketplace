import html from '../utils/html.js';
import { common } from '../utils/consts.js';

export default {
    props: {
        type: {
            type: String,
            required: true
        },
        data: {
            type: String
        }
    },

    computed: {
        in_tx () {
            return this.$state.in_tx;
        },
    },

    render () {
        if (this.type === 'sell' || this.type === 'buy') {
            return html`
                <div class="button ${this.type} ${this.in_tx ? 'disabled' : ''}">
                    <img src="./assets/icon-button.svg"/>
                    <span class="button__text">${this.type}</span>
                </div>
            `;
        } else if (this.type === 'close') {
            return html`
                <div class="button ${this.type}">
                    <img src="./assets/icon-cancel.svg"/>
                    <span class="button__text">close</span>
                </div>
            `;
        } else if (this.type === 'copy') {
            return html`
                <div class="button ${this.type}">
                    <img src="./assets/icon-copy-blue.svg"/>
                    <span class="button__text">copy and close</span>
                </div>
            `;
        }
    },

    methods: {

    }
}
