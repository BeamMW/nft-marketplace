<template>
  <div v-if="is_approved" class="likes-container" :disabled="!can_act" v-on="{click: liked ? onUnlike : onLike}">
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

    // Only a first vote is paid from the pool, so un-liking needs no funds.
    can_vote () {
      return !this.item.error && this.$state.balance_reward > 0
    },

    can_unvote () {
      return !this.item.error
    },

    can_act () {
      return this.liked ? this.can_unvote : this.can_vote
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

    is_headless () {
      return this.$state.is_headless
    },
  },

  methods: {
    onLike (ev) {
      ev.stopPropagation()

      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_vote) {
        return nftsStore.likeNFT(this.id)
      }
      
      return false
    },

    onUnlike (ev) {
      ev.stopPropagation()

      if (this.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      if (this.can_unvote) {
        return nftsStore.unlikeNFT(this.id)
      }

      return false
    }
  }
}
</script>
