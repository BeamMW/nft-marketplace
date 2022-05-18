<template>
  <div class="select-container" :style="style">
    <img :src="`../assets/${icon}.svg`" alt="icon" :style="icon_style">
    <slot></slot>
  </div>
</template>

<style scoped lang="stylus">
.select-container {
  position: relative
  cursor: pointer
  width: 100%

  & > img {
    position: absolute
    z-index: 999
    user-select: none
    opacity: 0.9
  }
}
</style>

<script>

export default {
  props: {
    border_radius: {
      type: String,
      default: '10px',
    },
    left: {
      type: String,
      required: false,
      default: '20px'
    },
    top: {
      type: String,
      required: false,
      default: '18px'
    },
    selected: {
      type: Boolean,
      default: false
    }
  },

  computed: {
    style() {
      return {
        'border-radius': this.border_radius,
        'border': this.selected ? '1px solid rgba(0, 246, 210, 0.6)' : '1px solid transparent',
      }
    },

    icon_style() {
      return {
        'top': this.top,
        'left': this.left,
      }
    },

    icon() {
      return this.selected ? 'icon-selected' : 'icon-unselected'
    }
  }
}
</script>
