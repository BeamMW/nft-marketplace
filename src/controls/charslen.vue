<template>
  <div class="info">
    <span>{{ tip_computed }}</span> 
    <span>{{ value }} / {{ max_length }} </span>
  </div>
</template>

<style scoped lang="stylus">
  .info {
    display: flex
    justify-content: space-between
    font-family: 'SFProDisplay', sans-serif
    opacity: 0.7
    font-style: italic
    font-weight: 400
    font-size: 12px
    
    &[readonly] {
      color: rgba(255, 255, 255, 0.35)
    }
  }
</style>
<script>
export default {
  props: {
    max_length:{
      type: Number,
      default: 10,
      required: true
    },
    value: {
      type: Number,
      default: 0,
      required: false
    },
    tip: {
      type: String,
      default: undefined,
      required: false
    },
  },

  computed: {
    tip_computed () {
      if (this.tip === undefined) {
        return `${this.max_length} characters max`
      }
      return this.tip
    }
  }
}
</script>