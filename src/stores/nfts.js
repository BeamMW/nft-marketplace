import LazyItems from 'stores/lazy-items'
import imagesStore from 'stores/images'
import artistsStore from 'stores/artists'
import formats from 'stores/formats'
import {versions, cid} from 'stores/consts'
import {computed} from 'vue'
import utils from 'utils/utils'
import router from 'router'
import likesStore from './likes'

//
// Custom modes
//   user:collection     -> keys: collection, status
//   artist:collection   -> keys: collection
//   user:liked          -> keys: [status+liked]
//   liker:liked         -> keys: [satus+my_like]
class NFTSStore extends LazyItems {
  constructor () {
    super({
      objname: 'nft', 
      versions: [versions.NFT_VERSION],
      modes: [
        'moderator', 
        'user', 
        'user:sale', 
        'owned', 
        'owned:sale', 
        'artist:sold', 
      ],
      extraDBKeys: [
        'collection',
        '[status+liked]',
        '[status+my_like]'
      ]
    })
    
    this._current_coll_mode = undefined
    this._coll_modes = [
      {name: 'artist:collection:liked:',
        make_loader(collid) {
          return (store) => {
            console.log(`loader for artist:collection:liked:${collid}`)
            return store
              .where({'collection': collid, 'liked': 1})
          }
        }
      }, 
      {name: 'artist:collection:sale:', 
        make_loader(collid) {
          return (store) => {
            console.log(`loader for artist:collection:sale:${collid}`)
            return store
              .where({'collection': collid, 'sale': 1})
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

  defaultStatus() {
    let status = super.defaultStatus()
    return Object.assign(status, {
      'approved_liked': 0, 
      'approved_i_liked': 0
    })
  }

  reset (global, db) {
    super.reset(global, db)

    this._allocMode({
      mode: 'user:liked', 
      loader: (store) => {
        console.log(`loader for user:liked:${this._store_name}`)
        return store
          .where(['status', 'liked'])
          .equals(['approved', 1])
      }
    })

    this._allocMode({
      mode: 'liker:liked', 
      loader: (store) => {
        console.log(`loader for liker:liked:${this._store_name}`)
        return store
          .where(['status', 'my_like'])
          .equals(['approved', 1])
      }
    })
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
      if (author.approved_cnt) return `by <span style="color:#00f6d2">${author.label}</span>`
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
      if (awork.approved) return awork.description
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

  async onStart(status, items) {
    await super.onStart(status, items)
    
    let ids = items.map(item => item.id)
    let rawLikes = await likesStore.getLikes(ids)

    let likes = {}
    for(let idx in ids) {
      let id = ids[idx]
      likes[id] = rawLikes[idx]
    }

    status.likes = likes
  }

  async onItem(status, item) {
    let old = await super.onItem(status, item)

    item.my_like = (status.likes[item.id] || {}).value ? 1 : 0
    item.liked = (item.likes > 0) ? 1 : 0

    //
    // Approved and ...
    //
    if (old && old.approved) {
      //
      // ... and liked by someone
      //
      if (old.likes > 0) {
        status.approved_liked--
      }

      //
      // ... and liked by me
      //
      if (old.my_like) {
        status.approved_i_liked--
      }
    }

    if (item.approved) {
      //
      // ... and liked by someone
      //
      if (item.likes > 0) {
        status.approved_liked++
      }

      //
      // ... and liked by me
      //
      if (item.my_like) {
        status.approved_i_liked++
      }
    }

    return old
  }

  async onEnd(status) {
    await super.onEnd(status)
    this._getMode('user:liked').total  = status.approved_liked
    this._getMode('liker:liked').total = status.approved_i_liked
    delete status.likes
  }

  async detectOwned(item) {
    if (utils.isDesktop()) {
      return item.owned
    }

    utils.ensureField(item, 'owner', 'string')
    let {res} = await utils.invokeContractAsync({
      role: 'user',
      action: 'is_my_key',
      nft_id: item.id,
      key: item.owner,
      cid
    })
    
    utils.ensureField(res, 'key', 'number')
    return res.key
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
      new_owner: to,
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