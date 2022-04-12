<template>
  <hdr title="test table"/>
  <tbl title="nft traiding history"
       :columns="columns"
       :dataset="mock_data"
       :image_idx="0"
       :bold_idx="1"
       :ellepsis_idx="3"
  />
</template>

<style lang="stylus" scoped>

</style>

<script>
import hdr from './page-title.vue'
import tbl from './table.vue'

import DATA from '../utils/mock-data.js'

export default {
  components: {
    hdr,
    tbl
  },

  data() {
    return {
      columns: [
        {id: 'author', name: 'Author'},
        {id: 'book', name: 'Book'},
        {id: 'year', name: 'Year'},
        {id: 'long', name: 'long word'}
      ]
    }
  },

  computed: {
    mock_data() {
      return DATA
    }
  },

  methods: {
    renderHeader(value, idx) {
      if (idx === 1) {
        return 'TEST PASSED'
      }

      return value
    }
  }
}
</script>

