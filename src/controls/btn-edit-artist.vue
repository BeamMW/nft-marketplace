<template>
  <div>
    <messageModal ref="messageModal"/>
    <btn v-if="is_artist" 
         height="36px" 
         width="36px" 
         :disabled="in_set_artist"
         :color="edit_btn_color"
         padding="0px" 
         radius="10px" 
         tooltip="edit artist info"
         @click="onEditArtist"
    >
      <img :src="edit_btn_icon">
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
  </div>
</template>

<script>
import btn from 'controls/button'
import artistsStore from 'stores/artists'
import messageModal from 'components/message-modal'

export default {
  components: {
    btn,
    messageModal
  },

  computed: {
    is_artist() {
      return artistsStore.is_artist
    },
    in_set_artist() {
      return !!artistsStore.artist_tx
    },
    self () {
      return artistsStore.self || {}
    },
    edit_btn_color() {
      if (this.self.approved) return undefined
      if (this.self.pending) return '#ed69ff'
      return 'red'
    },
    edit_btn_icon() {
      if (this.self.approved) return require('assets/pencil.svg')
      if (this.self.pending) return require('assets/pencil-blue.svg')
      return require('assets/pencil-blue.svg')
    }
  },

  methods: {
    onEditArtist() {
      if (!this.$state.is_desktop) {
        this.$refs.messageModal.open('Edit an artist', 'It is not possible change an artist in web & mobile wallets.<br>Please download desktop wallet and try from there.')
        return
      }
      artistsStore.toEditSelf()
    },
    onBecomeArtist() {
      if (!this.$state.is_desktop) {
        this.$refs.messageModal.open('Become an artist', 'It is not possible become an artist in web & mobile wallets.<br>Please download desktop wallet and try from there.')
        return
      }
      artistsStore.toBecomeArtist()
    }
  }
}
</script>
