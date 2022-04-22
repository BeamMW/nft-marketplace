<template>
  <list v-model:page="page"
        emptymsg="There are no collections at the moment"
        component="collection"
        mode="user"
        :store="store"
  />
</template>

<script>
import list from './items-list.vue'
import collsStore from 'stores/collections'

export default {
  components: {
    list
  },

  computed: {
    store () {
      return collsStore
    }
  }
}
</script>
