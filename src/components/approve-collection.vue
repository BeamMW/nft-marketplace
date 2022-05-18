<template>
  <div class="approve-collection" :class="{'error': item.error}">
    <div class="row">
      <div v-if="item.error" class="info-error">
        Failed to load Collection<br>Collection ID: {{ item.id }}<br>{{ item.error }}
      </div>
      <div v-else class="info">
        <div class="type">Name</div>
        <div class="value">{{ item.label }}</div>
        <div v-if="item.website" class="type">Website</div>
        <div v-if="item.website" class="value"><span class="link" @click="onWebsite">{{ item.website }}</span></div>
        <div v-if="item.twitter" class="type">Twitter</div>
        <div v-if="item.twitter" class="value"><span class="link" @click="onTwitter">{{ item.twitter }}</span></div>
        <div v-if="item.instagram" class="type">Instagram</div>
        <div v-if="item.instagram" class="value"><span class="link" @click="onInstagram">{{ item.instagram }}</span></div>
      </div>
      <imagePreivew :image="item.cover" 
                    :default="def_cover"
                    radius="10px"
                    width="348px"
                    height="108px"
                    cover
      >
        <moderationStatus :item="item" short/>
      </imagePreivew>
    </div>
    <div v-if="item.description" class="info">
      <div class="type">Description</div>
      <div class="value">{{ item.description }}</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.approve-collection {
  width: 912px
  background-color: rgba(255, 255, 255, 0.05)
  border-radius: 10px
  box-sizing: border-box
  padding: 20px 20px 20px 70px
  display:flex
  flex-direction: column
  font-size: 14px

  & .row {
    display: flex
    flex-direction: row
  }

  & .info-error {
    flex: 1
  }

  & .info {
    flex: 1
    display: grid
    margin-right: 20px
    grid-template-columns: 100px auto
    align-content: start

    & > .type {
      color: #8da1ad
      margin-bottom: 16px
      user-select: none
    }

    & > .value {
      color: #fff

      & > .link {
        cursor: pointer

        &:hover {
          text-decoration: underline
        }
      }
    }
  }
}
</style>

<script>
import imagePreivew from './image-preview.vue'
import moderationStatus from './moderation-status.vue'
import {def_images} from 'utils/consts'

export default {
  components: {
    imagePreivew,
    moderationStatus
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  emits: [
    'selected'
  ],

  data () {
    return {
      def_cover: def_images.artist_banner
    }
  },

  methods: {
    onWebsite(ev) {
      ev.stopPropagation()
      alert('site')
    },
    onTwitter(ev) {
      ev.stopPropagation()
      alert('twitter')
    },
    onInstagram(ev){
      ev.stopPropagation()
      alert('instagram')
    }
  }
}
</script>
