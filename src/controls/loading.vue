<template>
  <div v-if="!wait" class="app-loading">
    <img src="~assets/loading.svg" width="204" height="236"/>
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
  justify-content: center
  align-items: center
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
  },
  data() {
    return {
      wait: false
    }
  },
  mounted() {
    /* 
      If we switch between pages in gallery loader appears and then disappears quickly. 
      This looks like blinking. So we delay it and display only if items are taking long time to load.
    */
    this.history = this.$store.getPrevPage()
    if (this.history) {
      this.wait = true
    }
    setTimeout(() => {
      this.wait = false
    }, 1500)
  }
}
</script>
