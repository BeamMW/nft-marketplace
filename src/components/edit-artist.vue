<template>
  <messageModal ref="messageModal"/>
  <div class="edit-artist-container">
    <template v-if="edit_self">
      <pageTitle title="Edit your Artist"/>
      <p class="description">
        <i>After your account is changed it would not be visible until reviewed by a moderator.<br>
          NFTs from this collection would still appear in the gallery.
        </i>
      </p>
    </template>
    <template v-else>
      <pageTitle title="Become an Artist"/>
      <p class="description">
        To become a publisher you need to set up a username.<br>
        Registration will allow you to publish and manage your NFTs.<br>
        <i>After your account is created it would not be visible until reviewed by a moderator.</i>
      </p>
    </template>
    <div class="scrollable">
      <div class="fields">
        <div class="col-first">
          <formInput v-model="label"
                     label="Artist Name*"
                     :valid="label_valid"
                     :max_length="25"
                     :readonly="edit_self || in_set_artist"
                     style="margin-bottom:58px;margin-top:0"
          />
          <formInput v-model="website"
                     label="Website"
                     placeholder="https://website.name/"
                     img="globe"
                     :max_length="150"
                     :valid="website_valid"
                     :readonly="in_set_artist"
          />
          <formInput v-model="twitter"
                     label="Twitter"
                     placeholder="twitter"
                     img="twitter"
                     :max_length="15"
                     :valid="twitter_valid"
                     :readonly="in_set_artist"
                     :counter="false"
                     :allowed="twitter_allowed"
          />
          <formInput v-model="instagram"
                     label="Instagram"
                     placeholder="instagram"
                     img="instagram"
                     :max_length="30"
                     :valid="instagram_valid"
                     :readonly="in_set_artist"
                     :counter="false"
                     :allowed="instagram_allowed"
          />
        </div>
        <div class="col-second">
          <textArea v-model="about"
                    label="About me"
                    :valid="about_valid"
                    :max_length="75"
                    :readonly="in_set_artist"
          />
          <div class="uploads-container" style="margin-top:45px;">
            <addImage v-model="banner"
                      v-model:error="banner_error"
                      accept="image/jpeg,image/png,image/svg+xml"
                      title="Add an artist banner<br>(*.jpg, *.png, *.svg)<br>&nbsp;"  
                      height="135px"
                      cover
                      :show_error="false"
                      :min_width="1400"
                      :min_height="260"
                      :readonly="in_set_artist" 
            />
            <div class="ellipse" :style="ellipse_style">
              <addImage v-model="avatar"
                        v-model:error="avatar_error"
                        accept="image/jpeg,image/png,image/svg+xml"
                        title="Add an artist image<br>(*.jpeg, *.png, *.svg)"
                        height="136px"
                        width="136px"
                        cover
                        rounded
                        :show_error="false"
                        :min_width="136"
                        :min_height="136"
                        :readonly="in_set_artist" 
              />  
            </div>
            <div v-if="!avatar_valid || !banner_valid" class="error_msg">
              <p class="error">{{ banner_error || avatar_error }}</p>
            </div>
          </div>
        </div>
      </div>
      <p v-if="in_set_artist" class="intx_text">
        Artist info cannot be changed while artist transaction is in progress
      </p>
      <div class="actions">
        <btn text="cancel" @click="$store.toBack()">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn :text="edit_self ? 'update artist' : 'create account'" 
             color="green" 
             :disabled="!can_submit || in_set_artist" 
             @click="onSetArtist"
        >
          <img src="~assets/create.svg"/>
        </btn>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .edit-artist-container {
    width: 100%
    height: 100%
    display: flex
    flex-direction: column

    .description {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 10px 0px 30px 0px
      font-family: 'SFProDisplay', sans-serif

      & > i {
        opacity: 0.7
        display: block
        margin-top: 6px
      }
    }

    .intx_text {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 30px 0px 0px 0px
      font-family: 'SFProDisplay', sans-serif
    }

    .scrollable {
      overflow-y: overlay
      overflow-x: hidden
      flex: 1

      .fields {
        padding: 0 30px
        display: flex

        .col-first {
          flex-basis: 50%

          & > *:not(:last-child) {
            margin-bottom: 20px
          }
        }

        .col-second {
          flex-basis: 50%
          padding-left: 30px
          height: 420px

          & > *:not(:last-child) {
            margin-bottom: 20px
          }

          .uploads-container {
            position: relative

            .ellipse {
              border-radius: 74px
              width: 150px
              height: 150px
              position: relative
              left: 50%
              transform: translate(-50%, -30%)
              display: flex
              justify-content: center
              align-items: center

              .avatar {
                height: 136px
                width: 136px
                background-color: rgba(26, 246, 214, 0.1)
                border-radius: 68px

                .text {
                  height: 100%
                  display: flex
                  justify-content: center
                  align-items: center
                  font-size: 14px
                  color: #1af6d6
                  cursor: pointer

                  &[readonly] {
                    opacity: 0.6
                  }
                }

                &[readonly] {
                  opacity: 0.6
                }

                .remove {
                  background-color: rgba(0, 0, 0, 0.7)
                  position: absolute
                  left: 50%
                  top: 50%
                  transform: translate(-50%,-50%)
                  z-index: 2
                  border-radius: 9999px
                  padding: 7px 7px 3px 7px
                  cursor: pointer
                }

                .files {
                  visibility:hidden
                  width: 0
                }
              
                .image {
                  width: 100%
                  height: 100%
                  object-fit: cover
                  border-radius: 9999px
                  border: 1px dashed transparent
                }
              }
            }
          
            .error_msg {
              text-align: center
              margin-top: -42px
              margin-bottom: 26px

              & > p {
                font-style: italic
                font-size: 12px
                font-weight: 400
                margin: 0
              }
            }
          }
        }
      }

      .actions {
        display:flex
        justify-content: center
        margin-top: 30px
        margin-bottom: 10px

        & > *:not(:first-child) {
          margin-left: 30px
        }
      }
    }
  }
