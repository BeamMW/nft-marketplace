<template>
  <div class="price-input-container">
    <div class="input">
      <input id="sell-input" 
             :value="price"
             type="text" 
             class="elem"
             :placeholder="placeholder"
             @keydown="onKey"
             @paste="onPaste"
             @input="$emit('update:price', $event.target.value)"
      />
      <span class="text">BEAM</span>
    </div>
    <!-- TODO: add USD support 
    <span class="price">{{ price }} USD</span>  
    -->
  </div>
</template>

<style scoped lang="stylus">
.price-input-container {
  .input {
    margin-top: 20px
    height: 45px
    width: 100%
    border-radius: 10px
    background-color: rgba(255, 255, 255, 0.08)
    display: flex
    flex-direction: row
    align-items: center

    .elem {
      background: transparent
      border: none
      font-size: 24px
      resize: none
      color: var(--color)
      height: 100%
      padding: 0
      border-radius: 10px
      padding: 12px 15px
      max-width: 292px

      &:not(.err):not(:read-only):focus {
        background-color: rgba(255, 255, 255, 0.12)
        outline: none
      }

      &::placeholder {
        color: var(--color)
      }
    }

    .text {
      margin-right: 12px
      font-size: 16px
      margin-left: auto
      color: var(--color)
    }
  }

  .price {
    display: block
    margin-top: 6px
    font-size: 12px
    color: var(--color)
  }
}
</style>

<script>
import utils from '../utils/utils.js'

export default {
  props: {
    color: {
      type: String,
      default: '#fff'
    },
    placeholder: {
      type: String,
      default: '0',
      required: false
    },
    price: {
      type: String,
      default: '',
      required: true
    },
  },
  emits: ['update:price'],

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

      const current = this.price
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