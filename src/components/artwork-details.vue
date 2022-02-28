<template>
  <div class="details">
    <div class="back-btn" v-on:click="onBack">
      <img src="~assets/icon-back.svg"/>
      Back to Gallery
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .details {
    display: flex
    flex-direction: column
    margin-top: 15px

    .back-btn {
      display: flex
      align-items: center
      cursor: pointer
      font-size: 14px
      font-weight: bolder
      letter-spacing: 1.5px
      opacity: 0.5

      img {
        margin-right: 0.25em
        width: 14px
        height: 14px
      }
    }
  }
</style>

<script>
export default {
  emits: ['back'],
  methods: {
    onBack(ev) {
      this.$emit('back')
    }
  }
}
</script>