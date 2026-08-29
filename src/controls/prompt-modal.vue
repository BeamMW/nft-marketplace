<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">{{ options.title }}</div>
      <div v-if="options.message" class="message">{{ options.message }}</div>

      <priceInput v-if="is_amount"
                  v-model="value"
                  class="input amount"
                  :aid="options.aid || 0"
      />
      <formInput v-else
                 v-model="value"
                 class="input"
                 :max_length="options.max_length"
                 :tip="options.tip"
                 :allowed="options.allowed"
                 :placeholder="options.placeholder"
      />

      <div class="controls">
        <btn text="cancel" @click="onCancel">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn :text="options.ok_text || 'proceed'"
             color="green"
             :disabled="!valid"
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

    .title {
      font-size: 18px
      font-weight: bold
      color: #fff
    }

    .message {
      max-width: 460px
      margin: 24px 0px 22px 0px
      font-size: 14px
      line-height: 1.5
      text-align: center
      color: rgba(255, 255, 255, 0.7)
    }

    .input {
      width: 460px
      max-width: 70vw

      &.amount {
        align-self: stretch
      }
    }

    .controls {
      margin-top: 35px
      display: flex
      flex-direction: row

      & > *:not(:first-child) {
        margin-left: 30px
      }
    }
  }
</style>

<script>
import {nextTick} from 'vue'
import modal from 'controls/modal'
import btn from 'controls/button'
import formInput from 'controls/form-input'
import priceInput from 'controls/price-input'

// Styled replacement for prompt(). open() resolves with the entered value,
// or undefined when cancelled.
export default {
  components: {
    modal, btn, formInput, priceInput
  },

  data () {
    return {
      value: '',
      options: {},
      resolve: undefined
    }
  },

  computed: {
    is_amount () {
      return this.options.mode === 'amount'
    },

    valid () {
      let value = String(this.value || '').trim()

      if (!value) {
        return false
      }

      if (this.is_amount) {
        return parseFloat(value) > 0
      }

      if (this.options.validate) {
        return !!this.options.validate(value)
      }

      return true
    }
  },

  methods: {
    open (options) {
      this.options = options || {}
      this.value = ''
      this.$refs.modal.open()

      nextTick(() => {
        // content is teleported into #modals, so $el will not find it
        let host = document.getElementById('modals')
        if (!host) return

        let inputs = host.querySelectorAll('.content input')
        let input = inputs[inputs.length - 1]
        if (input) input.focus()
      })

      return new Promise((resolve) => {
        this.resolve = resolve
      })
    },

    settle (result) {
      let resolve = this.resolve
      this.resolve = undefined
      this.$refs.modal.close()
      if (resolve) resolve(result)
    },

    onCancel () {
      this.settle(undefined)
    },

    onProceed () {
      if (!this.valid) {
        return
      }
      this.settle(String(this.value).trim())
    }
  }
}
</script>
