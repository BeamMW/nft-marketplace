<template>
  <div class="approve-nft" :class="{'error': item.error}">
    <div class="info">
      <div class="type">Name</div>
      <div class="value">{{ item.label }}</div>
      <div v-if="item.description" class="type">Description</div>
      <div v-if="item.description" class="value">{{ item.description }}</div>
    </div>
    <imagePreivew :image="item.image" 
                  :default="def_artwork"
                  radius="10px"
                  width="100px"
                  height="100px"
                  cover
    >
      <moderationStatus :item="item" short/>
    </imagePreivew>
  </div>
</template>

<style lang="stylus" scoped>
.approve-nft {
  width: 912px
  min-height: 140px
  background-color: rgba(255, 255, 255, 0.05)
  border-radius: 10px
  box-sizing: border-box
  padding: 20px 20px 20px 70px
  display:flex
  flex-direction: row

  & > .info {
    flex: 1
    display: grid
    margin-right: 20px
    grid-template-columns: 100px auto
    align-content: start

    & > .type {
      font-size: 14px
      color: #8da1ad
      margin-bottom: 16px
    }

    & > .value {
      font-size: 14px
      color: #fff
    }
  }
}
</style>

<script>
import imagePreivew from './image-preview.vue'
import {def_images} from 'utils/consts'
import moderationStatus from './moderation-status.vue'

export default {
  components: {
    imagePreivew,
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
      def_artwork: def_images.artwork,
    }
  },
}
</script>
