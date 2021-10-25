import upload  from './upload.js'
import artwork from './artwork.js'

export default {
    computed: {
        changed_txs () {
            return this.$state.changed_txs
        },
        is_artist () {
            return this.$state.is_artist
        },
        artists () {
            return this.$state.artists
        },
        artworks () {
            return this.$state.artworks
        }
    },

    components: {
        artwork, upload
    },

    template: `
        <div class="vertical-container">
            <upload v-bind:is_artist="is_artist"/>
            <div class="artworks">
                <artwork v-for="artwork in artworks"
                    v-bind:id="artwork.id"
                    v-bind:title="artwork.title"
                    v-bind:artist="(artists[artwork.pk] || {}).label"
                    v-bind:bytes="artwork.bytes"
                    v-bind:owned="artwork.owned"
                    v-bind:approved="artwork.approved"
                    v-bind:price="artwork.price"
                    v-bind:in_tx="artwork.in_tx"
                    v-on:sell="onSellArtwork"
                    v-on:buy="onBuyArtwork"
                />
            </div>
        </div>    
    `,

    watch: {
        changed_txs: {
            // TODO: this is not optimal
            handler () {
                //this.loadItems()
            }
        },
        artists: {
            handler () {
                alert("artists changed")
            }
        },
        artworks: {
            handler () {
                alert("artworks changed")
            }
        }
    },

    methods: {
        onBuyArtwork (id) {
            this.$store.buyArtwork(id)
        },

        onSellArtwork (id) {
            try {
                // TODO: show custom dialog and suport float values
                // TODO: may be show dialog in
                let price = prompt("Enter the price in BEAM")
                if (price == null) return
                price = parseInt(price) * 100000000
                this.$store.sellArtwork(id, price)
            } 
            catch (err) {
                this.$store.setError(err, "Failed to sell an item")
            }
        }
    }
}
