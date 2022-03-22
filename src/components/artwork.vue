<template>
  <div class="artwork">
    <!---- Preview OR Loading ---->
    <div class="preview-container" @click="onDetails">
      <artPreview :artwork="artwork"/>
    </div>

    <!---- Delete Artwork Button ---->
    <img v-if="is_admin" class="delete" src="~assets/icon-delete.svg" @click="onDelete"/>

    <!---- First info row ---->
    <div class="info-row">
      <!---- Title ---->
      <div v-if="loading" class="title">Loading...</div>
      <div v-else-if="error" class="title">Failed to load artwork</div>
      <div v-else class="title">{{ title }}</div>
      
      <!---- Likes ----->
      <div class="likes" :disabled="!can_vote" v-on="{click: liked ? onUnlike : onLike}">
        <img :src="'./assets/icon-heart' + (liked ? '-red' : '') + '.svg'"/>
        <span>{{ likes_cnt }}</span>
      </div>
    </div>

    <!---- Second info row, author ---->
    <div class="info-row">
      <span v-if="loading" class="author">Loading...</span>
      <span v-else-if="error" class="author"></span>
      <span v-else class="author">
        {{ ['by', author].join(' ') }}
      </span>
    </div>

    <!---- Third info row, price/buy/sell ----->
    <div class="price-row">
      <artPrice :artwork="artwork"/>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .artwork {
    margin-right: 16px
    margin-bottom: 16px
    display: flex
    flex-direction: column
    width: 284px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px
    position:relative

    & .preview-container {
      height: 360px
      background-color: rgba(240, 205, 205, 0.1)
      display: flex
      align-items: center
      justify-content: center
      overflow: hidden
      border-top-left-radius: 10px
      border-top-right-radius: 10px
      cursor: pointer
    }

    & > .delete {
      position: absolute
      left: 263px
      top: 5px
      width: 15px
      cursor: pointer
    }

    & > .info-row {
      box-sizing: border-box
      padding: 0 14px 0px 14px
      display: flex
      align-items: center
      flex-direction: row
      margin-bottom: 3px
      overflow: hidden

      & .title {
        font-size: 16px
        color: #fff
        white-space: nowrap
        text-overflow: ellipsis
        overflow: hidden
        padding-top: 20px
        flex: 1
      }

      & .likes {
        display: flex
        align-items: center
        cursor: pointer
        box-sizing: border-box
        padding-top: 20px

        & > span {
          padding-left: 5px
        }
      }

      & .author {
        font-size: 12px
        color: rgba(255, 255, 255, 0.5)
        display: inline-block
        white-space: nowrap
        overflow: hidden
        text-overflow: ellipsis
      }
    }

    & .price-row {
      min-height: 41px
      margin: 10px 0 20px 0
      padding: 0 12px 0 14px
      display: flex
      align-items: center
      flex-direction: row
    }
  }
</style>

<script>
import artPrice from './artwork-price.vue'
import artPreview from './artwork-preview.vue'

export default {
  components: {
    artPrice,
    artPreview
  },

  props: {
    artwork: {
      type: Object,
      required: true,
    }
  },

  emits: [
    'like', 
    'unlike', 
    'delete', 
    'details'
  ],

  computed: {
    is_admin () {
      return this.$state.is_admin
    },

    is_headless () {
      return this.$state.is_headless
    },

    id () {
      return this.artwork.id
    },

    title () {
      return this.artwork.title
    },
        
    likes_cnt () {
      return this.artwork.impressions
    },

    liked () {
      return !!this.artwork.my_impression
    },
        
    can_vote () {
      return this.$state.balance_reward > 0
    },

    loading () {
      return this.artwork.loading
    },

    artists () {
      return this.$state.artists
    },

    author () {
      return (this.artists[this.artwork.pk_author] || {}).label
    },

    error () {
      return !!this.artwork.error
    }
  },

  methods: {
    onLike (ev) {
      if (this.is_headless) 
      {
        this.$store.switchToHeaded()  
      } 
      else {
        if (this.can_vote) {
          this.$emit('like', this.id)
        }
      }
    },

    onUnlike (ev) {
      if (this.can_vote) {
        this.$emit('unlike', this.id)
      }
    },

    onDelete (ev) {
      this.$emit('delete', this.id)
    },

    onDetails(ev) {
      this.$store.showArtworkDetails(this.id)
    }
  }
}
</script>