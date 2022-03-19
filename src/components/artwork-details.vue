<template>
  <div id="container" class="container">
    <backBtn @click="$router.go(-1)"/>
    <div class="details-row"> 
      <div class="artwork-container">
        <div class="artwork-box">
          <artPreview key="" :artwork="artwork"/>
        </div>
      </div>
      <div class="info-container">
        <div class="info-box">
          <div v-if="title || !error" class="title">{{ title || "Loading..." }}</div>
          <div v-else class="title">Failed to load artwork</div>
          <div v-if="author" class="author">by <span @click="onAuthor">{{ author }}</span></div>
          <div v-else-if="error" class="author">{{ artwork.error }}</div>
          <div v-else class="author">Loading...</div>
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
      <div v-for="(sale, index) in sales.slice().reverse()" :key="`sale-${index}`" class="row">
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
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    padding-top: 15px
    width: 100%
    height: 100%

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
        margin: 20px 0
        opacity: 0.2
        border-top: solid 1px #fff
      }

      & .artwork-container {
        flex: 0 0 50%
        box-sizing: border-box
        padding-right: 10px
        max-width: 50%
        
        & .artwork-box {
          background-color: rgba(255, 255, 255, 0.05)
          border-radius: 10px
          min-height: 360px
          display: flex
          align-items: center
          justify-content: center
          overflow: hidden

          & > img {
            max-width: 100%
            max-height: 360px
            object-fit: contain
          }
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
          padding: 35px 40px 
          display: flex
          flex-direction: column

          & .title {
            font-size: 20px
            font-weight: bold
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
</style>

<script>
import artPrice from './artwork-price.vue'
import backBtn from './back-btn.vue'
import artPreview from './artwork-preview.vue'
import utils from '../utils/utils.js'
import {binary_search} from '../utils/search.js'
/*
import "ag-grid-community/dist/styles/ag-grid.css"
import "ag-grid-community/dist/styles/ag-theme-alpine.css"
import { AgGridVue } from "ag-grid-vue3"
*/

export default {
  components: {
    artPrice,
    artPreview,
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
      sales: undefined
    }
  },

  computed: {
    title () {
      return this.artwork.title
    },

    artists () {
      return this.$state.artists
    },

    author () {
      return (this.artists[this.artwork.pk_author] || {}).label
    },

    artwork () {
      let all = this.$state.artworks
      let idx = binary_search(all, a => a.id == this.id ? 0 : a.id < this.id ? 1 : -1)
      return all[idx]
    },

    error () {
      return !!this.artwork.error
    }
  },
  
  created() {
    // TODO: also update on new block
    this.$store.getSalesHistory(this.id, (sales) => {
      this.sales = sales
    })
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