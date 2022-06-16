<template>
  <div v-if="is_approved" class="likes-container" :disabled="!can_vote" v-on="{click: liked ? onUnlike : onLike}">
    <div>{{ likes }}</div>
    <img :src="like_icon"/>
  </div>
</template>

<style lang="stylus" scoped>
  & .likes-container {
    display: flex
    align-items: center
    cursor: pointer
    box-sizing: border-box
    background-color: rgba(0, 0, 0, 0.6)
    border-radius: 10px

    & > div {
      padding: 6px 0px 8px 12px
    }
    
    & > img {
      padding: 8px
    }
  }
</style>

<script>
import nftsStore from 'stores/nfts'

export default {
  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  computed: {
    id () {
      return this.item.id
    },

    is_approved() {
      return this.item.approved
    },

    can_vote () {
      return !this.item.error && this.$state.balance_reward > 0
    },

    liked () {
      return !!this.item.my_like
    },

    likes () {
      return this.item.likes || 0
    },

    like_icon() {
      let liked = require('assets/heart-red.svg')
      let unliked = require('assets/heart.svg')
      return this.liked ? liked : unliked
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
    }
  }
}
</script>
