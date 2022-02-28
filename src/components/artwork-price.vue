<template>
  <!---- has price & owned, display change price / remove from sale options ---->
  <span v-if="price && owned" class="container">
      <img src="~assets/icon-beam.svg"/>
      <span class="amount">{{amount}}</span>
      <span class="curr">BEAM</span>
      <img class="dots" src="~assets/icon-actions.svg" @click="onSaleMenu">
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
      <span class="amount">{{amount}}</span>
      <span class="curr">BEAM</span>
      <artButton 
        class="button" type="buy" 
        @click="onBuy"
      />
  </span>

  <!---- doesn't have price & owned, can sell ---->
  <span v-if="!price && owned" class="container">
    <artButton 
      class="button" type="sell"
      @click="onSell" 
    />
  </span>

  <!---- doesn't have price & not owned, 
         can be anything - not approved yet, not sold by
         owner &c. Just dispaly that it is not on sale
  ---->
  <span v-if="!price && !owned" class="not-for-sale">
      Not for sale
  </span>
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    line-height: 2
    flex-wrap: wrap
    align-items: center
    width: 100%

    & .amount, & .curr {
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
import { common, popups } from '../utils/consts.js'
import artButton from './art-button.js'
import popupMenu from './popup-menu.vue'

export default {
  props: {
      artwork: {
        type: Object,
        required: true,
      }
  },

  components: {
    artButton,
    popupMenu
  },

  computed: {
    is_headless () {
      return this.$state.is_headless
    },

    id () {
      return this.artwork.id
    },
    
    owned () {
      return this.artwork.owned
    },
    
    price () {
      return this.artwork.price
    },
    
    amount () {
      if (this.price) {
        return (this.price.amount / common.GROTHS_IN_BEAM).toFixed(8).replace(/([0-9]+(\.[0-9]+[1-9])?)(\.?0+$)/,'$1')
      }
    }
  },

  methods: {
    onSaleMenu(ev) {
        this.$refs.saleMenu.open(ev)
    },

    onChangePrice (id) {
      try {
          this.$store.setPopupType(popups.CHANGE_PRICE);
          this.$store.setIdToSell(this.id);
          this.$store.changePopupState(true);
      } 
      catch (err) {
          this.$store.setError(err, "Failed to sell an item");
      }
    },

    onRemoveFromSale () {
      this.$store.sellArtwork(this.id, 0);
    },

    onBuy () {
      if (this.is_headless) return this.$store.switchToHeaded()
      this.$store.buyArtwork(this.id)
    },

    onSell () {
      try {
        this.$store.setPopupType(popups.SELL)
        this.$store.setIdToSell(this.id)
        this.$store.changePopupState(true)
      } 
      catch (err) {
          this.$store.setError(err, "Failed to sell an item")
      }
    }
  }
}
</script>
