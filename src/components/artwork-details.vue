<template>
  <div id="container" class="container">
    <backBtn/>
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
            <div class="separator"/>
            <div class="creator">
              <p>Creator: <span :class="{'error': error}" v-html="by_author"></span></p>
              <p>Collection: <span>{{ collection }}</span></p>
            </div>
            <!-- <div class="collection">
              <p>Collection: <span>Master Splinter</span></p>
            </div> -->
            <div class="price">
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
  .container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    padding-top: 15px
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
        
        .description {
          font-family: 'SFProDisplay', sans-serif
          font-style: normal
          font-weight: 400
          font-size: 16px
          line-height: 19px
          display: -webkit-box
          -webkit-line-clamp: 3
          -webkit-box-orient: vertical
          overflow: hidden
          margin-top: 26px
        }

        .separator {
          height: 1px
          margin: 20px 0
          opacity: 0.2
          border-top: solid 1px #fff
        }

        .creator {
          font-family: 'SFProDisplay', sans-serif

          & > p:first-child {
            margin-bottom: 12px
          }

          & > p {
            font-style: normal
            font-weight: 400
            font-size: 16px
            line-height: 19px
            margin: 0

            & > span {
              color: #00f6d2
            }
          }
        }

        .collection {
          font-style: normal
          font-weight: 400
          font-size: 16px
          line-height: 19px

          & > span {
            font-style: normal
            font-weight: 400
            font-size: 16px
            line-height: 19px
            color: #00f6d2
          }
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
            padding: 28px 40px 
            display: flex
            flex-direction: column

            & .title {
              font-size: 20px
              font-weight: bold
              font-style: normal
              color: #fff
              white-space: nowrap
              text-overflow: ellipsis
              overflow: hidden
            }

            & .author {
              font-size: 16px
              color: #fff
              margin-top: 8px

              & > span {
                color: #00f6d2
                margin-left: 4px
              }
            }

            & .price {
              margin-top: auto
            }
          }
        }
      }
    }
  }
</style>

<script>
import artPrice from './artwork-price'
import backBtn from './back-btn.vue'
import preview from './image-preview'
import artistsStore from 'stores/artists'
import artsStore from 'stores/artworks'
import collsStore from 'stores/collections'
import utils from 'utils/utils'
import {def_images} from 'utils/consts'
/*
import "ag-grid-community/dist/styles/ag-grid.css"
import "ag-grid-community/dist/styles/ag-theme-alpine.css"
import { AgGridVue } from "ag-grid-vue3"
*/

export default {
  components: {
    artPrice,
    preview,
    backBtn
  },

  props: {
    id: {
      type: Number,
      required: true
    }
  },

  emits: [
    'back'
  ], 

  data () {
    return {
      sales: undefined,
      def_artwork: def_images.artwork
    }
  },

  computed: {
    artwork () {
      return artsStore.getItem(this.id)
    },

    error () {
      return !!this.artwork.error
    },
    
    title () {
      return this.artwork.label
    },

    description () {
      return this.artwork.description
    },

    by_author () {
      return this.artwork.by_author
    },

    image () {
      return this.artwork.image
    },

    artists () {
      return artistsStore.artists
    },
    collection () {
      let col  = collsStore.user_items.find(c => c.id == this.id)
      return (col || {}).label
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
      // TODO: enable in the future 
      // this.$store.setFilterByArtist(this.artwork.pk_author)
      // this.$emit('back')
    }
  }
}
</script>