<template>
  <collection :item="item"/>
</template>

<script>
import collection from './collection'

export default {
  components: {
    collection,
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  emits: [
    'selected'
  ],

  methods: {
  }
}
</script>
