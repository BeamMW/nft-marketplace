<template>
  <div class="home-container">
    <div class="hero">
      <!-- transparent variant; appicon.svg has an opaque circle for the app tile -->
      <img src="~assets/logo.svg" class="hero-logo" alt="BEAM NFT Gallery"/>
      <div class="hero-title">BEAM NFT Gallery</div>
      <div class="hero-subtitle">
        Confidential NFTs on Beam - mint, collect and trade artwork with privacy by default.
      </div>
      <searchInput v-model="search"
                   class="hero-search"
                   :max_length="40"
                   placeholder="Search by NFT, collection or artist..."
                   :suggestions="suggestions"
                   @search="onSearch"
                   @select="onSuggestion"
      />
      <div class="hero-actions">
        <btn text="browse the gallery" color="green" height="38px" @click="toGallery">
          <img src="~assets/proceed.svg"/>
        </btn>
        <btn text="my collection" color="blue" height="38px" @click="toMyPage">
          <img src="~assets/user.svg"/>
        </btn>
      </div>
    </div>

    <div class="stats">
      <div class="stat">
        <div class="stat-value">{{ shown.nfts }}</div>
        <div class="stat-title">NFTs</div>
      </div>
      <div class="stat">
        <div class="stat-value">{{ shown.colls }}</div>
        <div class="stat-title">Collections</div>
      </div>
      <div class="stat">
        <div class="stat-value">{{ shown.artists }}</div>
        <div class="stat-title">Artists</div>
      </div>
      <div class="stat">
        <div class="stat-value">
          <amount :amount="shown.volume" :aid="0" size="28px"/>
        </div>
        <div class="stat-title">Trade volume</div>
      </div>
    </div>

    <template v-if="featured.length">
      <div class="section-title">Featured collections</div>
      <div class="featured">
        <collection v-for="item in featured" :key="item.id" :item="item" mode="user"/>
      </div>
    </template>
  </div>
</template>

<style scoped lang="stylus">
  .home-container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    width: 100%
    height: 100%
    overflow-y: auto
    overflow-x: hidden

    & > .hero {
      display: flex
      flex-direction: column
      align-items: center
      text-align: center
      flex-shrink: 0
      padding: 30px 20px 30px 20px

      & > .hero-logo {
        /* glyph fills less of the canvas without the backing circle */
        width: 100px
        height: 100px
        margin-bottom: 12px
      }

      & > .hero-title {
        font-family: 'SFProDisplay', sans-serif
        font-size: 42px
        font-weight: bold
        color: #fff
      }

      & > .hero-subtitle {
        margin-top: 14px
        max-width: 620px
        font-size: 16px
        line-height: 1.5
        color: rgba(255, 255, 255, 0.6)
      }

      & > .hero-search {
        width: 100%
        /* matches the BANS search width */
        max-width: 650px
        margin-top: 30px
      }

      & > .hero-actions {
        display: flex
        flex-direction: row
        margin-top: 25px

        & > *:not(:first-child) {
          margin-left: 20px
        }
      }
    }

    & > .stats {
      display: flex
      flex-direction: row
      flex-wrap: wrap
      flex-shrink: 0
      justify-content: center
      margin: 0 -10px

      & > .stat {
        display: flex
        flex-direction: column
        align-items: center
        box-sizing: border-box
        min-width: 160px
        margin: 10px
        padding: 20px
        border-radius: 10px
        background-color: rgba(255, 255, 255, 0.05)

        & > .stat-value {
          display: flex
          align-items: center
          justify-content: center
          font-size: 28px
          font-weight: bold
          color: #00f6d2
        }

        & > .stat-title {
          margin-top: 6px
          opacity: 0.5
          font-size: 12px
          text-transform: uppercase
          letter-spacing: 1.5px
          color: #fff
        }
      }
    }

    & > .section-title {
      flex-shrink: 0
      padding: 40px 0 20px 0
      color: rgba(255, 255, 255, 0.5)
      font-size: 14px
      text-transform: uppercase
      font-weight: bold
      letter-spacing: 3.1px
      user-select: none
    }

    & > .featured {
      display: flex
      flex-wrap: wrap
      flex-shrink: 0
      justify-content: center
      align-self: center
      box-sizing: border-box
      width: 100%
      /* three cards at their natural width plus the gaps between them */
      max-width: 1398px
      padding: 0 12px
      margin: -8px auto 30px auto

      & > * {
        /* narrow down rather than wrap - the wallet window is a lot
           narrower than the browser one the layout was drawn in */
        flex: 1 1 280px
        max-width: 442px
        margin: 8px
      }
    }
  }

  /* the wallet window is 1024x852 at its smallest, and the app gets less
     than that once the wallet's own chrome is taken off */
  @media (max-height 900px) {
    .home-container {
      & > .hero {
        padding: 16px 20px

        & > .hero-logo {
          width: 72px
          height: 72px
          margin-bottom: 8px
        }

        & > .hero-title {
          font-size: 32px
        }

        & > .hero-subtitle {
          margin-top: 10px
        }

        & > .hero-search {
          margin-top: 20px
        }

        & > .hero-actions {
          margin-top: 18px
        }
      }

      & > .stats > .stat {
        padding: 14px 20px
      }

      & > .section-title {
        padding: 24px 0 14px 0
      }
    }
  }

  /* below this three cards cannot share a row without turning unreadable */
  @media (max-width 900px) {
    .home-container > .featured > * {
      flex-basis: 100%
    }
  }
