<template>
  <!-- title to props -->
  <h1 class="table__title">{{ title }}</h1> 
  <div class="table" :style="table_style">
    <div v-for="cell in rows" 
         :key="cell.value" 
         :class="cell.isHeaderCell ? 'header' : 'cell'"
         :style="cellStyle(cell)"
    >
      <span v-if="cell.isImage">
        <img class="icon" src="~assets/icon-beam.svg">
        {{ cell.value }}
      </span>
      <span v-else :class="{ 'bold': cell.isBold }">{{ cell.value }}</span>

      <img v-if="cell.isHeaderCell" class="arrows" src="~assets/sort-arrows.svg">
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.table__title {
  width: 100%
  font-family: 'SF Pro Display', sans-serif
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
  border-radius: 10px 10px 0 0
  font-size: 14px
  font-weight: 100
  line-height: 24px
  font-family: 'SF Pro Display', sans-serif
  overflow: hidden

  & span {
    display: flex
    align-items: center
  }

  & .arrows {
    margin-left: 8px
  }
  
  & .icon {
    margin-right: 8px
  }

  & .amount {
    font-weight: 100
  }

  & .bold {
    font-weight: bold
  }
}

.header {
  height: 46px
  display: flex
  align-items: center
  padding-left: 20px
  color: rgba(255, 255, 255, 0.5)

  &:last-child {
    padding-right: 20px
  }
}

.cell {
  height: 56px
  display: flex
  align-items: center
  padding-left: 20px

  &:last-child {
    padding-right: 20px
  }
}
</style>

<script>
// import amount from './amount.vue'

export default {
  components: {
    // amount
  },
  
  props: {
    title: {
      type: String,
      default: undefined
    },
    columns: {
      type: Array,
      default: undefined,
      required: true
    },
    dataset: {
      type: Array,
      default: undefined,
      required: true
    },
    image_idx: {
      type: Number,
      default: null
    },
    bold_idx: {
      type: Number,
      default: null
    },
    ellepsis_idx: {
      type: Number,
      default: null
    }
  },

  computed: {
    rows() {
      const headerCells = this.columns.map(column => ({value: column.name, isHeaderCell: true}))
      const bodyCells = this.dataset
        .map((item, rowIdx) => {
          const row = this.columns.map(column => ({
            value: item[column.id] || 'n/a',
            isHeaderCell: false,
            isEvenRow: rowIdx % 2 === 0,
            isImage: false,
            isBold: false
          }))

          // render cell
          if (this.image_idx !== null) {
            row[this.image_idx].isImage = true
          }

          if (this.bold_idx !== null) {
            row[this.bold_idx].isBold = true
          }

          if (this.ellepsis_idx !== null) {
            const el = row[this.ellepsis_idx].value

            if (el.length <= 12) return row
            
            const head = el.slice(0, 6)
            const tail = el.slice(el.length - 6, el.length)
            
            row[this.ellepsis_idx].value = `${head}...${tail}`
          }

          return row
        })
        .flat()
        
      // console.log(headerCells, bodyCells)
      return [...headerCells, ...bodyCells]
    },

    table_style() {
      return `display: grid; grid-template-columns: ${this.columns.map(() => 'auto').join(' ')}`
    },
  },
  
  methods: {
    cellStyle(cell) {
      if (cell.isHeaderCell) {
        return 'background-color: rgba(255, 255, 255, 0.08)'
      }

      if (cell.isEvenRow) {
        return 'background-color: rgba(255, 255, 255, 0.01)'
      }

      return 'background-color: rgba(255, 255, 255, 0.05)'
    }
  }
}
</script>

  & tr {
    max-height: 57px
  }
  
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

    .ardataset {
      margin-left: 5px
    }
  }

  & td:first-child {
    padding: 20px 20px
  }

  & td {
    padding-right: 20px
    text-align: left

    border: 1px solid cyan

    & img {
      margin: -20px
    }
  }

         :style="cell.isEvenRow && !cell.isHeaderCell ? 'background-color: cyan' : 'background-color: magenta'"
