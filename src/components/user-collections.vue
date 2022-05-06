<template>
  <list v-model:page="page"
        items_name="collections"
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
