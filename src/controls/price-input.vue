<template>
  <div class="price-input">
    <formInput v-model="computedValue"
               :label="label"
               :readonly="readonly"
               @keydown="onKey"
               @paste="onPaste"
    >
      <span class="beam" :class="{'readonly': readonly}">BEAM</span>
    </formInput>
  </div>
</template>

<style lang="stylus">
  .price-input {
    input {
      font-size: 24px !important
      padding-top: 8px !important
      padding-bottom: 10px !important
    }

    .beam {
      font-size: 20px
      padding-right: 12px

      &.readonly {
        color: rgba(255, 255, 255, 0.3)
      }
    }
  }
</style>

<script>
import formInput from 'controls/form-input'
import utils from 'utils/utils'

export default {
  components: {
    formInput
  },
  props: {
    label: {
      type: String,
      default: '',
      required: false
    },
    placeholder: {
      type: String,
      default: '0',
      required: false
    },
    readonly: {
      type: Boolean,
      default: false,
      required: false
    },
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: String,
      default: '',
      required: true
    },
  },
  
  emits: [
    'update:modelValue'
  ],

  computed: {
    computedValue: {
      get() {
        return this.modelValue
      },
      set (val) {
        this.$emit('update:modelValue', val)
      }
    }
  },

  methods: {    
    onKey(ev) {
      if (ev.isComposing || ev.keyCode === 229 || ev.ctrlKey || ev.altKey || ev.metaKey) {
        return
      }

      const specialKeys = [
        'Backspace', 'Tab', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp',
        'Control', 'Delete', 'F5'
      ]

      if (specialKeys.indexOf(ev.key) !== -1) {
        return
      }

      const current = ev.target.value
      const next = current.concat(ev.key)
      if (!utils.handleString(next)) {
        ev.preventDefault()
      }
    },

    onPaste(ev) {
      if (ev.clipboardData != undefined) {
        const text = ev.clipboardData.getData('text')
        if (!utils.handleString(text)) {
          ev.preventDefault()
        }
      }
    },
  }
}
</script>