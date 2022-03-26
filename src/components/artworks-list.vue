<template>
  <div class="artworks-container">
    <template v-if="artworks.length > 0">
      <div ref="artslist" class="artworks" @scroll="onScroll">
        <artwork v-for="artwork in visible_artworks"
                 :key="artwork.id"
                 :artwork="artwork"
        />
      </div>
      <paginator :current="page"
                 :total="pages"
                 @page-changed="onPage"
      />
    </template>
    <div v-else class="empty">
      <img src="~assets/icon-empty-gallery.svg"/>
      <div class="text">There are no NFTs at the moment</div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .artworks-container {
    display: flex
    flex-direction: column
    min-height: 0

    .artworks {
      display: flex
      flex-wrap: wrap
      overflow-y: auto
      overflow-x: hidden
    }

    .empty {
      margin: 100px auto 0 auto
      display: flex
      align-items: center
      flex-direction: column
      user-select: none

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
import {common} from '../utils/consts.js'
import artwork from './artwork.vue'
import paginator from './paginator.vue'

export default {
  components: {
    artwork,
    paginator
  },

  props: {
    artworks: {
      type: Array,
      default: () => []
    },
    page: {
      type: Number,
      default: 1
    }
  },

  emits: [
    'update:page'
  ],

  computed: {
    visible_artworks () {
      let all = this.artworks
      let result  = []
      let start = (this.page - 1) * common.ITEMS_PER_PAGE
      let end   = Math.min(start + common.ITEMS_PER_PAGE, all.length)
      for (let idx = start; idx < end; ++idx) {
        result.push(all[idx])
      }
      return result
    },
    pages() {
      let total = this.artworks.length
      return total ? Math.ceil(total / common.ITEMS_PER_PAGE) : 1
    }
  },

  mounted() {
    if(this.$route.hash) {
      this.$refs.artslist.scrollTop = this.$route.hash.substr(1)
    }
  },
  
  methods: {
    onScroll(ev) {
      let pos = ev.target.scrollTop
      this.$router.replace({name: this.$route.name, hash: `#${pos}`})
    },

    onPage(page) {
      this.$emit('update:page', page)
      this.$refs.artslist.scrollTop = 0
    } 
  }
}
</script>
