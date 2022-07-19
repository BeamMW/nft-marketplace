<template>
  <div :style="ctrlStyle">
    <div class="add-image-container" :style="borderStyle" :readonly="readonly" :class="{'error': error || (modelValue || {}).error}">
      <div v-if="modelValue && !readonly" class="remove" :style="remove_style">
        <img src="~/assets/remove.svg" @click="onRemove"/>
      </div>
      <img v-if="modelValue && modelValue.object" ref="image" :src="modelValue.object" class="image" :style="image_style" @load="onLoad"/>
      <div v-if="modelValue && modelValue.loading" class="text" :readonly="readonly">Loading...</div>
      <div v-if="modelValue && modelValue.error" class="text" :readonly="readonly">Failed to load image</div>
      <label v-if="!modelValue" class="text" :for="input_id" :readonly="readonly" v-html="title"/>
      <input v-if="!readonly"
             :id="input_id"
             ref="input"
             type="file"
             class="files"
             :accept="accept"
             @change="onUpload"
      />
    </div>
    <div v-if="error && show_error" class="error">
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
    height: 100%

    &[readonly] {
      opacity: 0.6
    }

    .remove {
      background-color: rgba(0, 0, 0, 0.7)
      position: absolute
      z-index: 2
      border-radius: 9999px
      cursor: pointer
      width: 34px
      height: 34px
      display: flex
      align-items: center
      justify-content: center

      &>img {
        width: 20px
        height: 20px
      }
    }

    .image {
      width: 100%
      height: 100%
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
import utils from 'utils/utils'
import {common, genUniqueID} from 'utils/consts'

export default {
  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: Object,
      required: false,
      default: undefined
    },
    error: {
      type: String,
      required: false,
      default:undefined
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
    min_height: {
      type: Number,
      required: false,
      default: undefined
    },
    min_width: {
      type: Number,
      required: false,
      default: undefined
    },
    title: {
      type: String,
      default: '',
      required: true
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
    },
    show_error: {
      type: Boolean,
      default: true
    },
    rounded: {
      type: Boolean,
      default: false
    },
  },

  emits: [
    'update:modelValue',
    'update:error'
  ],

  data() {
    return {
      input_id: genUniqueID()
    }
  },

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
        'border-radius': this.rounded ? `${parseInt(this.height)/2}px` : '10px'
      }
    },
    image_style() {
      let res = {
        'border-radius': this.rounded ? `${parseInt(this.height)/2}px` : '10px'
      }
      
      if (this.cover) {
        res['object-fit'] = 'cover'
      }

      if (this.contain) {
        res['object-fit'] = 'contain'
      }

      return res
    },
    remove_style() {
      if (this.rounded) {
        return {
          'left': `${parseInt(this.height)/2-17}px`,
          'top': `${parseInt(this.height)/2-17}px`
        }
      }
      return {
        'top': '20px',
        'right': '20px'
      }
    },
  },
  methods: {
    onRemove () {
      this.$refs.input.value = ''
      this.$emit('update:modelValue', null)
      this.$emit('update:error', null)
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
    },
    onLoad() {
      console.log('!!!!Image', this.modelValue)
      //
      // Check file size
      //
      let file = this.modelValue.file
      if (file) {
        if (file.size >= common.MAX_IMAGE_SIZE) {
          let error = `Image size must be less than ${utils.formatBytes(common.MAX_IMAGE_SIZE)}`
          this.$emit('update:error', error)
          return
        }
      }

      //
      // Check image size
      //
      if (this.min_width || this.min_height) {
        let type = file ? file.type : this.modelValue.mime_type
        if (type !== 'image/svg+xml') {
          let werr = this.min_width && this.$refs.image.naturalWidth < this.min_width
          let herr = this.min_height && this.$refs.image.naturalHeight < this.min_height
          if (werr || herr) {
            this.$emit('update:error', `Image size must be at least ${this.min_width}x${this.min_height}px`) 
            return
          }
        }
      }

      //
      // Everything is OK
      //
      this.$emit('update:error', undefined)
    }
  }
}
</script>
