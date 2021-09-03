import html    from '../utils/html.js'
import utils   from '../utils/utils.js'
import upload  from './upload.js'
import asset   from './asset.js'
import loading from './loading.js'

export default {
    props: {
        changed_txs: {
            type: Array,
            default: []
        }
    },

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
                    v-bind:artist="(artists[item.pk] || {}).label"
                    v-bind:owned="item.owned"
                    v-bind:approved="item.approved"
                    v-bind:price="item.price"
                    v-bind:in_tx="item.in_tx"
                    v-on:tx-started="onTxStarted"
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

    watch: {
        changed_txs: {
            handler (old, val) {
                this.loadItems()
            }
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

            this.loadItems()
        },

        loadItems () {
            utils.invokeContract(`role=user,action=view_all,cid=${this.$root.cid},id=1`, 
                (...args) => {
                    this.onLoadItems(...args)
                }
            )
        },

        onLoadItems (err, res) {
            if (err) {
                return this.$root.setError(err)
            }

            utils.ensureField(res, "items", "array")

            let items = []
            for (const item of res.items) {
                if (item.id < 12) continue
            
                if (item["price.aid"] != undefined) {
                    item.price = {
                        aid: item["price.aid"],
                        amount: item["price.amount"]
                    }
                }

                // TODO: optimize
                let found = false
                for (let idx = 0; idx < this.items.length; ++idx) {
                    let old = this.items[idx]
                    if (old.id == item.id) {
                        item.title = old.title
                        item.bytes = old.bytes
                        found = true
                    }
                }

                item.in_tx = false
                items.push(item)
                if (!found) {
                    this.loadItem(items.length - 1, item.id)
                }   
            }
            
            this.items = items
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
        },

        onTxStarted(id) {
            for (let item of this.items) {
                if (item.id == id) {
                    item.in_tx = true
                }
            }
        }
    }
}
