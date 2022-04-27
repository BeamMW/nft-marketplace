<template>
  <div class="table-container">
    <hdr title="test table"/>
    <tbl class="tbl"
         title="nft trading history"
         :columns="columns"
         :dataset="dataset"
         :sort_params="sort_params"
         @sort="sortBy"
    />
  </div>
</template>

<style lang="stylus" scoped>
.table-container {
  height: 100%
  display: flex
  flex-direction: column
}
</style>

<script>
import hdr from './page-title.vue'
import tbl from './table.vue'

import DATA from '../utils/mock-table-data.js'

export default {
  components: {
    hdr,
    tbl
  },

  data() {
    return {
      columns: [
        { 
          id: 'coin',
          caption: 'Coin',
          width: '100px',
          render: (value) => {
            return `
              <img src="./assets/icon-beam.svg">
              ${value.toUpperCase()}
            `
          }
        },
        {
          id: 'amount',
          caption: 'Amount',
          width: '100px',
          render: (value) => { return `+${value}`}
        },
        {
          id: 'usd',
          caption: 'USD Value',
          width: '100px',
          render: (value) => {
            return `
              <span style="font-weight: bold">+${value}</span>
            `
          }
        },
        {
          id: 'event',
          caption: 'Event',
          width: '70px'
        },
        {
          id: 'from',
          caption: 'From',
          render: (value) => {
            if (value.length <= 12) return value
            
            const head = value.slice(0, 6)
            const tail = value.slice(value.length - 6, value.length)
            
            return `${head}...${tail}`
          }
        },
        {
          id: 'to',
          caption: 'To',
          render: (value) => {
            if (value.length <= 12) return value
            
            const head = value.slice(0, 6)
            const tail = value.slice(value.length - 6, value.length)
            
            return `${head}...${tail}`
          }
        },
        {
          id: 'date',
          caption: 'Date',
        },
      ],
      dataset: DATA,
      sort_params: {
        columnIdx: null,
        ascending: false
      }
    }
  },

  methods: {
    sortBy(cellIdx) {
      if (this.sort_params.columnIdx !== cellIdx) {
        this.sort_params.ascending = false
      }
      
      const sortedData = [...DATA]

      sortedData.sort((a, b) => {
        const aValue = a[cellIdx]
        const bValue = b[cellIdx]

        if (Number(aValue)) {
          return !this.sort_params.ascending ? (aValue - bValue) : (bValue - aValue)  
        } 

        return !this.sort_params.ascending ? aValue.localeCompare(bValue) : bValue.localeCompare(aValue)
      })

      this.sort_params.ascending = !this.sort_params.ascending ? true : false
      this.dataset = sortedData
      this.sort_params.columnIdx = cellIdx
    }
  }
}
</script>
