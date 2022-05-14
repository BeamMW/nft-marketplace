<template>
  <div :class="{'top-message': true, 'pending': pending, 'rejected': rejected}">
    {{ rejected ? 'Rejected by moderator' : 'Pending for approval' }}
  </div>
</template>

<style lang="stylus" scoped>
  & > .top-message {
    position: absolute
    left: 0
    top: 0
    font-size: 15px
    width: 100%
    height: 40px
    display: flex
    justify-content: center
    align-items: center
    padding-bottom: 5px
    border-top-left-radius: 10px
    border-top-right-radius: 10px
    backdrop-filter: blur(4px)

    &.pending {
      background-color: rgba(23, 46, 43, 0.8)
      color: rgba(255, 255, 255, 0.75)
    }

    &.rejected {
      background-color: rgba(255, 116, 107, 0.8)
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
    }
  },

  computed: {
    pending () {
      return this.item.status === 'pending'
    },
    rejected () {
      return this.item.status === 'rejected'
    }
  }
}
</script>
