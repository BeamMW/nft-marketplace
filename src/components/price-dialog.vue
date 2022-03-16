<template>
  <modal ref="modal">
    <div class="title">
      Set the price
    </div>

    <div class="input">
      <input id="sell-input"
             v-model="price"
             type="text" 
             class="elem"
             placeholder="0"
      />
      <span class="text">BEAM</span>
    </div>

    <div class="fee">
      <span class="title">Fee</span>
      <span class="value">0.011 BEAM</span>
    </div>

    <div class="info">
      Small transaction fee must be paid
    </div>

    <div class="controls">
      <artButton type="cancel" @click="close"/>
      <artButton id="proceed" class="disabled" type="proceed" @click="onProceed"/>
    </div>
  </modal>
</template>

<style scoped lang="stylus">
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
    background-color: rgba(255, 255, 255, .05)
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
        margin-right: 8px
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
</style>

<script>
import modal from './modal.vue'
import artButton from './art-button.js'
import utils from '../utils/utils.js'
import {common} from '../utils/consts.js'

export default {
  components: { 
    modal, artButton
  }, 
  emits:['sell-artwork'],

  data() {  
    return {
      price: '',
    } 
  },
  watch: {
    price(value){ 
      this.price = value 
      this.validatePrice(value) 
    },  
  },
    
  methods: {

    validatePrice(value) {
      const sellInput = document.getElementById('sell-input')

      if (sellInput) {
        document.getElementById('sell-input').addEventListener('keydown', (event) => {
          const specialKeys = [
            'Backspace', 'Tab', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp',
            'Control', 'Delete', 'F5'
          ]

          if (specialKeys.indexOf(event.key) !== -1) {
            return
          }

          const current = document.getElementById('sell-input').value
          const next = current.concat(event.key)
            
          if (!utils.handleString(next)) {
            event.preventDefault()
          }
        })

        document.getElementById('sell-input').addEventListener('paste', (event) => {
          if (event.clipboardData !== undefined) {
            const text = event.clipboardData.getData('text')
            if (!utils.handleString(text)) {
              event.preventDefault()
            }
          }
        })

        let disabled = parseFloat(value || '0') == 0
        let proceed = document.getElementById('proceed')
                
        if (disabled) {
          proceed.classList.add('disabled')
        } 
                
        if (!disabled) {
          proceed.classList.remove('disabled')   
        }
      }
    },

    onProceed() {
      let proceed = document.getElementById('proceed')
      if (!proceed.classList.contains('disabled')) {
        const current = document.getElementById('sell-input').value
        const price = parseFloat(current) * common.GROTHS_IN_BEAM
        this.$emit('sell-artwork',price)
        this.close()
        this.price = ''
      }
    },

    open () {
      this.$refs.modal.open()
    },

    close () {
      this.$refs.modal.close()
      this.price = ''
    }
  }
}
</script>