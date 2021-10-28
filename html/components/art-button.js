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

    },

    render () {
        return html`
            <div class="button ${this.type}">
                <img src="./assets/icon-button.svg"/>
                <span class="button__text">${this.type}</span>
            </div>
        `
    },

    methods: {

    }
}
