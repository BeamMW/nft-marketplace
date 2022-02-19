<template>
    <div class="error">
        <div>
            <pre>{{errtext}}</pre>
            <span class="restart">Restarting in {{errleft}}</span>
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
    word-break:      break-word
    color:           #C55B61 

    & pre {
        font-family: 'SFProDisplay', sans-serif
        font-weight: normal
        white-space: pre-wrap
    }
}
</style>

<script>
import utils from '../utils/utils.js'
export default {
    props: {
        error: {
            default: undefined
        },
        context: {
            type: String,
            default: "Error occured"
        }
    },

    data () {
        return {
            errleft: 0
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
    },

    computed: {
        errtext () {
            // strip off some long unncessary binary stuff that might occur here
            if (this.error.answer && this.error.answer.result) {
                if( this.error.answer.result.raw_data) {
                    this.error.answer.result.raw_data = "--excluded--"
                }

                const maxLen = 50
                if (this.error.answer.result.output && this.error.answer.result.output.length > maxLen) {
                    this.error.answer.result.output = this.error.answer.result.output.substring(0, maxLen) + " --excluded--"
                }
            }
            
            let serr = utils.formatJSON(this.error)
            return [this.context, serr].join('\n')
        }
    }
}
</script>