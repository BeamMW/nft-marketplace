<template>
  <div class="my-page">
    <pageTitle title="my page">
      <btnEditArtist/>
      <btnWallet/>
      <btnKey/>
    </pageTitle>
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
      <list v-if="show_owned"
            class="list"
            items_name="NFTs"
            component="nft"
            mode="owner"
            :new_component="is_artist? 'create-nft' : ''"
            :store="nftsStore"
      />
      <list v-if="show_sale"
            class="list"
            items_name="on sale NFTs"
            component="nft"
            mode="owner:sale"
            :store="nftsStore"
      />
      <list v-if="show_sold"
            class="list"
            items_name="sold NFTs"
            component="nft"
            mode="artist:sold"
            :store="nftsStore"
      />
      <list v-if="show_liked"
            class="list"
            items_name="liked NFTs"
            component="nft"
            mode="liker:liked"
            :store="nftsStore"
      />
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .my-page {
    width: 100%
    height: 100%
    display: flex
    flex-direction: column

    & > .gallery-container {
      flex: 1
      box-sizing: border-box
      display: flex
      flex-direction: column
      min-height: 0
      margin-top: 15px

      & > .list {
        margin-top: 20px
        flex: 1
      }
    }
  }
</style>

<script>
import pageTitle from './page-title.vue'
import btnWallet from './btn-wallet'
import btnEditArtist from './btn-edit-artist'
import btnKey from './btn-key'
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'
import artistsStore from 'stores/artists'
import {my_tabs} from 'utils/consts.js'
import tabsctrl from './tabs.vue'
import list from './items-list.vue'

export default {
  components: {
    pageTitle,
    btnEditArtist,
    btnWallet,
    btnKey,
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
    nftsStore() {
      return nftsStore
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
    show_owned() {
      return this.active_tab === my_tabs.OWNED_NFTS
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
        res.push({id: my_tabs.COLLECTIONS, name: 'Collections'})
      }

      res.push({id: my_tabs.OWNED_NFTS, name: 'NFTs'})
      res.push({id: my_tabs.SALE_NFTS,  name: 'On Sale'})

      if (this.is_artist) {
        res.push({id: my_tabs.SOLD_NFTS,  name: 'Sold'})
      }

      res.push({id: my_tabs.LIKED_NFTS, name: 'Liked'})
      return res
    }
  }
}
</script>
