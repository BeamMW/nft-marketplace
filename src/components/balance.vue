<template>
  <publicKeyModal ref="keyModal"/>
  <pageTitle title="balance"/>
  <div class="balance-container">
    <div :class="[show_balance ? '' : 'hidden', 'balance']">
      <div class="balance__amount">
        <span class="balance__description">Current balance</span>
        <amount/>
      </div>

      <div class="balance__options">
        <btn v-if="balance_beam" 
             class="withdraw" 
             text="withdraw"
             text_color="blue"
             color="transparent"
             height="20px"
             padding="0"
             @click="onWithdrawClick"
        >
          <img src="~assets/icon-receive.svg">
        </btn>
      </div>
    </div>

    <div :class="[show_balance ? '' : 'hidden', 'balance']">
      <div class="balance__amount">
        <span class="balance__description">Total sold NFT amount</span>
        <amount/>
      </div>

      <div class="balance__amount">
        <span class="balance__description">NFT sold</span>
        <span class="balance__amount-sold">{{ nft_sold }}</span>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.balance-container {
  display: grid
  grid-template-columns: 1fr 1fr
  column-gap: 20px
  margin-bottom: 10px
  margin-top: 5px

  .balance {
    background-color: rgba(255, 255, 255, 0.1)
    display: grid
    grid-template-columns: auto auto
    border-radius: 10px
    padding: 20px 20px
    
    &__amount {
      display: flex
      flex-grow: 1
      flex-direction: column
    }

    &__description {
      font-size: 12px
      color: rgba(255, 255, 255, 0.5)
    }
    
    &__options {
      display: flex
      justify-content: flex-end
      align-items: flex-end
    }

    &__amount-sold {
      display: flex
      flex-grow: 1
      align-items: center
      margin-top: 6px
    }
  }
}

.withdraw {
  margin-bottom: 3px
  margin-left: 30px
}
</style>

<script>
import {common} from '../utils/consts.js'
import publicKeyModal from './public-key-dialog.vue'
import btn from './button.vue'
import pageTitle from './page-title.vue'
import amount from './amount.vue'

export default {
  components: {
    publicKeyModal,
    btn,
    pageTitle,
    amount
  },

  props: {
    nft_sold: {
      type: Number,
      default: 14 // for example
    }
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

    onWithdrawClick () {
      this.$store.withdrawBEAM()
    }
  }
}
</script>
