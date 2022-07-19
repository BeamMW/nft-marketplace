<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">{{ name }} Artist Key</div>
      <div class="data">
        <span>{{ computed_key }}</span>
        <btn class="icon-copy" color="transparent" padding="11px 0 11px 10px" @click="onCopy">
          <img src="~assets/copy.svg"/>
        </btn>
      </div>
      <div class="controls">
        <btn text="close" @click="close">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn text="copy and close" color="green" @click="onCopyAndClose">
          <img src="~assets/copy-blue.svg"/>
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
import modal from 'controls/modal'
import btn from 'controls/button'
import utils from 'utils/utils'
import artistsStore from 'stores/artists'

export default {
  components: { 
    modal, btn
  },

  props: {
    name: {
      type: String,
      default: 'Your'
    },
    artist_key: {
      type: String,
      default: undefined
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

    open () {
      this.$refs.modal.open()
    },

    close () {
      this.$refs.modal.close()
    }
  }
}
</script>