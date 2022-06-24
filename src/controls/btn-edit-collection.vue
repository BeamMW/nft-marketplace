<template>
  <messageModal ref="messageModal"/>
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
    <img width="18" src="~assets/pencil.svg">
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
import messageModal from 'components/message-modal'

export default {
  components: {
    btn,
    messageModal
  },

  props: {
    item: {
      type: Object,
      required: true
    }
  },

  methods: {
    onEdit(ev) {
      ev.stopPropagation()
      if (!this.$state.is_desktop) {
        this.$refs.messageModal.open('Edit collection', 'It is not possible edit colleciton in web & mobile wallets.<br>Please download desktop wallet and try from there.')
        return
      }
      if (!this.item.owned || this.item.error) {
        return
      }
      this.item.store.toEditItem(this.item.id)
    }
  }
}
</script>
