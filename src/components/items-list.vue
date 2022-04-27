<template>
  <div class="list-container">
    <template v-if="items.length > 0 || new_component">
      <div class="list-wrap">  
        <div ref="itemslist" class="list" @scroll="onScroll">
          <component :is="component" v-for="item in items" 
                     :key="item.id"
                     :item="item"
          />
        </div>
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

    & > .list-wrap {
      overflow-y: auto
      overflow-x: hidden

      & > .list {
        display: flex
        flex-wrap: wrap        
        margin: -8px -8px 0 -8px

        & > * {
          margin: 8px
        }
      }
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
import paginator from './paginator.vue'

export default {
  components: {
    paginator
  },

  props: {
    store: {
      type: Object,
      required: true
    },
    mode: {
      type: String,
      default: 'user'
    },
    component: {
      type: String,
      required: true,
      default: undefined
    },
    emptymsg: {
      type: String,
      default: ''
    },
    gap: {
      type: String,
      default: '16px'
    },
    new_component: {
      type: String,
      default: ''
    }
  },

  computed: {
    items () {
      return this.store.getItems(this.mode)
    },
    total() {
      return this.store.getTotal(this.mode)
    },
    pages() {
      return this.store.getPages(this.mode)
    },
    page() {
      return this.store.getPage(this.mode)
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
      this.$refs.itemslist.scrollTop = 0
      this.store.setPage(this.mode, page)
    } 
  }
}
</script>
