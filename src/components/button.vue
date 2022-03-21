<template>
  <button class="button"
          :class="[transparency, color, icon_position, disabled_button, hover_effect]"
          :disabled="disabled"
  >
    <slot></slot>
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
    color: {  // magenta, blue, green, semiTransparent, darkBlue
      type: String,
      default: undefined,
    },
    icon: { // icon source
      type: String,
      default: undefined,
    },
    icon_right: { // to put the icon to the right of the text
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
    margin_left: { // button's left margin
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
      default: undefined
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
        default_gap: '5',
      }
    }
  },

  computed: {
    has_icon () {
      return !!this.$slots.default
    },
    icon_position() {
      if (this.has_icon && !this.text) {
        return 'singleIcon'
      }
      
      if (this.icon_right === true) {
        return 'trailingIcon'
      }

      return undefined
    },
    transparency() {
      if (this.transparent === true) {
        return 'transparent'
      }

      if (this.semi_transparent === true) {
        return 'semiTransparent'
      }

      return undefined
    },
    color_text() {
      if (this.text && this.text_color) {
        return {
          'color': this.text_colors[this.text_color],
        }
      }

      return undefined
    },
    disabled_button() {
      if (this.disabled) {
        return 'disabled'
      }

      return undefined
    },
    gap_inner() {
      if (typeof this.gap === 'number') {
        return {'width': `${this.gap}px`}
      }

      if (this.has_icon && this.text) {
        return {'width': `${this.styles.default_gap}px`}
      }

      return undefined
    },
    hover_effect() {
      if (this.hover === false) {
        return 'hoverDisabled'
      }
      return undefined
    }
  },
}
</script>
