<template>
  <div v-show="show" class="popup-menu" :style="style" tabindex="0">
    <slot></slot>
  </div>
</template>

<style scoped lang="stylus">
  .popup-menu {
    border-radius: 10px
    z-index:  999
    position: fixed
    outline:  none
    padding:  10px 20px

    :slotted(.item) {
      color: #fff
      padding: 6px 0
      cursor: pointer
      display: flex
      align-items: center
      line-height: normal !important

      &:hover {
        color: #00f6d2
      }

      &>img {
        margin-right: 10px
      }
    }
  }
</style>

<script>
import {nextTick} from 'vue'
import utils from 'utils/utils.js'

export default {
  data() {
    return {
      left: 0,
      top: 0,
      show: false,
    }
  },

  computed: {
    style() {
      return {
        top: this.top + 'px',
        left: this.left + 'px',
        'background-color': utils.getStyles().background_popup,
      }
    }
  },

  methods: { 
    close () {
      this.show = false
      this.left = 0
      this.top  = 0
    },

    open(ev) {
      this.show = true
      
      nextTick(() => {
        const elementHeight = this.$el.offsetHeight
        const largestHeight = window.innerHeight - elementHeight
        const elementWidth  = this.$el.offsetWidth
        const largestWidth  = window.innerWidth - elementWidth

        let top = ev.clientY
        if (top > largestHeight) {
          top = largestHeight
        }

        let left = ev.clientX
        if (left > largestWidth) {
          left = largestWidth
        }

        this.left = left
        this.top = top

        let downAway = evc => {
          if (!this.$el.contains(evc.target)) {
            document.removeEventListener('mousedown', downAway, true)
            this.close()
          }
        }

        let clickAway = evc => {
          if (ev != evc) {
            document.removeEventListener('click', clickAway, true)
            this.close()
          }
        }
        
        let scrollAway = evc => {
          document.removeEventListener('scroll', scrollAway, scroll)
          this.close()
        }

        document.addEventListener('mousedown', downAway, true)
        document.addEventListener('click', clickAway, true)
        document.addEventListener('scroll', scrollAway, true)
      })
    }
  },
}
</script>