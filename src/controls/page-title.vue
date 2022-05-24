<template>
  <div class="page-title-container">
    <backBtn/>
    <div class="title">{{ title }}</div>
    <div class="title-slot">
      <slot></slot>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .page-title-container {
    min-height: 45px 
    display: flex
    flex-flow: row
    justify-content: center
    align-items: center
    user-select: none

    & > * {
      flex-grow: 1
      flex-basis: 0
    }

    & > .title {
      display: flex
      justify-content: center
      align-items: center
      font-family: 'SFProDisplay', sans-serif
      font-weight: 700
      font-size: 14px
      line-height: 17px
      text-transform: uppercase
      letter-spacing: 3.1px
      color: #fff
    }

    & > .title-slot {
      display: flex
      justify-content: flex-end
      align-items: center

      & > :slotted(*) {
        margin-left: 12px
        margin-top: 7px

        &:last-child {
          margin-right: 6px
        }
      }
    }
  }
</style>

<script>
import backBtn from 'controls/btn-back'

export default {
  components: {
    backBtn
  },

  props: {
    title: {
      type: String,
      required: true
    }
  }
}
</script>
