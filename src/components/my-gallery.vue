<template>
  <div class="gallery-container">
    <tabsctrl v-model="active_tab" :tabs="tabs"/>
    <list v-if="show_collections"
          class="list"
          items_name="collections"
          component="collection"
          new_component="create-collection"
          mode="artist"
          :store="collsStore"
    />
    <list v-if="show_created"
          class="list"
          items_name="NFTs"
          component="artwork"
          new_component="create-nft"
          mode="artist"
          :store="artsStore"
    />
    <list v-if="show_owned"
          class="list"
          items_name="NFTs"
          component="artwork"
          mode="owner"
          :store="artsStore"
    />
    <list v-if="show_sale"
          class="list"
          items_name="on sale NFTs"
          component="artwork"
          mode="owner:sale"
          :store="artsStore"
    />
    <list v-if="show_sold"
          class="list"
          items_name="sold NFTs"
          component="artwork"
          mode="artist:sold"
          :store="artsStore"
    />
    <list v-if="show_liked"
          class="list"
          items_name="liked NFTs"
          component="artwork"
          mode="liker:liked"
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
    is_admin() {
      return this.$state.is_admin
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
    show_sale() {
      return this.active_tab == my_tabs.SALE_NFTS
    },
    show_sold() {
      return this.active_tab == my_tabs.SOLD_NFTS
    },
    show_liked() {
      return this.active_tab == my_tabs.LIKED_NFTS
    },
    tabs() {
      let res = []

      if (this.is_artist) {
        res.push({id: my_tabs.COLLECTIONS, name: 'Created Collections'})
        res.push({id: my_tabs.CREATED_NFTS, name: 'Created NFTs'})
        res.push({id: my_tabs.OWNED_NFTS, name: 'Owned NFTs'})
      }

      if (!this.is_artist) {
        res.push({id: my_tabs.OWNED_NFTS, name: 'All'})
      }

      res.push({id: my_tabs.SALE_NFTS,  name: 'On Sale'})

      if (this.is_artist) {
        res.push({id: my_tabs.SOLD_NFTS,  name: 'Sold'})
      }

      res.push({id: my_tabs.LIKED_NFTS, name: 'Liked'})
      return res
    },
  }
}
</script>
