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
import btn from 'controls/button'

export default {
  components: {
    btn
  },
  computed: {
    btn_color() {
      if (artistsStore.self.approved) return 'transparent'
      if (artistsStore.self.pending) return '#ed69ff'
      return 'red'
    },
    btn_image() {
      if (artistsStore.self.approved) return require('assets/user.svg')
      if (artistsStore.self.pending) return require('assets/user-blue.svg')
      return require('assets/user-blue.svg')
    },
    my_name () {
      return (artistsStore.self || {}).label
    },
  }
}
</script>
