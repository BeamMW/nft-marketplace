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
  height: 100%

  & > img {
    position: absolute
    z-index: 999
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
      required: true
    },
    top: {
      type: String,
      required: true
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
        'border': this.selected ? '1px solid #00f6d2' : '1px solid transparent',
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
