<template>
  <priceModal ref="priceModal" @sell-nft="onSellNFT"/>
  <!--- has price, so display it --->
  <span v-if="price" class="price-container">
    <amount :amount="price.amount" :size="mode == 'normal' ? '18px' : '14px'"/>
    
    <!--- has price and owned, display change price / remove from sale options --->
    <template v-if="owned">
      <img class="dots" src="~assets/actions.svg" @click="onSaleMenu"/>
      <popupMenu ref="saleMenu">
        <div class="item" @click="onChangePrice">
          <img src="~assets/change.svg"/>
          update the price
        </div>
        <div class="item" @click="onRemoveFromSale">
          <img src="~assets/eye-crossed.svg"/>
          remove from sale
        </div>
      </popupMenu>
    </template>

    <!---- has price but not owned, can buy ---->
    <template v-if="!owned">
      <btn v-if="compact" 
           text="buy" 
           color="transparent" 
           padding="2px 7px 5px 7px"
           text_color="magenta"
           @click="onBuy"
      />
      <btn v-else 
           text="buy" 
           color="magenta"
           height="33px" 
           @click="onBuy"
      >
        <img src="~assets/buy-sell.svg">
      </btn>
    </template>  
  </span>

  <span v-if="!price" class="price-container">
    <!---- doesn't have price & owned, can sell ---->
    <template v-if="owned">
      <btn v-if="compact" 
           text="sell" 
           color="transparent" 
           padding="0px 7px 0px 7px"
           height="31px"
           text_color="blue"
           @click="onSell"
      />
      <btn v-else 
           text="sell" 
           color="blue" 
           height="33px"
           @click="onSell"
      >
        <img src="~assets/buy-sell.svg">
      </btn>
    </template>

    <!---- doesn't have price & not owned & author, means sold ----> 
    <!-- TODO: take into account moderation / ban -->
    <span v-if="!owned && author" class="not-for-sale">Sold</span>

    <!---- doesn't have price & not owned & author,     
           can be anything - not approved yet, not sold by
           owner &c. Just dispaly that it is not on sale
    ---->
    <span v-if="!owned && !author" class="not-for-sale">Not for sale</span>
  </span>
</template>

<style scoped lang="stylus">
.price-container {
  display: flex
  flex-wrap: wrap
  align-items: center
  width: 100%

  & .dots {
    margin-left: auto
    cursor: pointer
    margin-right: -4px
  }

  & .button {
    margin-left: auto
  }
}

.not-for-sale {
  margin-left: auto
  opacity: 0.5
  font-size: 14px
  font-style: italic
  color: #fff
}
</style>

<script>
import priceModal from 'components/price-modal'
import btn from 'controls/button'
import popupMenu from 'controls/popup-menu'
import amount from 'controls/amount'
import nftsStore from 'stores/nfts'
import artistsStore from 'stores/artists'

export default {
  components: {
    btn,
    popupMenu,
    priceModal,
    amount
  },

  props: {
    nft: {
      type: Object,
      required: true,
    },
    mode: {
      type: String,
      default: 'normal'
    }
  },

  computed: {
    is_headless() {
      return this.$state.is_headless
    },

    id() {
      return this.nft.id
    },

    owned() {
      return this.nft.owned
    },

    author () {
      return artistsStore.my_id === artistsStore.author
    },

    price () {
      return this.nft.price
    },

    compact() {
      return this.mode == 'compact'
    }
  },

  methods: {
    onSaleMenu(ev) {
      ev.stopPropagation()
      this.$refs.saleMenu.open(ev)
    },

    onChangePrice(ev) {
      ev.stopPropagation()
      this.$refs.priceModal.open()
    },

    onSell (ev) {
      ev.stopPropagation()
      this.$refs.priceModal.open()
    },

    onBuy (ev) {
      ev.stopPropagation()
      if (this.is_headless) return this.$store.switchToHeaded()
      nftsStore.buyNFT(this.id)
    },

    onSellNFT (price) {
      nftsStore.setPrice(this.id, price)
    },

    onRemoveFromSale (ev) {
      ev.stopPropagation()
      nftsStore.setPrice(this.id, 0)
    },
  }
}
</script>
