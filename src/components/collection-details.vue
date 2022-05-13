<template>
  <pageTitle :title="title"/>
  <loading v-if="collection_new === undefined" text="Loading Collection"/>
  <notFound v-else-if="collection_new === null" text="Collection Not Found"/>
  <div v-else class="collection-container">
    <publicKeyModal ref="keyModal" :name="author_name" :artist_key="author_key"/>
    <imagePreview :image="cover" width="100%" height="170px" cover>
      <div class="info-container">
        <div class="left">
          <imagePreview :image="avatar" radius="36px 36px" class="avatar"/>
          <div class="info">
            <span class="name">{{ author_name }}</span>
            <span class="about">{{ author_about }}</span>
          </div>
        </div>
        <div class="right">
          <div class="social">
            <div>
              <btn v-if="website" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onWebsite">
                <img src="~assets/glob-green.svg">
              </btn>
              <btn v-if="twitter" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onTwitter">
                <img src="~assets/twitter-green.svg">
              </btn>
              <btn v-if="instagram" color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onInstagram">
                <img src="~assets/instagram-green.svg">
              </btn>
              <btn color="transparent" height="20px" padding="5px 7px 5px 7px" @click="onKey">
                <img src="~assets/icon-key-small.svg">
              </btn>
            </div>  
          </div>
          <div class="stats">
            <div>
              <div class="value">{{ artworks_count }}</div>
              <span>{{ artworks_count == 1 ? 'NFT' : 'NFTs' }}</span>
            </div>
            <div>
              <div>
                <img src="~assets/icon-beam.svg">
                <span class="value">{{ min_price }}</span>
              </div>
              <span>min price</span>
            </div>
            <div>
              <div>
                <img src="~assets/icon-beam.svg">
                <span class="value">{{ trade_volume }}</span>
              </div>
              <span>trade volume</span>
            </div>
          </div>
        </div>
      </div>
    </imagePreview>  

    <div :class="{'block': true, 'block-long': long_text}">
      <span ref="desc" :class="{'desc-short': !full_text}">
        {{ description }} {{ description }} {{ description }} {{ description }} {{ description }}
      </span>
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
    <!--div :class="{'block': true, 'block-long': long_text}">
      <div class="left">
        <div class="artist">
          <imagePreview :image="avatar" radius="36px 36px" class="avatar"/>
          <div class="info">
            <span class="name">{{ author_name }}</span>
            <span class="about">{{ author_about }}</span>
          </div>
        </div>
        
      </div>
    </div-->
    <tabsctrl v-model="active_tab" :tabs="tabs"/>
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
        -webkit-line-clamp: 3
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
import pageTitle from './page-title'
import btn from './button'
import tabsctrl from './tabs'
import imagePreview from './image-preview.vue'
import collsStore from 'stores/collections'
import loading from './loading'
import notFound from './not-found'
import utils from 'utils/utils'
import {useObservable} from '@vueuse/rxjs'
import {computed} from 'vue'
import publicKeyModal from './public-key-dialog'
import {coll_tabs} from 'utils/consts'

export default {
  components: {
    pageTitle,
    btn,
    imagePreview,
    loading,
    notFound,
    publicKeyModal,
    tabsctrl
  },

  props: {
    id: {
      type: Number,
      required: true
    },
  },

  setup (props) {
    let collObservable = computed(() => {
      return useObservable(collsStore.getLazyItem('manager', props.id))
    })

    let collection_new = computed(() => {
      let result = collObservable.value.value
      if (!result) {
        return result
      }
      return result.length == 0 ? null : result[0]
    })  

    return {
      collection_new
    }
  },

  data() {
    return {
      full_text: false,
      long_text: false,
      tabs: [
        {id: coll_tabs.ALL_NFTS, name: 'All'},
        {id: coll_tabs.SALE_NFTS, name: 'On Sale'},
        {id: coll_tabs.LIKED_NFTS, name: 'Liked'},
      ]
    }
  },

  computed: {
    title () {
      return this.collection_new && !this.collection_new.error ? this.collection_new.label : ''
    },
    cover() {
      return this.collection_new.cover
    },
    avatar() {
      return this.collection_new.avatar
    },
    author_name() {
      return this.collection_new.author_name
    },
    author_about() {
      return this.collection_new.author_about
    },
    author_key() {
      return this.collection_new.author
    },
    instagram() {
      return this.collection_new.instagram || this.collection_new.author_instagram
    },
    twitter() {
      return this.collection_new.twitter || this.collection_new.author_twitter
    },
    website() {
      return this.collection_new.website || this.collection_new.author_website
    },
    description() {
      return this.collection_new.description
    },
    artworks_count() {
      return this.collection_new.artworks_count
    },
    min_price() {
      let value = this.collection_new.min_price ? this.collection_new.min_price.value : 0
      return utils.formatAmount3(value)
    },
    trade_volume() {
      let value = this.collection_new.total_sold ? this.collection_new.total_sold.volume : 0
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
    }
  },

  updated() {
    if (!this.full_text) {
      const scrollHeight = this.$refs.desc.scrollHeight
      const offsetHeight = this.$refs.desc.offsetHeight
      this.long_text = scrollHeight > offsetHeight
    }
  },

  methods: {
    onKey() {
      this.$refs.keyModal.open()
    },
    onInstagram() {
      let link = `https://www.instagram.com/${this.instagram.substring(1)}`
      window.open(link, '_blank')
    },
    onTwitter() {
      let link = `https://www.twitter.com/${this.twitter.substring(1)}`
      window.open(link, '_blank')
    },
    onWebsite() {
      let link = this.website
      window.open(link, '_blank')
    }
  }
}
</script>
