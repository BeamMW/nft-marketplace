<template>
  <button ref="button"         
          :class="{
            'button': true,
            'reverse': reverse,
            'transparent': color == 'transparent',
            'disabled': disabled,
            'no-hover': !hover
          }"
          :disabled="disabled"
          :style="button_style"
          @mouseover="onMouseOver"
  >
    <slot></slot>
    <span class="text" :style="text_style">{{ text }}</span>
    <span v-if="tooltip" ref="tooltip" class="tooltip" :style="tip_style">{{ tooltip }}</span>
  </button>
</template>

<style scoped lang="stylus">
.button {
  font-family: 'SFProDisplay', sans-serif
  white-space: nowrap
  box-sizing: border-box
  display: flex
  align-items: center
  justify-content: center
  font-size: 14px
  border: none
  cursor: pointer
  background-color: rgba(255, 255, 255, 0.1)
  user-select: none
  position: relative

  &:not(.disabled):hover {
    box-shadow: 0 0 8px white
  }

  &.disabled {
    opacity: 0.6
    cursor: auto

    & :slotted(img) {
      opacity: 0.35
    }
  }

  &:focus {
    outline: none
  }

  &.reverse {
    flex-direction: row-reverse
  }

  &.transparent {
    border-radius: 0

    &:not(.disabled):hover {
      box-shadow: none
    }
  }

  &.no-hover:hover {
    box-shadow: none
  }

  & > span {
    padding-bottom: 1px
  }

  & > .tooltip {
    box-sizing: border-box
    visibility: hidden
    backdrop-filter: blur(10px)
    color: #fff
    border-radius: 6px
    padding: 11px 11px
    position: fixed
    z-index: 1
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
    opacity: 0.85
  }

  &:hover .tooltip {
    visibility: visible
  }
}
</style>

<script>
import utils from 'utils/utils'

export default {
  props: {
    color: {  // magenta, blue, green, transparent, dark-blue
      type: String,
      default: undefined
    },
    icon: { // icon source
      type: String,
      default: undefined,
    },
    reverse: { // to put the icon to the right of the text
      type: Boolean,
      default: false,
    },
    text: {
      type: String,
      default: undefined,
    },
    text_color: { // available colors from data.text_colors
      type: String,
      default: undefined,
    },
    text_bold: {
      type: Boolean,
      default: true,
    },
    disabled: {
      type: Boolean,
      default: false,
    },
    hover: {
      type: Boolean,
      default: true
    },
    gap: {
      type: String,
      default: '9px'
    },
    padding: {
      type: String,
      default: '11px 25px 11px 22px'
    },
    height: {
      type: String,
      default: '38px'
    },
    width: {
      type: String,
      default: 'fit-content'
    },
    radius: {
      type: String,
      default: '50px'
    },
    tooltip: {
      type: String,
      default: ''
    }
  },

  data() {
    return {
      text_colors: {
        magenta: '#da68f5',
        blue: '#0bccf7',
        green: '#00f6d2',
        dark_blue: '#032e49',
        white: '#ffffff',
      },
      button_colors: {
        magenta: '#da68f5',
        blue: '#0bccf7',
        green: '#00f6d2',
        transparent: 'transparent',
        carnation:'#f9605b',
        red: '#ff746b',
      },
      tip_left: -1,
      tip_top: -1
    }
  },

  computed: {
    has_icon () {
      return !!this.$slots.default
    },

    text_style() {
      if (!this.text) {
        return
      }

      let tcolor = this.text_color 
      if (!tcolor) {
        tcolor = this.color == 'transparent' || this.color == undefined ? 'white' : 'dark_blue'
      }
      
      if (this.text_colors[tcolor]) {
        tcolor = this.text_colors[tcolor]
      }

      let res = {
        'color': tcolor,
        'font-weight': this.text_bold ? 'bold' : 'normal'
      }
  
      if (this.has_icon) {
        let tgap_pos = this.reverse ? 'right' : 'left'
        res[`margin-${tgap_pos}`] = this.gap
      }

      if (this.disabled) {
        res['opacity'] = 0.3
      }

      return res
    },

    button_style() {
      return {
        'background-color': this.button_colors[this.color] || this.color,
        'padding': this.padding,
        'height': this.height,
        'width': this.width,
        'border-radius': this.radius
      }
    },

    tip_style() {
      let res = {
        background: utils.getStyles().background_popup
      }

      if (this.tip_left != -1) {
        res['left'] = [this.tip_left, 'px'].join('')
      }

      if (this.tip_top != -1) {
        res['top'] = [this.tip_top, 'px'].join('')
      }

      return res
    }
  },

  methods: {
    onMouseOver() {
      let tooltip = this.$refs.tooltip
      
      if (!tooltip) {
        return
      }

      let tipRect  = tooltip.getBoundingClientRect()
      let tipRight = tipRect.right
      let windowWidth  = window.innerWidth
      
      if (tipRight > windowWidth) {
        this.tip_left = windowWidth - tipRect.width
      }

      let button = this.$refs.button
      let btnRect = button.getBoundingClientRect()
      let btnBottom = btnRect.bottom
      this.tip_top  = btnBottom + 12
    }
  }
}
</script>
