<template>
  <!-- TODO: handle author loading error (first collection) -->
  <pageTitle :title="title"/>
  <loading v-if="collection === undefined" text="Loading Collection"/>
  <notFound v-else-if="collection === null" text="Collection Not Found"/>
  <div v-else class="collection-container">
    <keyModal ref="keyModal" :name="author_error ? '' : author_name" :artist_key="author_key"/>
    <preview :class="{'error': collection.error}" :image="cover" width="100%" height="170px" :default="def_banner" cover>
      <div class="info-container">
        <div class="left">
          <preview :class="{'error': collection.author_error}" 
                   :image="avatar" 
                   :default="def_avatar" 
                   :show_text="false" 
                   radius="36px 36px" 
                   class="avatar"
          />
          <div class="info">
            <span class="name">{{ author_name }}</span>
            <span class="about">{{ author_about }}</span>
          </div>
        </div>
        <div class="right">
          <div class="social">
            <div>
              <btn v-if="website" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onWebsite">
                <img src="~assets/globe-green.svg">
              </btn>
              <btn v-if="twitter" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onTwitter">
                <img src="~assets/twitter-green.svg">
              </btn>
              <btn v-if="instagram" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onInstagram">
                <img src="~assets/instagram-green.svg">
              </btn>
              <btn color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onKey">
                <img src="~assets/key-small.svg">
              </btn>
            </div>  
          </div>
          <div class="stats">
            <div>
              <div class="value">{{ nfts_count }}</div>
              <span>{{ nfts_count == 1 ? 'NFT' : 'NFTs' }}</span>
            </div>
            <div>
              <div>
                <img src="~assets/beam.svg">
                <span class="value">{{ min_price }}</span>
              </div>
              <span>floor price</span>
            </div>
            <div>
              <div>
                <img src="~assets/beam.svg">
                <span class="value">{{ trade_volume }}</span>
              </div>
              <span>trade volume</span>
            </div>
          </div>
        </div>
      </div>
    </preview>  

    <!-- TODO: same layout as in approve containers -->
    <div :class="{'block': true, 'block-long': long_text}">
      <span ref="desc" :class="{'desc-short': !full_text}">{{ description }}</span>
      <btn v-if="long_text && !full_text"
           class="btn-right"
           text="See more" 
           height="20px"
           color="transparent"
           text_color="green"
           padding="0px 7px 0px 7px"
           :text_bold="false"
           @click="full_text = true"
      />  
      <btn v-if="long_text && full_text"
           class="btn-center"
           text="See less" 
           height="20px"
           color="transparent"
           text_color="green"
           padding="0px 7px 0px 7px"
           :text_bold="false"
           @click="full_text = false"
      /> 
    </div>
    <div v-if="debug && collection.error" class="debug error">
      {{ collection.error }}
    </div>
    <tabsctrl v-model="active_tab" :tabs="tabs"/>
    <!--- TODO: set collection to the current one when opening 'new-nft' -->
    <list v-if="show_all"
          class="list"
          items_name="NFTs"
          component="nft"
          :new_component="owned && safe_mode == 'artist' ? 'create-nft' : ''"
          :new_component_props="{'selected_collection': id}"
          :mode="`${safe_mode}:collection:${id}`"
          :store="nftsStore"
          :component_props="{'mode': safe_mode}"
    />
    <list v-if="show_sale"
          class="list"
          items_name="NFTs"
          component="nft"
          :new_component="owned && safe_mode == 'artist' ? 'create-nft' : ''"
          :mode="`${safe_mode}:collection:sale:${id}`"
          :store="nftsStore"
          :component_props="{'mode': safe_mode}"
    />
    <list v-if="show_liked"
          class="list"
          items_name="NFTs"
          component="nft"
          :new_component="owned && safe_mode == 'artist' ? 'create-nft' : ''"
          :mode="`${safe_mode}:collection:liked:${id}`"
          :store="nftsStore"
          :component_props="{'mode': safe_mode}"
    />
  </div>
</template>