</style>

<script>
import messageModal from 'components/message-modal'
import formInput from 'controls/form-input'
import textArea from 'controls/textarea'
import pageTitle from 'controls/page-title'
import btn from 'controls/button'
import addImage from 'controls/add-image'
import artistsStore from 'stores/artists'
import validators from 'utils/validators'
import utils from 'utils/utils'
import {common} from 'utils/consts'

export default {
  components: {
    messageModal,
    formInput, 
    textArea, 
    pageTitle,
    addImage,
    btn
  },

  props: {
    id: {
      type: String,
      required: false,
      default: ''
    }
  },

  data () {
    return {
      label_: undefined,
      website_: undefined,
      twitter_: undefined,
      instagram_: undefined,
      about_: undefined,
      banner_: undefined,
      banner_error: undefined,
      avatar_: undefined,
      avatar_error: undefined
    }
  },

  computed: {
    in_set_artist() {
      return !!artistsStore.artist_tx
    },
    edit_self () {
      return !!(this.id === artistsStore.my_id && artistsStore.is_artist)
    },
    avatar_style() {
      return {
        'border' :  this.avatar ? '1px dashed transparent' : (this.in_set_artist ? '1px dashed rgba(26, 246, 214, 0.7)' : '1px dashed #1AF6D6'),
      }
    },
    ellipse_style() {
      return {
        'background-color': utils.getStyles().background_main
      }
    },
    label_valid() {
      let value = this.label
      return !value || value.length <= 25
    },
    website_valid() {
      let value = this.website
      return !value || (value.length <= 150 && validators.url(value))
    },
    twitter_allowed() {
      return validators.twitter_allowed()
    },
    twitter_valid() {
      let value = this.twitter
      return !value || validators.twitter(value)
    },
    instagram_allowed() {
      return validators.instagram_allowed()
    },
    instagram_valid() {
      let value = this.instagram
      return !value || validators.instagram(value)
    },
    about_valid() {
      let value = this.about
      return !value || value.length <= 75
    },
    banner_valid() {
      return !this.banner || !this.banner_error
    },
    avatar_valid() {
      return !this.avatar || !this.avatar_error
    },
    can_submit () {
      return this.label && this.label_valid &&
             this.website_valid &&
             this.twitter_valid &&
             this.instagram_valid &&
             this.about_valid &&
             this.banner_valid &&
             this.avatar_valid
    },
    artist () {
      return this.id ? artistsStore.artists[this.id] : undefined
    },
    label: {
      get () {
        return this.label_ != undefined ? this.label_ : (this.artist || {}).label
      },
      set (val) {
        console.log('set label: ', val)
        this.label_ = val
      }
    },
    website: {
      get () {
        return this.website_ != undefined ? this.website_ : (this.artist || {}).website
      },
      set (val) {
        this.website_ = val
      }
    },
    twitter: {
      get () {
        return this.twitter_ != undefined ? this.twitter_ : (this.artist || {}).twitter
      },
      set (val) {
        this.twitter_ = val
      }
    },
    instagram: {
      get () {
        return this.instagram_ != undefined ? this.instagram_ : (this.artist || {}).instagram
      },
      set (val) {
        this.instagram_ = val
      }
    },
    about: {
      get () {
        return this.about_ != undefined ? this.about_ : (this.artist || {}).about
      },
      set (val) {
        this.about_ = val
      }
    },
    banner: {
      get () {
        if (this.banner_ === null) {
          return undefined
        }
        return this.banner_ || (this.artist || {}).banner
      },
      set (val) {
        this.banner_ = val
      }
    },
    avatar: {
      get () {
        if (this.avatar_ === null) {
          return undefined
        }
        return this.avatar_ || (this.artist || {}).avatar
      },
      set (val) {
        this.avatar_ = val
      }
    },
    max_image_size() {
      return utils.formatBytes(common.MAX_IMAGE_SIZE)
    },
  },

  methods: {    
    async onSetArtist() {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 

      try {
        let data = {
          website:   this.website,
          twitter:   this.twitter,
          instagram: this.instagram,
          about:     this.about,
          avatar:    this.avatar,
          banner:    this.banner
        }        
        let txid = await artistsStore.setArtist(this.label, data)
        if (!txid) {
          // user cancelled
          return
        }
      }
      catch(err) {
        if (err.error === 'label already exists') {
          this.$refs.messageModal.open(
            'Duplicate artist name',
            'Artist with the same name already exists.<br>Choose another name and retry.',
            true)
          return
        }
        throw err
      }
      this.$store.toBack()
    }
  }
}
</script>