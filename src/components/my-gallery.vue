<template>
  <div class="gallery-container">
    <tabsctrl v-model="active_tab" :tabs="tabs"/>
    <list v-if="show_collections"
          class="list"
          emptymsg="There are no collections at the moment"
          component="collection"
          new_component="create-collection"
          mode="artist"
          :store="collsStore"
    />
    <list v-if="show_created"
          class="list"
          emptymsg="There are no NFTs at the moment"
          component="artwork"
          new_component="create-nft"
          mode="artist"
          :store="artsStore"
    />
    <list v-if="show_owned"
          class="list"
          emptymsg="There are no NFTs at the moment"
          component="artwork"
          mode="owner"
          :store="artsStore"
    />
  </div>
</template>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    min-height: 0
    margin-top: 15px
    
    & > .list {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'
import artistsStore from 'stores/artists'
import {my_tabs} from 'utils/consts.js'
import tabsctrl from './tabs.vue'
import list from './items-list.vue'

export default {
  components: {
    tabsctrl,
    list
  },

  computed: {
    active_tab: {
      get() {
        return this.$state.my_active_tab
      },
      set(value) {
        this.$store.setMyTab(value)
      }
    },
    collsStore() {
      return collsStore
    },
    artsStore() {
      return artsStore
    },
    is_artist() {
      return artistsStore.is_artist
    },
    show_collections() {
      return this.active_tab === my_tabs.COLLECTIONS
    },
    show_created() {
      return this.active_tab === my_tabs.CREATED_NFTS
    },
    show_owned() {
      return this.active_tab == my_tabs.OWNED_NFTS
    },
    tabs() {
      let res = []
      if (this.is_artist) {
        res.push({id: my_tabs.COLLECTIONS, name: 'Collections'})
        res.push({id: my_tabs.CREATED_NFTS, name: 'NFTs'})
      }
      res.push({id: my_tabs.OWNED_NFTS, name: 'All'})
      return res
    },
  }
}
</script>
