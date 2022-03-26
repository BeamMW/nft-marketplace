<template>
  <div class="gallery-container">
    <controls/>
    <div v-if="show_collections">Collections</div>
    <div v-else>NFTs</div>
  </div>
</template>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%
  }
</style>

<script>
// TODO: headless
import controls from './gallery-controls.vue'
import {tabs} from '../utils/consts.js'

export default {
  components: {
    controls
  },

  computed: {
    show_collections () {
      return this.$state.gallery_tab == tabs.COLLECTIONS
    }
  }
}
</script>
