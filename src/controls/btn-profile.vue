<template>
  <btn height="34px"
       :text="my_name"
       tooltip="show profile"
       @click="$store.toMyPage"
  >
    <img src="~assets/user.svg">
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
    my_name () {
      return (artistsStore.self || {}).label
    },
  }
}
</script>
