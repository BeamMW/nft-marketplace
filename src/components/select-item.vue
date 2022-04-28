<template>
  <div class="select-container" :style="style" @click="select">
    <img :src="`../assets/${getImg()}.svg`" alt="icon" :style="placeCycle">
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
    place: {
      type: Object,
      default() {
        return {
          right: '8px',
          top: '8px',
          left: 'initial',
        }
      },
    },
  },

  emits: [
    'select-item'
  ],

  data () {
    return {
      selected: false,
      img: 'icon-unselected',
    }
  },

  computed: {
    style() {
      return {
        'border-radius': this.border_radius,
        'border': this.selected ? '1px solid #00f6d2' : '1px solid transparent',
      }
    },

    is_admin() {
      return this.$state.admin
    },

    placeCycle() {
      return {
        'top': this.place.top,
        'left': this.is_admin ? '50px': this.place.left,
        'right': this.is_admin ? '50px': this.place.right,
      }
    },

  },

  methods: {
    select () {
      this.selected = !this.selected
    },
    getImg () {
      return this.selected ? 'icon-selected' : 'icon-unselected'
    },
  }
}
</script>
