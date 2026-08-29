<template>
  <div class="amount-container" :style="text_style">
    <assetIcon :aid="aid" :size="icon_size"/>
    <div>
      <div>{{ value }} {{ unit_name }}</div>
      <div v-if="info">{{ info }}</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.amount-container {
  display: flex
  font-family: 'SFProDisplay', sans-serif
  font-weight: bolder

  & > *:not(:first-child) {
    margin-left: 8px
  }
}
</style>

<script>
import utils from 'utils/utils'
import assetsStore from 'stores/assets'
import assetIcon from 'controls/asset-icon'

export default {
  components: {
    assetIcon
  },

  props: {
    amount: {
      type: Number,
      required: true,
      default: 0
    },
    aid: {
      type: Number,
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
    compact: {
      type: Boolean,
      default: false
    }
  },

  computed: {
    value () {
      return utils.formatAmountFixed(this.amount, 3)
    },
    unit_name () {
      return assetsStore.get(this.aid).unit_name
    },
    icon_size () {
      return parseInt(this.size.substring(0, this.size.length - 2)) + 6
    },
    text_style () {
      return {
        'font-size': this.size
      }
    },
    icon_style () {
      return {
        'width': `${this.icon_size}px`
      }
    }
  }
}
</script>
