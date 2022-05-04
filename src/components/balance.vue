<template>
  <publicKeyModal ref="keyModal"/>
  <pageTitle title="balance"/>
  <div class="balance-container">
    <div class="balance">
      <div>
        <div class="description">Current balance</div>
        <amount :amount="balance"/>
      </div>

      <div v-if="balance" class="withdraw">
        <btn text="withdraw"
             text_color="blue"
             color="transparent"
             height="20px"
             padding="0px"
             @click="onWithdrawClick"
        >
          <img src="~assets/icon-receive.svg">
        </btn>
      </div>
    </div>

    <div class="balance">
      <div>
        <div class="description">Total sold NFT amount</div>
        <amount/>
      </div>

      <div>
        <div class="description">NFT sold</div>
        <div>{{ nft_sold }}</div>
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
    padding: 20px

    & .description {
      font-size: 12px
      color: rgba(255, 255, 255, 0.5)
      margin-bottom: 7px
    }
    
    & .withdraw {
      align-self: end
      display: flex
      justify-content: flex-end
      padding-bottom: 10px
    }
  }
}
</style>

<script>
import publicKeyModal from './public-key-dialog.vue'
import btn from './button.vue'
import pageTitle from './page-title.vue'
import amount from './amount.vue'
import artistsStore from 'stores/artists.js'

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
    balance () {
      return this.$state.balance
    },

    my_artist_name () {
      let artist = artistsStore.self
      return (artist || {}).label
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
