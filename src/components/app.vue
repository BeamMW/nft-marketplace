<template>
  <span id="modals"/>
  <errorModal v-if="error" :error="error"/>
  <loading v-if="loading" id="app-container" :text="loading"/>
  <div v-else id="app-container" class="app-container">
    <router-view/>
  </div>
</template>

<style lang="stylus" scoped>
.app-container {
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
  }
}
</script>