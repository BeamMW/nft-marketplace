import html    from '../utils/html.js';
import loading from './loading.js';
import dot     from './dot.js';
import { common } from '../utils/consts.js';

export default {
    props: {
        id: {
            type: Number,
            required: true
        },
        author: {
            type: String,
            required: false
        },
        title: {
            type: String,
            required: true,
            default: ""
        },
        bytes: {
            type: Object,
            default: []
        },
        owned: {
            type: Number,
            default: 0
        },
        price: {
            type: Object,
            default: undefined
        },
        in_tx: {
            type: Boolean,
            default: false
        }
    },

    emits: ['buy', 'sell'],

    computed: {
        ownerText () {
            let res = "";
            if (this.author) {
                res = ["by", this.author].join(' ');
            }
            if (this.owned) {
                return res += (res ? " | " : "") + "You own this artwork";
            }
            return res ? res : "Loading...";
        },
    },

    render () {
        return html`
            <div class="artwork">
                ${this.renderPreview()}
                <div class="info-row">
                    <span class="artwork-title">${this.renderDot()}<span>${this.title || "Loading..."}</span></span>
                </div>
                <div class="info-row">
                    <span class="artwork-owner">${this.ownerText}</span>
                </div>
                <div class="info-row ${this.price ? '' : 'hidden'}">
                    ${this.renderPrice()}
                </div>
            </div>
        `
    },

    methods: {
        renderPreview() {
            if (this.bytes.length) {
                let image = URL.createObjectURL(new Blob([this.bytes], {type: 'image/jpeg'}))
                return html`
                    <div class="preview-container">
                        <img src=${image}/>
                    </div>
                `
            } else {
                return html`
                    <div class="preview-container">
                        <${loading}/>
                    </div>
                `
            }
        },

        renderDot() {
            if (!this.owned) {
                return ""
            }

            let state = 'green'
            return html`<${dot} state=${state}/>`
        },
        
        renderPrice() {
            // if has price - just display it and return
            if (this.price) {
                let amount = this.price.amount / common.GROTHS_IN_BEAM;
                
                // TODO: if owned && has price should be able to cancel or change price
                //       probably not for the first version
                if (this.owned) {
                    return html`<span class="normal bold">${amount} BEAM</span>`
                }
                
                // if not owned can only buy
                return html`
                    <span>
                        <a href="#" onclick=${this.onBuy} class="buy ${this.in_tx ? 'darker' : ''}">BUY</a>
                        <span class="normal bold ml-hem">${amount} BEAM</span>
                    </span>
                `
            }

            // doesn't have price and is own image
            if (this.owned) {
                return html`<a href="#" class="buy ${this.in_tx ? 'darker' : ''}" onclick=${this.onSell}>SELL</a>`
            }

            // doesn't have price and is not own image
            // means can be anything - not approved yet, not sold by
            // owner &c. Just dispaly that it is not on sale
            return html`<span class='small darker'>Not on sale</span>`
        },

        onBuy (ev) {
            ev.preventDefault()
            if (!this.in_tx) {
                this.$emit('buy', this.id)
            }
        },

        onSell (ev) {
            ev.preventDefault()
            if (!this.in_tx) {
                this.$emit('sell', this.id)
            }
        }
    }
}
