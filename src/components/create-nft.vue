<template>
  <div v-if="has_collections" class="new-box" @click="onNewNFT">
    Create new NFT
  </div>
  <div v-else class="new-box" @click="onNewCollection">
    To create new NFT, you need<br>
    to have a collection.<br><br>
    Click here to create<br>a collection.
  </div>
</template>

<style lang="stylus" scoped>
.new-box {
  width: 213px
  height: 309px
  border-radius: 10px
  border: 1px dashed rgba(26, 246, 214, 0.5)
  background-color: rgba(26, 246, 214, 0.1)
  cursor: pointer
  color: rgba(26, 246, 214, 0.7)
  font-size: 14px

  display: flex
  align-items: center
  justify-content: center
  text-align: center
}
</style>

<script>
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'

export default {
  props: {
    selected_collection: {
      type: Number,
      default: undefined,
      required: false
    } 
  },
  computed: {
    has_collections() {
      return collsStore.getAllItemsCount('artist') > 0
    }
  },

  methods: {
    onNewNFT () {
      nftsStore.toNewItem(this.selected_collection)
    },
    
    onNewCollection() {
      collsStore.toNewItem()
    }
  } 
}
</script>