<template>
  <div class="preview-container" :style="style">
    <div v-if="loading && show_text" class="loading">Loading...</div>
    <!--div v-if="error && show_text" class="error">Failed to load image</div-->
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

    & > div {
      position: absolute
    }

    & > .loading {
      color: dimgray
    }

    & > .error {
      color: dimgray
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
    show_text: {
      type: Boolean,
      default: true
    },
    default: {
      type: String,
      default: ''
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
      
      return (this.default || undefined)
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
