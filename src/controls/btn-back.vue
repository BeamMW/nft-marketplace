<template>
  <div class="back-btn" @click="onBack()">
    <img src="~assets/back.svg"/>
    {{ text }}
  </div>
</template>

<style scoped lang="stylus">
  .back-btn {
    display: flex
    align-items: center
    cursor: pointer
    font-size: 14px
    font-weight: bolder
    letter-spacing: 1.5px
    opacity: 0.9

    & img {
      margin-right: 0.25em
      width: 14px
      height: 14px
    }
  }
</style>

<script>
import utils from 'utils/utils'

export default {
  props: {
    text: {
      type: String,
      required: false,
      default: utils.isCompact() ? '' : 'back'
    },
  },
  methods: {
    onBack() {
      this.$store.toBack()
    }
  }
}
</script>
