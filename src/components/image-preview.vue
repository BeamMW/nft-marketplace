<template>
  <div class="preview-container" :style="style">
    <div v-if="loading && show_loading" class="darker">Loading...</div>
    <div v-if="error && show_loading" class="darker">Failed to load image</div>
    <img v-if="src" :src="src" :style="image_style">
  </div>  
</template>

<style scoped lang="stylus">
  .preview-container {
    background-color: rgba(240, 205, 205, 0.07)
    display: flex
    align-items: center
    justify-content: center
    overflow: hidden

    & > img {
      max-width: 100%
      max-height: 100%
      object-fit: cover
    }

    & > .darker {
      opacity: 0.5
    }
  }
</style>

<script>
export default {
  props: {
    image: {
      type: [Object, String],
      required: false,
      default: undefined
    },
    height: {
      type: String,
      required: false,
      default: ''
    },
    width: {
      type: String,
      required: false,
      default: ''
    },
    radius: {
      type: String,
      required: false,
      default: '10px 10px 0 0'
    },
    cover: {
      type: Boolean,
      default: false
    },
    show_loading: {
      type: Boolean,
      default: true
    }
  },

  computed: {
    src () {
      if (typeof this.image == 'string') {
        return this.image
      }

      if (this.image && this.image.object) {
        return this.image.object
      }
      
      return undefined
    },

    loading () {
      return (this.image || {}).loading
    },

    error () {
      return (this.image || {}).error
    },

    style () {
      let res = {
        'border-radius': this.radius,
      }

      if (this.height) {
        res['height'] = this.height
      }

      if (this.width) {
        res['width'] = this.width
      }

      if (this.$attrs && this.$attrs['onClick']) {
        res['cursor'] = 'pointer'
      }

      return res
    },

    image_style() {
      let res = {}

      if (this.cover) {
        res['width'] = '100%'
        res['height'] = '100%'
        res['object-fit'] = 'cover%'
      }

      return res
    }
  }
}
</script>
