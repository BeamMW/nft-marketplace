<template>
  <loading v-if="artwork === undefined" 
           text="Loading NFT"
  />
  <div v-else-if="artwork == null">
    NFT Not Found  
  </div>
  <div v-else id="container" class="artwork-details-container">
    <pageTitle/>
    <div class="content-wrapper" :class="{'error': error}">
      <div class="details-row"> 
        <div class="artwork-container">
          <div>
            <preview :image="image" 
                     :default="def_artwork" 
                     :cover="!(image || {}).object"
                     height="360px" 
                     radius="0"
            />
          </div>
        </div>
        <div class="info-container">
          <div class="info-box">
            <div class="title" :class="{'error': error}">{{ title }}</div>
            <div class="description" :class="{'error': error}">{{ description }}</div>
            <div class="bottom">
              <div class="separator"/>
              <div class="author" :class="{'error': error}">Creator: <span>{{ author_name }}</span></div>
              <!-- TODO: make collection clickable and navigate to it on click -->
              <div class="collection" :class="{'error': error || collection_error}">Collection: <span>{{ collection_name }}</span></div>
              <div class="separator"/>
              <artPrice :artwork="artwork"/>
            </div>
          </div>
        </div>
      </div>
      <div class="table-title">Nft trading history</div>
      <div class="table-head"> 
        <div width="120px">Coin</div>
        <div width="200px">Amount</div>
        <div v-if="sales && sales.length" class="sorted">
          Height
          <img src="~assets/icon-down.svg" class="arrow"/>
        </div>
        <div v-else>
          Height
        </div>
      </div>  
      <div v-if="sales != undefined && sales.length" class="table-body">
        <div v-for="(sale, index) in sales.slice().reverse()" :key="`sale-${sales.length - index}`" class="row">
          <div>
            <img src="~assets/icon-beam.svg"/>
            <span class="curr">BEAM</span>
          </div>
          <div class="text">{{ formatAmount(sale.amount) }}</div>
          <div class="text">{{ formatHeight(sale.height) }}</div>
        </div>
      </div>
      <div v-else-if="sales !== undefined && !sales.length" class="table-body-empty">
        <img src="~assets/icon-wallet-empty.svg"/>
        <div>Trading history is empty</div>
      </div>  
      <div v-else class="table-body-loading">
        Loading...
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .artwork-details-container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    width: 100%
    height: 100%

    & .content-wrapper {
      display: flex
      flex-direction: column
      flex: 1

      & .table-title {
        box-sizing: border-box
        padding: 30px 0 20px 0
        color: rgba(255, 255, 255, 0.5)
        font-size: 14px
        text-transform: uppercase
        font-weight: bold
        letter-spacing: 3.1px
      }
      
      /*
      *  TABLE HEAD
      */ 
      & .table-head {
        border-radius: 10px 10px 0 0
        background-color: rgba(255, 255, 255, 0.08)
        font-weight: 700
        font-size: 14px
        color: rgba(255, 255, 255, 0.5)
        display: flex

        & > div {
          padding: 9px 0px 12px 0px
          text-align: center

          &.sorted {
            color: #fff

            .arrow {
              width:8px
              height:8px
              margin-left:3px
            }
          }
        }
        
        & > div:nth-child(1) {
          width: 120px
        }

        & > div:nth-child(2) {
          width: 200px
        }

        & > div:nth-child(3) {
          flex: 1
        }
      }

      & .table-body-loading {
        padding: 35px 25px 0 25px
        text-align: center
        color: rgba(255, 255, 255, 0.5)
      }

      & .table-body-empty {
        padding: 35px 25px 0 25px
        text-align: center
        color: rgba(255, 255, 255, 0.5)

        & img {
          padding-bottom: 10px
        }
      }

      /*
      *  TABLE BODY
      */ 
      & .table-body {
        display: block
        overflow-y: overlay

        & .row {
          display: flex
          padding: 12px 0 11px 0

          & > div {
            display: flex
            align-items: center
            justify-content: center
          }

          .curr {
            padding: 0 0 3px 10px
            line-height: 17px
            box-sizing: border-box
          }

          .text {
            padding: 0 0 3px 0
          }

          & > div:nth-child(1) {
            width: 120px
          }

          & > div:nth-child(2) {
            width: 200px
          }

          & > div:nth-child(3) {
            flex: 1
          }
        }

        & > div:nth-child(odd) {
          background: rgba(255, 255, 255, 0.04)
        }

        & > div:nth-child(even) {
          background: rgba(255, 255, 255, 0.02)
        }
      }

      & .details-row {
        display: flex
        flex-direction: row
        margin-top: 15px

        .separator {
          height: 1px
          margin: 20px 0 15px 0
          opacity: 0.2
          border-top: solid 1px #fff
        }

        & .artwork-container {
          flex: 0 0 50%
          box-sizing: border-box
          max-width: 50%
          padding-right: 10px

          & > div {
            background-color: rgba(255, 255, 255, 0.05)
            border-radius: 10px
            min-height: 360px
            overflow: hidden
          }
        }

        & .info-container {
          flex: 0 0 50%
          box-sizing: border-box
          padding-left: 10px
          max-width: 50%
          
          & .info-box {
            background-color: rgba(255, 255, 255, 0.05)
            min-height: 360px
            border-radius: 10px
            height: 100%
            box-sizing: border-box
            padding: 20px 27px 
            display: flex
            flex-direction: column

            & > .title {
              font-size: 20px
              font-weight: bold
              color: #fff
              white-space: nowrap
              text-overflow: ellipsis
              overflow: hidden
            }

            & > .description {
              color: white
              margin-top: 20px
              font-size: 16px
            }

            & .bottom {
              margin-top: auto
              
              & > .author {
                font-size: 16px
                color: #fff
                display: flex
                flex-direction: row

                & > span {
                  color: #00f6d2
                  margin-left: 4px
                  white-space: nowrap
                  text-overflow: ellipsis
                  overflow: hidden
                }
              }

              & > .collection {
                font-size: 16px
                color: #fff
                margin-top: 6px
                display: flex
                flex-direction: row

                & > span {
                  color: #00f6d2
                  margin-left: 4px
                  white-space: nowrap
                  text-overflow: ellipsis
                  overflow: hidden
                }
              }
            }
          }
        }
      }
    }
  }
