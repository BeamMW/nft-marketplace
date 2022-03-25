<template>
  <div class="amount">
    <img src="~assets/icon-beam.svg"/>
    <div class="value">
      <span>{{ balance_beam }}</span>
      <span>BEAM</span>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.amount {
  height: 26px
  display: flex
  align-items: center
  margin-top: 6px

  & .value {
    display: flex
    font-size: 20px
    padding: 0 0 2px 0
    font-weight: bold
    color: #fff

    & > * {
      margin-left: 6px
    }
  }
}
</style>

<script>
import {common} from '../utils/consts'

export default {
  computed: {
    balance_beam() {
      return this.$state.balance_beam / common.GROTHS_IN_BEAM
    }
  }
}
</script>