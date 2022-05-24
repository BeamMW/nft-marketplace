<template>
  <btn v-if="item.owned" 
       padding="0px" 
       radius="6px" 
       width="35px"
       height="35px"
       color="rgba(0, 0, 0, 0.6)"
       class="btn-edit"
       :hover="false"
       @click="onEdit"
  >
    <img width="18" src="~assets/icon-pencil.svg">
  </btn>
</template>

<style lang="stylus" scoped>
  .btn-edit {
    position: absolute
    bottom: 10px
    right: 10px
  }
</style>

<script>
import btn from 'controls/button'

export default {
  components: {
    btn
  },

  props: {
    item: {
      type: Object,
      required: true
    }
  },

  methods: {
    onEdit(ev) {
      if (!this.item.owned || this.item.error) {
        return
      }
      this.item.store.toEditItem(this.item.id)
      ev.stopPropagation()
    }
  }
}
</script>
