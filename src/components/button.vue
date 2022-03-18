<template>
  <button class="button"
          :class="[transparency, color, icon_position, disabled_button, hover_effect]"
          :style="left_margin"
          :disabled="disabled"
  >
    <img v-if="icon" :src="icon"/>
    <span :style="gap_inner"></span>
    <span class="text" :style="color_text">{{ text }}</span>
  </button>
</template>

<style scoped lang="stylus">
$font = 'SFProDisplay', sans-serif
$magenta = #da68f5
$blue = #0bccf7
$green = #00f6d2
$semiTransparent = rgba(255, 255, 255, 0.1)
$darkBlue = #032e49
$white = #ffffff
$marginLeft = 30px

.button {
  font-family: $font
  min-height: 38px
  min-width: min-content
  padding: 11px 25px
  border-radius: 50px
  display: flex
  flex-wrap: wrap
  align-items: center
  color: $darkBlue
  font-size: 14px
  font-weight: bold
  border: none
  cursor: pointer

  &:not(.disabled):hover {
    box-shadow: 0 0 8px white
  }

  &:focus {
    outline: none
  }

  & .text {
    line-height: 1
    font-weight: bold
    align-self: center
  }
}

.disabled {
  opacity: 0.3
  cursor: auto !important
}

.transparent {
  min-height: min-content
  border-radius: 0
  background-color: transparent
  padding: 0

  &:not(.disabled):hover {
    box-shadow: none
  }
}

.semiTransparent {
  background-color: $semiTransparent
}

.trailingIcon {
  flex-direction: row-reverse
}

.singleIcon {
  img {
    margin: 0
  }
}

.marginLeft {
  margin-left: $marginLeft
}

.hoverDisabled {
  &:not(.disabled):hover {
    box-shadow: none
  }
}

.magenta {
  background-color: $magenta
}

.blue {
  background-color: $blue
}

.green {
  background-color: $green
}

.black {
  background-color: $black
  color: #fff
}
</style>

<script>
export default {
  props: {
    transparent: {
      type: Boolean,
      default: false,
    },
    semi_transparent: {
      type: Boolean,
      default: false,
    },
    // magenta, blue, green, semiTransparent, darkBlue
    color: {
      type: String,
      default: String,
    },
    // icon source
    icon: {
      type: String,
      default: String,
    },
    // left, right
    icon_right: {
      type: Boolean,
      default: false,
    },
    text: {
      type: String,
      default: String,
    },
    // available colors from data.text_colors
    text_color: {
      type: String,
      default: String,
    },
    margin_left: {
      type: [Number, Boolean],
      default: false
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
      type: Number,
      default: Number
    }
  },

  data() {
    return {
      text_colors: {
        magenta: '#da68f5',
        blue: '#0bccf7',
        green: '#00f6d2',
        semi_transparent: 'rgba(255, 255, 255, 0.1)',
        dark_blue: '#032e49',
        white: '#ffffff',
      },
      styles: {
        default_gap: '50',
        default_margin: '30'
      }
    }
  },

  computed: {
    icon_position() {
      if (this.icon && !this.text) {
        return 'singleIcon'
      }
      
      if (this.icon_right === true) {
        return 'trailingIcon'
      }

      return ''
    },
    transparency() {
      if (this.transparent === true) {
        return 'transparent'
      }

      if (this.semi_transparent === true) {
        return 'semiTransparent'
      }

      return ''
    },
    color_text() {
      if (this.text && this.text_color) {
        return {
          'color': this.text_colors[this.text_color],
        }
      }

      return ''
    },
    disabled_button() {
      if (this.disabled) {
        return 'disabled'
      }

      return ''
    },
    gap_inner() {
      if (this.gap) {
        return {'width': `${this.gap}px`}
      }

      if (this.icon && this.text) {
        return {'width': `${this.styles.default_gap}px`}
      }

      return ''
    },
    left_margin() {
      if (this.margin_left === true) {
        return {'margin-left': `${this.styles.default_margin}px`}
      }

      if (this.margin_left) {
        return {'margin-left': `${this.margin_left}px`}
      }

      return ''
    },
    hover_effect() {
      if (this.hover === false) {
        return 'hoverDisabled'
      }

      return ''
    }
  },
}
</script>
