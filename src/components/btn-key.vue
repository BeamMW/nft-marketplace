<template>
  <div>
    <publicKeyModal ref="keyModal"/>
    <btn height="36px" 
         width="36px" 
         padding="0px" 
         radius="10px"
         @click="onClick"
    >
      <img src="~assets/icon-key.svg">
    </btn>
  </div>
</template>

<script>
import btn from './button'
import publicKeyModal from './public-key-dialog'

export default {
  components: {
    btn,
    publicKeyModal
  },

  methods: {
    onClick () {
      this.$refs.keyModal.open()
    }
  }
}
</script>