</style>

<script>
import btn from 'controls/button'
import searchInput from 'controls/search-input'
import collection from 'components/collection'
import amount from 'controls/amount'
import nftsStore from 'stores/nfts'
import collsStore from 'stores/collections'
import artistsStore from 'stores/artists-lazy'
import router from 'router'
import {def_images, common} from 'utils/consts'

const FEATURED_COUNT = 3

// The four stats arrive at different times - the NFT count comes from a
// separate status call and lands last. Waiting for them to stop moving and
// then running one animation is what makes the tiles finish together.
const COUNT_SETTLE = 300
const COUNT_DURATION = 900

export default {
  components: {
    btn,
    searchInput,
    collection,
    amount
  },

  setup () {
    return {
      all_colls: collsStore.getLazyAllItems('user'),
      all_artists: artistsStore.getLazyAllItems('user')
    }
  },

  data () {
    return {
      search: '',
      counted: {nfts: 0, colls: 0, artists: 0, volume: 0},
      counting: false
    }
  },

  computed: {
    nfts_total () {
      return nftsStore.getAllItemsCount('user') || 0
    },

    colls_total () {
      return (this.all_colls || []).length
    },

    artists_total () {
      return (this.all_artists || []).length
    },

    // BEAM only - volumes in other assets are not comparable without a rate,
    // so they are left out rather than summed in.
    total_volume () {
      let total = 0

      for (let coll of (this.all_colls || [])) {
        let sold = (coll || {}).total_sold || {}
        if (Number(sold.aid || 0) !== 0) continue
        total += Number(sold.volume || 0)
      }

      return total
    },

    shown () {
      return {
        nfts: Math.round(this.counted.nfts),
        colls: Math.round(this.counted.colls),
        artists: Math.round(this.counted.artists),
        // whole BEAM while it runs so the decimals do not flicker, the
        // exact figure once it lands
        volume: this.counting
          ? Math.round(this.counted.volume / common.GROTHS_IN_BEAM) * common.GROTHS_IN_BEAM
          : this.counted.volume
      }
    },

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

    // Most traded first, newest as a tie-break. BEAM only - volumes in other
    // assets are not comparable without a rate.
    featured () {
      const sold = (coll) => {
        let stats = (coll || {}).total_sold || {}
        return Number(stats.aid || 0) === 0 ? stats : {}
      }
      const volume = (coll) => Number(sold(coll).volume || 0)
      const count  = (coll) => Number(sold(coll).count || 0)

      let colls = (this.all_colls || []).slice()
      colls.sort((a, b) => (volume(b) - volume(a)) || (count(b) - count(a)) || (b.id - a.id))
      return colls.slice(0, FEATURED_COUNT)
    }
  },

  watch: {
    nfts_total: 'scheduleCount',
    colls_total: 'scheduleCount',
    artists_total: 'scheduleCount',
    total_volume: 'scheduleCount'
  },

  mounted () {
    this.scheduleCount()
  },

  unmounted () {
    clearTimeout(this._settle)
    cancelAnimationFrame(this._frame)
  },

  methods: {
    scheduleCount () {
      clearTimeout(this._settle)
      this._settle = setTimeout(() => this.runCount(), COUNT_SETTLE)
    },

    // One animation drives all four tiles, so they share a start and an end.
    runCount () {
      let from = Object.assign({}, this.counted)
      let to = {
        nfts: this.nfts_total,
        colls: this.colls_total,
        artists: this.artists_total,
        volume: this.total_volume
      }

      cancelAnimationFrame(this._frame)
      this.counting = true

      let started = 0
      let step = (now) => {
        if (!started) started = now

        let progress = Math.min(1, (now - started) / COUNT_DURATION)
        let eased = 1 - Math.pow(1 - progress, 3)

        for (let key in to) {
          this.counted[key] = from[key] + (to[key] - from[key]) * eased
        }

        if (progress < 1) {
          this._frame = requestAnimationFrame(step)
          return
        }

        this.counting = false
      }

      this._frame = requestAnimationFrame(step)
    },

    // An artist becomes an author filter, a collection opens directly.
    onSuggestion (item) {
      if (item.type === 'artist') {
        this.search = ''
        let patch = {search: '', search_authors: '', author: item.id}
        nftsStore.setQuery(patch)
        collsStore.setQuery(patch)
        return this.toGallery()
      }

      collsStore.toDetails(item.id, 'user')
    },

    // Navigate on submit only - doing it per keystroke unmounts this page
    // mid-typing and the input loses focus.
    onSearch (value) {
      // an artist's name should find their work too
      let needle = String(value || '').trim().toLowerCase()
      let authors = needle
        ? (this.all_artists || [])
          .filter(artist => artist && artist.id &&
            String(artist.label || '').toLowerCase().indexOf(needle) !== -1)
          .map(artist => artist.id)
          .join(',')
        : ''

      let patch = {search: value, search_authors: authors}
      nftsStore.setQuery(patch)
      collsStore.setQuery(patch)
      this.toGallery()
    },

    toGallery () {
      router.push({name: 'gallery'})
    },

    toMyPage () {
      this.$store.toMyPage()
    }
  }
}
</script>
