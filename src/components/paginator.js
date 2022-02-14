import html from '../utils/html.js'

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

  template: `
    <div class="paginator">
      <span class="prev" :disabled="!hasPrev" v-on:click="hasPrev ? changePage(current - 1) : undefined">Prev</span>
      <span class="page" v-if="hasFirst" v-on:click="changePage(1)">1</span>
      <span class="page" v-if="hasFirst">...</span>
      <span v-for="page in pages" class="page" :selected="current == page" v-on:click="changePage(page)">
        {{page}}
      </span>
      <span class="page" v-if="hasLast">...</span>
      <span class="page" v-if="hasLast" v-on:click="changePage(total)">{{total}}</span>
      <span class="next" :disabled="!hasNext" v-on:click="hasNext ? changePage(current + 1) : undefined">Next</span>
    </div>
  `
}
