<template>
  <div class="list-container">
    <template v-if="items.length > 0">
      <div ref="itemslist" class="list" :style="style" @scroll="onScroll">
        <component :is="component" v-for="item in visible_items" 
                   :key="item.id"
                   :item="item"
        />
      </div>
      <paginator :current="page"
                 :total="pages"
                 @page-changed="onPage"
      />
    </template>
    <div v-else class="empty">
      <img src="~assets/icon-empty-gallery.svg"/>
      <div class="text">{{ emptymsg }}</div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .list-container {
    display: flex
    flex-direction: column
    min-height: 0

    & > .list {
      display: flex
      flex-wrap: wrap
      overflow-y: auto
      overflow-x: hidden
      gap: 16px
    }

    .empty {
      margin: 100px auto 0 auto
      display: flex
      align-items: center
      flex-direction: column
      user-select: none

      & > .text {
        margin-top: 30px
        opacity: 0.5
        font-size: 14px
        line-height: 1.43
        text-align: center
        color: #fff
      }
    }
  }
</style>

<script>
import {common} from 'utils/consts.js'
import paginator from './paginator.vue'

export default {
  components: {
    paginator
  },

  props: {
    items: {
      type: Array,
      default: () => []
    },
    emptymsg: {
      type: String,
      default: ''
    },
    component: {
      type: String,
      required: true,
      default: undefined
    },
    page: {
      type: Number,
      default: 1
    },
    gap: {
      type: String,
      default: '16px'
    }
  },

  emits: [
    'update:page'
  ],

  computed: {
    visible_items () {
      let all = this.items
      let result  = []
      let start = (this.page - 1) * common.ITEMS_PER_PAGE
      let end   = Math.min(start + common.ITEMS_PER_PAGE, all.length)
      for (let idx = start; idx < end; ++idx) {
        result.push(all[idx])
      }
      return result
    },
    pages() {
      let total = this.items.length
      return total ? Math.ceil(total / common.ITEMS_PER_PAGE) : 1
    },
    style () {
      return {
        'gap': this.gap
      }
    }
  },

  mounted() {
    if(this.items.length && this.$route.hash) {
      this.$refs.itemslist.scrollTop = this.$route.hash.substr(1)
    }
  },

  methods: {
    onScroll(ev) {
      let pos = ev.target.scrollTop
      this.$router.replace({name: this.$route.name, hash: `#${pos}`})
    },

    onPage(page) {
      this.$emit('update:page', page)
      this.$refs.itemslist.scrollTop = 0
    } 
  }
}
</script>
