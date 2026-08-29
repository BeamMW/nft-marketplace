<template>
  <span class="asset-icon" :style="style">
    <img v-if="is_beam" src="~assets/beam.svg" :width="size" :height="size"/>
    <!-- inlined: currentColor tinting does not work through <img> -->
    <svg v-else :width="size" :height="size" viewBox="0 0 26 26" xmlns="http://www.w3.org/2000/svg">
      <defs>
        <radialGradient :id="grad_id" cx="50%" cy="50%" r="50%" fx="50%" fy="50%">
          <stop offset="0%" stop-color="transparent"/>
          <stop offset="100%" stop-color="#042548"/>
        </radialGradient>
      </defs>
      <g fill="none" fill-rule="evenodd">
        <circle cx="13" cy="13" r="11.636" fill="currentColor"/>
        <circle cx="13" cy="13" r="11.636" :fill="`url(#${grad_id})`" stroke="currentColor" stroke-width="2"/>
        <g fill="#fff" transform="translate(7.15 7.54)">
          <path d="M5.44 0l5.438 8.962H0L5.438 0v2.817L2.664 7.466l2.775-.001 2.776.001L5.44 2.817V0zM3.72 6.923l1.72-2.952 1.72 2.952-1.72-.003-1.72.003z"/>
        </g>
      </g>
    </svg>
  </span>
</template>

<style scoped lang="stylus">
.asset-icon {
  display: inline-flex
  align-items: center
  justify-content: center
  vertical-align: middle
  flex-shrink: 0
}
</style>

<script>
import {assetColor, genUniqueID} from 'utils/consts'

export default {
  props: {
    aid: {
      type: Number,
      default: 0
    },
    size: {
      type: [Number, String],
      default: 20
    }
  },

  data () {
    // gradient ids must be unique per instance or they collide in the document
    return {
      grad_id: `asset-grad-${genUniqueID()}`
    }
  },

  computed: {
    is_beam () {
      return !this.aid
    },

    style () {
      return {
        color: assetColor(this.aid),
        width: `${parseInt(this.size)}px`,
        height: `${parseInt(this.size)}px`
      }
    }
  }
}
</script>
