<template>
  <div class="gallery-container">
    <controls/>
    <div v-if="show_collections">
    </div>
    <artworks v-else v-model:page="page"
              class="artworks"
              :artworks="artworks"
    />
  </div>
</template>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%

    .artworks {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
// TODO: headless
import controls from './gallery-controls.vue'
import artworks from './artworks-list.vue'
import {tabs} from '../utils/consts.js'

export default {
  components: {
    controls,
    artworks
  },

  computed: {
    show_collections () {
      return this.$state.gallery_tab == tabs.COLLECTIONS
    },

    artworks () {
      return this.$state.artworks
    },

    page: {
      get() {
        return this.$state.current_page
      },
      set (value) {
        this.$store.setCurrentPage(value)
      }
    }
  }
}
</script>
