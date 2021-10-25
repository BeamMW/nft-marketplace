import html from '../utils/html.js'

export default {
    render () {
        return html`
            <div class="tx-warning">
                You cannot withdraw, sell or by while transaction is in progress. Please wait...
            </div>
        `
    },
}
