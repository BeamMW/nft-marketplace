<template>
  <div class="my-page">
    <pageTitle title="my page">
      <btnEditArtist/>
      <btnWallet/>
      <btnKey/>
    </pageTitle>
    <p v-if="self_pending" class="description pending">
      <i>
        Your account is pending for approval by moderator.<br>
        Artist name and pictures would not be shown in gallery. Approved NFTs and Collections would still be shown in gallery.
      </i>
    </p>
    <p v-if="self_rejected" class="description rejected">
      <i>
        Your account is rejected by moderator. Edit your profile to resubmit for approval.<br>
        Artist name and pictures would not be shown in gallery. Approved NFTs and Collections would still be shown in gallery.
      </i>
    </p>
    <div class="gallery-container">
      <tabsctrl v-model="active_tab" :tabs="tabs"/>
      <list v-if="show_collections"
            class="list"
            items_name="your collections"
            component="collection"
            new_component="create-collection"
            mode="artist"
            :store="collsStore"
      />
      <list v-if="show_owned"
            class="list"
            items_name="owned NFTs"
            component="nft"
            mode="owned"
            :new_component="is_artist? 'create-nft' : ''"
            :store="nftsStore"
      />
      <list v-if="show_sale"
            class="list"
            items_name="on sale NFTs"
            component="nft"
            mode="owned:sale"
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

    .description {
      font-size: 14px
      text-align: center
      margin: 0px 0px 10px 0px
      font-family: 'SFProDisplay', sans-serif

      &.pending {
        color: #ed69ff
        opacity: 0.8
      }

      &.rejected {
        color: #ff746b
        opacity: 0.8
      }

      & > i {
        display: block
        margin-top: 6px
      }
    }

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
import pageTitle from 'controls/page-title'
import btnWallet from 'controls/btn-wallet'
import btnEditArtist from 'controls/btn-edit-artist'
import btnKey from 'controls/btn-key'
import tabsctrl from 'controls/tabs'
import list from 'controls/lazy-list'
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'
import artistsStore from 'stores/artists'
import {my_tabs} from 'utils/consts'

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

      res.push({id: my_tabs.OWNED_NFTS, name: 'Owned NFTs'})
      res.push({id: my_tabs.SALE_NFTS,  name: 'On Sale'})

      if (this.is_artist) {
        res.push({id: my_tabs.SOLD_NFTS,  name: 'Sold'})
      }

      res.push({id: my_tabs.LIKED_NFTS, name: 'Liked by Me'})
      return res
    },
    self_pending () {
      return artistsStore.self.pending
    },
    self_rejected () {
      return artistsStore.self.rejected
    }
  }
}
</script>
