<template>
    <error v-if="error" v-bind:error="error.error" v-bind:context="error.context"/>
    <loading v-else-if='loading'/>
    <template v-else>
        <div class="warning">
            V1 gallery is in read-only mode. You can only view content and withdraw funds.
        </div>
        <router-view></router-view>
    </template>    
</template>

<style scoped lang="stylus">
.warning {
    border: 1px solid rgba(255, 0, 0, 0.8)
    text-align: center
    padding: 5px 0px 8px 0px
    margin-bottom: 20px
    background-color: rgba(255, 0, 0, 0.5)
}
</style>

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