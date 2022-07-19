<template>
  <btn height="36px" 
       width="36px" 
       padding="0px" 
       radius="10px"
       tooltip="show balance"
       @click="onClick"
  >
    <img src="~assets/wallet.svg">
  </btn>
</template>

<script>
import btn from 'controls/button'

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
