<template>
  <div class="admin-column">
    <div class="admin-row">
      <input type="button" value="Add new artist" @click="onAddArtist"/>
      <span v-if="!artists_count">Add artists to upload artworks</span>
      <span v-else>
        <span>Upload for artist:&nbsp;&nbsp;</span>
        <select v-model="$state.selected_artist" class="upload_artist">
          <option v-for="artist in artists" :key="artist.key" :value="{key: artist.key, label: artist.label}">
            {{ artist.label }} - {{ artist.key.substring(0, 4) }}..{{ artist.key.slice(-4) }}
          </option>
        </select>
        &nbsp;&nbsp;
        <input type="file" multiple accept="image/apng, image/avif, image/gif, image/jpeg, image/png, image/svg+xml, image/webp"      
               :disabled="!$state.selected_artist"
               @change="onUploadArtwork"
               @click="value=null"
        />
      </span>
    </div>
    <div class="admin-row">
      <span>Rewards left: {{ balance_reward }}</span>&nbsp;&nbsp;&nbsp;
      <input type="button" value="Add Rewards" @click="onAddRewards"/>
      <input type="button" value="Show Stats" :disabled="pending_artworks != 0" @click="onStats"/>
      <span v-if="pending_artworks">Artworks to load: {{ pending_artworks }}</span>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .admin-column {
    display: flex
    flex-direction: column
    margin-bottom: 10px
    margin-top: 5px

    select.upload_artist {
      font-size: 0.95em
    }

    .admin-row {
      display:         flex
      align-items:     center
      justify-content: center

      &:not(:first-child) {
        margin-top: 15px
      }

      & > input {
        margin-right: 20px
      }
    }
}
</style>

<script>
import {common} from '../utils/consts.js'

export default {
  computed: {
    artists() {
      return this.$state.artists
    },
    artists_count() {
      return this.$state.artists_count
    },
    balance_reward() {
      return this.$state.balance_reward  / common.GROTHS_IN_BEAM
    },
    pending_artworks() {
      return this.$state.pending_artworks
    }
  },

  methods: {
    onAddRewards() {
      try {
        let amount = prompt('Enter amount int REWARD TOKEN')
        if (amount == null) {
          return
        }
        amount = parseInt(amount) // THIS IS NOT IN GROTHs
        this.$store.addRewards(amount)
      } 
      catch (err) {
        this.$store.setError(err, 'Failed to add rewards')
      }
    },

    onAddArtist () {
      try {
        // TODO: show normal dialog, no " and , in name parsed[1] (name), max 100 bytes; 
        //       parsed[0] (key) is alphanumeric 
        let data = prompt('Enter: artistid,name')
        if (data != null) {
          let parsed = data.split(',')
          if (parsed.length != 2) throw 'No comma found'
          this.$store.addArtist(parsed[0],parsed[1])
        }
      }
      catch(err) {
        this.$store.setError(err, 'Failed to parse artist data')
      }
    },

    onUploadArtwork (ev) {
      let files = ev.target.files
      for (let idx = 0; idx < files.length; idx++) {
        let file = files[idx]
        if (!file) { 
          continue
        }
        this.$store.uploadArtwork(file, this.$state.selected_artist.key, this.$state.selected_artist.label)
      }
    },

    onStats () {
      this.$store.showStats()
    }
  }
}
</script>