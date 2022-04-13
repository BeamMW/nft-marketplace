<template>
  <div class="loading darker">
    {{ error ? error : 'Loading...' }}
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
  user-select:     none
  
  .darker {
    opacity: 0.5
  }
}
</style>

<script>
export default {
  props: {
    error: {
      type: String,
      default: '',
      required: false
    }
  },
}
</script>