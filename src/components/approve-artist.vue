<template>
  <div class="approve-artist" :class="{'error': item.error}">
    <div v-if="item.error" class="info-error">
      Failed to load Artist<br>Artist ID: {{ item.id }}<br>{{ item.error }}
    </div>
    <div v-else class="row">
      <div class="type">
        <div>Name</div>
        <div v-if="item.website">Website</div>
        <div v-if="item.twitter">Twitter</div>
        <div v-if="item.instagram">Instagram</div>
        <div v-if="item.about">About</div>
      </div>
      <div class="info">
        <div class="images">
          <preview class="banner"
                   :image="item.banner" 
                   :default="def_banner"
                   radius="10px"
                   width="432px"
                   height="135px"
                   cover
          />
          <div class="ellipse" :style="ellipse_style">
            <preview :image="item.avatar" 
                     :default="def_avatar"
                     width="120px"
                     height="120px"
                     radius="120px"
            />
          </div>
        </div>
        <div>{{ item.label }}</div>
        <!-- do not remove space and &nbsp; or clamp would not work -->
        <div v-if="item.website" class="link clamp" @click="onWebsite">{{ item.website }} &nbsp;</div>
        <div v-if="item.twitter" class="link" @click="onTwitter">@{{ item.twitter }}</div>
        <div v-if="item.instagram" class="link" @click="onInstagram">@{{ item.instagram }}</div>
        <div v-if="item.about">{{ item.about }}</div>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.approve-artist {
  background-color: rgba(255, 255, 255, 0.05)
  border-radius: 10px
  box-sizing: border-box
  padding: 20px 20px 4px 70px
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
      min-width: 0
      flex: 1

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

      & > .images {
        float: right
        margin: 0px 0px 0px 15px !important
        height: 220px
        position: relative

        .ellipse {
          position: absolute
          left: 164px
          top: 81px
          border-radius: 74px
          width: 132px
          height: 132px
          background-color: orange
          display: flex
          justify-content: center
          align-items: center
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
    preview
  },

  props: {
    item: {
      type: Object,
      required: true,
    }
  },

  data () {
    return {
      def_banner: def_images.artist_banner,
      def_avatar: def_images.artist_avatar
    }
  },

  computed: {
    ellipse_style() {
      return {
        'background-color': '#232323'
      }
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