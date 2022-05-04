<template>
  <div class="my-page">
    <pageTitle title="my page">
      <div class="actions">
        <btn v-if="is_artist" 
             :height="height" 
             :width="width" 
             :disabled="in_set_artist"
             padding="0px" 
             radius="10px" 
             @click="onEditArtist"
        >
          <img src="~assets/icon-pencil.svg">
        </btn>

        <btnWallet/>
        <btnKey/>

        <btn v-if="!is_artist" 
             :height="height" 
             :width="width" 
             :disabled="in_set_artist"
             radius="10px" 
             padding="0px" 
             tooltip="become an artist" 
             @click="onBecomeArtist"
        >
          <img src="~assets/add-user.svg"/>
        </btn>
      </div>
    </pageTitle>
    <myGallery class="gallery"/>
  </div>
</template>

<style scoped lang="stylus">
  .my-page {
    width: 100%
    height: 100%
    display: flex
    flex-direction: column

    & > .gallery {
      flex: 1
      box-sizing: border-box
    }

    .actions {
      display: flex
      justify-content: flex-end
      align-items: center

      & > * {
        margin-left: 12px
        margin-top: 7px

        &:last-child {
          margin-right: 7px
        }
      }
    }
  }
</style>

<script>
import pageTitle from './page-title.vue'
import btn from './button.vue'
import btnWallet from './button-wallet.vue'
import btnKey from './button-key.vue'
import artistsStore from 'stores/artists.js'
import myGallery from './my-gallery'

export default {
  components: {
    pageTitle,
    btn,
    btnWallet,
    btnKey,
    myGallery
  },

  data() {
    return {
      width: '36px',
      height: '36px'
    }
  },

  computed: {
    is_artist() {
      return artistsStore.is_artist
    },
    in_set_artist() {
      return !!artistsStore.artist_tx
    }
  },

  methods: {
    onEditArtist() {
      artistsStore.toEditSelf()
    },
    onBecomeArtist() {
      artistsStore.toBecomeArtist()
    }
  },
}
</script>
