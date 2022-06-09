<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">{{ title }}</div>
      <div class="message" v-html="message"/>
      <div class="controls">
        <btn text="close" :color="error ? 'red' : undefined" @click="close">
          <img v-if="error" src="~assets/cancel-blue.svg">
          <img v-else src="~assets/cancel.svg"/>
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
      margin-top: 30px
      text-align: center
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
import modal from 'controls/modal'
import btn from 'controls/button'

export default {
  components: { 
    modal, btn
  },

  data() {
    return {
      error: false,
      title: '',
      message: ''
    }
  },

  methods: {
    open (title, message, error) {
      this.error = error
      this.title = title
      this.message = message
      this.$refs.modal.open()
    },
    close () {
      this.$refs.modal.close()
    }
  }
}
</script>