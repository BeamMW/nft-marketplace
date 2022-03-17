<template>
  <button class="button"
          :class="[transparency, color, side_icon, disabled_button]"
          :disabled="disabled"
  >
    <img v-if="icon" :src="icon"/>
    <span class="text" :style="color_text">{{ text }}</span>
  </button>
</template>

<style scoped lang="stylus">
$magenta = #da68f5;
$blue = #0bccf7;
$green = #00f6d2;
$semiTransparent = rgba(255, 255, 255, 0.1);
$darkBlue = #032e49;
$white = #ffffff;
$marginIcon = 8px;

.button {
  height: 38px;
  padding: 11px 25px;
  border-radius: 50px;
  display: flex;
  flex-wrap: wrap;
  align-items: center;
  color: $darkBlue;
  font-size: 14px;
  font-weight: bold;
  cursor: pointer;
  border: none;

  &:not(.disabled):hover {
    box-shadow: 0 0 8px white;
  }

  &:focus {
    outline: none;
  }

  & .text {
    line-height: 1;
  }
}

.disabled {
  opacity: 0.3;
  cursor: auto !important;
}

.transparent {
  height: min-content;
  background: transparent;
  padding: 0;

  &:not(.disabled):hover {
    box-shadow: none;
  }
}

.semiTransparent {
  background: $semiTransparent;
}

.leadingIcon {
  img {
    margin-right: $marginIcon;
  }
}

.trailingIcon {
  flex-direction: row-reverse;

  img {
    margin-left: $marginIcon;
  }
}

.singleIcon {
  img {
    margin: 0;
  }
}

.magenta {
  background-color: $magenta;
}

.blue {
  background-color: $blue;
}

.green {
  background-color: $green;
}

.black {
  background-color: $black;
  color: #fff;
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
    icon_position: {
      type: String,
      default: 'left',
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
    disabled: {
      type: Boolean,
      default: false,
    },
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
    }
  },

  computed: {
    side_icon() {
      if (this.icon_position === 'right') {
        return 'trailingIcon'
      }

      if (this.icon && !this.text) {
        return 'singleIcon'
      }

      return 'leadingIcon'
    },
    transparency() {
      if (this.transparent) {
        return 'transparent'
      }

      if (this.semi_transparent) {
        return 'semiTransparent'
      }

      return ''
    },
    color_text() {
      if (this.text && this.text_color) {
        return {
          color: this.text_colors[this.text_color],
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
  },
}
</script>

