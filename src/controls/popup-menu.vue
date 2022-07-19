<template>
  <Teleport to="#modals">
    <div v-show="show" ref="menu" class="popup-menu" :style="style" tabindex="0">
      <slot></slot>
    </div>
  </Teleport>
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
      white-space: nowrap
      user-select: none

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
import utils from 'utils/utils'

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
        let menu = this.$refs.menu
        let menuRect  = menu.getBoundingClientRect()

        const largestHeight = window.innerHeight 
        const largestWidth  = window.innerWidth

        let top = ev.clientY
        if (top + menuRect.height > largestHeight) {
          top = largestHeight - menuRect.height
        }

        let left = ev.clientX
        if (left + menuRect.width > largestWidth) {
          left = largestWidth - menuRect.width
        }

        this.left = left
        this.top = top

        let removeListeners 

        let downAway = evc => {
          if (evc.which === 3 && !this.$refs.menu.contains(evc.target)) {
            evc.stopPropagation()
            removeListeners()
            this.close()
          }
        }

        let clickAway = evc => {
          console.log('click away')
          if (!this.$refs.menu.contains(evc.target)) {
            evc.stopPropagation()
          }
          removeListeners()
          this.close()
        }
        
        let scrollAway = evc => {
          removeListeners()
          this.close()
        }

        removeListeners = () => {
          document.removeEventListener('mousedown', downAway, true)
          document.removeEventListener('click', clickAway, true)
          document.removeEventListener('scroll', scrollAway, true)
        }

        document.addEventListener('mousedown', downAway, true)
        document.addEventListener('click', clickAway, true)
        document.addEventListener('scroll', scrollAway, true)
      })
    }
  },
}
</script>