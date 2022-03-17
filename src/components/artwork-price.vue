<template>
  <artworkPriceModal ref="priceModal" @sell-artwork="onSellArtwork"/>
  <!---- has price & owned, display change price / remove from sale options ---->
  <span v-if="price && owned" class="container">
    <img src="~assets/icon-beam.svg"/>
    <span class="amount">{{ amount }}</span>
    <span class="curr">BEAM</span>
    <img class="dots" src="~assets/icon-actions.svg" @click="onSaleMenu"/>
    <popupMenu ref="saleMenu">
      <div class="item" @click="onChangePrice">
        <img src="~assets/icon-change.svg"/>
        update the price
      </div>
      <div class="item" @click="onRemoveFromSale">
        <img src="~assets/icon-eye-crossed.svg"/>
        remove from sale
      </div>
    </popupMenu>
  </span>

  <!---- has price but not owned, can buy ---->
  <span v-if="price && !owned" class="container">
    <img src="~assets/icon-beam.svg"/>
    <span class="amount">{{ amount }}</span>
    <span class="curr">BEAM</span>
    <artButton text="buy" 
               color="magenta"
               icon="./assets/icon-button.svg"
               @click="onBuy"
    />
  </span>

  <!---- doesn't have price & owned, can sell ---->
  <span v-if="!price && owned" class="container">
    <artButton text="sell" 
               color="blue"
               icon="./assets/icon-button.svg"
               @click="onSell"
    />
  </span>

  <!---- doesn't have price & not owned, 
         can be anything - not approved yet, not sold by
         owner &c. Just dispaly that it is not on sale
  ---->
  <span v-if="!price && !owned" class="not-for-sale"> Not for sale </span>
</template>

<style scoped lang="stylus">
.container {
  display: flex
  line-height: 2
  flex-wrap: wrap
  align-items: center
  width: 100%

  & .amount
  & .curr {
    margin-left: 5px
    font-size: 18px
    font-weight: bold
    color: #fff
  }

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
import utils from '../utils/utils.js'
import artButton from './button.vue'
import popupMenu from './popup-menu.vue'
import artworkPriceModal from './price-dialog.vue'

export default {
  components: {
    artButton,
    popupMenu,
    artworkPriceModal,
  },

  props: {
    artwork: {
      type: Object,
      required: true,
    },
  },

  computed: {
    is_headless() {
      return this.$state.is_headless
    },

    id() {
      return this.artwork.id
    },

    owned() {
      return this.artwork.owned
    },

    price() {
      return this.artwork.price
    },

    amount() {
      if (this.price) {
        return utils.formatAmount(this.price.amount)
      }
      return undefined
    },
  },

  methods: {
    onSaleMenu(ev) {
      this.$refs.saleMenu.open(ev)
    },

    onChangePrice() {
      this.$refs.priceModal.open()
    },

    onSell () {
      this.$refs.priceModal.open()
    },

    onSellArtwork (price) {
      this.$store.sellArtwork(this.id, price)
    },

    onRemoveFromSale () {
      this.$store.sellArtwork(this.id, 0)
    },

    onBuy () {
      if (this.is_headless) return this.$store.switchToHeaded()
      this.$store.buyArtwork(this.id)
    }
  }
}
</script>
