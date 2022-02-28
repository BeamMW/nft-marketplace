import loading from './item-loading.js'
import artPrice from './artwork-price.vue'

export default {
    props: {
        artwork: {
            type: Object,
            required: true,
        }
    },

    emits: ['delete', 'details'],

    components: {
        loading,
        artPrice
    },

    computed: {
        is_admin () {
            return this.$state.is_admin
        },

        is_headless () {
            return this.$state.is_headless
        },

        id () {
            return this.artwork.id
        },

        title () {
            return this.artwork.title
        },

        image () {
            let bytes = this.artwork.bytes
            if (bytes) {
                return URL.createObjectURL(new Blob([bytes], {type: this.artwork.mime_type}))
            }
        },
        
        likes_cnt () {
            return this.artwork.impressions
        },

        liked () {
            return !!this.artwork.my_impression
        },
        
        can_vote () {
            return this.$state.balance_reward > 0
        },

        loading () {
            return this.artwork.loading
        },

        error () {
            return this.artwork.error
        },

        artists () {
            return this.$state.artists
        },

        author () {
            return (this.artists[this.artwork.pk_author] || {}).label
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
            if (this.can_vote) {
                this.$emit('unlike', this.id)
            }
        },

        onDelete (ev) {
            this.$emit("delete", this.id)
        },

        onDetails(ev) {
            this.$emit("details", this.id)
        }
    }
}
