<template>
  <img v-if="image" class="preview" :src="image">
  <loading v-else :error="error"/>
</template>

<style scoped lang="stylus">
  .preview {
    max-width: 100%
    max-height: 100%
    object-fit: contain
  }
</style>

<script>
import loading from './item-loading.vue'

export default {
  components: {
    loading
  },
  
  props: {
    artwork: {
      type: Object,
      required: true
    }
  },

  computed: {
    image () {
      let bytes = this.artwork.bytes
      if (bytes) {
        return URL.createObjectURL(new Blob([bytes], {type: this.artwork.mime_type}))
      }
      return undefined
    },

    error () {
      return !!this.artwork.error
    }
  }
}
</script>
