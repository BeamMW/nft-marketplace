<template>
  <publicKeyModal ref="keyModal"/>
  <pageTitle title="my page">
    <div class="options">
      <btn :hover="false" :height="height" :width="width" :padding="padding">
        <img src="~assets/icon-pencil.svg">
      </btn>

      <btnWallet @click="onBalanceClick"/>
      
      <btn :hover="false" :height="height" :width="width" :padding="padding" @click="onShowKeyClick">
        <img src="~assets/icon-key.svg">
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
    margin-left: 20px
  }
}
</style>

<script>
import pageTitle from './page-title.vue'
import btn from './button.vue'
import btnWallet from './button-wallet.vue'
import publicKeyModal from './public-key-dialog.vue'

import {styles} from '../utils/consts'

export default {
  components: {
    pageTitle,
    btn,
    btnWallet,
    publicKeyModal
  },

  data() {
    return {
      width: styles.WIDTH,
      height: styles.HEIGHT,
      padding: styles.PADDING
    }
  },

  methods: {
    onShowKeyClick() {
      this.$refs.keyModal.open()
    },
    onBalanceClick() {
      this.$store.toBalance()
    }
  },
}
</script>
