<template>
  <div class="list-container">
    <div v-if="items === undefined" class="empty">
      <img src="~assets/icon-empty-gallery.svg"/>
      <!-- TODO: delay showing text a bit, otherwise it blinks on every page switch -->
      <div class="text">Loading {{ items_name }}...</div>
    </div>
    <template v-else-if="items && items.length > 0 || new_component">
      <div class="list-wrap">  
        <div ref="itemslist" class="list" @scroll="onScroll">
          <template v-if="selectable">
            <selectItem v-for="item in items" 
                        :key="item.id" 
                        :left="selector_left" 
                        :top="selector_top" 
                        :selected="selected.includes(item.id)"
                        @click="onSelected(item.id)"
            >
              <component :is="component" 
                         :item="item"
              />
            </selectitem>
          </template>
          <template v-else>
            <component :is="component" v-for="item in items"
                       :key="item.id"
                       :item="item"
            />
          </template>
          <component :is="new_component" v-if="show_new"/>
        </div>
      </div>
      <paginator :current="page"
                 :total="pages"
                 @page-changed="onPage"
      />
    </template>
    <div v-else class="empty"> 
      <img src="~assets/icon-empty-gallery.svg"/>
      <div class="text">There are no {{ items_name }} at the moment</div>
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
import paginator from './paginator'
import selectItem from './select-item'
import {useObservable} from '@vueuse/rxjs'
import {computed} from 'vue'

export default {
  components: {
    paginator,
    selectItem
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
    items_name: {
      type: String,
      default: 'items'
    },
    gap: {
      type: String,
      default: '16px'
    },
    new_component: {
      type: String,
      default: ''
    },
    selected: {
      type: Array,
      default: undefined
    },
    selector_left: {
      type: String,
      default: '20px'
    },
    selector_top: {
      type: String,
      default: '18px'
    }
  },

  emits: [
    'selected'
  ],

  setup (props) {
    const itemsObservable = computed(() => useObservable(props.store.getLazyPageItems(props.mode)))
    const items = computed(() => itemsObservable.value.value)
    const page = computed(() => props.store.getPage(props.mode))
    const pages = computed(() => props.store.getPages(props.mode))
    const show_new = computed(() => props.new_component && page.value === pages.value)
    const selectable = computed(() => !!props.selected)

    return {
      items,
      page,
      pages,
      show_new,
      selectable
    }
  },

  mounted() {
    if(this.items && this.items.length && this.$route.hash) {
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
    },
    onSelected(id) {
      this.$emit('selected', id)
    }
  }
}
</script>
