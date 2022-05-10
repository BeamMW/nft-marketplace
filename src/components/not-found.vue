<template>
  <div :class="{'not-found': true, 'error': error}">
    <img src="~assets/icon-wallet-empty.svg"/>
    <div class="text">{{ text }}</div>
  </div>
</template>

<style lang="stylus" scoped>
.not-found {
  text-align: center
  margin: 0 auto
  margin-top: 50px

  & > .text {
    font-size: px
    font-weight: 400
    user-select: none
    padding: 15px 25px 0 25px
    color: rgba(255, 255, 255, 0.5)
  }
}
</style>

<script>
export default {
  props: {
    text: {
      type: String,
      default: 'Not Found'
    },
    error: {
      type: Boolean,
      default: false
    }
  }
}
</script>
