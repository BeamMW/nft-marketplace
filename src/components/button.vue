<template>
  <button :class="{
            'button': true,
            'reverse': reverse,
            'transparent': color == 'transparent',
            'disabled': disabled,
            'no-hover': !hover
          }"         
          :disabled="disabled"
          :style="button_style"
  >
    <slot></slot>
    <span class="text" :style="text_style">{{ text }}</span>
  </button>
</template>

<style scoped lang="stylus">
.button {
  font-family: 'SFProDisplay', sans-serif
  min-height: 38px
  min-width:  min-content
  padding: 11px 22px
  border-radius: 50px
  display: flex
  flex-wrap: wrap
  align-items: center
  color: $darkBlue
  font-size: 14px
  font-weight: bold
  border: none
  cursor: pointer
  background-color: rgba(255, 255, 255, 0.1)

  &:not(.disabled):hover {
    box-shadow: 0 0 8px white
  }

  &.disabled {
    opacity: 0.6
    cursor: auto !important
  }

  &:focus {
    outline: none
  }

  & .text {
    font-weight: bold
  }

  &.reverse {
    flex-direction: row-reverse
  }

  &.transparent {
    min-height: min-content
    border-radius: 0
    padding: 0

    &:not(.disabled):hover {
      box-shadow: none
    }
  }

  &.no-hover:hover {
    box-shadow: none
  }
}
</style>

<script>
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
      default: 5
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
      }
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

      let tcolor = this.text_color || (this.color == 'transparent' || this.color == undefined ? 'white' : 'dark-blue')
      let res = {
        'color': this.text_colors[tcolor]
      }
  
      if (this.has_icon) {
        let tgap_pos = this.reverse ? 'right' : 'left'
        res[`margin-${tgap_pos}`] = `${this.gap}px`
      }

      return res
    },

    button_style() {
      return {
        'background-color': this.button_colors[this.color]
      }
    }
  },
}
</script>
