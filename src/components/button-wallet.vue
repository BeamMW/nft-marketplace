<template>
  <btn class="button__wallet" 
       height="34px" 
       width="34px" 
       padding="0" 
       :hover="false" 
  >
    <img src="~assets/icon-wallet.svg">
  </btn>
</template>

<style lang="stylus" scoped>
.button__wallet {
  border-radius: 10px
}
</style>

<script>
import btn from './button.vue'

export default {
  components: {
    btn
  }
}
</script>
