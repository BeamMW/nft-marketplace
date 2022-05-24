<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">
        Set the price
      </div>
      <priceInput v-model="price" color="#0bccf7" placeholder="0" @trigger-key="onKey" @trigger-paste="onPaste"/>
      <!-- div class="fee">
        <span class="title">Fee</span>
        <span class="value">0.011 BEAM</span>
      </div -->
      <div class="info">
        Small transaction fee must be paid
      </div>
      <div class="controls">
        <btn text="cancel" @click="close">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn id="proceed"
             text="proceed"
             color="blue"
             :disabled="!price_valid" 
             @click="onProceed"
        >
          <img src="~assets/proceed.svg"/>
        </btn>
      </div>
    </div>
  </modal>
</template>

<style scoped lang="stylus">
  .content {
    display: flex
    flex-direction: column
    align-items: center
  
    & > .title {
      font-size: 18px
      font-weight: bold
      color: #fff
      margin-bottom: 30px
    }

    .fee {
      display: flex
      flex-direction: row
      margin-top: 10px
      align-self: flex-start
      
      & > .title {
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
      margin-top: 20px
      opacity: 0.7
      font-size: 14px
      font-style: italic
      color: #fff
    }

    .controls {
      margin-top: 30px
      display: flex
      flex-direction: row

      & > *:not(:first-child) {
        margin-left: 30px
      }
    }

    .disabled {
      opacity: 0.3
      cursor: auto !important
    }
  }
</style>

<script>
import modal from 'controls/modal'
import btn from 'controls/button'
import priceInput from 'controls/price-input'
import utils from 'utils/utils'
import {common} from 'utils/consts'

export default {
  components: { 
    modal, btn, priceInput
  }, 
  
  emits:[
    'sell-nft'
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

    onProceed() {
      if (!this.price_valid) {
        return
      }
      let price = parseFloat(this.price) * common.GROTHS_IN_BEAM
      this.$emit('sell-nft',price)
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