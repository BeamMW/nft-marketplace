<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">{{ title }}</div>
      <div class="message">{{ message }}</div>
      <div class="controls">
        <btn text="close" @click="close">
          <img src="~assets/cancel.svg"/>
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
import utils from 'utils/utils'
import artistsStore from 'stores/artists'

export default {
  components: { 
    modal, btn
  },

  data() {
    return {
      title: '',
      message: ''
    }
  },

  computed: {
    computed_key () {
      return this.artist_key || artistsStore.my_id
    },
  },

  methods: {
    onCopyAndClose() {
      this.onCopy()
      this.close()
    },
  
    onCopy() {
      utils.copyText(this.computed_key)
    },

    open (title, message) {
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