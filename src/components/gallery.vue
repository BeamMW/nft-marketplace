<template>
  <div id="container" class="vertical-container">
    <headless v-if="is_headless"></headless>
    <balance v-else></balance>
    <adminui v-if="is_admin"/>
    <artctrls v-if="details_id == -1"/>
    <adetails v-if="details_id != -1"
              :artwork="details"
              @back="onDetailsBack"
    />
    <template v-else-if="artworks.length > 0">
      <div ref="artslist" class="artworks">
        <artwork v-for="artwork in artworks"
                 :key="artwork.id"
                 :artwork="artwork"
                 @like="onLikeArtwork"
                 @unlike="onUnlikeArtwork"
                 @delete="onDeleteArtwork"
                 @details="onDetails"
        />
      </div>
      <paginator :current="current_page"
                 :total="total_pages"
                 @page-changed="onPageChanged"
      />
    </template>
    <div v-else class="empty-gallery">
      <img class="empty-gallery__icon" src="~assets/icon-empty-gallery.svg"/>
      <div class="empty-gallery__text">There are no artworks at the moment.</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
</style>

<script>
import adminui   from './admin-ui.vue'
import artwork   from './artwork.vue'
import balance   from './balance.vue'
import headless  from './headless.vue'
import artctrls  from './artworks-controls.vue'
import paginator from './paginator.vue'
import adetails  from './artwork-details.vue'
import {common}  from '../utils/consts.js'

export default {
  components: {
    artwork, 
    adminui, 
    balance, 
    artctrls, 
    headless, 
    paginator, 
    adetails
  },

  data () {
    return {
      details_id: -1,
    }
  },

  computed: {
    is_admin () {
      return this.$state.is_admin
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
</script>