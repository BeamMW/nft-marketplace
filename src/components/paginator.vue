<template>
   <div class="paginator">
      <span class="prev" :disabled="!hasPrev" v-on:click="hasPrev ? changePage(current - 1) : undefined">Prev</span>
      <span class="page" v-if="hasFirst" v-on:click="changePage(1)">1</span>
      <span class="page" v-if="hasFirst">...</span>
      <span v-for="page in pages" :key="page" class="page" :selected="current == page" v-on:click="changePage(page)">
        {{page}}
      </span>
      <span class="page" v-if="hasLast">...</span>
      <span class="page" v-if="hasLast" v-on:click="changePage(total)">{{total}}</span>
      <span class="next" :disabled="!hasNext" v-on:click="hasNext ? changePage(current + 1) : undefined">Next</span>
    </div>
</template>

<style lang="stylus" scoped>
.paginator {
    display: flex
    justify-content: center
    align-items: center
    margin-top: 10px
    margin-bottom: 2px

    & > .prev,
    & > .next {
        width: 68px !important
        height: 22px
        border-radius: 2px
        background-color: rgba(255, 255, 255, .1)
        color: rgba(255, 255, 255, .5)
        font-size: 14px
        text-align: center
        user-select: none

        &[disabled="true"]{
           color: rgba(255, 255, 255, .15)
        }

        &:hover:not([disabled="true"]) {
           color: rgba(255, 255, 255, .9)
           cursor: pointer
        }
    }

    & > .prev {
        margin: 0 13px 0 0
    }

    & > .next {
        margin: 0 0 0 16px
    }
    
    & > .page {
        margin: 3px 7px
        font-size: 14px
        color: rgba(255, 255, 255, 0.5)
        cursor: pointer
        user-select: none

        &[selected="true"] {
          cursor: auto
          color: #00f6d2
        }

        &:hover:not([selected="true"]) {
          color: rgba(255, 255, 255, .9)
          cursor: pointer
        }
    }
}
</style>

<script>
export default {
  props: {
    current: {
      type: Number,
      default: 1
    },
    total: {
      type: Number,
      default: 0
    },
    pageRange: {
      type: Number,
      default: 2
    }
  },

  computed: {
    hasFirst () {
      return this.rangeStart !== 1
    },

    hasLast () {
      return this.rangeEnd < this.total
    },

    hasPrev () {
      return this.current > 1
    },

    hasNext () {
      return this.current < this.total
    },

    rangeStart () {
      var start = this.current + this.pageRange <= this.total ? this.current - this.pageRange : this.total - this.pageRange
      return (start > 0) ? start : 1
    },

    rangeEnd () {
      var end = this.rangeStart + (this.current > this.pageRange ? this.pageRange * 2 : this.pageRange)
      return (end < this.total) ? end : this.total
    },

    pages () {
      var pages = []
      for (var i = this.rangeStart; i <= this.rangeEnd; i++) {
        pages.push(i)
      }
      return pages
    },
  }, 

  emits: ['page-changed'],

  methods: {
    changePage (page) {
      if (page != this.current) {
        this.$emit('page-changed', page)
      }
    }
  },
}
</script>