import html    from '../utils/html.js';
import loading from './loading.js';
import dot     from './dot.js';
import artButton from './art-button.js';
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
        },
        likes_cnt: {
            type: Number,
            default: 0,
        },
        liked: {
            type: Boolean,
            default: false
        },
        can_vote: {
            type: Boolean,
            default: true
        },        
    },

    emits: ['buy', 'sell'],

    components: {
        artButton
    },

    computed: {
        ownerText () {
            let res = "";
            if (this.author) {
                res = ["by", this.author].join(' ');
            }
            // if (this.owned) {
            //     return res += (res ? " | " : "") + "You own this artwork";
            // }
            return res ? res : "Loading...";
        },
        can_change_price () {
            return this.owned && this.price
        }
    },

    render () {
        return html`
            <div class="artwork">
                ${this.renderPreview()}
                <div class="info-row">
                    <span class="artwork-title">
                        <span>
                            ${this.title || "Loading..."}
                        </span>
                    </span>
                    ${this.renderLikes()}
                </div>
                <div class="info-row">
                    <span class="artwork-owner">${this.ownerText}</span>
                    ${this.renderMine()}
                </div>
                <div class="artwork-bottom-row">
                    ${this.renderPrice()}
                </div>
            </div>
        `
    },

    methods: {
        //${!this.can_vote}
        renderLikes () {
            return html`
                <span class="artwork-likes pointer-cursor" onclick=${this.liked ? this.onUnlike : this.onLike} disabled=${!this.can_like}>
                    <img src="./assets/icon-heart${this.liked ? '-red' : ''}.svg"/>
                    <span class="artwork-likes__text">
                        ${this.likes_cnt}
                    </span>
                </span>
            `
        },

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

        renderMine() {
            if (this.owned) {
                return html`<div class="mine">
                    <span>mine</span>
                </div>`;
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
                let amount = (this.price.amount / common.GROTHS_IN_BEAM).toFixed(8).replace(/([0-9]+(\.[0-9]+[1-9])?)(\.?0+$)/,'$1');
                
                // TODO: if owned && has price should be able to cancel or change price
                //       probably not for the first version
                if (this.owned) {
                    return html`<span class="artwork-can-buy">
                        <img src="./assets/icon-beam.svg"/>
                        <span class="artwork-can-buy__amount">${amount}</span>
                        <span class="artwork-can-buy__curr">BEAM</span>
                    </span>`
                }
                
                // if not owned can only buy
                return html`
                    <span class="artwork-can-buy">
                        <img src="./assets/icon-beam.svg"/>
                        <span class="artwork-can-buy__amount">${amount}</span>
                        <span class="artwork-can-buy__curr">BEAM</span>
                        <${artButton} class="artwork-can-buy__button" onclick=${this.onBuy} type="buy"/>
                    </span>
                `
            }

            // doesn't have price and is own image
            if (this.owned) {
                return html`<${artButton} class="artwork-can-buy__button" onclick=${this.onSell} type="sell"/>`
            }

            // doesn't have price and is not own image
            // means can be anything - not approved yet, not sold by
            // owner &c. Just dispaly that it is not on sale
            return html`<span class='not-on-sale'>Not for sale</span>`
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
        },

        onLike (ev) {
            ev.preventDefault()
            if (this.can_vote) {
                this.$emit('like', this.id)
            }
        },

        onUnlike (ev) {
            ev.preventDefault()
            if (this.can_vote) {
                this.$emit('unlike', this.id)
            }
        },

        onChangePrice (ev) {
            ev.preventDefault()
            if (this.can_change_price) {
                alert('change price')
            }
        }
    }
}
