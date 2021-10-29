import html from '../utils/html.js';
import { common } from '../utils/consts.js';

export default {
    props: {
        type: {
            type: String,
            required: true
        },
    },

    computed: {
        in_tx () {
            return this.$state.in_tx;
        },
    },

    render () {
        return html`
            <div class="button ${this.type} ${this.in_tx ? 'disabled' : ''}">
                <img src="./assets/icon-button.svg"/>
                <span class="button__text">${this.type}</span>
            </div>
        `
    },

    methods: {

    }
}
