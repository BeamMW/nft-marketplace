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
            items_name="collections to be reviewed"
            component="approve-collection"
            mode="moderator"
            selectable
            :store="collsStore"
            :selected="selected_collections"
            @selected="onCollectionSelected"
      />
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
        <btn text="approve"
             color="green"
             :disabled="!any_selected"
             @click="onApprove"
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
import nftsStore from 'stores/nfts'
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
      selected_nfts: [],
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
    nftsStore() {
      return nftsStore
    },
    active_selected() {
      return this.show_collections ? this.selected_collections : this.selected_nfts
    },
    any_selected() {
      return this.active_selected.length > 0
    },
    active_approve() {
      return this.show_collections ? this.$store.approveCollections : this.$store.approveNFTs
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
    onCancel() {
      this.active_selected.length = 0
    },
    onReject() {
      this.active_approve.call(this.$store, this.active_selected, false)
    },
    onApprove() {
      this.active_approve.call(this.$store, this.active_selected, true)
    }
  }
}
</script>
