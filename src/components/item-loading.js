import html from '../utils/html.js'

export default {
    props: {
        error: {
            type: Boolean,
            default: false,
            required: false
        }
    },

    render () {
        return html`
            <div class="loading darker">
                ${ this.error ? 'Failed to load artwork' : 'Loading...'}
            </div>
        `
    },
}
