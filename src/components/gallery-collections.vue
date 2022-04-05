<template>
  <list v-model:page="page"
        emptymsg="There are no collections at the moment"
        component="collection"
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
      return this.$state.collections
    },

    page: {
      get() {
        return this.$state.gallery_collections_page
      },
      set (value) {
        this.$store.setGalleryCollectionsPage(value)
      }
    }
  }
}
</script>
