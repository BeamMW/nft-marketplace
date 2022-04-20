<template>
  <span id="modals"/>
  <error v-if="error" :error="error.error" :context="error.context" :debug="true"/>
  <loading v-else-if="loading"/>
  <div v-else id="app-container" class="app-container">
    <router-view></router-view>
  </div>
</template>

<style lang="stylus" scoped>
.app-container {
  width: 100%
  height: 100%
}
</style>

<script>
import loading from './loading.vue'
import error from './error.vue'

export default {
  components: { 
    loading, error
  },
  computed: {
    loading () {
      return  this.$state.loading
    },
    error () {
      return this.$state.error
    }
  }
}
</script>