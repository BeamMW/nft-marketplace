<template>
  <publicKeyModal ref="keyModal"/>
  <div class="balance-container">
    <div :class="[show_balance ? '' : 'hidden', 'balance']">
      <img src="~assets/icon-beam.svg"/>
      <div class="value">
        {{ balance_beam }} BEAM
      </div>
      <div v-if="balance_beam" class="withdraw" @click="onWithdraw">
        <img class="icon" src="~assets/icon-receive.svg"/>
        <span class="text">withdraw</span>
      </div>
    </div>
    <div class="user" @click="onShowKey">
      <div v-if="my_artist_name" class="name">
        <img class="icon" src="~assets/icon-user.svg"/>
        <span class="text">{{ my_artist_name }}</span>
      </div>
      <div class="key">
        <img class="icon" src="~assets/icon-key.svg"/>
        <span class="text">Show my public key</span>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.balance-container {
  display: flex
  flex-direction: row
  justify-content: space-between
  align-content: center
  margin-bottom: 10px

  .balance {
    width: 442px
    height: 78px
    border-radius: 10px
    background-color: rgba(255,255,255,0.1)
    display: flex
    flex-direction: row
    align-items: center
    padding: 0 20px
    padding-left: 20px

    .value {
      margin-left: 10px
      font-size: 20px
      font-weight: bold
      color: #fff
    }

    .withdraw {
      margin-left: auto
      display: flex
      align-items: center
      cursor: pointer
    }

    .text {
      margin-left: 8px
      font-size: 14px
      font-weight: bold
      color: #0bccf7
    }
  }

  .user {
    display: flex
    flex-direction: column
    align-items: center

    .icon {
      width: 16px
      height: 16px
    }

    .name {
      padding-bottom: 12px
      display: flex
      align-items: center
      
      .text {
        font-size: 18px
        font-weight: bold
        color: #fff
        margin-left: 7px
      }
    }

    .key {
      padding: 14px
      border-radius: 10px
      background-color: rgba(255, 255, 255, 0.1)
      display: flex
      align-items: center
      cursor: pointer

      .text {
        font-size: 16px
        color: #00f6d2
        margin-left: 10px
      }

      .connect {
        font-size: 16px
        color: #00f6d2
      }
    }
  }
}
</style>

<script>
import {common} from '../utils/consts.js'
import publicKeyModal from './public-key-dialog.vue'

export default {
  components: {
    publicKeyModal
  },

  computed: {
    balance_beam () {
      return this.$state.balance_beam / common.GROTHS_IN_BEAM
    },
    my_artist_name () {
      let artist = this.$state.artists[this.$state.my_artist_keys[0]] ||
                         this.$state.artists[this.$state.my_artist_keys[1]]
      return (artist || {}).label
    },
    show_balance () {
      return this.$state.is_artist || this.$state.balance_beam
    }
  },

  methods: {
    onShowKey () {
      this.$refs.keyModal.open()
    },

    onWithdraw () {
      this.$store.withdrawBEAM()
    }
  }
}
</script>