</style>

<script>
import artPrice from './artwork-price'
import preview from './image-preview'
import artistsStore from 'stores/artists'
import artsStore from 'stores/artworks'
import collsStore from 'stores/collections'
import utils from 'utils/utils'
import {def_images} from 'utils/consts'
import pageTitle from './page-title'
import {useObservable} from '@vueuse/rxjs'
import {Observable} from 'rxjs'
import {computed} from 'vue'

/*
import "ag-grid-community/dist/styles/ag-grid.css"
import "ag-grid-community/dist/styles/ag-theme-alpine.css"
import { AgGridVue } from "ag-grid-vue3"
*/

export default {
  components: {
    artPrice,
    preview,
    pageTitle
  },

  props: {
    id: {
      type: Number,
      required: true
    }
  },

  setup(props) {
    let artworkObservable = computed(() => useObservable(artsStore.getLazyItem('manager', props.id)))
    let artwork = computed(() => {
      let result = artworkObservable.value.value
      if (!result) {
        return result
      }
      return result.length == 0 ? null : result[0]
    })

    let collsObservable = computed(() => {
      if (!artwork.value) return new Observable(subscriber => subscriber.next(artwork.value))
      if (!artwork.value.collection) return new Observable(subscriber => subscriber(null))
      return useObservable(collsStore.getLazyItem('manager', artwork.value.collection))
    })  
     
    let collection = computed(() => {
      let result = collsObservable.value.value
      if (!result) {
        return result
      }
      return result.length == 0 ? null : result[0]
    })
    
    return {
      artwork,
      collection
    }
  },

  data () {
    return {
      sales: undefined,
      def_artwork: def_images.artwork
    }
  },

  computed: {
    error () {
      return !!this.artwork.error
    },
    
    title () {
      return this.artwork.label
    },

    description () {
      return this.artwork.description
    },

    author_name () {
      return this.artwork.author_name
    },

    collection_name() {
      if (this.collection === null) return 'Error: not found'
      if (this.collection === undefined) return 'Loading...'
      return this.collection.label
    },

    collection_error() {
      return this.collection === null || (this.collection && this.collection.error)
    },

    image () {
      return this.artwork.image
    },

    artists () {
      return artistsStore.artists
    }
  },
  
  created() {
    (async () => {
      this.sales = await artsStore.getSales(this.id)
    })()
  },

  methods: {  
    formatAmount(amount) {
      return utils.formatAmount(amount)
    },
    formatHeight(height) {
      return utils.formatHeight(height)
    },
    onAuthor () {
      // FUTURE
    }
  }
}
</script>