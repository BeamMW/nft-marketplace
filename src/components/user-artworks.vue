<template>
  <list v-model:page="page"
        items_name="NFTs"
        component="artwork"
        mode="user"
        :store="store"
  />  
</template>

<script>
import list from './items-list'
import artsStore from 'stores/artworks'

export default {
  components: {
    list
  },

  computed: {
    store () {
      return artsStore
    }
  }
}
</script>
