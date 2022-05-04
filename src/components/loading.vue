<template>
  <div class="app-loading">
    <div class="title">{{ text }}</div>
    <span>Please wait...</span>
  </div>
</template>

<style lang="stylus" scoped>
.app-loading {
  text-align: center
  margin: 0 auto
  margin-top: 50px

  .title {
    font-size: 22px
    font-weight: 400
    user-select: none
    margin-bottom: 8px
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
