<template>
  <div class="preview-container" :style="style">
    <div v-if="loading && show_text" class="loading">Loading</div>
    <img v-if="src" :class="{'preview': true, 'error': error}" :src="src" :style="image_style"/>
    <slot></slot>
  </div>  
</template>

<style scoped lang="stylus">
  .preview-container {
    background-color: rgba(240, 205, 205, 0.07)
    display: flex
    align-items: center
    justify-content: center
    overflow: hidden
    position: relative
    user-select: none
    
    &>.preview {
      width: 100%
      height: 100%
      position: absolute
    }

    &>.loading {
      position: absolute
      z-index: 100
      border-radius: 10px
      backdrop-filter: blur(30px) brightness(70%)
      padding: 3px 10px 6px 10px
      color: rgba(255, 255, 255, 0.85)
      font-size: 14px
    }

    &>.error {
      color: black
      z-index: 100
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
    contain: {
      type: Boolean,
      default: false
    },
    show_text: {
      type: Boolean,
      default: true
    },
    text_color: {
      type: String,
      default: '#fff'
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

    debug () {
      return this.$state.debug
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
        res['object-fit'] = 'cover'
      }

      if (this.contain) {
        res['object-fit'] = 'contain'
      }

      return res
    },

    text_style() {
      return {
        'color': this.text_color
      }
    }
  }
}
</script>
