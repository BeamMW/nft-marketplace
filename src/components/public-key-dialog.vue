<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">Artist key</div>
      <div class="data">
        <span>{{ artist_key }}</span>
        <btn class="icon-copy" color="transparent" padding="11px 0 11px 10px" @click="onCopy">
          <img src="~assets/icon-copy.svg"/>
        </btn>
      </div>
      <div class="controls">
        <btn text="close" @click="close">
          <img src="~assets/icon-cancel.svg"/>
        </btn>
        <btn text="copy and close" color="green" @click="onCopyAndClose">
          <img src="~assets/icon-copy-blue.svg"/>
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

    .data {
      margin-top: 30px
      display: flex
      flex-direction: row
      align-items: center

      .icon-copy {
        cursor: pointer
      }
    }
      
    .controls {
      margin-top: 30px
      display: flex
      flex-direction: row

      & > *:not(:first-child) {
        margin-left: 30px
      }
    }
  }
</style>

<script>
import modal from './modal.vue'
import btn from './button.vue'
import artistsStore from 'stores/artists.js'

export default {
  components: { 
    modal, btn
  },

  computed: {
    artist_key () {
      return artistsStore.my_id
    },
  },

  methods: {
    onCopyAndClose() {
      this.onCopy()
      this.close()
    },
  
    onCopy() {
      var textArea = document.createElement('textarea')
      textArea.style.position = 'fixed'
      textArea.value = this.artist_key
      document.body.appendChild(textArea)
      textArea.focus()
      textArea.select()
            
      try {
        return document.execCommand('copy')
      } 
      catch (ex) {
        return false
      } 
      finally {
        document.body.removeChild(textArea)
      }
    },

    open () {
      this.$refs.modal.open()
    },

    close () {
      this.$refs.modal.close()
    }
  }
}
</script>