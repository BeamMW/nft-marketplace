<template>
  <div class="approve-nft" :class="{'error': item.error}">
    <div v-if="item.error" class="info-error">
      Failed to load NFT<br>NFT ID: {{ item.id }}<br>{{ item.error }}
    </div>
    <template v-else>
      <div class="info">
        <div class="type">Name</div>
        <div class="value">{{ item.label }}</div>
        <div v-if="item.description" class="type">Description</div>
        <div v-if="item.description" class="value">{{ item.description }}</div>
      </div>
      <preview :image="item.image" 
               :default="def_nft"
               radius="10px"
               width="200px"
               height="200px"
               cover
      />
    </template>
  </div>
</template>

<style lang="stylus" scoped>
.approve-nft {
  background-color: rgba(255, 255, 255, 0.05)
  border-radius: 10px
  box-sizing: border-box
  padding: 20px 20px 20px 70px
  display:flex
  flex-direction: row
  font-size: 14px

  & > .info-error {
    flex: 1
  }

  & > .info {
    flex: 1
    display: grid
    margin-right: 20px
    grid-template-columns: 100px auto
    align-content: start

    & > .type {
      color: #8da1ad
      margin-bottom: 16px
      user-select: none
    }

    & > .value {
      color: #fff
    }
  }
}
</style>

<script>
import preview from 'controls/preview'
import {def_images} from 'utils/consts'

export default {
  components: {
    preview,
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  data () {
    return {
      def_nft: def_images.nft
    }
  },
}
</script>
