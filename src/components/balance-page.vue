<template>
  <keyModal ref="keyModal"/>
  <pageTitle title="balance">
    <btnEditArtist/>
    <btnKey/>
  </pageTitle>
  <div :class="['balance-container', compact ? 'compact' : '']">
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
          <img src="~assets/receive.svg">
        </btn>
      </div>
    </div>

    <div class="balance">
      <div>
        <div class="description">Total sold NFT amount</div>
        <amount :amount="total_sold.volume"/>
      </div>

      <div>
        <div class="description">NFT sold</div>
        <div>{{ total_sold.count }}</div>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.balance-container {
  margin-bottom: 10px
  margin-top: 20px

  &:not(.compact) {
    display: grid
    grid-template-columns: 1fr 1fr
    column-gap: 20px
  }

  &.compact {
    display: grid
    grid-template-columns: 1fr
    row-gap: 20px
  }

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
import keyModal from 'components/key-modal'
import btn from 'controls/button'
import btnKey from 'controls/btn-key'
import btnEditArtist from 'controls/btn-edit-artist'
import pageTitle from 'controls/page-title'
import amount from 'controls/amount'
import artistsStore from 'stores/artists'
import utils from 'utils/utils'

export default {
  components: {
    keyModal,
    btn,
    pageTitle,
    amount,
    btnEditArtist,
    btnKey
  },

  props: {
    nft_sold: {
      type: Number,
      default: 0
    }
  },

  data () {
    return {
      compact: utils.isCompact()
    }
  },

  computed: {
    balance () {
      return this.$state.balance_beam
    },

    my_name () {
      let self = artistsStore.self
      return (self || {}).label
    },

    total_sold () {
      let self = artistsStore.self
      if (self) return self.total_sold
      return {count: 0, volume: 0, aid: 0}
    }
  },

  methods: {
    onWithdrawClick () {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 
      this.$store.withdrawBEAM()
    }
  }
}
</script>
