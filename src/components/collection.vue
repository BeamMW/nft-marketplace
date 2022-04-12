<template>
  <div class="collection">
    <preview class="preview" :image="item.cover_image" height="140px"/>
    <div class="info-row">  
      <div class="avatar">
        <preview :image="item.author_image" 
                 :show_loading="false"
                 width="72px" 
                 height="72px" 
                 radius="36px 36px"
        />
      </div>
      <div class="text">
        <div class="label">{{ item.label }}</div>
        <div class="author">{{ item.author }}</div>
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
  </div>
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
      padding: 16px 16px 20px 20px

      & > .avatar {
        margin-right: 16px
      }

      & > .text {
        font-family: 'SFProDisplay', sans-serif
        
        & > .label {
          font-weight: 700
          font-size: 16px
        }

        & > .author {
          margin-top: 4px
          font-size: 14px
        }

        & > .description {
          font-size: 12px
          color: rgba(255, 255, 255, 0.5)
          margin-top: 10px
          display: -webkit-box
          -webkit-line-clamp: 3
          -webkit-box-orient: vertical
          overflow: hidden
        }
        & > .items-info {
          display:flex
          font-size: 12px
          
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
  }
}
</script>