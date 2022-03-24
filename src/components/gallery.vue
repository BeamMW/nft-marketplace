<template>
  <div id="container" class="vertical-container">
    <headless v-if="is_headless"></headless>
    <!-- <balance v-else></balance> -->
    <adminui v-if="is_admin"/>
    <artctrls/>
    <!---- uncomment the line below to see an example button ----->
    <!---- <PRESENTATION/> ---->
    <template v-if="artworks.length > 0">
      <div ref="artslist" class="artworks" @scroll="onScrollArtwork">
        <artwork v-for="artwork in artworks"
                 :key="artwork.id"
                 :artwork="artwork"
                 @like="onLikeArtwork"
                 @unlike="onUnlikeArtwork"
                 @delete="onDeleteArtwork"
        />
      </div>
      <paginator :current="current_page"
                 :total="total_pages"
                 @page-changed="onPageChanged"
      />
    </template>
    <div v-else class="empty">
      <img src="~assets/icon-empty-gallery.svg"/>
      <div class="text">There are no artworks at the moment</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  #container {
    .artworks {
      display: flex
      flex-wrap: wrap
      overflow-y: auto
      overflow-x: hidden
      margin-top: 20px
    }

    .empty {
      margin: 100px auto 0 auto
      display: flex
      align-items: center
      flex-direction: column

      & > .text {
        margin-top: 30px
        opacity: 0.5
        font-size: 14px
        line-height: 1.43
        text-align: center
        color: #fff
      }
    }
  }
</style>

<script>
import adminui   from './admin-ui.vue'
import artwork   from './artwork.vue'
// import balance   from './balance.vue'
import headless  from './headless.vue'
import artctrls  from './artworks-controls.vue'
import paginator from './paginator.vue'
import {common}  from '../utils/consts.js'

// uncomment to see an example button
// import PRESENTATION from './button-presentation.vue'

export default {
  components: {
    artwork, 
    adminui, 
    // balance, 
    artctrls, 
    headless, 
    paginator,
    // uncomment to see an example button
    // PRESENTATION 
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
    }
  },

  mounted() {
    if(this.$route.hash) {
      this.$refs.artslist.scrollTop = this.$route.hash.substr(1)
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

    onScrollArtwork(ev) {
      let pos = ev.target.scrollTop
      this.$router.replace({name: this.$route.name, hash: `#${pos}`})
    }
  }
}
</script>