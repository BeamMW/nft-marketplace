<template>
  <div class="gallery-controls">
    <tabsctrl v-model:active="active_tab" :tabs="tabs">
      <div v-if="show_nfts" class="selectors">
        <!-- <selector
                v-on:selected="onAuthor"
                :options="artist_options"
                :selected="active_filter_by_artist"
                title="Author"
                v-if="artist_options.length"
            /> -->
        <selector v-model:selected="nfts_sort_by"
                  :options="selector_options"
                  title="Sort by"
        />
      </div>

      <!-- for testing purposes -->
      <btn text="collection details" color="blue" @click="$store.toCollectionDetails"/>

      <btn class="user" height="34px"
           :text="my_artist_name"
           @click="$store.toMyPage"
      >
        <img src="~assets/icon-user.svg">
      </btn>
    </tabsctrl>
  </div>
</template>

<style scoped lang="stylus">
  .gallery-controls {
    width: 100%
    display: flex

    .selectors {
      padding-right: 12px
      margin-top: 4px
    }
  
    & button {
      margin-left: 12px
      margin-top: 7px

      &:last-child {
        margin-right: 6px
      }
    }
  }
</style>

<script>
import tabsctrl from './tabs.vue'
import btn from './button.vue'
import selector from './selector.vue'
import {tabs, sort} from '../utils/consts.js'

// TODO: headless
export default {
  components: {
    tabsctrl,
    selector,
    btn
  },

  data () {
    return {
      tabs: [
        {id: tabs.COLLECTIONS, name: 'Collections'},
        {id: tabs.NFTS, name: 'NFTs'},
      ],
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
    nfts_sort_by: {
      get () {
        return this.$state.sort_by
      },
      set (value) {
        this.$store.setSortBy(value)
        // TODO:
        // this.scrollToTop()
      }
    },
    show_nfts () {
      return this.$state.gallery_tab == tabs.NFTS
    },
    is_artist () {
      return this.$state.is_artist
    },
    my_artist_name () {
      if (this.is_artist) {
        let artist = this.$state.artists[this.$state.my_artist_keys[0]] ||
                   this.$state.artists[this.$state.my_artist_keys[1]]
        return (artist || {}).label
      }
      return ''
    }
  }
}
</script>