<style lang="stylus" scoped>
  .collection-container {
    margin-top: 5px
    border-top-left-radius: 10px
    border-top-right-radius: 10px
    display: flex
    flex-direction: column
    box-sizing: border-box
    width: 100%
    height: 100%
    overflow: hidden

    & > .debug {
      width: 100%
      margin-bottom: 10px
      text-align: center
    }

    & > .list {
      margin-top: 20px
      flex: 1
    }

    & > .block {
      display: flex
      flex-direction: column
      padding: 20px 20px 25px 20px
      background-color: rgba(255, 255, 255, 0.05)
      border-bottom-left-radius: 10px
      border-bottom-right-radius: 10px
      margin-bottom: 15px
      color: #8897a8
      font-size: 14px

      & .desc-short {
        display: -webkit-box
        -webkit-line-clamp: 2
        -webkit-box-orient: vertical
        overflow: hidden
      }

      & .btn-right {
        margin-left: auto
      }

      & .btn-center {
        margin-left: auto
        margin-right: auto
      }
    }

    & > .block-long {
      padding-bottom: 10px
    }

    .info-container {
      display: flex
      justify-content: space-between
      align-items: flex-end
      width: 100%
      height: 100%
      padding: 12px 12px 12px 12px
      box-sizing: border-box
      z-index: 100

      & > .left {
        display: flex
        flex-direction: row
        border-radius: 10px
        backdrop-filter: blur(30px) brightness(40%)
        margin-right: 20px
        padding: 8px 18px 9px 15px
        box-sizing: border-box

        & > .avatar {
          margin-right: 16px
          min-width: 72px
          min-height: 72px
          width: 72px
          height: 72px
        }

        & > .info {
          display: flex
          flex-direction: column
          justify-content: center

          & > .name {
            font-weight: bold
            font-size: 16px
            margin-bottom: 4px
            overflow: hidden
            text-overflow: ellipsis
          }
        }
      }

      .right {
        .social {
          height: 16px
          display: flex
          justify-content: flex-end
          align-items: center
          margin-bottom: 20px

          & > * {
            backdrop-filter: blur(30px) brightness(40%)
            display: flex
            border-radius: 5px
            padding: 5px 5px

            & > *:not(:first-child) {
              margin-left: 6px
            }
          }
        }

        .stats {
          height: 89px
          box-sizing: border-box
          padding: 16px 0
          border-radius: 10px
          backdrop-filter: blur(30px) brightness(40%)
          display: flex
          flex-direction: row

          & > div {
            min-width: 110px
            box-sizing: border-box
            padding: 0px 10px
            display: flex
            flex-direction: column
            align-items: center
            justify-content: center
            
            & > div {
              display: flex
              margin-bottom: 4px

              & > img {
                margin-right: 8px
              }
            }
            
            & > span:not(.value) {
              font-size: 14px
            }

            & .value {
              display: flex
              font-size: 16px
            }
          }

          & > *:not(:last-child) {
            border-right: 1px solid rgba(255, 255, 255, 0.1)
          }
        }
      }
    }
  }
</style>

<script>
import keyModal from 'components/key-modal'
import pageTitle from 'controls/page-title'
import btn from 'controls/button'
import tabsctrl from 'controls/tabs'
import preview from 'controls/preview'
import loading from 'controls/loading'
import notFound from 'controls/not-found'
import list from 'controls/lazy-list'
import collsStore from 'stores/collections'
import artistsStore from 'stores/artists'
import nftsStore from 'stores/nfts'
import utils from 'utils/utils'
import {coll_tabs, def_images} from 'utils/consts'

