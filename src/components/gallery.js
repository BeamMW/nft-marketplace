import adminui from  './admin-ui.js'
import artwork from  './artwork.js'
import balance from  './balance.js'
import headless from './headless.js'
import artworksControls from './artworks-controls.js'
import { popups, tabs, common } from '../utils/consts.js'
import publicKeyPopup from './public-key-popup.js'
import paginator from './paginator.vue'
import adetails from './artwork-details.vue'

export default {
    computed: {
        is_admin () {
            return this.$state.is_admin;
        },
        artists () {
            return this.$state.artists
        },
        active_tab () {
            return this.$state.active_tab
        },
        artworks () {
            let all = this.$state.artworks
            let result  = []
            let start = (this.current_page - 1) * common.ITEMS_PER_PAGE
            let end   = Math.min(start + common.ITEMS_PER_PAGE, all.length)
            for (let idx = start; idx < end; ++idx) {
                result.push(all[idx])
            }
            return result
        },
        can_vote () {
            return this.$state.balance_reward > 0;
        },
        is_popup_visible() {
            return this.$state.is_popup_visible;
        },
        is_headless () {
            return this.$state.is_headless
        },
        current_page () {
            return this.$state.current_page
        },
        total_pages () {
            return this.$state.total_pages
        }
    },

    data () {
        return {
            details_id: -1
        }
    },

    components: {
        artwork, adminui, balance, artworksControls, publicKeyPopup, headless, paginator, adetails
    },

    template: `
        <publicKeyPopup v-if="is_popup_visible"></publicKeyPopup>
        <div class="vertical-container" id="container">
            <headless v-if="is_headless"></headless>
            <balance v-else></balance>
            <adminui v-if="is_admin"/>
            <artworksControls v-if="details_id == -1"></artworksControls>
            <adetails v-if="details_id != -1"
                v-on:back="onDetailsBack"
            />
            <template v-else-if="artworks.length > 0">
                <div class="artworks" ref="artslist">
                    <artwork v-for="artwork in artworks"
                    v-bind:id="artwork.id"
                    v-bind:title="artwork.title"
                    v-bind:author="(artists[artwork.pk_author] || {}).label"
                    v-bind:bytes="artwork.bytes"
                    v-bind:mime_type="artwork.mime_type"
                    v-bind:owned="artwork.owned"
                    v-bind:price="artwork.price"
                    v-bind:likes_cnt="artwork.impressions"
                    v-bind:liked="artwork.my_impression == 1"
                    v-bind:can_vote="can_vote"
                    v-bind:is_admin="is_admin"
                    v-bind:error="artwork.error"
                    v-bind:loading="artwork.loading"
                    v-on:sell="onSellArtwork"
                    v-on:buy="onBuyArtwork"
                    v-on:like="onLikeArtwork"
                    v-on:unlike="onUnlikeArtwork"
                    v-on:change_price="onChangePrice"
                    v-on:remove_from_sale="onRemoveFromSale"
                    v-on:delete="onDeleteArtwork"
                    v-on:details="onDetails"
                    />
                </div>
                <paginator
                    v-bind:current="current_page"
                    v-bind:total="total_pages"
                    v-on:page-changed="onPageChanged"
                />
            </template>
            <div class="empty-gallery" v-else>
                <img class="empty-gallery__icon" src="./assets/icon-empty-gallery.svg"/>
                <div class="empty-gallery__text">There are no artworks at the moment.</div>
            </div>
        </div>
    `,

    methods: {
        onBuyArtwork (id) {
            this.$store.buyArtwork(id);
        },

        onSellArtwork (id) {
            try {
                this.$store.setPopupType(popups.SELL);
                this.$store.setIdToSell(id);
                this.$store.changePopupState(true);
            } 
            catch (err) {
                this.$store.setError(err, "Failed to sell an item");
            }
        },

        onRemoveFromSale (id) {
            this.$store.sellArtwork(id, 0);
        },

        onChangePrice (id) {
            try {
                this.$store.setPopupType(popups.CHANGE_PRICE);
                this.$store.setIdToSell(id);
                this.$store.changePopupState(true);
            } 
            catch (err) {
                this.$store.setError(err, "Failed to sell an item");
            }
        },

        onLikeArtwork(id) {
            this.$store.likeArtwork(id)
        },

        onUnlikeArtwork(id) {
            this.$store.unlikeArtwork(id)
        },

        onDeleteArtwork(id) {
            this.$store.deleteArtwork(id)
        },

        onPageChanged(page) {
            this.$store.setCurrentPage(page)
            this.$refs.artslist.scrollTop = 0
        },

        onDetails (id) {
            this.details_id = id
        },

        onDetailsBack() {
            this.details_id = -1
        }
    }
}
