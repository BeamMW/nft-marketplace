<template>
  <div class="container">
    <backBtn text="Back to Gallery" @click="$emit('back')"/>
    <div class="details">
      <div class="title">{{title}}</div>
      <div class="separator"/>
      <div class="author">Creator: {{author}}</div>
      <div class="separator"/>
      <artPrice v-bind:artwork="artwork"/>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    flex-direction: column
    margin-top: 15px

    & .details {
      margin-top: 15px
      border: none
      background-color: rgba(240, 205, 205, 0.05)
      border-radius: 20px
      width: calc(100% - 100px)
      padding: 35px 50px

      & .title {
        font-size: 18px
        font-weight: bold
        color: #fff
      }

      & .author {
        font-size: 16px
        color: #fff
      }

      & .separator {
        height: 1px
        margin: 20px 0
        opacity: 0.2
        border-top: solid 1px #fff
      }
    }
  }
</style>

<script>
import artPrice from './artwork-price.vue'
import backBtn from './back-btn.vue'

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
    backBtn
  }
}
</script>