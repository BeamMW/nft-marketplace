<template>
  <div :class="{
    'info': true,
    'readonly': readonly,
  }"
  >
    <span>{{ max_length }} characters max</span> 
    <span v-if="show_counter">{{ value }} / {{ max_length }} </span>
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
    
    &.readonly {
      color: rgba(255, 255, 255, 0.35)
    }
  }
</style>
<script>
export default {
  props: {
    readonly: {
      type: Boolean,
      default: false,
      required: false
    },
    max_length:{
      type: Number,
      default: 10,
      required: true
    },
    show_counter:{
      type: Boolean,
      default: true,
      required: false
    },
    value: {
      type: Number,
      default: 0,
      required: false
    }

  }
}
</script>