<template>
  <div class="error">
    <div>
      <pre>{{ errtext }}</pre>
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
import utils from 'utils/utils.js'
export default {
  props: {
    error: {
      default: undefined,
      type: [String, Object]
    },
    context: {
      type: String,
      default: ''
    }
  },

  data () {
    return {
      errleft: 0
    }
  },

  computed: {
    errtext () {
      if (typeof this.error === 'string') {
        return [this.context || 'Error occured', this.error].join('\n')
      }

      if (this.error instanceof Error) {
        return this.error.stack
      }
      
      let err = Object.assign({}, this.error)
      // strip off some long unncessary binary stuff that might occur here
      if (err.answer && err.answer.result) {
        if(err.answer.result.raw_data) {
          err.answer.result.raw_data = '--excluded--'
        }

        const maxLen = 50
        if (err.answer.result.output && err.answer.result.output.length > maxLen) {
          err.answer.result.output = err.answer.result.output.substring(0, maxLen) + ' --excluded--'
        }
      }
            
      let serr = utils.formatJSON(err)
      return [this.context || 'Error occured', serr].join('\n')
    }
  },

  mounted () {
    this.errleft = 5
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