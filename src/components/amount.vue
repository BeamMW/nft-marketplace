<template>
  <div class="amount-container" :style="text_style">
    <img src="~assets/icon-beam.svg" :style="icon_style"/>
    <div>
      <div>{{ value }} BEAM</div>
      <div v-if="info">{{ info }}</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.amount-container {
  display: flex
  align-items: center
  font-family: 'SFProDisplay', sans-serif
  font-weight: bolder

  & > *:not(:first-child) {
    margin-left: 8px
  }
}
</style>

<script>
import utils from 'utils/utils.js'

export default {
  props: {
    amount: {
      type: Number,
      required: true,
      default: 0
    },
    size: {
      type: String,
      default: '18px'
    },
    info: {
      type: String,
      default: ''
    },
  },

  computed: {
    value () {
      return utils.formatAmount(this.amount)
    },
    text_style () {
      return {
        'font-size': this.size
      }
    },
    icon_style () {
      return {
        'width': `${parseInt(this.size.substring(0, this.size.length-2)) + 6}px`
      }
    }
  }
}
</script>