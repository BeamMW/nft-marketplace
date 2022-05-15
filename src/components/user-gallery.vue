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
    <collections v-if="show_collections" class="list"/>
    <artworks v-else class="list"/>
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
import artworks from './user-artworks.vue'
import collections from './user-collections.vue'
import tabsctrl from './tabs'
import {tabs, sort} from 'utils/consts'
import btn from './button'
import btnKey from './btn-key'
import btnWallet from './btn-wallet'
import btnProfile from './btn-profile'

export default {
  components: {
    artworks,
    collections,
    tabsctrl,
    btn,
    btnKey,
    btnWallet,
    btnProfile
  },

  data () {
    return {
      tabs: [
        {id: tabs.COLLECTIONS, name: 'Collections'},
        {id: tabs.NFTS, name: 'NFTs'},
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
        return this.$state.gallery_tab
      },
      set (value) {
        this.$store.setGalleryTab(value)
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
      return this.$state.gallery_tab == tabs.COLLECTIONS
    }
  },
  methods: {
    onAdmin() {
      this.$store.toAdmin()
    }
  }
}
</script>
