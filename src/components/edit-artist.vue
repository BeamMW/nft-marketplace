<template>
  <div class="container">
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
                   :max_length="40"
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
                    accept="image/jpeg;image/png;image/svg+xml"
                    title="Add an artist banner<br>(*.jpg, *.png, *.svg)"  
                    height="135px"
                    cover
                    :readonly="in_set_artist" 
                    :error="banner_valid ? '' : ' '"
          />
          <img src="~assets/elipse.svg" alt="avatar" class="elipse"/>
          <div class="avatar" :style="avatarStyles" :readonly="in_set_artist">
            <div v-if="avatar && !in_set_artist" class="remove">
              <img src="~assets/remove.svg" alt="remove avatar" @click="onRemoveAvatar"/>
            </div>
            <img v-if="avatar && avatar.object" :src="avatar.object" alt="avatar" class="image" :class="{'error': !avatar_valid}"/>
            <label v-if="!avatar" class="text" :readonly="in_set_artist" for="avatar">Add an artist image</label>
            <input v-if="!in_set_artist" id="avatar"
                   ref="avatar"
                   type="file"
                   accept="image/jpeg;image/png;image/svg+xml"  
                   class="files"
                   @change="onUploadAvatar"
            />
          </div>
          <div v-if="!avatar_valid || !banner_valid" class="error_msg">
            <p class="error">image cannot be larger than 250kb</p>
          </div>
        </div>
      </div>
    </div>
    <p v-if="in_set_artist" class="intx_text">
      Artist info cannot be changed while artist transaction is in progress
    </p>
    <div class="actions">
      <btn text="cancel" @click="$router.go(-1)">
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
</template>

<style scoped lang="stylus">
  .container {

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
        margin-left: 30px

        & > *:not(:last-child) {
          margin-bottom: 20px
        }

        .uploads-container {
          position: relative

          .elipse {
            position: absolute
            left: 50%
            transform: translate(-50%, -30%)
            filter: grayscale(100%) brightness(0%) sepia(100%) hue-rotate(-50deg) saturate(600%) contrast(0.8)
          }

          .avatar {
            display: flex
            align-items: center
            justify-content: center
            height: 120px
            width: 120px
            background-color: rgba(26, 246, 214, 0.1)
            border-radius: 9999px
            position: relative
            left: 50%
            transform: translate(-50%, -26%)
            cursor:pointer

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
        
          .error_msg {
            text-align: center
            margin-top: -16px

            .error {
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
      margin-top: 40px

      & > *:not(:first-child) {
        margin-left: 30px
      }
    }
  }
</style>

<script>
import formInput from 'controls/form-input'
import textArea from 'controls/textarea'
import pageTitle from 'controls/page-title'
import btn from 'controls/button'
import addImage from 'controls/add-image'
import artistsStore from 'stores/artists'
import validators from 'utils/validators'
import router from 'router'

export default {
  components: {
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
      avatar_: undefined
    }
  },

  computed: {
    in_set_artist() {
      return !!artistsStore.artist_tx
    },
    edit_self () {
      return !!(this.id === artistsStore.my_id && artistsStore.is_artist)
    },
    avatarStyles() {
      return {
        'border' :  this.avatar ? '1px dashed transparent' : (this.in_set_artist ? '1px dashed rgba(26, 246, 214, 0.7)' : '1px dashed #1AF6D6'),
      }
    },
    label_valid() {
      let value = this.label
      return !value || value.length <= 100
    },
    website_valid() {
      let value = this.website
      return !value || validators.url(value)
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
      return !this.banner || validators.image(this.banner)
    },
    avatar_valid() {
      return !this.avatar || validators.image(this.avatar)
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
    }
  },

  methods: {    
    loadImage(e, cback) {
      let file = e.target.files[0]
      let reader = new FileReader()
      reader.readAsDataURL(file)
      reader.onload = (e) => {
        cback(e.target.result, file)
      }
    },

    onUploadAvatar(e) {
      this.loadImage(e, (object, file) => {
        this.avatar_ = {object, file}
      })
    },
    
    onRemoveAvatar() {
      this.avatar_ = null
      this.$refs.avatar.value = ''
    },

    async onSetArtist() {
      let data = {
        website:   this.website,
        twitter:   this.twitter,
        instagram: this.instagram,
        about:     this.about,
        avatar:    this.avatar,
        banner:    this.banner
      }
      await artistsStore.setArtist(this.label, data)
      router.go(-1)
    }
  }
}
</script>