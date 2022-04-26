<template>
  <div class="select-container" :style="style" @click="select">
    <img :src="`../assets/${getImg()}.svg`" alt="icon" :class="[is_admin ? 'admin_img' : '','img']">
    <slot></slot>
  </div>
</template>

<style scoped lang="stylus">
.select-container {
  position: relative
  cursor: pointer

  & > img {
    position: absolute
    z-index: 999
  }

  .img {
    right: 8px
    top: 8px
  }

  .admin_img {
    right: 50px
    top: 8px
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
        'border': this.selected ? '1px solid #00f6d2' : '',
      }
    },
    is_admin() {
      return this.$state.is_admin
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
