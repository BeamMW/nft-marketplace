<template>
  <Teleport to="#modals">
    <div v-if="show" class="modal-dialog">
      <div class="modal-content" :style="style">
        <slot/>
      </div>
    </div>
  </Teleport>
</template>

<style lang="stylus" scoped>
.modal-dialog {
  width: 100%
  height: 100%
  overflow: hidden
  position: fixed
  top: 0px
  display: flex
  justify-content: center
  align-items: center
  z-index: 100
  background-color: transparent
    
  .modal-content {
    padding: 40px
    border-radius: 10px
  }
}
</style>

<script>
import utils from '../utils/utils.js'

export default {
  data () {
    return {
      show: false,
    }
  },

  computed: {
    style() {
      return {
        'background-color': utils.getStyles().background_popup,
      }
    },

    id () {
      return Math.random()
    }
  },

  methods: {
    close () {
      this.show = false
      document.getElementById('app-container').style.opacity = 1
    },

    open () {
      this.show = true
      document.getElementById('app-container').style.opacity = 0.3
    }
  }
}
</script>