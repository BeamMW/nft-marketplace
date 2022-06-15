<template>
  <div :class="{'nft': true, 'error': item.error}" @click="onDetails">
    <!---- Preview OR Loading ---->
    <preview :image="item.image" 
             :default="def_nft"
             height="213px" 
             text_color="dimgray"
             contain
    >
      <div v-if="error && debug" class="debug error">{{ error }}</div>
      <moderationStatus :item="item"/>
      <likes class="likes" :item="item"/>
    </preview>
    
    <!---- Delete NFT Button ---->
    <img v-if="is_admin" class="delete" src="~assets/delete.svg" @click="onDelete"/>

    <!---- First info row ---->
    <div class="info-row">
      <!---- Title ---->
      <div class="title" :class="{'error': item.error}">{{ label }}</div>
    </div>

    <!---- Second info row, author ---->
    <div class="info-row" :class="{'error': item.author_error}">
      <span class="author" v-html="by_author"></span>
    </div>

    <!---- Third info row, price/buy/sell ----->
    <div class="price-row">
      <price :nft="item" mode="compact"/>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .nft {
    display: flex
    flex-direction: column
    width: 213px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px
    position:relative
    overflow: hidden
    cursor: pointer

    & .debug {
      text-align: center
      color: black
      padding: 0px 5px
    }

    & > .delete {
      position: absolute
      left: 190px
      top: 8px
      width: 15px
      cursor: pointer
    }

    & > .info-row {
      box-sizing: border-box
      padding: 0 12px 3px 12px
      display: flex
      align-items: center
      flex-direction: row
      overflow: hidden

      & .title {
        font-size: 14px
        color: #fff
        white-space: nowrap
        text-overflow: ellipsis
        overflow: hidden
        padding-top: 14px
        flex: 1
      }

      & .author {
        font-size: 12px
        color: rgba(255, 255, 255, 0.5)
        display: inline-block
        white-space: nowrap
        overflow: hidden
        text-overflow: ellipsis
      }
    }

    & .likes {
      position: absolute
      bottom: 9px
      right: 8px
    }

    & .price-row {
      min-height: 31px
      margin: 3px 0 7px 0
      padding: 0 12px 0 14px
      display: flex
      align-items: center
      flex-direction: row
    }
  }
</style>

<script>
import price from 'controls/price'
import preview from 'controls/preview'
import likes from 'controls/likes'
import moderationStatus from 'controls/moderation-status'
import nftsStore from 'stores/nfts'
import artistsStore from 'stores/artists'
import {def_images} from 'utils/consts'

export default {
  components: {
    price,
    preview,
    likes,
    moderationStatus
  },

  props: {
    item: {
      type: Object,
      required: true,
    },
    mode: {
      type: String,
      default: 'user'
    },
  },

  data () {
    return {
      // eslint-disable-next-line no-undef
      def_nft: def_images.nft
    }
  },

  computed: {
    label () {
      return this.$state.debug ? `[${this.item.id}] - ${this.item.label}` : this.item.label
    },

    is_admin () {
      return this.$state.is_admin
    },

    is_headless () {
      return this.$state.is_headless
    },

    id () {
      return this.item.id
    },

    by_author() {
      return this.mode === 'artist' && this.item.author === artistsStore.my_key ? this.item.by_author : this.item.safe_by_author
    },

    error() {
      return this.item.error
    },

    debug() {
      return this.$state.debug
    }
  },

  methods: {
    onDelete (id) {
      this.$store.deleteNFT(this.id)
    },

    onDetails(ev) {
      nftsStore.toDetails(this.id, this.mode)
    }
  }
}
</script>