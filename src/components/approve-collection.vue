<template>
  <div class="approve-collection" :class="{'error': item.error}">
    <div v-if="item.error" class="info-error">
      Failed to load Collection<br>Collection ID: {{ item.id }}<br>{{ item.error }}
    </div>
    <div v-else class="row">
      <div class="type">
        <div>Artist</div>
        <div>Name</div>
        <div v-if="item.website">Website</div>
        <div v-if="item.twitter">Twitter</div>
        <div v-if="item.instagram">Instagram</div>
        <div v-if="item.description">Description</div>
      </div>
      <div class="info">
        <preview :image="item.cover" 
                 :default="def_cover"
                 radius="10px"
                 width="348px"
                 height="108px"
                 cover
                 style="float: right; margin-left: 15px; margin-bottom: 10px;"
        >
        </preview>
        <div>{{ item.author_name }}</div>
        <div>{{ item.label }}</div>
        <!-- do not remove space and &nbsp; or clamp would not work -->
        <div v-if="item.website" class="link clamp" @click="onWebsite">{{ item.website }} &nbsp;</div>
        <div v-if="item.twitter" class="link" @click="onTwitter">@{{ item.twitter }}</div>
        <div v-if="item.instagram" class="link" @click="onInstagram">@{{ item.instagram }}</div>
        <div v-if="item.description">{{ item.description }}</div>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.approve-collection {
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

    & > .type {
      width: 100px
     
      & > div {
        color: #8da1ad
        user-select: none
        display: inline-block
        width: 100px

        &:not(:last-child) {
          margin-bottom: 12px
        }
      }
    }

    & > .info {
      display: block
      width: 100%
      min-width: 0

      & > div {
        &:not(:last-child) {
          margin-bottom: 12px
        }

        &.link {
          cursor: pointer
          color: rgba(0, 246, 210, 1)
          
          &.clamp {
            display: -webkit-box
            -webkit-line-clamp: 1
            -webkit-box-orient: vertical
            overflow: hidden
          }

          &:hover {
            text-decoration: underline
          }
        }
      }
    }
  }

  & .info-error {
    flex: 1
  }
}
</style>

<script>
import preview from 'controls/preview'
import {def_images} from 'utils/consts'

export default {
  components: {
    preview,
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
      this.$store.openUrl(this.item.website)
    },
    onTwitter(ev) {
      ev.stopPropagation()
      this.$store.openTwitter(this.item.twitter)
    },
    onInstagram(ev){
      ev.stopPropagation()
      this.$store.openInstagram(this.item.instagram)
    }
  }
}
</script>
