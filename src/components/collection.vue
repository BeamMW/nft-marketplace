<template>
  <!--selectItem-->
  <div :class="{'collection': true, 'error': item.error}">
    <preview class="preview" 
             :image="item.cover"
             :default="def_banner"
             height="140px" 
             cover
             @click="onDetails"
    >
      <btn v-if="item.owned" 
           padding="0px" 
           radius="6px" 
           class="btn-edit"
           width="35px"
           height="35px"
           color="rgba(0, 0, 0, 0.6)"
           :hover="false"
           @click="onEdit"
      >
        <img width="18" src="~assets/icon-pencil.svg">
      </btn>
      <moderationStatus :item="item"/>
    </preview>
    <div class="info-row">  
      <div class="avatar" :class="{'error': item.author_error}">
        <preview :image="item.avatar" 
                 :show_text="false"
                 :default="def_avatar"
                 width="72px" 
                 height="72px" 
                 radius="36px 36px"
        />
      </div>
      <div class="text">
        <div class="label" :class="{'error': item.error}">{{ coll_name }}</div>
        <div class="author" :class="{'error': item.author_error}" v-html="item.by_author"></div>
        <div class="description">{{ item.description }}</div>
        <hr class="line"/>
        <div class="items-info" :class="{'error': item.error}">
          <div class="count">
            <div class="text">{{ item.artworks_count }}</div>
            <div>{{ item.artworks_count == 1 ? 'item' : 'items' }}</div>
          </div>
          <amount :amount="item.total_sold_price" size="12px" info="trade volume" class="icon_styles"/>
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

    & > .preview {
      .btn-edit {
        position: absolute
        bottom: 8px
        right: 10px
      }
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
import preview from './image-preview'
import amount from './amount'
import collsStore from 'stores/collections'
import {def_images} from 'utils/consts'
import btn from './button'
import moderationStatus from './moderation-status'

export default {
  components: {
    preview,
    amount,
    btn,
    moderationStatus
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  data () {
    return {
      def_avatar: def_images.artist_avatar,
      def_banner: def_images.artist_banner
    }
  },

  computed: {
    coll_name () {
      return [this.$state.debug ? `[${this.item.id}] - ` : '', this.item.label].join('')
    }
  },

  methods: {
    onEdit (ev) {
      if (!this.item.owned || this.item.error) {
        return
      }
      collsStore.toEditItem(this.item.id)
      ev.stopPropagation()
    },
    onDetails (ev) {
      collsStore.toDetails(this.item.id)
    }
  }
}
</script>