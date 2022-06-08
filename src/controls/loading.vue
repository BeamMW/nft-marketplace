<template>
  <div class="app-loading">
    <img src="~assets/loading.svg"/>
    <div class="title">{{ text }}</div>
    <div>Please wait...</div>
  </div>
</template>

<style lang="stylus" scoped>
.app-loading {
  text-align: center
  width: 100%
  height: 100%
  display: flex
  flex-direction: column
  align-content: center
  justify-content: center
  box-sizing: border-box
  padding-bottom: 40px
  font-size: 14px
  user-select: none

  .title {
    font-size: 17px
    font-weight: 400
    user-select: none
    margin: 20px 0px 3px 0px
  }
}
</style>

<script>
export default {
  props: {
    text: {
      type: String,
      default: 'BEAM Gallery is loading'
    }
  }
}
</script>
