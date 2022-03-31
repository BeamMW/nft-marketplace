<template>
  <div class="banner" :style="bannerStyles">
    <div v-if="banner" class="remove">
      <img src="~/assets/remove.svg" alt="remove banner" @click="onRemoveBanner"/>
    </div>
    <img v-if="banner" :src="banner" alt="avatar" class="image" :class="{'error': !valid}"/>
    <label v-if="!banner" class="text" for="banner">{{ title }}</label>
    <input id="banner"
           ref="imageUploader"
           type="file"
           :accept="accepts"
           class="files"
           @click="resetImageUploader"
           @change="onUploadBanner"
    />
  </div>
  <div v-if="!valid" class="error_msg">
    <p class="error">image cannot be larger than 250kb</p>
  </div>
</template>

<style scoped lang="stylus">
  .banner {
    display: flex
    align-items: center
    justify-content: center
    position:relative
    height: 100%
    background-color: rgba(26, 246, 214, 0.1)
    border-radius: 10px
    cursor: pointer

    .remove {
      background-color: rgba(0, 0, 0, 0.7)
      position: absolute
      z-index: 2
      top: 20px
      right: 20px
      border-radius: 9999px
      padding: 7px 7px 3px 7px
      cursor: pointer
    }

    .image {
      width: 100%
      height: 100%
      object-fit: cover
      border-radius: 10px
      border: 1px dashed transparent

      &.error {
        filter: grayscale(100%) brightness(40%) sepia(100%) hue-rotate(-50deg) saturate(600%) contrast(0.8)
      }
    }
    
    .files {
      visibility:hidden
      width: 0
    }

    .text {
      width: 100%
      height: 100%
      display: flex
      justify-content: center
      align-items: center
      font-size: 14px
      color: #1af6d6
      cursor: pointer
    }
  }

  .error_msg {
    margin-top: -18px

    .error {
      font-size: 12px
      font-weight: 400
      font-style: italic
      text-align: right
    }
  }
</style>

<script>
export default {
  props: {
    banner: {
      type: String,
      required: true
    },
    valid: {
      type: Boolean,
      default: true,
      required: false
    },
    title: {
      type: String,
      default: '',
      required: true
    },
    accepts: {
      type: Array,
      required: true
    }
  },

  emits: [
    'remove', 'upload'
  ],

  computed: {
    bannerStyles() {
      return {
        'border' :  this.banner ? '1px dashed transparent' : '1px dashed #1AF6D6',
      }
    },
  },
  methods: {
    resetImageUploader() {
      this.$refs.imageUploader.value = ''
    },
    onRemoveBanner () {
      this.$emit('remove')
    },
    onUploadBanner(e) {
      this.$emit('upload',e)

    }
  }
}
</script>
