<template>
  <btn v-if="is_artist" 
       height="36px" 
       width="36px" 
       :disabled="in_set_artist"
       padding="0px" 
       radius="10px" 
       @click="onEditArtist"
  >
    <img src="~assets/pencil.svg">
  </btn>
  <btn v-else
       height="36px" 
       width="36px" 
       :disabled="in_set_artist"
       radius="10px" 
       padding="0px" 
       tooltip="become an artist" 
       @click="onBecomeArtist"
  >
    <img src="~assets/add-user.svg"/>
  </btn>
</template>

<script>
import btn from 'controls/button'
import artistsStore from 'stores/artists'

export default {
  components: {
    btn
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
  }
}
</script>
