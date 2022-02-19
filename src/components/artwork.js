import html from '../utils/html.js';
import loading from './item-loading.js';
import artButton from './art-button.js';
import popupMenu from './popup-menu.vue'
import { common } from '../utils/consts.js';
import { nextTick } from 'vue';

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
            type: Uint8Array,
            default: new Uint8Array()
        },
        mime_type: {
            type: String,
            default: "image/jpeg"
        },
        owned: {
            type: Number,
            default: 0
        },
        price: {
            type: Object,
            default: undefined
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
        is_admin: {
            type: Boolean,
            default: false
        },
        loading: {
            type: Boolean,
            default: true
        },
        error: {
            default: undefined
        }
    },

    emits: ['buy', 'sell', 'change_price', 'delete'],

    components: {
        loading,
        artButton,
        popupMenu
    },

    computed: {
        is_headless () {
            return this.$state.is_headless
        },
        amount () {
            if (this.price) {
                return (this.price.amount / common.GROTHS_IN_BEAM).toFixed(8).replace(/([0-9]+(\.[0-9]+[1-9])?)(\.?0+$)/,'$1')
            }
        },
        image () {
            if (this.bytes.length) {
                return URL.createObjectURL(new Blob([this.bytes], {type: this.mime_type}))
            }
        }
    },

    template:`
        <div class="artwork">
            
            <!---- Preview OR Loading ---->
            <div class="preview-container">
                <img v-if="image" :src="image"/>
                <loading v-else :error="!!error"/>
            </div>

            <!---- Delete Artwork Button ---->
            <img v-if="is_admin" class="artwork-delete" src="./assets/icon-delete.svg" v-on:click="onDelete"/>
        
            <!---- First info row ---->
            <div class="info-row">
                <!---- Title ---->
                <span class="artwork-title">{{title || "Loading..."}}</span>
                
                <!---- Likes ----->
                <span class="artwork-likes pointer-cursor" v-on="{click: liked ? onUnlike : onLike}" :disabled="can_vote">
                    <img :src="'./assets/icon-heart' + (liked ? '-red' : '') + '.svg'"/>
                    <span class="artwork-likes__text">{{likes_cnt}}</span>
                </span>
            </div>

            <!---- Second info row, author ---->
            <div class="info-row">
                <span class="artwork-author">
                    {{ this.author ? ['by', this.author].join(' ') : 'Loading...' }}
                </span>
            </div>

            <!---- Third info row, price/sell/change ----->
            <div class="artwork-price-row">

                <!---- has price & owned, display change price / remove from sale options ---->
                <span v-if="price && owned" class="artwork-can-buy">
                    <img src="./assets/icon-beam.svg"/>
                    <span class="artwork-can-buy__amount">{{amount}}</span>
                    <span class="artwork-can-buy__curr">BEAM</span>
                    <img class="artwork-can-buy__dots" src="./assets/icon-actions.svg" v-on:click="showOnSaleMenu">
                    <popupMenu ref="saleMenu">
                        <div class="item" v-on:click="onChangePrice">
                            <img src="./assets/icon-change.svg"/>
                            update the price
                        </div>
                        <div class="item" v-on:click="onRemoveFromSale">
                            <img src="./assets/icon-eye-crossed.svg"/>
                            remove from sale
                        </div>
                    </popupMenu>
                </span>

                <!---- has price but not owned, can buy ---->
                <span v-if="price && !owned" class="artwork-can-buy">
                    <img src="./assets/icon-beam.svg"/>
                    <span class="artwork-can-buy__amount">{{amount}}</span>
                    <span class="artwork-can-buy__curr">BEAM</span>
                    <artButton class="artwork-can-buy__button" type="buy" v-on:click="onBuy"/>
                </span>

                <!---- doesn't have price & owned, can sell ---->
                <artButton v-if="!price && owned" class="artwork-can-buy__button" v-on:click="onSell" type="sell"/>

                <!---- doesn't have price & not owned, 
                       can be anything - not approved yet, not sold by
                       owner &c. Just dispaly that it is not on sale
                ---->
                <span v-if="!price && !owned" class="not-on-sale">
                    Not for sale
                </span>
            </div>
        </div>
    `,

    methods: {
        onBuy (ev) {
            ev.preventDefault()
            if (this.is_headless) {
                this.$store.switchToHeaded()  
            } 
            else {
                this.$emit('buy', this.id)
            }
        },

        onSell (ev) {
            ev.preventDefault()
            this.$emit('sell', this.id)
        },

        onLike (ev) {
            ev.preventDefault()
            if (this.is_headless) 
            {
                this.$store.switchToHeaded()  
            } 
            else {
                if (this.can_vote) {
                    this.$emit('like', this.id)
                }
            }
        },

        onUnlike (ev) {
            ev.preventDefault()
            if (this.can_vote) {
                this.$emit('unlike', this.id)
            }
        },

        showOnSaleMenu(ev) {
            ev.preventDefault()
            this.$refs.saleMenu.open(ev)
        },

        onChangePrice (ev) {
            ev.preventDefault()
            this.$emit('change_price', this.id)
        },

        onDelete (ev) {
            ev.preventDefault()
            this.$emit("delete", this.id)
        },

        onRemoveFromSale(ev) {
            ev.preventDefault()
            this.$emit("remove_from_sale", this.id)
        }
    }
}
