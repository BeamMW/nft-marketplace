<template>
  <div class="price-input">
    <formInput v-model="computedValue"
               :label="label"
               :readonly="readonly"
               @keydown="onKey"
               @paste="onPaste"
    >
      <span class="asset" :class="{'readonly': readonly}">
        <assetIcon :aid="aid" :size="20" class="asset-icon"/>
        {{ unit_name }}
      </span>
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

    .asset {
      display: flex
      align-items: center
      font-size: 20px
      padding-right: 12px
      white-space: nowrap

      & > .asset-icon {
        margin-right: 6px
      }

      &.readonly {
        color: rgba(255, 255, 255, 0.3)
      }
    }
  }
</style>

<script>
import formInput from 'controls/form-input'
import utils from 'utils/utils'
import assetsStore from 'stores/assets'
import assetIcon from 'controls/asset-icon'

export default {
  components: {
    formInput,
    assetIcon
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
    aid: {
      type: Number,
      default: 0
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
    unit_name () {
      return assetsStore.get(this.aid).unit_name
    },
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