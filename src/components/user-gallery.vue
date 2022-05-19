<template>
  <div class="gallery-container">
    <tabsctrl v-model="active_tab" class="tabs" :tabs="tabs">
      <!-- div v-if="show_nfts" class="selectors">
        <selector
              v-on:selected="onAuthor"
              :options="artist_options"
              :selected="active_filter_by_artist"
              title="Author"
              v-if="artist_options.length"
        />
        <selector v-model:selected="nfts_sort_by"
                  :options="selector_options"
                  title="Sort by"
        />
      </div -->
      <btn v-if="can_admin" 
           :text="admin_btn_text"
           class="slotted-button"
           text_color="green"
           color="transparent"
           padding="11px 10px"
           @click="onAdmin"
      />
      <btnWallet class="slotted-button"/>
      <btnKey class="slotted-button"/>
      <btnProfile class="slotted-button"/>
    </tabsctrl>
    <!-- searchInput v-model:search="search" class="search_container" :max_length="20" placeholder="Search by artist, NFT or collection name..."/>
    {{ search }} -->
    <list v-if="show_collections"
          class="list"
          items_name="collections"
          component="collection"
          mode="user"
          :store="collsStore"
    />
    <list v-if="show_nfts"
          class="list"
          items_name="NFTs"
          component="nft"
          mode="user"
          :store="nftsStore"
    />
    <list v-if="show_sale"
          class="list"
          items_name="NFTs om sale"
          component="nft"
          mode="user:sale"
          :store="nftsStore"
    />
    <list v-if="show_liked"
          class="list"
          items_name="liked NFTs"
          component="nft"
          mode="user:liked"
          :store="nftsStore"
    />
  </div>
</template>

<style lang="stylus">
.tabs {
  & > .selectors {
    padding-right: 12px
    margin-top: 4px
  }

  & .slotted-button {
    margin-left: 12px
    margin-top: 7px

    &:last-child {
      margin-right: 6px
    }
  }
}
</style>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%

    & > .search_container {
      display: flex
      justify-content: flex-end
      margin-top: 10px
    }

    & > .list {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
// TODO: headless
import tabsctrl from './tabs'
import {user_tabs, sort} from 'utils/consts'
import btn from './button'
import btnKey from './btn-key'
import btnWallet from './btn-wallet'
import btnProfile from './btn-profile'
import list from './items-list.vue'
import nftsStore from 'stores/nfts'
import collsStore from 'stores/collections'

export default {
  components: {
    tabsctrl,
    btn,
    btnKey,
    btnWallet,
    btnProfile,
    list
  },

  data () {
    return {
      tabs: [
        {id: user_tabs.COLLECTIONS, name: 'Collections'},
        {id: user_tabs.NFTS, name: 'NFTs'},
        {id: user_tabs.SALE_NFTS, name: 'On Sale'},
        {id: user_tabs.LIKED_NFTS, name: 'Liked'},
      ],
      search: '',
      selector_options: [
        {name: 'Added: Oldest to Newest', id: sort.OLDEST_TO_NEWEST},
        {name: 'Added: Newest to Oldest', id: sort.NEWEST_TO_OLDEST},
        {name: 'Price: Low to High', id: sort.PRICE_ASC},
        {name: 'Price: High to Low', id: sort.PRICE_DESC},
        {name: 'Likes: Low to High', id: sort.LIKES_ASC},
        {name: 'Likes: High to Low', id: sort.LIKES_DESC}
      ]
    }
  },

  computed: {
    active_tab: {
      get () {
        return this.$state.user_active_tab
      },
      set (value) {
        this.$store.setUserTab(value)
      }
    },
    can_admin () {
      return this.$state.is_moderator || this.$state.is_admin
    },
    admin_btn_text () {
      let role = ''

      if (this.$state.is_moderator) {
        role = 'moderator'
      }

      if (this.$state.is_admin) {
        role = 'admin'
      } 

      return `Open ${role} panel`
    },
    show_collections () {
      return this.$state.user_active_tab == user_tabs.COLLECTIONS
    },
    show_nfts () {
      return this.$state.user_active_tab == user_tabs.NFTS
    },
    show_sale () {
      return this.$state.user_active_tab == user_tabs.SALE_NFTS
    },
    show_liked () {
      return this.$state.user_active_tab == user_tabs.LIKED_NFTS
    },
    nftsStore () {
      return nftsStore
    },
    collsStore () {
      return collsStore
    }
  },
  methods: {
    onAdmin() {
      this.$store.toAdmin()
    }
  }
}
</script>
