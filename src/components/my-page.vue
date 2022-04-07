<template>
  <publicKeyModal ref="keyModal"/>
  <pageTitle title="my page">
    <div class="options">
      <btn v-if="is_artist" 
           :height="height" 
           :width="width" 
           padding="0px" 
           radius="10px" 
           @click="onEditArtist"
      >
        <img src="~assets/icon-pencil.svg">
      </btn>

      <btnWallet @click="onBalanceClick"/>
      
      <btn :height="height" :width="width" radius="10px" padding="0px" @click="onShowKeyClick">
        <img src="~assets/icon-key.svg">
      </btn>

      <btn v-if="!is_artist" text="become an artist" color="green" height="34px" @click="onBecomeArtist">
        <img src="~assets/add-user.svg"/>
      </btn>
    </div>
  </pageTitle>
</template>

<style scoped lang="stylus">
.options {
  display: flex
  justify-content: flex-end
  align-items: center

  & > button {
    border-radius: 10px
    margin-left: 12px
    margin-top: 7px

    &:last-child {
      margin-right: 7px
    }
  }
}
</style>

<script>
import pageTitle from './page-title.vue'
import btn from './button.vue'
import btnWallet from './button-wallet.vue'
import publicKeyModal from './public-key-dialog.vue'
import artistsStore from 'store/store-artists.js'

export default {
  components: {
    pageTitle,
    btn,
    btnWallet,
    publicKeyModal
  },

  data() {
    return {
      width: '34px',
      height: '34px'
    }
  },

  computed: {
    is_artist() {
      return artistsStore.is_artist
    }
  },

  methods: {
    onShowKeyClick() {
      this.$refs.keyModal.open()
    },
    onBalanceClick() {
      this.$store.toBalance()
    },
    onEditArtist() {
      artistsStore.toEditArtist()
    },
    onBecomeArtist() {
      artistsStore.toBecomeArtist()
    }
  },
}
</script>
