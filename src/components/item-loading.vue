<template>
    <div class="loading darker">
    {{ this.error ? 'Failed to load artwork' : 'Loading...' }}
    </div>
</template>


<style lang="stylus" scoped>
.loading {
    display:         flex
    align-items:     center
    justify-content: center
    height:          100%
    width:           100%
    overflow:        hidden
    word-break:      break-word   

    .darker {
        opacity: 0.5
    }
}
</style>

<script>
export default {
    props: {
        error: {
            type: Boolean,
            default: false,
            required: false
        }
    },
}
</script>