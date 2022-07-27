<template>
  <div class="tabs-container">
    <div v-for="tab of tabs"
         :key="tab.id"
         :class="[ modelValue === tab.id ? 'tab-active' : '','tab-item']" 
         @click="$emit('update:modelValue', tab.id)"
    >
      <div class="title">{{ tab.name }}</div>
      <div v-if="modelValue === tab.id" class="bottom-line"></div>
    </div>
    <div class="slot">
      <slot></slot>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .tabs-container {
    display: flex
    flex-direction: row
    align-items: center
    width: 100%
    user-select: none

    .slot {
      margin-left: auto
      display: flex
      align-items: center
    }

    .tab-item {
      color: rgba(255, 255, 255, 0.3)
      font-size: 12px
      font-weight: bold
      cursor: pointer
      white-space: nowrap

      .title {
        padding: 4px 16px
        text-transform: uppercase
      }
    }

    .tab-active {
      color: #fff

      .bottom-line {
        height: 2px
        width: 100%
        box-shadow: 0 0 5px 0 rgba(0, 246, 210, 0.7)
        background-color: #00f6d2
      }
    }
  }
</style>

<script>
// TODO: headless
export default {
  props: {
    tabs: {
      type: Array,
      required: true
    },
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: Number,
      required: true
    }
  },

  emits: [
    'update:modelValue'
  ]
}
</script>
