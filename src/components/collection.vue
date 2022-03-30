<template>
  <div class="collection" @click="$store.toCollectionDetails(item.id)">
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
      </div>
    </div>
  </div>
</template>
    
<style scoped lang="stylus">
  .collection {
    margin-right: 20px
    margin-bottom: 20px
    width: 431px
    border: none
    background-color: rgba(240, 205, 205, 0.05)
    border-radius: 10px
    cursor: pointer

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
      }
    }
  }
</style>

<script>
import preview from './image-preview.vue'

export default {
  components: {
    preview
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  }
}
</script>