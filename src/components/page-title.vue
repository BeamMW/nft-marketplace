<template>
  <div class="container">
    <backBtn class="left"/>
    <div class="title">{{ title }}</div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    flex-flow: row
    justify-content: center

    .left {
      position: absolute
      top: 0
      left: 0
    }

    .title {
      font-family: 'SFProDisplay', sans-serif
      font-weight: 700
      font-size: 14px
      line-height: 17px
      text-transform: uppercase
      letter-spacing: 3.1px
      color: #fff
      padding-top: 2px
    }
  }
</style>

<script>
import backBtn from './back-btn.vue'

export default {
  components: {
    backBtn
  },

  props: {
    title: {
      type: String,
      required: true
    }
  }
}
</script>
