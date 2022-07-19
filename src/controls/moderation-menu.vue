<template>
  <popupMenu ref="moderationMenu">
    <div class="item" @click="onReject">
      <img src="~assets/eye-crossed.svg"/>
      Reject and hide
    </div>
    <div class="item" @click="onResubmit">
      <img src="~assets/change.svg"/>
      Submit to moderator
    </div>
  </popupMenu>
</template>

<script>
import popupMenu from 'controls/popup-menu'

export default {
  components: {
    popupMenu
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  computed: {
    can_admin() {
      return this.$state.is_moderator
    }
  },

  methods: {
    open(ev) {
      if (this.can_admin && this.item.approved) {
        ev.stopPropagation()
        this.$refs.moderationMenu.open(ev)
      }
    },
    onReject(ev) {
      ev.stopPropagation()
      this.$store.setItemsStatus(this.item.type, [this.item.id], 'rejected')  
    },
    onResubmit(ev) {
      ev.stopPropagation()
      this.$store.setItemsStatus(this.item.type, [this.item.id], 'pending')  
    },
  }
}
</script>
