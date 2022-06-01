<template>
  <div v-if="!approved"
       :class="{'top-message': true, 'pending': pending, 'rejected': rejected}" 
       :style="style"
  >
    {{ rejected ? (short ? 'Rejected' : 'Rejected by moderator') : (short ? 'Pending' : 'Pending for approval') }}
  </div>
</template>

<style lang="stylus" scoped>
  & > .top-message {
    position: absolute
    left: 0
    top: 0
    width: 100%
    display: flex
    justify-content: center
    align-items: center
    border-top-left-radius: 10px
    border-top-right-radius: 10px
    backdrop-filter: blur(4px)

    &.pending {
      background-color: rgba(23, 46, 43, 0.85)
      color: rgba(255, 255, 255, 0.75)
    }

    &.rejected {
      background-color: rgba(255, 116, 107, 0.85)
      color: rgba(255, 255, 255, 0.9)
    }
  }
</style>

<script>
export default {
  props: {
    item: {
      type: Object,
      required: true,
    },
    short: {
      type: Boolean,
      default: false,
    }
  },

  computed: {
    approved() {
      return this.item.approved
    },
    pending () {
      return this.item.pending
    },
    rejected () {
      return this.item.rejected
    },
    style() {
      let res = {
        'height': this.short ? '25px' : '40px',
        'font-size': this.short ? '14px' : '15px'
      }
      return res
    }
  }
}
</script>
