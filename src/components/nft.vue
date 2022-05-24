<template>
  <div :class="{'nft': true, 'error': item.error}">
    <!---- Preview OR Loading ---->
    <preview :image="item.image" 
             :default="def_nft"
             height="213px" 
             text_color="dimgray"
             :cover="!(item.image || {}).object"
             @click="onDetails"
    >
      <moderationStatus :item="item"/>
      <div v-if="is_approved" class="likes" :disabled="!can_vote" v-on="{click: liked ? onUnlike : onLike}">
        <div>{{ likes_cnt }}</div>
        <img :src="'/assets/icon-heart' + (liked ? '-red' : '') + '.svg'"/>
      </div>
    </preview>
    
    <!---- Delete NFT Button ---->
    <img v-if="is_admin" class="delete" src="~assets/icon-delete.svg" @click="onDelete"/>

    <!---- First info row ---->
    <div class="info-row">
      <!---- Title ---->
      <div class="title" :class="{'error': item.error}">{{ item.label }}</div>
    </div>

    <!---- Second info row, author ---->
    <div class="info-row" :class="{'error': item.author_error}">
      <span class="author" v-html="item.by_author"></span>
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
      display: flex
      position: absolute
      bottom: 9px
      right: 8px
      align-items: center
      cursor: pointer
      box-sizing: border-box
      background-color: rgba(0, 0, 0, 0.6)
      border-radius: 10px

      & > div {
        padding: 8px 6px 9px 8px
      }
      
      & > img {
        padding: 8px
      }
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
import moderationStatus from 'controls/moderation-status'
import nftsStore from 'stores/nfts'
import {def_images} from 'utils/consts'

export default {
  components: {
    price,
    preview,
    moderationStatus
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  data () {
    return {
      def_nft: def_images.nft,
    }
  },

  computed: {
    is_admin () {
      return this.$state.is_admin
    },

    is_headless () {
      return this.$state.is_headless
    },

    id () {
      return this.item.id
    },

    is_approved() {
      return this.item.status === 'approved'
    },
                
    can_vote () {
      return !this.item.error && this.$state.balance_reward > 0
    },

    liked () {
      return !!this.item.my_impression
    },

    likes_cnt () {
      return this.item.impressions
    },
  },

  methods: {
    onLike (ev) {
      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_vote) {
        nftsStore.likeNFT(this.id)
      }

      ev.stopPropagation()
      return false
    },

    onUnlike (ev) {
      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_vote) {
        nftsStore.unlikeNFT(this.id)
      }

      ev.stopPropagation()
      return false
    },

    onDelete (id) {
      this.$store.deleteNFT(this.id)
    },

    onDetails(ev) {
      nftsStore.toDetails(this.id)
    }
  }
}
</script>