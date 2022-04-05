<template>
  <list v-model:page="page"
        emptymsg="There are no NFTs at the moment"
        component="artwork"
        gap="16px"
        :items="items"
  />  
</template>

<style scoped lang="stylus">
</style>

<script>
import list from './items-list.vue'

export default {
  components: {
    list
  },

  computed: {
    items () {
      return this.$state.artworks
    },

    page: {
      get() {
        return this.$state.gallery_artworks_page
      },
      set (value) {
        this.$store.setGalleryArtworksPage(value)
      }
    }
  }
}
</script>
