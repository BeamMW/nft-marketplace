<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">
        Set the price
      </div>
      <div class="input">
        <input id="sell-input" 
               ref="input"
               v-model="price"
               type="text" 
               class="elem"
               placeholder="0"
               @keydown="onKey"
               @paste="onPaste"
        />
        <span class="text">BEAM</span>
      </div>
      <div class="fee">
        <span class="title">Fee</span>
        <span class="value">0.011 BEAM</span>
      </div>
      <div class="info">
        Small transaction fee must be paid {{ price_valid }}
      </div>
      <div class="controls">
        <artButton text="cancel"
                   text_color="white"
                   :semi_transparent="true"
                   icon="./assets/icon-cancel.svg"
                   @click="close"
        />
        <artButton id="proceed"
                   text="proceed"
                   color="blue"
                   icon="./assets/icon-receive-proceed.svg"
                   :margin_left="true"
                   :class="{disabled: !price_valid}" 
                   @click="onProceed"
        />
      </div>
    </div>
  </modal>
</template>

<style scoped lang="stylus">
  .content {
    display: flex
    flex-direction: column
    align-items: center
  
    .title {
      font-size: 18px
      font-weight: bold
      color: #fff
    }

    .input {
      margin-top: 20px
      height: 45px
      width: 100%
      border-radius: 10px
      background-color: rgba(255, 255, 255, 0.05)
      display: flex
      flex-direction: row
      align-items: center

      .elem {
        background: rgba(255, 255, 255, 0)
        border: none
        font-size: 24px
        resize: none
        color: #0bccf7
        height: 100%
        padding: 0
        border-radius: 10px
        padding: 8px
        max-width: 292px
          
        &:focus {
          outline-width: 0
          color: #0bccf7
        }

        &::placeholder {
          color: #0bccf7
        }
      }

      .text {
        margin-right: 12px
        font-size: 16px
        margin-left: auto
        color: #ffffff
      }
    }

    .fee {
      display: flex
      flex-direction: row
      margin-top: 20px
      align-self: flex-start
      
      .title {
        opacity: 0.5
        font-size: 12px
        color: #fff
      }

      .value {
        font-size: 14px
        font-weight: 500
        color: #0bccf7
        margin-left: 10px
      }
    }

    .info {
      margin-top: 30px
      opacity: 0.7
      font-size: 14px
      font-style: italic
      color: #fff
    }

    .controls {
      margin-top: 30px
      display: flex
      flex-direction: row
    }

    .disabled {
      opacity: 0.3
      cursor: auto !important
    }
  }
</style>

<script>
import modal from './modal.vue'
import artButton from './button.vue'
import utils from '../utils/utils.js'
import {common} from '../utils/consts.js'

export default {
  components: { 
    modal, artButton
  }, 
  
  emits:[
    'sell-artwork'
  ],

  data() {  
    return {
      price: '',
    } 
  },

  computed: {
    price_valid () {
      return parseFloat(this.price || '0') > 0
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

      const current = this.$refs.input.value
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

    onProceed() {
      if (!this.price_valid) {
        return
      }
      let price = parseFloat(this.price) * common.GROTHS_IN_BEAM
      this.$emit('sell-artwork',price)
      this.close()
    },

    open () {
      this.price = ''
      this.$refs.modal.open()
    },

    close () {
      this.$refs.modal.close()
    }
  }
}
</script>