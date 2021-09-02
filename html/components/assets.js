import html    from '../utils/html.js'
import utils   from '../utils/utils.js'
import upload  from './upload.js'
import asset   from './asset.js'
import loading from './loading.js'

export default {
    components: {
        asset, upload
    },

    template: `
        <div class="vertical-container">
            <upload/>
            <div class="items">
                <asset v-for="item in items"
                    v-bind:id="item.id"
                    v-bind:title="item.title"
                    v-bind:bytes="item.bytes"
                    v-bind:artist="artists[item.pk].label"
                />
            </div>
        </div>    
    `,

    data () {
        return {
            artists: {},
            items: []
        }
    },

    mounted () {
        utils.invokeContract(`role=manager,action=view_artists,cid=${this.$root.cid}`, (...args) => this.onLoadArtists(...args))
    },

    methods: {
        onLoadArtists(err, res) {
            if (err) {
                return this.$root.setError(err)
            }

            utils.ensureField(res, "artists", "array")
            this.rawArtists = res.artists
            
            for (const artist of res.artists) {
                this.artists[artist.key] = artist
            }

            utils.invokeContract(`role=user,action=view_all,cid=${this.$root.cid},id=1`, (...args) => this.onLoadItems(...args))
        },

        onLoadItems(err, res) {
            if (err) {
                return this.$root.setError(err)
            }

            utils.ensureField(res, "items", "array")
            this.rawItems = res.items

            for (const item of res.items) {
                if (item.id < 7) continue
                //if (!item.approved) continue
                
                this.items.push(item)
                this.loadItem(this.items.length - 1, item.id)
            }
        },

        loadItem(idx, id) {
            utils.invokeContract(`role=user,action=download,cid=${this.$root.cid},id=${id}`,
                (err, res) => {
                    if (err) {
                        return this.$root.setError(err)
                    }

                    utils.ensureField(res, "data", "string")
                    var data = utils.hexDecodeU8A(res.data)

                    // check version
                    if (data[0] != 1) {
                        throw `Invalid format version: ${data[0]}`
                    }

                    // parse name
                    let nend = data.findIndex(val => val == 0)
                    if (nend == -1 || nend + 1 == data.length) {
                        alert("unable")
                        throw `Unable to parse image name`
                    }

                    let rawName = data.subarray(1, nend)
                    let name = (new TextDecoder()).decode(rawName)

                    // parse bytes
                    let bytes = data.subarray(nend + 2)

                    // save parsed
                    var item = this.items[idx]
                    item.title = name
                    item.bytes = bytes
                }
            )
        }
    }
}
