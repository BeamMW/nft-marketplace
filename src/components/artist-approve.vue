<template>
  <div class="container">
    <div v-for="artist in artists" id="container" :key="artist.id" class="content-wrapper">
      <selectItem :place="{
        left: '20px',
        top: '20px',
        right: 'initial',
      }"
      >
        <div class="details-row"> 
          <div class="info-container">
            <div class="col-first">
              <p v-if="artist.label">Artist name</p>
              <p v-if="artist.webiste">Website</p>
              <p v-if="artist.twitter">Twitter</p>
              <p v-if="artist.instagram">Instagram</p>
              <p v-if="artist.about">About me</p>
            </div>
            <div class="col-second">
              <p>{{ artist.label }}</p>
              <p>{{ artist.webiste }}</p>   
              <p>{{ artist.twitter }}</p>
              <p>{{ artist.instagram }}</p>
              <p>{{ artist.about }}</p>
            </div>
          </div>
          <div class="artist-container">
            <div class="banner">
              <img v-if="artist.banner?.object" :src="artist.banner?.object" alt="banner" class="image"/>
              <img v-else src="~assets/artist-default-banner.svg" alt="banner" class="image"/>
              <div class="avatar">
                <img v-if="artist.banner?.object" :src="artist.avatar?.object" alt="avatar"/>
                <img v-else src="~assets/artist-default-avatar.svg" alt="avatar"/>
              </div>
            </div>
          </div>
        </div>
      </selectItem>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    padding-top: 15px
    width: 100%
    height: 100%

    & .content-wrapper {
      display: flex
      flex-direction: column
      flex: 1

      & .details-row {
        box-sizing: border-box
        display: flex
        flex-direction: row
        background-color: rgba(255, 255, 255, 0.05)
        border-radius: 10px
        height: 241px
        padding: 23px 60px 23px 70px
        margin-bottom: 10px

        & .info-container {
          flex: 0 0 44%
          box-sizing: border-box
          max-width: 50%
          display: flex
          font-family: 'SFProDisplay', sans-serif
          font-style: normal
          font-weight: 400
          font-size: 14px
          line-height: 17px
          margin-right: 10px

          & > .col-first {
            margin-right: 24px

            & > p {
              white-space: nowrap
              color: rgba(255,255,255, 0.5)
            }
          }

          & > .col-second {
            & > p {
              color: rgba(255,255,255, 1)
            }

            & > p:last-child {
              display: -webkit-box
              -webkit-line-clamp: 3
              -webkit-box-orient: vertical
              overflow: hidden
            }
          }
        }

        & .artist-container {
          flex: 0 0 56%
          box-sizing: border-box
          max-width: 56%
          position: relative

          .banner {
            display: flex
            align-items: start
            justify-content: center
            height: 100%

            .image {
              width: 100%
              height: 135px
              object-fit: cover
              border-radius: 10px
              border: 1px dashed transparent
            }

            .avatar {
              display: flex
              align-items: center
              justify-content: center
              height: 120px
              width: 120px
              background-color: rgba(26, 246, 214, 0.1)
              border-radius: 9999px
              position: absolute
              left: 50%
              top: 56%
              transform: translate(-50%, -26%)

              & > img {
                width: 100%
                height: 100%
                object-fit: cover
                border-radius: 9999px
                border: 1px dashed transparent
              }
            }
          }
        }
      }
    }
  }
</style>

<script>
import artistsStore from 'stores/artists'
import selectItem from './select-item.vue'

export default {
  components: {
    selectItem,
  },

  data () {
    return {
      defaultBannerSrc: '~assets/artist-default-banner.svg',
      defualtAvatarSrc: '~assets/artist-default-avatar.svg'
    }
  },

  computed: {
    artists() {
      return artistsStore.artists
    }
  },
}
</script>