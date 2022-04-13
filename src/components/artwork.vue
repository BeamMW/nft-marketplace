<template>
  <div class="artwork">
    <!---- Preview OR Loading ---->
    <preview :image="item.image" height="200px" @click="onDetails"/>

    <!---- Delete Artwork Button ---->
    <img v-if="is_admin" class="delete" src="~assets/icon-delete.svg" @click="onDelete"/>

    <!---- First info row ---->
    <div class="info-row">
      <!---- Title ---->
      <div class="title">{{ item.label }}</div>
      <!---- TODO: enable Likes ----->
      <!--div class="likes" :disabled="!can_vote" v-on="{click: liked ? onUnlike : onLike}">
        <img :src="'./assets/icon-heart' + (liked ? '-red' : '') + '.svg'"/>
        <span>{{ likes_cnt }}</span>
      </div-->
    </div>

    <!---- Second info row, author ---->
    <div class="info-row">
      <span class="author">{{ item.by_author }}</span>
    </div>

    <!---- Third info row, price/buy/sell ----->
    <div class="price-row">
      <price :artwork="item" mode="compact"/>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .artwork {
    display: flex
    flex-direction: column
    width: 213px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px
    position:relative

    & > .delete {
      position: absolute
      left: 190px
      top: 5px
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

      & .likes {
        display: flex
        align-items: center
        cursor: pointer
        box-sizing: border-box
        padding-top: 20px

        & > span {
          padding-left: 5px
        }
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
import price from './artwork-price.vue'
import preview from './image-preview.vue'

export default {
  components: {
    price,
    preview
  },

  props: {
    item: {
      type: Object,
      required: true,
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
                
    can_vote () {
      return !this.item.error && this.$state.balance_reward > 0
    },
  },

  methods: {
    onLike () {
      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_vote) {
        this.$store.likeArtwork(this.id)
      }
    },

    onUnlike () {
      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_vote) {
        this.$store.unlikeArtwork(this.id)
      }
    },

    onDelete (id) {
      this.$store.deleteArtwork(this.id)
    },

    onDetails(ev) {
      this.$store.toArtworkDetails(this.id)
    }
  }
}
</script>