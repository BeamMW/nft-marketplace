<template>
  <span id="modals"/>
  <errorModal v-if="error" :error="error"/>
  <loading v-if="loading" id="container" :text="loading"/>
  <div v-else id="container" class="container">
    <router-view/>
  </div>
</template>

<style lang="stylus" scoped>
.container {
  width: 100%
  height: 100%
}
</style>

<script>
import errorModal from 'components/error-modal'
import loading from 'controls/loading'

export default {
  components: { 
    loading, errorModal
  },
  computed: {
    error () {
      return this.$state.error
    },
    debug () {
      return this.$state.debug
    },
    loading () {
      return this.$state.loading
    }
  },
  mounted() {
    document.addEventListener('keydown', event => {
      if (event.isComposing || event.keyCode === 229) {
        return
      }
      if (event.code === 'KeyD' && event.ctrlKey && event.shiftKey) {
        this.$store.toggleDebug()
      }
    }, true)
  }
}
</script>