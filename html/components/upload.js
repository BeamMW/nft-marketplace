export default {

    computed: {
        artists() {
            return this.$state.artists
        }
    },

    template: `
        <div class="upload">
            <input type="button" value="Add new artist" v-on:click.native="onAddArtist"/>
            <span v-if="!artists || !artists.length">Add artists to upload artworks</span>
            <span v-else>
                <span>Upload for artist:&nbsp;&nbsp;</span>
                <select name="upload_artist" v-model="$state.selected_artist">
                    <option v-for="artist in artists" v-bind:value="{key: artist.key, label: artist.label}">{{artist.label}}</option>
                </select>
                &nbsp;&nbsp;
                <input type="file" accept="image/png, image/jpeg" 
                    v-on:change.native="onUploadArtwork"
                    :disabled="!$state.selected_artist"
                />
            </span>
        </div>
        `,

    methods: {
        onAddArtist () {
            try {
                // TODO: show normal dialog, no " and , in name parsed[1] (name), max 100 bytes; parsed[0] (key) is alphanumeric 
                let data = prompt("Enter: artistid,name")
                if (data != null) {
                    let parsed = data.split(",")
                    if (parsed.length != 2) throw "No comma found"
                    this.$store.addArtist(parsed[0],parsed[1])
                }
            }
            catch(err) {
                this.$store.setError(err, "Failed to parse artist data")
            }
        },

        onUploadArtwork (ev) {
            let file = ev.target.files[0]
            if (!file) { 
                return
            }
            this.$store.uploadArtwork(file, this.$state.selected_artist.key, this.$state.selected_artist.label)
        }
    }
}
