<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">Public key</div>
      <div class="data">
        <span>{{ artist_key }}</span>
        <artButton icon="~assets/icon-copy.svg"
                   :transparent="true"
                   :margin_left="5"
                   @click="onClose"
        />
      </div>
      <div class="controls">
        <artButton text="close" 
                   :semi_transparent="true"
                   text_color="white"
                   icon="~assets/icon-cancel.svg"
                   @click="close"
        />
        <artButton data="artist_key" 
                   text="copy and close"
                   color="green"
                   icon="~assets/icon-copy-blue.svg"
                   :margin_left="true"
                   @click="onCopy"
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

    .data {
      margin-top: 30px
      display: flex
      flex-direction: row
      align-items: center

      .copy-icon {
        margin-left: 10px
        cursor: pointer
      }
    }
      
    .controls {
      margin-top: 30px
      display: flex
      flex-direction: row
    }
  }
</style>

<script>
import modal from './modal.vue'
import artButton from './button.vue'

export default {
  components: { 
    modal, artButton
  },

  computed: {
    artist_key () {
      return this.$state.my_artist_keys[0]
    },
  },

  methods: {
    onCopy() {
      var textArea = document.createElement('textarea')
      textArea.style.position = 'fixed'
      textArea.value = this.artist_key
      document.body.appendChild(textArea)
      textArea.focus()
      textArea.select()
      this.close()
            
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