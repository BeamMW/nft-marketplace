<template>
  <btn height="34px"
       :text="my_name"
       :color="btn_color"
       tooltip="show profile"
       @click="$store.toMyPage"
  >
    <img :src="btn_image">
  </btn>
</template>

<script>
import artistsStore from 'stores/artists'
import utils from 'utils/utils'
import btn from 'controls/button'

export default {
  components: {
    btn
  },
  computed: {
    self () {
      return artistsStore.self || {}
    },
    is_artist() {
      return artistsStore.is_artist
    },
    btn_color() {
      if (!this.is_artist || this.self.approved) return undefined
      if (this.self.pending) return '#ed69ff'
      return 'red'
    },
    btn_image() {
      if (!this.is_artist || this.self.approved) return require('assets/user.svg')
      if (this.self.pending) return require('assets/user-blue.svg')
      return require('assets/user-blue.svg')
    },
    my_name () {
      return utils.isCompact() ? '' : this.self.label
    },
  }
}
</script>
