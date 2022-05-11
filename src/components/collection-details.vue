<template>
  <pageTitle :title="title"/>
  <loading v-if="collection_new === undefined" text="Loading Collection"/>
  <notFound v-else-if="collection_new === null" text="Collection Not Found"/>
  <div v-else class="collection-container">
    <publicKeyModal ref="keyModal" :name="author_name" :artist_key="author_key"/>
    <!-- banner -->
    <imagePreview :image="cover" width="100vw" height="200px" cover>
      <btnEdit :item="collection_new"/> 
      <moderationStatus :item="collection_new"/>
    </imagePreview>  

    <div :class="!full_text ? 'block' : 'block-transformed'">
      <div class="left">
        <!-- artist info -->
        <div class="artist">
          <imagePreview :image="avatar" radius="36px 36px" class="avatar"/>
          <div class="info">
            <span class="name">{{ author_name }}</span>
            <span class="about">{{ author_about }}</span>
          </div>
        </div>

        <!-- collection description -->
        <div class="description">
          <span ref="desc" :class="!full_text ? 'desc' : ''">{{ description }}</span>
          <span v-if="!full_text && is_long_text" class="button-more" @click="full_text = true">See more</span>
        </div>
      </div>

      <div class="right">
        <!-- "social" block -->
        <div class="social">
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

        <!-- collection info -->
        <div v-if="!full_text" class="stats">
          <div>
            <span class="value">{{ artworks_count }}</span>
            <span>{{ artworks_count == 1 ? 'NFT' : 'NFTs' }}</span>
          </div>

          <div>
            <div>
              <img src="~assets/icon-beam.svg">
              <span class="value">{{ formatted_price }}</span>
            </div>
            <span>min price</span>
          </div>

          <div>
            <div>
              <img src="~assets/icon-beam.svg">
              <span class="value">{{ formatted_volume_traded }}</span>
            </div>
            <span>trade volume</span>
          </div>
        </div>
      </div>

      <!-- footer button -->
      <div v-if="full_text" class="footer">
        <btn text="See less"
             height="20px"
             color="transparent"
             :text_bold="false"
             @click="full_text = false"
        />
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .collection-container {
    border-radius: 10px
    display: flex
    flex-direction: column
    box-sizing: border-box
    width: 100%
    height: 100%

    & > .block {
      display: flex
      flex-direction: row
      padding: 20px 20px 25px 20px
      background-color: rgba(255, 255, 255, 0.05)
    }

    & > .block-transformed {
      grid-column: 1 / 3
      grid-row: 2 / 3
      display: grid
      padding: 20px
      background-color: rgba(255, 255, 255, 0.05)
    }

    .left {
      flex: 1
      & > .artist {
        display: flex
        margin: 0 20px 20px 0

        .avatar {
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

          & > .about {
            font-size: 14px            
            display: -webkit-box
            -webkit-line-clamp: 2
            -webkit-box-orient: vertical
            overflow: hidden
          }
        }
      }

      & > .description {
        color: #8897a8
        font-size: 14px
        display: flex
        flex-direction: column

        & > .desc {
          overflow: hidden
          text-overflow: ellipsis
          display: -moz-box
          -moz-box-orient: vertical
          display: -webkit-box
          -webkit-line-clamp: 3
          -webkit-box-orient: vertical
          line-clamp: 3
        }

        & > .button-more {
          cursor: pointer
          color: #00f6d2
          align-self: flex-end
          margin-right: 20px
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
          margin-left: 6px
        }
      }

      .stats {
        width: 100%
        height: 60px
        padding-top: 16px
        padding-bottom: 16px
        border-radius: 10px
        background-color: rgba(255, 255, 255, 0.05)
        display: grid
        grid-template-columns: 1fr 1fr 1fr 1fr

        & > div {
          display: grid
          grid-template-rows: 1fr 1fr
          align-items: center
          justify-items: center
          position: relative
          
          & > div {
            display: flex
            
            & img {
              margin-right: 5px
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

    & > .footer {
      grid-column: 1 / 3
      height: min-content
      display: flex
      justify-content: center
      align-items: center
      margin-top: 20px
      color: red
    }
  }
</style>

<script>
import pageTitle from './page-title'
import btn from './button'
import btnEdit from './btn-edit'
import imagePreview from './image-preview.vue'
import moderationStatus from './moderation-status'
import collsStore from 'stores/collections'
import collections from 'utils/collection-testing.js'
import loading from './loading'
import notFound from './not-found'
import {binary_search} from '../utils/search.js'
import formatter from '../utils/formatter.js'
import {useObservable} from '@vueuse/rxjs'
import {computed} from 'vue'
import publicKeyModal from './public-key-dialog'

export default {
  components: {
    pageTitle,
    btn,
    btnEdit,
    imagePreview,
    loading,
    notFound,
    moderationStatus,
    publicKeyModal
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
      is_long_text: false
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

    // NOT REFACTORED
    collection() {
      let all = collections
      let idx = binary_search(all, a => a.id == this.id ? 0 : a.id < this.id ? 1 : -1)
      return collections[idx]
    },
    price() {
      return this.collection.price
    },
    volume_traded() {
      return this.collection.volume_traded
    },
    formatted_price() {
      return formatter(this.collection.price)
    },
    formatted_volume_traded() {
      return formatter(this.collection.volume_traded)
    },
  },

  mounted() {
    /*const scrollHeight = this.$refs.desc.scrollHeight
    const offsetHeight = this.$refs.desc.offsetHeight

    if (scrollHeight > offsetHeight) {
      this.is_long_text = true
    }
    */
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
