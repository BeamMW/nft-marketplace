<template>
  <div class="gallery-container">
    <controls/>
    <collections v-if="show_collections" class="list"/>
    <artworks v-else class="list"/>
  </div>
</template>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%

    & > .list {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
// TODO: headless
import controls from './gallery-controls.vue'
import artworks from './user-artworks.vue'
import collections from './user-collections.vue'
import {tabs} from 'utils/consts.js'

export default {
  components: {
    controls,
    artworks,
    collections
  },

  computed: {
    show_collections () {
      return this.$state.gallery_tab == tabs.COLLECTIONS
    }
  }
}
</script>
