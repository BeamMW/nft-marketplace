<template>
  <div>
    <keyModal ref="keyModal"/>
    <btn height="36px" 
         width="36px" 
         padding="0px" 
         radius="10px"
         tooltip="show artist key"
         @click="onClick"
    >
      <img src="~assets/key.svg">
    </btn>
  </div>
</template>

<script>
import keyModal from 'components/key-modal'
import btn from 'controls/button'

export default {
  components: {
    btn,
    keyModal
  },

  methods: {
    onClick () {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 
      this.$refs.keyModal.open()
    }
  }
}
</script>
