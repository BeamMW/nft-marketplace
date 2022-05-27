<template>
  <span id="modals"/>
  <error v-if="error && debug" :text="errtext"/>
  <errorModal v-if="error && !debug" :text="errtext"/>
  <loading v-if="loading" :text="loading"/>
  <div v-else-if="!loading" id="app-container" class="app-container">
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
import error from 'components/error'
import errorModal from 'components/error-modal'
import loading from 'controls/loading'
import utils from 'utils/utils'

export default {
  components: { 
    loading, error, errorModal
  },
  computed: {
    loading () {
      return  this.$state.loading
    },
    error () {
      return this.$state.error
    },
    debug () {
      return this.$state.debug
    },
    errtext () {
      let formatError = (error) =>  {
        if (typeof error === 'string') {
          return error
        }
        
        if (error instanceof Error) {
          return error.cause ? [error.stack, formatError(error.cause)].join('\n') : error.stack
        }

        error = Object.assign({}, error)
        const maxLen = 50

        if (error.answer && error.answer.result) {
          if(error.answer.result.raw_data) {
            error.answer.result.raw_data = '--excluded--'
          }

          if (error.answer.result.output && error.answer.result.output.length > maxLen) {
            error.answer.result.output = error.answer.result.output.substring(0, maxLen) + ' --excluded--'
          }
        }
        return utils.formatJSON(error)
      }
      return formatError(this.error.error)
    }
  }
  // TODO: non-debug error handling
  /*
  methods: {
    onCopy () {
      let err = Object.assign({}, this.error.error)
      if (typeof this.error.error === 'string') {
        this.allErrorText = [this.error.context || 'Error occured', this.error.error].join(' ')
      }
      
      if (err.answer && err.answer.result) {
        if (err.answer.result.raw_data) {
          this.allErrorText = err.answer.result.raw_data
        }

        if (err.answer.result.output ) {
          this.allErrorText = err.answer.result.output
        }
      }

      if (this.$state.error.error instanceof Error && !this.allErrorText ) {
        this.allErrorText = this.error.error.stack
      }

      let textArea = document.createElement('textarea')
      textArea.style.position = 'fixed'

      textArea.value = this.allErrorText
      document.body.appendChild(textArea)
      textArea.focus()
      textArea.select()
    }
  }*/
}
</script>