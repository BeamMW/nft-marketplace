<template>
    <error v-if="error" v-bind:error="error.error" v-bind:context="error.context"/>
    <loading v-else-if='loading'/>
    <router-view v-else></router-view>
</template>

<script>
import loading from './loading.vue'
import error from './error.vue'

export default {
    computed: {
        loading () {
            return  this.$state.loading
        },
        error () {
            return this.$state.error
        }
    },

    components: { 
        loading, error
    }
}
</script>