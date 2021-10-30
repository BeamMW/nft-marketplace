import html from '../utils/html.js'

export default {
    render () {
        return html`
            <div class="tx-warning">
                You cannot withdraw, sell, buy or like while transaction is in progress. Please wait...
            </div>
        `
    },
}