export default {
  components: {
    pageTitle,
    btn,
    preview,
    loading,
    notFound,
    keyModal,
    tabsctrl,
    list
  },

  props: {
    id: {
      type: Number,
      required: true
    },
    mode: {
      type: String,
      default: 'user'
    }
  },

  setup (props) {
    let collection = collsStore.getLazyItem(props.id)
    return {
      collection
    }
  },

  data() {
    return {
      observer: undefined,
      full_text: false,
      long_text: false,
      tabs: [
        {id: coll_tabs.ALL_NFTS, name: 'All'},
        {id: coll_tabs.SALE_NFTS, name: 'On Sale'},
        {id: coll_tabs.LIKED_NFTS, name: 'Liked'},
      ],
      def_avatar: def_images.artist_avatar,
      def_banner: def_images.artist_banner
    }
  },

  computed: {
    owned() {
      return this.collection.owned
    },
    title () {
      if (!this.collection) return 'Loading...'
      let prefix = this.$state.debug ? `[${this.collection.id}] - ` : ''
      if (this.collection.error) return [prefix, 'Failed to load collection'].join('')
      return [prefix, this.show_safe ? this.collection.safe_label : this.collection.label].join('')
    },
    avatar() {
      return this.collection.safe_avatar
    },
    show_safe () {
      if (this.mode === 'artist' && this.collection.author === artistsStore.my_key) return false
      return true
    },
    safe_mode () {
      return this.show_safe ? 'user' : this.mode
    },
    approved () {
      return this.collection.approved
    },
    cover() {
      return this.show_safe ? this.collection.safe_cover : this.collection.cover
    },
    author_name() {
      if (this.$state.debug && this.collection.author_error) {
        return this.collection.author_error
      }
      return this.show_safe ? this.collection.safe_author_name : this.collection.author_name
    },
    author_about() {
      if(this.show_safe) {
        return this.collection.author_approved ? this.collection.author_about : ''
      }
      return this.collection.author_about
    },
    author_key() {
      return this.collection.author
    },
    author_error() {
      return this.collection.author_error
    },
    instagram() {
      if (this.show_safe) {
        if (this.approved && this.collection.instagram) return this.collection.instagram
        if (this.collection.author_approved && this.collection.author_instagram) return this.collection.author_instagram
        return undefined
      }
      return this.collection.instagram || this.collection.author_instagram
    },
    twitter() {
      if (this.show_safe) {
        if (this.approved && this.collection.twitter) return this.collection.twitter
        if (this.collection.author_approved && this.collection.author_twitter) return this.collection.author_twitter
        return undefined
      }
      return this.collection.twitter || this.collection.author_twitter
    },
    website() {
      if (this.show_safe) {
        if (this.approved && this.collection.website) return this.collection.website
        if (this.collection.author_approved && this.collection.author_website) return this.collection.author_website
        return undefined
      }
      return this.collection.website || this.collection.author_website
    },
    description() {
      return this.show_safe ? this.collection.safe_description : this.collection.description
    },
    nfts_count() {
      return (this.show_safe ? this.collection.approved_nfts_count : this.collection.nfts_count) || 0
    },
    min_price() {
      let value = this.collection.min_price.value 
      return utils.formatAmount3(value)
    },
    trade_volume() {
      let value = this.collection.total_sold.volume
      return utils.formatAmount3(value)
    },
    active_tab: {
      get() {
        return this.$state.coll_active_tab
      },
      set(value) {
        this.$store.setCollTab(value)
      }
    },
    show_all() {
      return this.active_tab === coll_tabs.ALL_NFTS
    },
    show_sale() {
      return this.active_tab === coll_tabs.SALE_NFTS
    },
    show_liked() {
      return this.active_tab === coll_tabs.LIKED_NFTS
    },
    nftsStore() {
      return nftsStore
    },
    debug() {
      return this.$state.debug
    }
  },

  updated() {
    if (!this.observer) {
      let desc = this.$refs.desc
      this.observer = new ResizeObserver(() => {
        this.calcLongText()
      })
      this.observer.observe(desc)
    }
  },

  unmounted() {
    if (this.observer) {
      this.observer.disconnect()
    }
  },

  methods: {
    calcLongText() {
      if (!this.full_text) {
        const scrollHeight = this.$refs.desc.scrollHeight
        const offsetHeight = this.$refs.desc.offsetHeight
        this.long_text = scrollHeight > offsetHeight
      }
    },
    onKey() {
      this.$refs.keyModal.open()
    },
    onInstagram() {
      this.$store.openInstagram(this.instagram)
    },
    onTwitter() {
      this.$store.openTwitter(this.twitter)
    },
    onWebsite() {
      this.$store.openUrl(this.website)
    }
  }
}
</script>
