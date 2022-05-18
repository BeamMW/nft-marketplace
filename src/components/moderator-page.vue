<template>
  <div class="admin-page">
    <pageTitle :title="page_title">
      <btnWallet/>
      <btnKey/>
      <btnProfile/>
    </pageTitle>
    <div class="gallery-container">
      <tabsctrl v-model="active_tab" class="tabs" :tabs="tabs"/>
      <list v-if="show_collections"
            class="list"
            items_name="collections"
            component="approve-collection"
            mode="moderator"
            selectable
            selector_left="6px"
            selector_top="6px"
            :store="collsStore"
            :selected="selected_collections"
            @selected="onCollectionSelected"
      />
      <list v-if="show_nfts"
            class="list"
            items_name="NFTs"
            component="approve-nft"
            mode="moderator"
            selectable
            selector_left="20px"
            selector_top="18px"
            :store="artsStore"
            :selected="selected_artworks"
            @selected="onArtworkSelected"
      />
      <div class="buttons">
        <btn text="cancel" 
             :disabled="!any_selected"
             @click="onCancel"
        >
          <img src="~assets/icon-cancel.svg"/>
        </btn>
        <btn text="reject"
             color="red"
             :disabled="!any_selected"
             @click="onReject"
        >
          <img src="~assets/icon-reject.svg"/>
        </btn>
        <btn text="publish"
             color="green"
             :disabled="!any_selected"
             @click="onPublish"
        >
          <img src="~assets/icon-ok.svg"/>
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
import tabsctrl from './tabs'
import pageTitle from './page-title'
import btnKey from './btn-key'
import btnWallet from './btn-wallet'
import btnProfile from './btn-profile'
import {admin_tabs} from 'utils/consts'
import list from './items-list.vue'
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'
import btn from './button.vue'

export default {
  components: {
    tabsctrl,
    pageTitle,
    btnKey,
    btnWallet,
    btnProfile,
    list,
    btn
  },

  data() {
    return {
      tabs: [
        {id: admin_tabs.COLLECTIONS, name: 'Collections'},
        {id: admin_tabs.NFTS, name: 'NFTs'}
      ],
      selected_artworks: [],
      selected_collections: []
    }
  },

  computed: {
    page_title() {
      return this.$state.is_admin ? 'admin panel' : 'moderator panel'
    },
    active_tab: {
      get() {
        return this.$state.admin_active_tab
      },
      set(value) {
        this.$store.setAdminTab(value)
      }
    },
    show_collections() {
      return this.active_tab == admin_tabs.COLLECTIONS
    },
    show_nfts() {
      return this.active_tab == admin_tabs.NFTS
    },
    collsStore() {
      return collsStore
    },
    artsStore() {
      return artsStore
    },
    active_selected() {
      return this.show_collections ? this.selected_collections : this.selected_artworks
    },
    any_selected() {
      return this.active_selected.length > 0
    },
    active_approve() {
      return this.show_collections ? this.$store.approveCollections : this.$store.approveArtworks
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
    onArtworkSelected(id) {
      if (this.selected_artworks.includes(id)) {
        this.selected_artworks = this.selected_artworks.filter(item => item != id)
      } else {
        this.selected_artworks.push(id)
      }
    },
    onCancel() {
      this.active_selected.length = 0
    },
    onReject() {
      this.active_approve.call(this.$store, this.active_selected, false)
    },
    onPublish() {
      this.active_approve.call(this.$store, this.active_selected, true)
    }
  }
}
</script>
