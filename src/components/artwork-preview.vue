<template>
  <img v-if="image" :src="image">
  <loading v-else :error="error"/>
</template>

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
