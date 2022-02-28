<template>
  <div class="container">
    <div class="back-btn" v-on:click="onBack">
      <img src="~assets/icon-back.svg"/>
      Back to Gallery
    </div>
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

    & .back-btn {
      display: flex
      align-items: center
      cursor: pointer
      font-size: 14px
      font-weight: bolder
      letter-spacing: 1.5px
      opacity: 0.5

      & img {
        margin-right: 0.25em
        width: 14px
        height: 14px
      }
    }

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

export default {
  props: {
    artwork: {
      type: Object,
      required: true
    },
    author: {
      type: String,
      required: true
    }
  },

  computed: {
    title () {
      return this.artwork.title
    }
  },

  emits: [
    'back'
  ],

  components: {
    artPrice
  },

  methods: {
    onBack(ev) {
      this.$emit('back')
    }
  }
}
</script>