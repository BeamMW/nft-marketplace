<template>
  <div :style="ctrlStyle">
    <div class="add-image-container" :style="borderStyle" :readonly="readonly" :class="{'error': error || (modelValue || {}).error}">
      <div v-if="modelValue && !readonly" class="remove">
        <img src="~/assets/remove.svg" @click="onRemove"/>
      </div>
      <img v-if="modelValue && modelValue.object" :src="modelValue.object" alt="avatar" class="image" :style="image_style"/>
      <div v-if="modelValue && modelValue.loading" class="text" :readonly="readonly">Loading...</div>
      <div v-if="modelValue && modelValue.error" class="text" :readonly="readonly">Failed to load image</div>
      <label v-if="!modelValue" class="text" for="image" :readonly="readonly" v-html="title"/>
      <input v-if="!readonly"
             id="image"
             ref="image"
             type="file"
             class="files"
             :accept="accept"
             @change="onUpload"
      />
    </div>
    <div v-if="error" class="error">
      {{ error }}
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .add-image-container {
    display: flex
    align-items: center
    justify-content: center
    position:relative
    background-color: rgba(26, 246, 214, 0.1)
    border-radius: 10px
    height: 100%

    &[readonly] {
      opacity: 0.6
    }

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
      border-radius: 10px
      border: 1px dashed transparent
    }
    
    .files {
      visibility:hidden
      width: 0
    }

    .text {
      width: 100%
      height: 100%
      display: flex
      text-align: center
      justify-content: center
      align-items: center
      font-size: 14px
      color: #1af6d6

      &:not([readonly]) {
        cursor: pointer
      }

      &[readonly] {
        opacity: 0.6
      }
    }
  }

  .error {
    font-size: 12px
    font-weight: 400
    font-style: italic
    text-align: right
    padding-right: 4px
  }
</style>

<script>
export default {
  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: Object,
      required: true,
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
    title: {
      type: String,
      default: '',
      required: true
    },
    error: {
      type: String,
      default: '',
      required: false
    },
    readonly: {
      type: Boolean,
      default: false,
      required: false
    },
    accept: {
      type: String,
      default: 'image/*',
      required: false
    },
    contain: {
      type: Boolean,
      default: false
    },
    cover: {
      type: Boolean,
      default: false
    }
  },

  emits: [
    'update:modelValue'
  ],

  computed: {
    ctrlStyle() {
      let res = {}

      if (this.height) {
        res.height = this.height
      }

      if (this.width) {
        res.width = this.width
      }

      return res
    },
    borderStyle() {
      return {
        'border': this.modelValue ? '1px dashed transparent' : '1px dashed #1AF6D6',
      }
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
    }
  },
  methods: {
    onRemove () {
      this.$refs.image.value = ''
      this.$emit('update:modelValue', null)
    },
    onUpload (e) {
      let file = e.target.files[0]
      let reader = new FileReader()
      reader.readAsDataURL(file)
      reader.onload = (e) => {
        this.$emit('update:modelValue', {
          object: e.target.result,
          file
        })
      }
    }
  }
}
</script>
