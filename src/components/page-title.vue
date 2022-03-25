<template>
  <div class="container">
    <backBtn/>
    <div class="title">{{ title }}</div>
    <div>
      <slot></slot>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    height: 47px
    display: flex
    flex-flow: row
    justify-content: center
    align-items: center

    & > * {
      flex-grow: 1
      flex-basis: 0
    }

    .title {
      display: flex
      justify-content: center
      align-items: center
      font-family: 'SFProDisplay', sans-serif
      font-weight: 700
      font-size: 14px
      line-height: 17px
      text-transform: uppercase
      letter-spacing: 3.1px
      color: #fff
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
