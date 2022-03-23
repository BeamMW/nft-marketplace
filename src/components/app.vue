<template>
  <span id="modals"/>
  <error v-if="error" :error="error.error" :context="error.context"/>
  <loading v-else-if="loading"/>
  <router-view v-else></router-view>
</template>

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