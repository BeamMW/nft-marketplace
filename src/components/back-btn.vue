<template>
  <div class="back-btn">
      <img src="~assets/icon-back.svg"/>
      {{text}}
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
    opacity: 0.5

    & img {
      margin-right: 0.25em
      width: 14px
      height: 14px
    }
  }
</style>

<script>
export default {
  props: {
    text: {
      type: String,
      required: false,
      default: "back"
    },
  }
}
</script>
