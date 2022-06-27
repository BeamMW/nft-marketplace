<template>
  <div v-if="!is_desktop" class="new-box">
    It is not possible to create collections in web and mobile wallets.<br>
    Please download desktop wallet and try from there.
  </div>
  <div v-else class="new-box" @click="onNewCollection">
    Create new collection
  </div>
</template>

<style lang="stylus" scoped>
.new-box {
  width: 442px
  height: 150px !important
  border-radius: 10px
  border: 1px dashed rgba(26, 246, 214, 0.5)
  background-color: rgba(26, 246, 214, 0.1)
  cursor: pointer
  color: rgba(26, 246, 214, 0.7)
  font-size: 14px

  display: flex
  align-items: center
  justify-content: center
  text-align: center
}
</style>

<script>
import collsStore from 'stores/collections'

export default {
  computed: {
    is_desktop() {
      return this.$state.is_desktop
    }
  },
  methods: {
    onNewCollection () {
      collsStore.toNewItem()
    }
  } 
}
</script>