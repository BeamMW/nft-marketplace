<template>
  <div v-if="debug" class="error">
    <div>
      <pre>{{ text }}</pre>
      <span class="restart">Restarting in {{ errleft }}</span>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.error {
  display:         flex
  align-items:     center
  justify-content: center
  height:          100%
  width:           100%
  overflow:        hidden
  color:           #c55b61 
  
  & > div {
    width: 70%
    
    & > pre {
      font-family: 'SFProDisplay', sans-serif
      font-weight: normal
      white-space: pre-wrap
    }
  }
}
</style>

<script>
export default {
  props: {
    text: {
      default: undefined,
      type: [String, Object]
    },
  },

  data () {
    return {
      errleft: 0
    }
  },

  mounted () {
    this.errleft = 10
    this.timeout = setInterval(() => {
      this.errleft -= 1
      if (this.errleft == 0) {
        clearInterval(this.timeout)
        this.$store.clearError()
      }
    }, 1000)
  }
}
</script>