import html    from '../utils/html.js'
import loading from './loading.js'
import dot     from './dot.js'

export default {
    props: {
        id: {
            type: Number,
            required: true
        },
        artist: {
            type: String,
            required: false
        },
        title: {
            type: String,
            required: true,
            default: "Loading..."
        },
        bytes: {
            type: Object,
            default: undefined
        },
        owned: {
            type: Number,
            default: 0
        },
        approved: {
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
            if (this.artist) return this.artist
            if (this.owned) return "You own this item"
            return "Somebody"
        },
    },

    render () {
        return html`
            <div class="item">
                ${this.renderPreview()}
                <div class="info-row">
                    <span class="small darker">${this.ownerText}</span>
                    <span class="small darker ${this.price && !this.in_tx ? '' : 'hidden'}">Price</span>
                </div>
                <div class="info-row">
                    <span>${this.renderDot()}<span class="normal bolder">${this.title}</span></span>
                    ${this.renderPrice()}
                </div>
            </div>
        `
    },

    methods: {
        renderPreview() {
            if (this.bytes) {
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
            if (!this.approved) state = "red"

            return html`<${dot} state=${state}/>`
        },
        
        renderPrice() {
            if (this.in_tx) {
                return html`<span class='small darker'>Transaction in progress</span>`
            }

            // if has price - just display it and return
            if (this.price) {
                let amount = this.price.amount / 100000000
                
                // if owned can cancel & change
                if (this.owned) {
                    return html`<span class="normal bold">${amount} BEAM</span>`
                }
                
                // if not owned can only buy
                return html`
                    <span>
                        <a href="#" onclick=${this.onBuy} class="buy">BUY</a>
                        <span class="normal bold ml-hem">${amount} BEAM</span>
                    </span>
                `
            }

            // doesn't have price and is own image
            if (this.owned) {
                // if already approved then can sell
                if (this.approved) {
                    return html`<a href="#" class="buy" onclick=${this.onSell}>SELL</a>`
                } 
                // otherwise wait for approval
                return html`<span class='small darker'>Waiting for approval</span>`
            }

            // doesn't have price and is not own image
            // means can be anything - not approved yet, not sold by
            // owner &c. Just dispaly that it is not on sale
            return html`<span class='small darker'>Not on sale</span>`
        },

        onBuy (ev) {
            ev.preventDefault()
            this.$emit('buy', this.id)
        },

        onSell (ev) {
            ev.preventDefault()
            this.$emit('sell', this.id)
        }
    }
}
