<template>
  <modal ref="modal" max_width="70%">
    <div class="error-modal">
      <div class="title">{{ title }}</div>
      <div class="description">
        Something went wrong. Gallery will be automatically reloaded.<br>
        If you come across this problem again, copy error message using the copy button send it to support.
      </div>
      <div class="content">
        <pre class="error-text">
          {{ errtext }}
        </pre>
        <btn class="btn-copy" color="transparent" padding="11px 0 11px 10px" @click="onCopy">
          <img src="~assets/copy.svg"/>
        </btn>
      </div>
      <div class="restart">Restarting in {{ errleft }}s</div>
      <btn color="carnation" padding="10px 30px 10px 30px" text="ok" @click="onOK">
        <img src="~assets/ok.svg"/> 
      </btn>
    </div>
  </modal>
</template>

<style scoped lang="stylus">
  .error-modal {
    display: flex
    flex-direction: column
    align-items: center
    font-family: 'SFProDisplay', sans-serif

    .title {
      font-family: 'SFProDisplay', sans-serif
      font-size: 18px
      font-weight: 700
      margin-bottom: 20px
      font-style: normal
      text-align: center
      color: #fff
    }

    .description {
      text-align: center
      font-size: 14px
      line-height: 21px
      font-style: normal
      font-weight: 400
      margin-bottom: 15px
    }

    .content {
      display: flex
      flex-direction: row
      align-items: center
      justify-content: center
      max-width: 100%
      overflow: hidden

      .error-text {
        font-family: 'SFProDisplay', sans-serif
        font-weight: normal
        color: rgba(255, 90, 90, 1)
        overflow: hidden
      }

      .btn-copy {
        cursor: pointer
        margin-left: 10px
        flex: 1
      }
    }
    
    .restart {
      color: rgba(255, 255, 255, 0.7)
      text-align: center
      margin: 15px 0px 25px
    }
  }
</style>

<script>
import modal from 'controls/modal'
import btn from 'controls/button'
import utils from 'utils/utils'

export default {
  components: {
    modal, btn
  },
  props: {
    error: {
      default: undefined,
      type: [String, Object]
    },
    title: {
      type: String,
      default: 'Gallery Error'
    },
  },
  emits: ['copy'],

  data () {
    return {
      errleft: 0,
      timeout: null,
    }
  },

  computed: {
    errtext() {
      let full = this.errfull
      if (this.$state.debug) {
        return full
      }

      const maxLines = 10
      let split = full.split('\n')
      
      if  (split.length > maxLines) {
        split = split.slice(0, maxLines)
        split.push('...')
      }
    
      return split.join('\n')
    },

    errfull() {
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
            // error.answer.result.raw_data = '--excluded--'
          }

          if (error.answer.result.output && error.answer.result.output.length > maxLen) {
            // error.answer.result.output = error.answer.result.output.substring(0, maxLen) + ' --excluded--'
          }
        }
        return utils.formatJSON(error)
      }
      return formatError(this.error.error)
    }
  },

  mounted () {
    this.errleft = 10
    this.$refs.modal.open()
    this.timeout = setInterval(() => {
      this.errleft -= 1
      if (this.errleft == 0) {
        this.close()
      }
    }, 1000)
  },

  methods: {
    onCopy() {
      utils.copyText(this.errfull)
    },
 
    onOK() {
      this.close()
    },

    close () {
      clearInterval(this.timeout)
      this.$refs.modal.close()
      this.$store.clearError()
    }
  }
}
</script>