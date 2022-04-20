<template>
  <btn height="36px" 
       width="36px" 
       padding="0px" 
       radius="10px"
       @click="onClick"
  >
    <img src="~assets/icon-wallet.svg">
  </btn>
</template>

<script>
import btn from './button.vue'

export default {
  components: {
    btn
  },

  methods: {
    onClick () {
      this.$store.toBalance()
    }
  }
}
</script>
