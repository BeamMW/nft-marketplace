<template>
  <selectItem :selected="selected" @select-item="onSelectItem">
    <div :class="
           {'collection': true, 
            'pointer-cursor': item.owned && !item.default,
            'error': item.error
           }" 
         @click="onDetails"
    >
      <preview class="preview" 
               :image="item.cover" 
               height="140px" 
               cover
      />
      <div class="info-row">  
        <div class="avatar" :class="{'error': item.author_error}">
          <preview :image="item.avatar" 
                   :show_loading="false"
                   width="72px" 
                   height="72px" 
                   radius="36px 36px"
          />
        </div>
        <div class="text">
          <div class="label" :class="{'error': item.author_error && item.default}">{{ item.label }}</div>
          <div class="author" :class="{'error': item.author_error}">{{ item.author_name }}</div>
          <div class="description">{{ item.description }}</div>
          <hr class="line"/>
          <div class="items-info">
            <div class="count">
              <div class="text">4</div>
              <div>Items</div>
            </div>
            <amount :amount="400000" size="12px" :info="true" class="icon_styles"/>
          </div>
        </div>
      </div>
      <div class="text">
        <div class="label" :class="{'error': item.author_error && item.default}">{{ coll_name }}</div>
        <div class="author" :class="{'error': item.author_error}">by <span>{{ item.author_name }}</span></div>
        <div class="description">{{ item.description }}</div>
        <hr class="line"/>
        <div class="items-info">
          <div class="count">
            <div class="text">4</div>
            <div>items</div>
          </div>
          <amount :amount="400000" size="12px" info="trade volume" class="icon_styles"/>
        </div>
      </div>
    </div>
  </selectItem>
</template>
    
<style scoped lang="stylus">
  .collection {
    width: 442px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px

    & > .preview {
      height: 140px
    }

    & > .info-row {
      display: flex
      flex-direction: row
      padding: 10px 16px 10px 20px

      & > .avatar {
        margin-right: 18px
      }

      & > .text {
        font-family: 'SFProDisplay', sans-serif
        flex: 1
        
        & > .label {
          font-weight: 700
          font-size: 16px
        }

        & > .author {
          font-size: 14px

          & > span {
            color: #00f6d2
          }
        }

        & > .description {
          font-size: 12px
          color: rgba(255, 255, 255, 0.5)
          margin-top: 6px
          display: -webkit-box
          -webkit-line-clamp: 3
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
              font-weight: 500
              color: #24c1ff
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
import preview from './image-preview.vue'
import amount from './amount.vue'
import collsStore from 'stores/collections'
import amount from './amount.vue'
import selectItem from './select-item.vue'
export default {
  components: {
    preview,
    amount
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },
  data: function () {
    return {
      selected: false
    }
  },

  computed: {
    coll_name () {
      return [this.$state.debug ? `[${this.item.id}] - ` : '', this.item.label].join('')
    }
  },

  methods: {
    onDetails () {
      if (!this.item.owned || this.item.default || this.item.error) {
        return
      }
      collsStore.toEditItem(this.item.id)
    },
    onSelectItem() { 
      this.selected = !this.selected

    }
  }
}
</script>