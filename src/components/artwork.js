import loading from './item-loading.js'
import artPrice from './artwork-price.vue'

export default {
    props: {
        artwork: {
            type: Object,
            required: true,
        },
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

    emits: ['delete', 'details'],

    components: {
        loading,
        artPrice
    },

    computed: {
        is_headless () {
            return this.$state.is_headless
        },

        image () {
            if (this.bytes.length) {
                return URL.createObjectURL(new Blob([this.bytes], {type: this.mime_type}))
            }
        },
        
        liked () {
            return !!this.artwork.my_impression
        }
    },

    template:`
        <div class="artwork">
            
            <!---- Preview OR Loading ---->
            <div class="preview-container">
                <img v-if="image" :src="image" v-on:click="onDetails">
                <loading v-else :error="!!error"/>
            </div>

            <!---- Delete Artwork Button ---->
            <img v-if="is_admin" class="artwork-delete" src="./assets/icon-delete.svg" v-on:click="onDelete"/>
        
            <!---- First info row ---->
            <div class="info-row">
                <!---- Title ---->
                <span class="artwork-title">{{title || "Loading..."}}</span>
                
                <!---- Likes ----->
                <span class="artwork-likes pointer-cursor" v-on="{click: liked ? onUnlike : onLike}" :disabled="!can_vote">
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

            <!---- Third info row, price/buy/sell ----->
            <div class="artwork-price-row">
                <artPrice v-bind:artwork="artwork"/>
            </div>
        </div>
    `,

    methods: {
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

        onDelete (ev) {
            ev.preventDefault()
            this.$emit("delete", this.id)
        },

        onDetails(ev) {
            ev.preventDefault()
            this.$emit("details", this.id)
        }
    }
}
