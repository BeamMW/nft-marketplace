import adminui from  './admin-ui.js'
import artwork from  './artwork.js'
import balance from  './balance.js'
import headless from './headless.js'
import artworksControls from './artworks-controls.js'
import { popups, common } from '../utils/consts.js'
import publicKeyPopup from './public-key-popup.js'
import paginator from './paginator.js'
import adetails from './artwork-details.vue'

import PRESENTATION from './button-presentation.vue'

export default {
    computed: {
        is_admin () {
            return this.$state.is_admin;
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
        },
        details () {
            for (let artwork of this.artworks) {
                if (artwork.id == this.details_id) {
                    return artwork
                }
            }
            return undefined
        }
    },

    data () {
        return {
            details_id: -1
        }
    },

    components: {
        artwork, adminui, balance, artworksControls, publicKeyPopup, headless, paginator, adetails, PRESENTATION
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
                v-bind:artwork="details"
            />
            <template v-else-if="artworks.length > 0">
                <div class="artworks" ref="artslist">
                    <artwork v-for="artwork in artworks"
                        v-bind:artwork="artwork"
                        v-on:like="onLikeArtwork"
                        v-on:unlike="onUnlikeArtwork"
                        v-on:delete="onDeleteArtwork"
                        v-on:details="onDetails"
                    />
                </div>
                <paginator
                    v-bind:current="current_page"
                    v-bind:total="total_pages"
                    v-on:page-changed="onPageChanged"
                />

                <!---- uncomment the line below to see an example button ----->
                <!---- <PRESENTATION/> ---->
                
            </template>
            <div class="empty-gallery" v-else>
                <img class="empty-gallery__icon" src="./assets/icon-empty-gallery.svg"/>
                <div class="empty-gallery__text">There are no artworks at the moment.</div>
            </div>
        </div>
    `,

    methods: {
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
