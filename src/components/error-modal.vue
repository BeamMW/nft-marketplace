<template>
  <modal ref="modal">
    <div class="content">
      <div class="title">
        {{ title }}
      </div>
      <div class="description">
        <p>Something went wrong. The page will be automatically restarted.</p>
        <p>If you came across this problem again, copy the following text and send it to support, please.</p>
      </div>
      <div class="error">
        <div>
          <div class="data">
            <span>{{ text }}</span>
            <btn class="icon-copy" color="transparent" padding="11px 0 11px 10px" @click="onCopy">
              <img src="~assets/icon-copy.svg"/>
            </btn>
          </div>
          <div class="restart">Restarting in {{ errleft }}</div>
        </div>
        <btn color="carnation" padding="10px 30px 10px 30px" text="ok" @click="close">
          <img src="~assets/icon-ok.svg"/> 
        </btn>
      </div>
    </div>
  </modal>
</template>

<style scoped lang="stylus">
  .content {
    display: flex
    flex-direction: column
    align-items: center
    font-family: 'SF Pro Display'

    .title {
      font-size: 18px
      font-weight: 700
      line-height: 21px
      text-align: center
      color: #fff
    }

    .description {
      text-align: center
      font-size: 14px
      line-height: 17px

      & > p {
        margin-bottom: 0px
      }
    }

    .error {
      display:         flex
      flex-direction:  column
      align-items:     center
      justify-content: center
      height:          100%
      width:           100%
      overflow:        hidden    
      filter:          unset
      padding-bottom:  10px

      & > div {
        width: 70%
        
        & > .data {
          font-family: 'SFProDisplay', sans-serif
          font-weight: normal
          white-space: pre-wrap
          display: flex
          justify-content: center
          flex-direction: row
          align-items: center
          color: rgba(255, 90, 90, 1)

          .icon-copy {
            cursor: pointer
          }
        }
      }
      .restart {
        color: #fff
        text-align: center
        margin: 24px 0px 41px
      }
    }
  }
</style>

<script>
import modal from './modal.vue'
import btn from './button.vue'

export default {
  components: {
    modal, btn
  },
  props: {
    text: {
      default: undefined,
      type: [String, Object]
    },
    title: {
      type: String,
      default: 'Error'
    },
  },
  emits: ['copy'],

  data () {
    return {
      errleft: 0,
      timeout: null,
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
    this.$refs.modal.open()

  },

  methods: {
    onCopy() {
      this.$emit('copy')
    },
 
    close() {
      clearInterval(this.timeout)
      this.$store.clearError()
      this.$refs.modal.close()
    }
  }
}
</script>