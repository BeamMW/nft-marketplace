<template>
  <div class="container">
    <backBtn @click="$emit('back')"/>
    <div class="details-row"> 
      <div class="artwork-container">
        <div class="artwork-box">
          <artPreview v-bind:artwork="artwork"/>
        </div>
      </div>
      <div class="info-container">
        <div class="info-box">
          <div class="title">{{title || "Loading..."}}</div>
          <div class="author" v-if="author">by <span @click="onAuthor">{{author}}</span></div>
          <div class="author" v-else>Loading...</div>
          <div class="price">
            <div class="separator"/>
            <artPrice v-bind:artwork="artwork"/>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    flex-direction: column
    margin-top: 15px

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
              color: #00F6D2
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

export default {
  props: {
    artwork: {
      type: Object,
      required: true
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
    }
  },

  emits: [
    'back'
  ],

  components: {
    artPrice,
    artPreview,
    backBtn
  },

  methods: {
    onAuthor () {
      // TODO: enable in the future 
      // this.$store.setFilterByArtist(this.artwork.pk_author)
      // this.$emit('back')
    }
  }
}
</script>