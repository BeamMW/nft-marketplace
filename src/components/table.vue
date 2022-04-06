<template>
  <hdr title=""/>
  <!-- title to props -->
  <h1 class="title">nft traiding history</h1> 
  <table class="table">
    <thead>
      <tr>
        <th v-for="(header, cellIdx) in headers" :key="cellIdx + header">
          <img src="~assets/icon-beam.svg">
          <span>{{ header_cell(header, cellIdx, 1) }}</span>
          <!-- <span>{{ header }}</span> -->
          <img src="~assets/sort-arrows.svg">
        </th>
      </tr>
    </thead>

    <tbody>
      <tr v-for="(item, rowIdx) in mock_data" :key="rowIdx + item">
        <td v-for="(value, cellIdx) in item" :key="rowIdx + cellIdx + value">
          <span>{{ value }}</span>
        </td>
      </tr>
    </tbody>
  </table>
</template>

<style lang="stylus" scoped>
table
tbody
thead
tr
th
td {
  border: 0
}

table {
  border-spacing: 0
}

.title {
  font-family: 'SFProDisplay', sans-serif
  font-weight: 700
  font-size: 14px
  line-height: 17px
  text-transform: uppercase
  letter-spacing: 3.1px
  color: rgba(255, 255, 255, 0.5)
  padding-bottom: 20px
  margin: 0
}

.table {
  width: 100%
  border-radius: 10px 10px 0 0
  font-size: 14px
  font-family: 'SFProDisplay', sans-serif
  overflow: hidden
  
  & tr:nth-child(odd) {
    background-color: rgba(255, 255, 255, 0.05)
  }

  & tr:nth-child(even) {
    background-color: rgba(255, 255, 255, 0.01)
  }

  & th:first-child {
    padding: 15px 20px
  }

  & th {
    min-width: 50px
    text-align: left
    padding-right: 20px
    color: rgba(255, 255, 255, 0.5)
    background-color: rgba(255, 255, 255, 0.08)
    white-space: nowrap

    img {
      margin-left: 5px
    }
  }

  & td:first-child {
    padding: 20px 20px
  }

  & td {
    padding-right: 20px
    text-align: left
  }
}
</style>

<script>
import hdr from './page-title.vue'

import DATA from '../utils/mock-data.js'

export default {
  components: {
    hdr,
  },
  
  data() {
    return {
      headers: ['Author', 'Book', 'Year']
    }
  },
  // props: {
  //   columns: {
  //     type: Array,
  //     default: undefined,
  //     required: true
  //   },
  //   rows: {
  //     type: Array,
  //     default: undefined,
  //     required: true
  //   }
  // }

  computed: {
    mock_data() {
      return DATA
    }
  },

  methods: {
    header_cell(value, idx, param) {
      if (idx === param) {
        return 'TEST'
      }

      return value
    }
  }

}
</script>

