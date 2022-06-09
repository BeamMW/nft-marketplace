import LazyItems from 'stores/lazy-items'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import formats from 'stores/formats'
import {versions, cid} from 'stores/consts'
import {computed} from 'vue'
import utils from 'utils/utils'
import router from 'router'

//
// Custom modes
//   user:collection     -> keys: collection, status
//   artist:collection   -> keys: collection
//
class NFTSStore extends LazyItems {
  constructor () {
    super({
      objname: 'nft', 
      versions: [versions.NFT_VERSION],
      modes: [
        'moderator', 
        'user', 
        'user:sale', 
        'user:liked', 
        'owned', 
        'owned:sale', 
        'artist:sold', 
        'liker:liked'
      ],
      extraDBKeys: ['collection']
    })
    
    this._current_coll_mode = undefined
    this._coll_modes = [
      {name: 'artist:collection:liked:',
        make_loader(collid) {
          return (store) => {
            console.log(`loader for artist:collection:liked:${collid}`)
            return store
              .where({'collection': collid})
              .and(item => item.lkes > 0)
          }
        }
      }, 
      {name: 'artist:collection:sale:', 
        make_loader(collid) {
          return (store) => {
            console.log(`loader for artist:collection:sale:${collid}`)
            return store
              .where({'collection': collid, 'owned': 1, 'sale': 1})
          }
        }
      },
      {name: 'artist:collection:',
        make_loader(collid) {
          return (store) => {
            console.log(`loader for artist:collection:${collid}`)
            return store.where({'collection': collid})
          }
        }
      }, 
      {name: 'user:collection:liked:',
        make_loader(collid) {
          return (store) => {
            console.log(`loader for user:collection:${collid}`)
            return store
              .where({
                'collection': collid,
                'status': 'approved'
              })
              .and(item => item.likes > 0)
          }
        }
      }, 
      {name: 'user:collection:sale:', 
        make_loader(collid) {
          return (store) => {
            console.log(`loader for user:collection:sale:${collid}`)
            return store
              .where({
                'collection': collid, 
                'owned': 1, 
                'sale': 1,
                'status': 'approved'
              })
          }
        }
      },
      {name: 'user:collection:',
        make_loader(collid) {
          return (store) => {
            console.log(`loader for user:collection:${collid}:${this._store_name}`)
            return store
              .where({
                'collection': collid,
                'status': 'approved'
              })
          }
        }
      }
    ]
  }

  _getMode(mode) {
    let result = super._getMode(mode) 

    if (!result) {
      if (this._current_coll_mode === mode) {
        return this._state[this._current_coll_mode]
      }

      if (this._current_coll_mode) {
        delete this._state[this._current_coll_mode]
        console.log('delete ' + this._current_coll_mode)
      }

      // create the requested mode
      let collid = -1
      let cmode  = undefined
      for(cmode of this._coll_modes) {
        if (mode.startsWith(cmode.name)) {
          collid = parseInt(mode.slice(cmode.name.length))
          break
        }
      }

      if (cmode && collid != -1) {
        let loader = cmode.make_loader(collid)
        result = this._allocMode({mode, loader, computed_total: true})
      }  

      this._current_coll_mode = mode
    }

    return result
  }

  fromContract(awork) {
    awork.description = awork.data.description 
    awork.image = awork.data.image
    return awork
  }

  fromDB(awork) {
    awork = Object.assign({}, awork)
    awork.store = this

    let author = artistsStore.loadArtist(awork.author)
    awork.author_error = computed(() => {
      return author.error
    })

    awork.author_name = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return author.label
    }) 

    awork.by_author = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      return `by <span style="color:#00f6d2">${author.label}</span>`
    })

    awork.safe_by_author = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      // Author name cannot be changed, so if already ever approved (approved_cnt > 0) it is safe to display
      if (author.approved_cnt) `by <span style="color:#00f6d2">${author.label}</span>`
      return '[author is in modeation]'
    })

    awork.safe_author_name = computed(() => {
      if (author.loading) return 'Loading...'
      if (author.error) return 'Failed to load author'
      // Author name cannot be changed, so if already ever approved (approved_cnt > 0) it is safe to display
      if (author.approved_cnt) return  author.label
      return '[author is in moderation]'
    })

    awork.safe_label = computed(() => {
      if (awork.approved) return awork.label
      return '[nft is in moderation]'
    })

    awork.safe_description = computed(() => {
      if (awork.approved) return awork.label
      return '[nft is in moderation]'
    })

    awork.safe_image = computed(() => {
      if (awork.approved) return awork.image
      return require('assets/nft-default.svg')
    })

    if (awork.error) {
      awork.image = {error: true}
    }

    if (!awork.error) {
      awork.image = imagesStore.fromDB(awork.image)
    }
    
    return awork
  }

  async toContract(label, data) {
    data.image = await imagesStore.toContract(data.image)
    return {
      label: formats.toContract(label),
      data: formats.toContract(data, versions.NFT_VERSION)
    }
  }

  async createNFT(collid, label, data, price) {
    ({label, data} = await this.toContract(label, data))
    
    let args = {
      role: 'artist',
      action: 'set_nft',
      collection_id: collid,
      label, data, cid
    }

    if (price) {
      args['aid'] = 0
      args['amount'] = price
    }

    return await utils.invokeContractAsyncAndMakeTx(args)
  }

  async setPrice (id, amount) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'set_price',
      aid: 0,
      amount, id, cid
    })
  }

  async buyNFT (id) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'buy',
      id, cid
    })
  }

  async getSales(id) {
    let {res} = await utils.invokeContractAsync({
      role: 'manager',
      action: 'view_nft_sales',
      id, cid
    })
    return res.sales
  }

  async likeNFT(id) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'vote',
      val: 1,
      id, cid, 
    })
  }

  async unlikeNFT(id) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'vote',
      val: 0,
      id, cid, 
    })
  }

  async transferNFT(id, to) {
    return await utils.invokeContractAsyncAndMakeTx({
      role: 'user',
      action: 'transfer',
      pkNewOwner: to,
      id, cid
    })
  }

  toNewItem(collid) {
    router.push({
      name: 'new-nft',
      params: {
        collid
      }
    })
  }
}

let nftsStore = new NFTSStore()
export default nftsStore