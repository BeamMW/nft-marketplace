<template>
  <img v-if="image" :src="image">
  <loading v-else :error="error"/>
</template>

<script>
import loading from './item-loading.js'

export default {
  props: {
    artwork: {
      type: Object,
      required: true
    }
  },

  components: {
    loading
  },

  computed: {
    image () {
      let bytes = this.artwork.bytes
      if (bytes) {
          return URL.createObjectURL(new Blob([bytes], {type: this.artwork.mime_type}))
      }
    },

    error () {
      return !!this.artwork.error
    }
  }
}
</script>
