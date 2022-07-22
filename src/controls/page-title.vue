<template>
  <div class="page-title-container">
    <div class="title">{{ title }}</div>
    <div class="overlay">
      <backBtn/>
      <div/>
      <div class="title-slot">
        <slot></slot>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .page-title-container {
    min-height: 45px 
    user-select: none
    display: flex
    justify-content: center
    align-items: center
    position: relative

    .overlay {
      width: 100%
      min-height: 100%
      position: absolute
      left: 0
      top: 0
      display: flex
      flex-flow: row
      justify-content: center
      align-items: center

      & > * {
        flex-grow: 1
        flex-basis: 0
      }

      & > .title-slot {
        display: flex
        justify-content: flex-end
        align-items: center

        & :slotted(*) {
          margin-left: 12px
          margin-top: 5px

          &:last-child {
            margin-right: 8px
          }
        }
      }
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
      flex: 1
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
      required: false,
      default: ''
    }
  }
}
</script>
