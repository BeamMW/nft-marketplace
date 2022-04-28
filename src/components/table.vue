<template>
  <div v-if="body_rows && body_rows.length" class="table">
    <h1 class="table-title">{{ title }}</h1> 
    <!-- columns -->
    <div class="table-head">
      <div v-for="(cell, columnIdx) in header_row" 
           :key="cell.id" 
           class="header"
           :style="cellStyle(cell)"
      >
        <span :style="sort_params.columnIdx === columnIdx ? active_column : ''" 
              @click="sort_by(columnIdx)"
        >
          {{ cell.caption }}
          <img v-if="sort_params.columnIdx === columnIdx && sort_params.ascending" src="~/assets/arrow-ascending.svg" class="arrow">
          <img v-if="sort_params.columnIdx === columnIdx && !sort_params.ascending" src="~/assets/arrow-descending.svg" class="arrow">
        </span>
      </div>
    </div>

    <!-- dataset -->
    <div class="table-body">
      <div v-for="row in body_rows"
           :key="row"
      >
        <div v-for="(cell, cellIdx) in row"
             :key="(cell, cellIdx)"
             class="cell"
             :style="cellStyle(header_row[cellIdx])"
        >
          <span v-if="header_row[cellIdx].render" v-html="header_row[cellIdx].render(cell)"></span>
          <span v-else>{{ cell }}</span>
        </div>
      </div>
    </div>
  </div>

  <!-- empty -->
  <div v-else-if="body_rows !== undefined && !body_rows.length" class="table-empty">
    <img src="~assets/icon-wallet-empty.svg"/>
    <div>{{ empty_message }}</div>
  </div>  

  <!-- loading -->
  <div v-else class="table-loading">
    Loading...
  </div>
</template>

<style lang="stylus" scoped>
.table-empty {
  padding: 35px 25px 0 25px
  text-align: center
  color: rgba(255, 255, 255, 0.5)

  & img {
    padding-bottom: 10px
  }
}

.table-loading {
  padding: 35px 25px 0 25px
  text-align: center
  color: rgba(255, 255, 255, 0.5)
}

.table-title {
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
  min-height: 0
  width: 100%
  display: flex
  flex-direction: column
  font-size: 14px
  font-weight: 100
  font-family: 'SF Pro Display', sans-serif
  line-height: 44px

  & span {
    display: flex
    align-items: center

    &:deep(img) {
      margin-right: 8px
    }
  }

  & .arrow {
    margin-left: 8px
  }
}

.table-head {
  display: flex
  border-radius: 10px 10px 0 0
  overflow: hidden
  min-height: 46px

  & .header {
    display: flex
    align-items: center
    padding-left: 20px
    color: rgba(255, 255, 255, 0.5)
    background-color: rgba(255, 255, 255, 0.08)
    user-select: none
    
    &:last-child {
      padding-right: 20px
    }

    & span {
      cursor: pointer
      line-height: 25px
    }
  }
}

.table-body {
  display: flex
  flex-direction: column
  overflow-x: hidden
  overflow-y: overlay

  & > div:nth-child(odd) {
    background-color: rgba(255, 255, 255, 0.05)
  }

  & > div:nth-child(even) {
    background-color: rgba(255, 255, 255, 0.01)
  }

  & > div {
    display: flex
    max-height: 56px
  }

  & .cell {
    height: 56px
    display: flex
    align-items: center
    padding-left: 20px
    white-space: nowrap
    overflow: hidden

    &:last-child {
      padding-right: 20px
    }
  }
}
</style>

<script>
export default {
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
    sort_params: {
      type: Object,
      default: () => ({
        columnIdx: undefined,
        ascending: false
      })
    },
    empty_message: {
      type: String,
      default: 'Trading history is empty'
    }
  },

  emits: ['sort'],

  computed: {
    header_row() {
      return this.columns
    },

    body_rows() {
      return this.dataset
    },

    active_column() {
      return 'color: white; font-weight: bold'
    }
  },

  methods: {
    cellStyle(cell) {
      return cell.width
        ? `width: ${cell.width}` 
        : 'flex: 1'
    },

    sort_by(columnIdx) {
      this.$emit('sort', columnIdx)
    }
  }
}
</script>
