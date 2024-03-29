<template>
  <!--selectItem-->
  <div :class="{'collection': true, 'error': item.error}" @click="onDetails" @contextmenu="onMenu">
    <moderationMenu ref="moderationMenu" :item="item"/>
    <preview :image="item.cover"
             :default="def_cover"
             height="140px" 
             cover
    >
      <div v-if="error && debug" class="debug error">{{ error }}</div>
      <btnEdit :item="item"/> 
      <moderationStatus :item="item"/>
    </preview>
    <div class="info-row">  
      <div class="avatar" :class="{'error': item.author_error}">
        <preview :image="avatar" 
                 :show_text="false"
                 :default="def_avatar"
                 width="72px" 
                 height="72px" 
                 radius="36px 36px"
        />
      </div>
      <div class="text">
        <div class="label" :class="{'error': item.error}">{{ coll_name }}</div>
        <div class="author" :class="{'error': item.author_error}" v-html="by_author"></div>
        <div class="description">{{ item.description }}</div>
        <hr class="line"/>
        <div class="items-info" :class="{'error': item.error}">
          <div class="count">
            <div class="text">{{ nfts_count }}</div>
            <div>{{ nfts_count == 1 ? 'NFT' : 'NFTs' }}</div>
          </div>
          <amount :amount="trade_volume" size="12px" info="trade volume" class="icon_styles"/>
        </div>
      </div>
    </div>
  </div>
  <!-- /selectitem -->
</template>
    
<style scoped lang="stylus">
  .collection {
    width: 442px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px
    position: relative
    overflow: hidden
    cursor: pointer

    & .debug {
      text-align: center
      color: black
      padding: 0px 5px
    }

    & > .info-row {
      display: flex
      flex-direction: row
      padding: 10px 16px 10px 20px

      & > .avatar {
        margin-right: 18px
      }

      & > .text {
        flex: 1
        
        & > .label {
          font-weight: 700
          font-size: 16px
          height: 22px
        }

        & > .author {
          font-size: 14px
        }

        & > .description {
          font-size: 12px
          color: rgba(255, 255, 255, 0.5)
          margin-top: 6px
          height: 32px
          display: -webkit-box
          -webkit-line-clamp: 2
          -webkit-box-orient: vertical
          overflow: hidden
        }

        & > .line {
          height: 1px
          opacity: 0.1
          background-color: white
          border: none
        }

        & > .items-info {
          display:flex
          font-size: 12px
          margin-top: 10px
          
          .count {
            display: flex
            flex-direction: column
            flex-basis: 20%
            
            & > .text {
              font-weight: bold
              color: white
            }
          }

          .icon_styles {
            height: 18px
          }
        }
      }
    }
  }
</style>

<script>
import preview from 'controls/preview'
import amount from 'controls/amount'
import moderationStatus from 'controls/moderation-status'
import btnEdit from 'controls/btn-edit-collection'
import collsStore from 'stores/collections'
import artistsStore from 'stores/artists'
import {def_images} from 'utils/consts'
import moderationMenu from 'controls/moderation-menu'

export default {
  components: {
    preview,
    amount,
    btnEdit,
    moderationStatus,
    moderationMenu
  },

  props: {
    item: {
      type: Object,
      required: true,
    },
    mode: {
      type: String,
      default: 'user'
    }
  },

  data () {
    return {
      def_avatar: def_images.artist_avatar,
      def_cover: def_images.artist_banner
    }
  },

  computed: {
    coll_name () {
      return this.$state.debug ? `[${this.item.id}] - ${this.item.label}` : this.item.label
    },
    avatar() {
      return this.mode === 'artist' && this.item.author === artistsStore.my_key ? this.item.avatar : this.item.safe_avatar
    },
    by_author() {
      return this.mode === 'artist' && this.item.author === artistsStore.my_key ? this.item.by_author : this.item.safe_by_author
    },
    nfts_count() {
      return (this.mode === 'artist' ? this.item.nfts_count : this.item.approved_nfts_count) || 0
    },
    debug() {
      return this.$state.debug
    },
    error() {
      return this.item.error || this.item.author_error
    },
    trade_volume() {
      return this.item.total_sold.volume
    }
  },

  methods: {
    onDetails (ev) {
      collsStore.toDetails(this.item.id, this.mode)
    },
    onMenu (ev) {
      ev.stopPropagation()
      this.$refs.moderationMenu.open(ev)
    }
  }
}
</script>