<template>
  <span id="modals"/>
  <error v-if="error && debug" :text="errtext"/>
  <errorModal v-if="error && !debug" :text="errtext" @copy="onCopy"/>
  <loading v-if="loading"/>
  <div v-else-if="!loading" id="app-container" class="app-container">
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
import errorModal from './error-modal.vue'
import utils from 'utils/utils.js'

export default {
  components: { 
    loading, error, errorModal
  },
  data () {
    return {
      allErrorText: null,
    }
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
      const maxLen = 50

      if (typeof this.error.error === 'string') {
        return [this.error.context || 'Error occured', this.error.error].join('/n')
      }

      if (this.error.error instanceof Error) {
        if (this.debug) {
          return this.error.error.stack
        } else {
          return this.error.error.stack.substring(0, maxLen) + ' --excluded--'
        }
      }
      
      let err = Object.assign({}, this.error.error)
      // strip off some long unncessary binary stuff that might occur here
      if (err.answer && err.answer.result) {
        if(err.answer.result.raw_data) {
          err.answer.result.raw_data = '--excluded--'
        }

        if (err.answer.result.output && err.answer.result.output.length > maxLen) {
          err.answer.result.output = err.answer.result.output.substring(0, maxLen) + ' --excluded--'
        }
      }
            
      let serr = utils.formatJSON(err)
      return [this.error.context || 'Error occured', serr].join('\n')
    }
  },
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
            
      try {
        return document.execCommand('copy')
      } 
      catch (ex) {
        return false
      } 
      finally {
        document.body.removeChild(textArea)
      }
    }
  }
}
</script>