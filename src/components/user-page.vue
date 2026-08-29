<template>
  <div class="gallery-container">
    <tabsctrl v-model="active_tab" class="tabs" :tabs="tabs">
      <btn v-if="!compact"
           height="36px"
           width="36px"
           padding="0px"
           radius="10px"
           tooltip="home"
           @click="toHome"
      >
        <img src="~assets/home.svg"/>
      </btn>
      <btn v-if="!compact && can_admin" 
           height="36px"
           width="36px"
           padding="0px"
           radius="10px"
           :tooltip="admin_btn_text"
           @click="onAdmin"
      >
        <img src="~assets/admin.svg"/>
      </btn>
      <btnWallet v-if="!compact"/>
      <btnKey v-if="!compact"/>
      <btnProfile/>
    </tabsctrl>
    <div class="toolbar">
      <searchInput v-model="search"
                   class="search_container"
                   :max_length="40"
                   placeholder="Search by NFT, collection or artist..."
                   :suggestions="suggestions"
                   @select="onSuggestion"
      />
      <div v-if="!compact" class="selectors">
        <formSelect v-if="show_nfts && artist_options.length > 1"
                    v-model="author_idx"
                    class="selector"
                    title="Author"
                    :options="artist_options"
        />
        <formSelect v-model="sort_idx"
                    class="selector"
                    title="Sort by"
                    :options="selector_options"
        />
      </div>
    </div>
    <list v-if="show_nfts"
          class="list"
          items_name="NFTs"
          component="nft"
          mode="user"
          :store="nftsStore"
    />
    <list v-if="show_collections"
          class="list"
          items_name="collections"
          component="collection"
          mode="user"
          :store="collsStore"
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
}

.toolbar {
  & .selector {
    min-width: 190px
  }
}
</style>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%

    & > .toolbar {
      display: flex
      flex-direction: row
      align-items: flex-end
      justify-content: flex-start
      flex-wrap: wrap
      margin-top: 10px

      & > .search_container {
        display: flex
        width: 320px
        align-self: flex-end
      }

      & > .selectors {
        display: flex
        flex-direction: row
        margin-left: 20px

        & > *:not(:first-child) {
          margin-left: 20px
        }
      }
    }

    & > .list {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
import tabsctrl from 'controls/tabs'
import btn from 'controls/button'
import btnKey from 'controls/btn-key'
import btnWallet from 'controls/btn-wallet'
import btnProfile from 'controls/btn-profile'
import list from 'controls/lazy-list'
import searchInput from 'controls/search-input'
import formSelect from 'controls/form-select'
import nftsStore from 'stores/nfts'
import collsStore from 'stores/collections'
import artistsStore from 'stores/artists-lazy'
import utils from 'utils/utils'
import {user_tabs, sort, force_admin_ui, def_images} from 'utils/consts'

export default {
  components: {
    tabsctrl,
    btn,
    btnKey,
    btnWallet,
    btnProfile,
    list,
    searchInput,
    formSelect
  },

  setup () {
    // lazy store - the direct one issues a request per artist
    return {
      all_artists: artistsStore.getLazyAllItems('user'),
      all_colls: collsStore.getLazyAllItems('user')
    }
  },

  data () {
    return {
      compact: utils.isCompact(),
      tabs: [
        {id: user_tabs.NFTS, name: 'NFTs'},
        {id: user_tabs.COLLECTIONS, name: 'Collections'},
        {id: user_tabs.SALE_NFTS, name: 'On Sale'},
        {id: user_tabs.LIKED_NFTS, name: 'Liked'},
      ],
      search: '',
      selector_options: [
        {name: 'Added: Newest to Oldest', id: sort.NEWEST_TO_OLDEST},
        {name: 'Added: Oldest to Newest', id: sort.OLDEST_TO_NEWEST},
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
      return force_admin_ui || this.$state.is_moderator || this.$state.is_admin
    },
    admin_btn_text () {
      let role = 'admin'

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
    },

    // Artists and collections are cached locally, NFTs are not.
    suggestions () {
      let needle = String(this.search || '').trim().toLowerCase()
      if (needle.length < 2) {
        return []
      }

      let matches = (list, type) => {
        return (list || [])
          .filter(item => item && item.id &&
            String(item.label || '').toLowerCase().indexOf(needle) !== -1)
          .map(item => {
            return {
              id: item.id,
              type,
              label: item.label,
              sub: type === 'artist' ? 'Artist' : 'Collection',
              // image objects, resolved to blob urls by the images store
              image: type === 'artist' ? item.avatar : (item.safe_cover || item.cover),
              default_image: type === 'artist'
                ? def_images.artist_avatar
                : def_images.artist_banner
            }
          })
      }

      return matches(this.all_artists, 'artist')
        .concat(matches(this.all_colls, 'collection'))
        .slice(0, 8)
    },

    // Read from the store query so the selectors cannot drift out of sync
    // with the filter that is actually applied.
    author_idx: {
      get () {
        let id = nftsStore.query.author
        if (!id) return 0
        let idx = this.artist_options.findIndex(option => option.id === id)
        return idx === -1 ? 0 : idx
      },
      set (idx) {
        let option = this.artist_options[idx]
        this.applyQuery({author: option ? option.id : ''})
      }
    },

    sort_idx: {
      get () {
        let idx = this.selector_options.findIndex(option => option.id === nftsStore.query.sort)
        return idx === -1 ? 0 : idx
      },
      set (idx) {
        let option = this.selector_options[idx]
        if (option) this.applyQuery({sort: option.id})
      }
    },

    artist_options () {
      let artists = (this.all_artists || []).filter(artist => artist && artist.id)

      let named = artists.map(artist => {
        return {name: artist.label || artist.id, id: artist.id}
      })

      named.sort((a, b) => {
        return String(a.name).localeCompare(String(b.name), undefined, {sensitivity: 'base'})
      })

      return [{name: 'All authors', id: ''}].concat(named)
    }
  },

  watch: {
    search (value) {
      this.applyQuery({search: value})
    }
  },

  methods: {
    // Picking an artist is an author filter, not a text search.
    onSuggestion (item) {
      if (item.type === 'artist') {
        this.search = ''
        this.applyQuery({search: '', search_authors: '', author: item.id})
        return
      }

      collsStore.toDetails(item.id, 'user')
    },

    matchingAuthors (search) {
      let needle = String(search || '').trim().toLowerCase()
      if (!needle) {
        return ''
      }

      return (this.all_artists || [])
        .filter(artist => artist && artist.id &&
          String(artist.label || '').toLowerCase().indexOf(needle) !== -1)
        .map(artist => artist.id)
        .join(',')
    },

    applyQuery (patch) {
      if (patch.search !== undefined) {
        patch = Object.assign({}, patch, {search_authors: this.matchingAuthors(patch.search)})
      }

      nftsStore.setQuery(patch)
      collsStore.setQuery(patch)
    },

    toHome() {
      this.$router.push({name: 'home'})
    },

    onAdmin() {
      this.$store.toAdmin()
    }
  }
}
</script>
