<template>
  <span>
    <slot class="slot"></slot>
    {{ value }}
  </span>
</template>

<style lang="stylus" scoped>
span {
  display: flex
}
  
.slot {
  color: red
}

</style>

<script>
export default {
  props: {
    value: {
      type: String,
      required: true,
    },
    image: {
      type: String,
      default: undefined
    },
    red: {
      type: Boolean,
      default: false
    }
  },

  computed: {
    is_image() {
      return this.image ? true : false
    },
    is_red() {
      const res = {'color': 'red'}
      
      return res
    }
  }
}
</script>

