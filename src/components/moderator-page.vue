<template>
  <div class="admin-page">
    <pageTitle :title="page_title">
      <btnWallet/>
      <btnKey/>
      <btnProfile/>
    </pageTitle>
    <div class="gallery-container">
      <tabsctrl v-model="active_tab" class="tabs" :tabs="tabs"/>
      <list v-if="show_nfts"
            class="list"
            items_name="NFTs to be reviewed"
            component="approve-nft"
            mode="moderator"
            selectable
            :store="nftsStore"
            :selected="selected_nfts"
            @selected="onNFTSelected"
      />
      <list v-if="show_collections"
            class="list"
            items_name="collections to be reviewed"
            component="approve-collection"
            mode="moderator"
            selectable
            :store="collsStore"
            :selected="selected_collections"
            @selected="onCollectionSelected"
      />
      <list v-if="show_artists"
            class="list"
            items_name="artists to be reviewed"
            component="approve-artist"
            mode="moderator"
            selectable
            :store="artistsStore"
            :selected="selected_artists"
            @selected="onArtistSelected"
      />
      <adminPage v-if="show_admin" class="list"/>
      <div v-if="show_buttons" class="buttons">
        <btn text="cancel" 
             :disabled="!any_selected"
             @click="onCancel"
        >
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn text="reject"
             color="red"
             :disabled="!any_selected"
             @click="onReject"
        >
          <img src="~assets/reject.svg"/>
        </btn>
        <btn text="approve"
             color="green"
             :disabled="!any_selected"
             @click="onApprove"
        >
          <img src="~assets/ok.svg"/>
        </btn>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.admin-page {
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

    & > .buttons {
      display: flex
      flex-direction: row
      justify-content: center
      margin: 20px 0px

      & > * {
        margin-right: 15px
      }
    }
  }
}
</style>

<script>
import adminPage from 'components/admin-page'
import tabsctrl from 'controls/tabs'
import pageTitle from 'controls/page-title'
import btn from 'controls/button'
import btnKey from 'controls/btn-key'
import btnWallet from 'controls/btn-wallet'
import btnProfile from 'controls/btn-profile'
import list from 'controls/lazy-list'
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'
import artistsStore from 'stores/artists-lazy'
import {admin_tabs} from 'utils/consts'

export default {
  components: {
    tabsctrl,
    pageTitle,
    btnKey,
    btnWallet,
    btnProfile,
    adminPage,
    list,
    btn
  },

  data() {
    return {
      selected_nfts: [],
      selected_collections: [],
      selected_artists: []
    }
  },

  computed: {
    page_title() {
      return this.$state.is_admin ? 'admin panel' : 'moderator panel'
    },
    tabs () {
      let tabs = []

      if (this.$state.is_moderator) {
        tabs.push({id: admin_tabs.NFTS, name: 'NFTs'})
        tabs.push({id: admin_tabs.COLLECTIONS, name: 'Collections'})
        tabs.push({id: admin_tabs.ARTISTS, name: 'Artists'})
      }

      if (this.$state.is_admin) {
        tabs.push({id: admin_tabs.ADMIN, name: 'Admin'})
      }

      return tabs
    },
    active_tab: {
      get() {
        return this.$state.admin_active_tab
      },
      set(value) {
        this.$store.setAdminTab(value)
      }
    },
    show_artists() {
      return this.active_tab === admin_tabs.ARTISTS
    },
    show_collections() {
      return this.active_tab == admin_tabs.COLLECTIONS
    },
    show_nfts() {
      return this.active_tab == admin_tabs.NFTS
    },
    show_admin() {
      return this.active_tab == admin_tabs.ADMIN
    },
    show_buttons() {
      return !this.show_admin
    },
    collsStore() {
      return collsStore
    },
    nftsStore() {
      return nftsStore
    },
    artistsStore() {
      return artistsStore
    },
    active_selected() {
      if (this.show_artists) return this.selected_artists
      if (this.show_collections) return this.selected_collections
      return this.selected_nfts
    },
    any_selected() {
      return this.active_selected.length > 0
    },
    active_approve() {
      if (this.show_artists) return this.$store.approveArtists
      if (this.show_collections) return this.$store.approveCollections
      return this.$store.approveNFTs
    }
  },

  methods: {
    onCollectionSelected(id) {
      if (this.selected_collections.includes(id)) {
        this.selected_collections = this.selected_collections.filter(item => item != id)
      } else {
        this.selected_collections.push(id)
      }
    },
    onNFTSelected(id) {
      if (this.selected_nfts.includes(id)) {
        this.selected_nfts = this.selected_nfts.filter(item => item != id)
      } else {
        this.selected_nfts.push(id)
      }
    },
    onArtistSelected(id) {
      if (this.selected_artists.includes(id)) {
        this.selected_artists = this.selected_artists.filter(item => item != id)
      } else {
        this.selected_artists.push(id)
      }
    },
    onCancel() {
      this.active_selected.length = 0
    },
    async onReject() {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      let txid = await this.active_approve.call(this.$store, this.active_selected, false)
      if (txid) this.active_selected.length = 0
    },
    async onApprove() {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 
      
      let txid = await this.active_approve.call(this.$store, this.active_selected, true)
      if (txid) this.active_selected.length = 0
    }
  }
}
</script>
