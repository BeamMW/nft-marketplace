<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">Transfer NFT</div>
      <div class="message">Paste the artist key of the person you want to transfer this NFT to</div>
      <formInput v-model="artist_key" 
                 class="input"
                 max_length="66"
                 tip="Valid key is 66 chars long"
                 :allowed="key_allowed"
      />
      <div class="controls">
        <btn text="close" @click="close">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn id="proceed"
             text="proceed"
             color="green"
             :disabled="!key_valid" 
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
      margin: 30px 0px 25px 0px
      font-size: 14px
    }

    .input {
      width: 570px
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
import formInput from 'controls/form-input'
import nftsStore from 'stores/nfts'

export default {
  components: { 
    modal, 
    btn,
    formInput
  },

  props: {
    id: {
      type: Number,
      required: true
    },
  },

  data() {
    return {
      artist_key: '',
      key_allowed: /^[a-zA-Z0-9]{1,66}$/
    }
  },

  computed: {
    key_valid() {
      return /^[a-zA-Z0-9]{66}$/.test(this.artist_key)
    }
  },

  methods: {
    open () {
      this.$refs.modal.open()
    },

    close () {
      this.$refs.modal.close()
    },

    async onProceed () {
      await nftsStore.transferNFT(this.id, this.artist_key)
      this.close()
    }
  }
}
</script>