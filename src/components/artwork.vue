<template>
  <div class="artwork">
    <!---- Preview OR Loading ---->
    <div class="preview-container" @click="onDetails">
      <artPreview :artwork="artwork"/>
    </div>

    <!---- Delete Artwork Button ---->
    <img v-if="is_admin" class="artwork-delete" src="~assets/icon-delete.svg" @click="onDelete"/>

    <!---- First info row ---->
    <div class="info-row">
      <!---- Title ---->
      <div v-if="loading" class="artwork-title">Loading...</div>
      <div v-else-if="error" class="artwork-title">Failed to load artwork</div>
      <div v-else class="artwork-title">{{ title }}</div>
      
      <!---- Likes ----->
      <div class="artwork-likes pointer-cursor" :disabled="!can_vote" v-on="{click: liked ? onUnlike : onLike}">
        <img :src="'./assets/icon-heart' + (liked ? '-red' : '') + '.svg'"/>
        <span class="artwork-likes__text">{{ likes_cnt }}</span>
      </div>
    </div>

    <!---- Second info row, author ---->
    <div class="info-row">
      <span v-if="loading" class="artwork-author">Loading...</span>
      <span v-else-if="error" class="artwork-author"></span>
      <span v-else class="artwork-author">
        {{ ['by', author].join(' ') }}
      </span>
    </div>

    <!---- Third info row, price/buy/sell ----->
    <div class="artwork-price-row">
      <artPrice :artwork="artwork"/>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
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
      this.$store.showDetails(this.id)
    }
  }
}
</script>