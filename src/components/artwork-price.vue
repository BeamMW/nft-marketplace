<template>
  <artworkPriceModal ref="priceModal" @sell-artwork="onSellArtwork"/>
  <!--- has price, so display it --->
  <span v-if="price" class="container">
    <amount :amount="price.amount" :size="mode == 'normal' ? '18px' : '14px'"/>
    
    <!--- has price and owned, display change price / remove from sale options --->
    <template v-if="owned">
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
      <btn v-else text="buy" color="magenta" @click="onBuy">
        <img src="~assets/icon-button.svg">
      </btn>
    </template>  
  </span>

  <span v-if="!price" class="container">
    <!---- doesn't have price & owned, can sell ---->
    <template v-if="owned">
      <btn v-if="compact" 
           text="sell" 
           color="transparent" 
           padding="2px 7px 5px 7px"
           text_color="blue"
           @click="onSell"
      />
      <btn v-else text="sell" color="blue" @click="onSell">
        <img src="~assets/icon-button.svg">
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
.container {
  display: flex
  line-height: 2
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
import btn from './button.vue'
import popupMenu from './popup-menu.vue'
import artworkPriceModal from './price-dialog.vue'
import amount from './amount.vue'
import artistsStore from 'stores/artists.js'

export default {
  components: {
    btn,
    popupMenu,
    artworkPriceModal,
    amount
  },

  props: {
    artwork: {
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
      return this.artwork.id
    },

    owned() {
      return this.artwork.owned
    },

    author () {
      return artistsStore.my_id === this.artwork.pk_author
    },

    price() {
      return this.artwork.price
    },

    compact() {
      return this.mode == 'compact'
    }
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
