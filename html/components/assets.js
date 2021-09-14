import utils   from '../utils/utils.js'
import upload  from './upload.js'
import asset   from './asset.js'

export default {
    props: {
        changed_txs: {
            type: Array,
            default: []
        },
        artist_key: {
            type: String,
            required: true
        },
        cid: {
            type: String,
            required: true
        }
    },

    components: {
        asset, upload
    },

    template: `
        <div class="vertical-container">
            <upload v-bind:is_artist="is_artist"/>
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
                    v-on:sell="onSellAsset"
                    v-on:buy="onBuyAsset"
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

    computed: {
        is_artist () {
            for (let aid in this.artists) {
                let artist = this.artists[aid]
                if (artist.key == this.artist_key) {
                    return true
                }
            }
            return false
        }
    },

    watch: {
        changed_txs: {
            handler () {
                this.loadItems()
            }
        }
    },

    mounted () {
        utils.invokeContract(
            `role=manager,action=view_artists,cid=${this.cid}`, 
            (...args) => this.onLoadArtists(...args)
        )
    },

    methods: {
        onLoadArtists(err, res) {
            if (err) return this.$root.setError(err)
        
            utils.ensureField(res, "artists", "array")
            for (const artist of res.artists) {
                this.artists[artist.key] = artist
            }

            this.loadItems()
        },

        loadItems () {
            utils.invokeContract(
                `role=user,action=view_all,cid=${this.cid},id=1`, 
                (...args) => this.onLoadItems(...args)
            )
        },

        onLoadItems (err, res) {
            if (err) return this.$root.setError(err)
        
            // TODO: load transactions and check if any item is in_tx
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
            utils.invokeContract(`role=user,action=download,cid=${this.cid},id=${id}`,
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

        onBuyAsset(id) {
            utils.invokeContract(
                `role=user,action=buy,id=${id},cid=${this.cid}`, 
                (...args) => this.makeTx(...args)
            )
        },

        onSellAsset(id) {
            try
            {
                // TODO: show custom dialog and suport float values
                let price = prompt("Enter the price in BEAM")
                if (price == null) return
                price = parseInt(price) * 100000000

                utils.invokeContract(
                    `role=user,action=set_price,id=${id},amount=${price},aid=0,cid=${this.cid}`, 
                    (err, sres, fres) => this.onMakeTx(err, sres, fres, id)
                )
            } 
            catch (err) 
            {
                this.$root.setError(err, "Failed to sell an item")
            }
        },

        onMakeTx (err, sres, full, id) {
            if (err) return this.$root.setError(err, "Failed to generate transaction request")
            utils.ensureField(full.result, "raw_data", "array")
            utils.callApi('process_invoke_data', {data: full.result.raw_data}, (err, res, full) => this.onSendToChain(err, res, full, id))
        },

        onSendToChain(err, res, full, id) {
            if (err) {
                if (utils.isUserCancelled(err)) return
                return this.$root.setError(err, "Failed to create transaction")
            }
            for (let item of this.items) {
                if (item.id == id) {
                    item.in_tx = true
                }
            }
        }
    }
}
