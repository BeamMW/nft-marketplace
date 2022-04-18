<template>
  <div class="select-container" :style="style" @click="select">
    <img :src="`../assets/${getImg()}.svg`" alt="icon">
    <slot></slot>
  </div>
</template>

<style scoped lang="stylus">
.select-container {
  position: relative
  cursor: pointer

  & > img {
    position: absolute
    right: 8px
    top: 8px
  }
}
</style>

<script>

export default {
  props: {
    selected:{
      type: Boolean,
    },
    border_radius: {
      type: String,
      default: '10px',
    },
  },
  emits: ['select-item'],
  data: function () {
    return {
      img: 'icon-unselected',
    }
  },
  computed: {
    style() {
      return {
        'border-radius': this.border_radius,
        'border': this.selected ? '1px solid #00f6d2' : '',
      }
    },
  },

  methods: {
    select: function () {
      this.$emit('select-item')
    },
    getImg() {
      return this.selected ? 'icon-selected' : 'icon-unselected'
    },

  }
}
</script>
