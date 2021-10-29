import html from '../utils/html.js';
import adminui from './admin-ui.js';
import artwork from './artwork.js';
import balance from './balance.js';
import warning from './tx-warning.js';
import artworksControls from './artworks-controls.js';
import { common } from '../utils/consts.js';

export default {
    computed: {
        in_tx () {
            return this.$state.in_tx;
        },
        is_admin () {
            return this.$state.is_admin;
        },
        artists () {
            return this.$state.artists;
        },
        active_tab () {
            return this.$state.active_tab
        },
        artworks () {
            let tab = this.$state.active_tab;
            return this.$state.artworks[tab];
        }
    },

    components: {
        artwork, adminui, balance, warning, artworksControls
    },

    template: `
        <div class="vertical-container">
            <balance></balance>
            <warning v-if="in_tx"></warning>
            <adminui v-if="!in_tx && is_admin"/>
            <artworksControls></artworksControls>
            <template v-if="artworks.length > 0">
                <div class="artworks">
                    <artwork v-for="artwork in artworks"
                    v-bind:id="artwork.id"
                    v-bind:title="artwork.title"
                    v-bind:author="(artists[artwork.pk_author] || {}).label"
                    v-bind:bytes="artwork.bytes"
                    v-bind:owned="artwork.owned"
                    v-bind:price="artwork.price"
                    v-bind:likes_cnt="artwork.impressions"
                    v-bind:liked="artwork.my_impression == 1"
                    v-bind:in_tx="in_tx"
                    v-on:sell="onSellArtwork"
                    v-on:buy="onBuyArtwork"
                    v-on:like="onLikeArtwork"
                    v-on:unlike="onUnlikeArtwork"
                    />
                </div>
            </template>
            <template v-else>
                <div class="empty-gallery">
                    <img class="empty-gallery__icon" src="./assets/icon-empty-gallery.svg"/>
                    <div class="empty-gallery__text">There are no artworks at the moment.</div>
                </div>
            </template>
        </div>    
    `,

    methods: {
        onBuyArtwork (id) {
            this.$store.buyArtwork(id);
        },

        onSellArtwork (id) {
            try {
                let price = prompt("Enter the price in BEAM (need to implement regrular dialog with float values)");
                if (price == null) {
                    return;
                }
                price = parseInt(price) * common.GROTHS_IN_BEAM;
                this.$store.sellArtwork(id, price);
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
        }
    }
}
