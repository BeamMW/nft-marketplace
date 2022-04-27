<template>
  <div class="gallery-container">
    <tabsctrl v-model:active="active_tab" :tabs="tabs"/>
    <list v-if="show_collections"
          class="list"
          emptymsg="There are no collections at the moment"
          component="collection"
          new_component="new-collection"
          mode="artist"
          :store="collsStore"
    />
  </div>
</template>

<style scoped lang="stylus">
  .gallery-container {
    display: flex
    flex-direction: column
    width: 100%
    height: 100%

    & > .list {
      margin-top: 20px
      flex: 1
    }
  }
</style>

<script>
import collsStore from 'stores/collections'
import artistsStore from 'stores/artists'
import {my_tabs} from 'utils/consts.js'
import tabsctrl from './tabs.vue'
import list from './items-list.vue'

export default {
  components: {
    tabsctrl,
    list
  },

  data() {
    return {
      active_tab: my_tabs.COLLECTIONS,
      tabs: [
        {id: my_tabs.COLLECTIONS, name: 'Collections'},
        {id: my_tabs.CREATED_NFTS, name: 'NFTs'},
        {id: my_tabs.OWNED_NFTS, name: 'Owned NFTs'},
      ],
    }
  },

  computed: {
    collsStore() {
      return collsStore
    },
    is_artist() {
      return artistsStore.is_artist
    },
    show_collections() {
      return this.active_tab === my_tabs.COLLECTIONS
    },
    show_created() {
      return this.active_tab === my_tabs.CREATED_NFTS
    },
    show_owned() {
      return this.active_tab == my_tabs.OWNED_NFTS
    }
  }
}
</script>
