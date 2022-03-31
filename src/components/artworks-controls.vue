<template>
  <div class="actions-container">
    <div class="artworks-controls">
      <div class="tabs">
        <span v-for="(tab) of tabs_sort_by"
              :key="tab.id"
              :class="[ active_tab === tab.id ? 'tab-active' : '','tab-item']" 
              @click="onTabClicked(tab.id)"
        >
          <div class="title">{{ tab.name }}</div>
          <div v-if="active_tab === tab.id" class="bottom-line"></div>
        </span>
      </div>
      <div class="selectors">
        <!-- <selector
                v-on:selected="onAuthor"
                :options="artist_options"
                :selected="active_filter_by_artist"
                title="Author"
                v-if="artist_options.length"
            /> -->
        <btn text="nft" color="green" padding="7px 12px" @click="onAddNft">
        </btn>
        <div style="margin-left:1px">
          <btn text="collection" color="green" padding="7px 12px" @click="onAddCollection">
          </btn>
        </div>
        <selector :options="selector_options"
                  :selected="active_sort_by"
                  title="Sort by"
                  @selected="onSortBy"
        />
        <btn text="become an artist" color="green" padding="7px 22px" @click="onBecomeArtist">
          <img src="~assets/add-user.svg"/>
        </btn>
      </div>
      <btn @click="onMyPage">
        <img src="~assets/icon-user.svg">
      </btn>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.actions-container {
  display: flex
  flex-direction: row

  .artworks-controls {
    display: flex
    flex-direction: row
    align-items: center
    width: 100%
        
    .tabs {
      display: flex
      flex-direction: row

      .tab-active {
        color: #fff
      }

      .tab-item {
        color: rgba(255, 255, 255, 0.3)
        font-size: 12px
        font-weight: bold
        cursor: pointer

        .title {
          padding: 4px 16px
          text-transform: uppercase
        }

        .bottom-line {
          height: 2px
          width: 100%
          box-shadow: 0 0 5px 0 rgba(0, 246, 210, 0.7)
          background-color: #00f6d2
        }
      }
    }
  }
    
  .selectors {
    display: flex
    margin-left: auto
    padding-right: 2px
    align-items: center

    & > div:not(:last-child) {
      margin-right: 20px
    }

    & > *:last-child {
      margin-right: 4px
    }
  }
}
</style>

<script>
import {tabs, sort} from '../utils/consts.js'
import selector from './selector.vue'
import btn from './button.vue'

export default {
  components: {
    selector,
    btn
  },

  data() {
    return {
      selector_options: [
        {name: 'Added: Oldest to Newest', sort_type: sort.OLDEST_TO_NEWEST},
        {name: 'Added: Newest to Oldest', sort_type: sort.NEWEST_TO_OLDEST},
        {name: 'Price: Low to High', sort_type: sort.PRICE_ASC},
        {name: 'Price: High to Low', sort_type: sort.PRICE_DESC},
        {name: 'Likes: Low to High', sort_type: sort.LIKES_ASC},
        {name: 'Likes: High to Low', sort_type: sort.LIKES_DESC}
      ],
    }
  },
  
  computed: {
    active_tab () {
      return this.$state.active_tab
    },
    artists () {
      return this.$state.artists
    },
    active_sort_by() {
      return this.$state.sort_by
    },
    tabs_sort_by() {
      return [
        {name: 'All', id:tabs.ALL},
        {name: 'MINE', id: tabs.MINE},
        {name: 'SALE', id: tabs.SALE},
        {name: 'SOLD', id: tabs.SOLD},
        {name: 'LIKED', id: tabs.LIKED}
      ]
    },
    artist_options () {
      let result = []
      if (this.active_tab != tabs.SOLD) {
        result.push({name: 'Everyone', key: 0})
        for(let aid in this.artists) {
          let artist = this.artists[aid]
          result.push({name: artist.label, key: artist.key})
        } 
      }
      return result
    },
    active_filter_by_artist () {
      let key = this.$state.filter_by_artist
      let artists = this.artist_options
      for (let idx = 0; idx <artists.length; ++idx) {
        if (key == artists[idx].key) {
          return idx
        }
      }
      return 0
    }
  },

  methods: {
    onBecomeArtist() {
      this.$store.becomeArtist()
    },

    onAddCollection() {
      this.$store.newCollection()
    },

    onAddNft() {
      this.$store.newNFT()
    },

    onTabClicked(id) {
      if (this.active_tab !== id) {
        this.$store.setActiveTab(id)
      }
    },   
    onSortBy(opt) {
      this.$store.setSortBy(opt.sort_type)
      this.scrollToTop()
    },
    onAuthor(opt) {
      this.$store.setFilterByArtist(opt.key)
      this.scrollToTop()
    },
    scrollToTop(){
      if (this.$parent.$refs.artslist) {
        this.$parent.$refs.artslist.scrollTop = 0
      }
    },
    onMyPage() {
      this.$store.toMyPage()
    }
  }
}
</script>