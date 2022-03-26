<template>
  <div class="amount-container">
    <img src="~assets/icon-beam.svg"/>
    <div>{{ value }} BEAM</div>
  </div>
</template>

<style lang="stylus" scoped>
.amount-container {
  display: flex
  font-size: 18px
  font-family: 'SFProDisplay', sans-serif
  font-weight: bolder

  & > *:not(:first-child) {
    margin-left: 8px
  }
}
</style>

<script>
import utils from '../utils/utils.js'

export default {
  props: {
    amount: {
      type: Number,
      required: true,
      default: 0
    }
  },

  computed: {
    value () {
      return utils.formatAmount(this.amount)
    }
  }
}
</script>