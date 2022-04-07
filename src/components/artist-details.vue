<template>
  <div class="container">
    <template v-if="edit_self">
      <pageTitle title="Edit your Artist Info"/>
      <p class="description">
        After your artist information is changed it would not be visible<br>
        until reviewed by moderator. NFTs would still appear in gallery.<br>
      </p>
    </template>
    <template v-else>
      <pageTitle title="Become an Artist"/>
      <p class="description">
        To become a publisher you need to set up a username.<br>
        Registration will allow you to publish and manage your NFTs.
      </p>
    </template>
    <div class="fields">
      <div class="col-first">
        <inputField v-model="label"
                    label="Artist Name*"
                    :valid="label_valid"
                    :max_length="100"
                    :readonly="edit_self || in_set_artist"
                    style="margin-bottom:60px;margin-top:0"
        />
        <inputField v-model="website"
                    label="Website"
                    placeholder="https://website.name/"
                    img="glob"
                    :max_length="250"
                    :valid="website_valid"
                    :readonly="in_set_artist"
        />
        <inputField v-model="twitter"
                    label="Twitter"
                    placeholder="@twitter"
                    img="twitter"
                    :max_length="16"
                    :valid="twitter_valid"
                    :readonly="in_set_artist"
        />
        <inputField v-model="instagram"
                    label="Instagram"
                    placeholder="@instagram"
                    img="instagram"
                    :max_length="31"
                    :valid="instagram_valid"
                    :readonly="in_set_artist"
        />
      </div>
      <div class="col-second">
        <textAreaField v-model="about"
                       label="About me"
                       :valid="about_valid"
                       :max_length="150"
                       :readonly="in_set_artist"
        />
        <div class="uploads-container">
          <addImage v-model:image="banner"
                    title="Add an artist banner"
                    accept="image/jpeg, image/png, image/svg+xml"      
                    :readonly="in_set_artist" 
                    :error="banner_valid ? '' : 'image cannot be larger than 250kb'"
          />
          <img src="~/assets/elipse.svg" alt="avatar" class="elipse"/>
          <div class="container-avatar">
            <div class="avatar" :style="avatarStyles" :readonly="in_set_artist">
              <div v-if="avatar" class="remove">
                <img src="~/assets/remove.svg" alt="remove avatar" @click="onRemoveAvatar"/>
              </div>
              <img v-if="avatar" :src="avatar.object" alt="avatar" class="image" :class="{'error': !avatar_valid}"/>
              <label v-if="!avatar" class="text" :readonly="in_set_artist" for="avatar">Add an artist image</label>
              <input v-if="!in_set_artist" id="avatar"
                     ref="avatar"
                     type="file"
                     accept="image/apng, image/avif, image/gif, image/jpeg, image/png, image/svg+xml, image/webp"      
                     class="files"
                     @change="onUploadAvatar"
              />
            </div>
            <div v-if="!avatar_valid" class="error_msg">
              <p class="error">image cannot be larger than 250kb</p>
            </div>
          </div>
        </div>
      </div>
    </div>
    <p v-if="in_set_artist" class="intx_text">
      Artist info cannot be changed while artist transaction is in progress
    </p>
    <div class="actions">
      <btn text="cancel" @click="$router.go(-1)">
        <img src="~assets/icon-cancel.svg"/>
      </btn>
      <btn :text="edit_self ? 'update info' : 'create account'" 
           color="blue" 
           :disabled="!can_submit || in_set_artist" @click="onSetArtist"
      >
        <img src="~assets/icon-create.svg"/>
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
    }

    .intx_text {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 35px 0px 0px 0px
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

          .container-avatar {

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
              
              .text {
                width: 100%
                height: 100%
                display: flex
                justify-content: center
                align-items: center
                font-size: 14px
                color: #1af6d6
                cursor: pointer
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

                .remove {
                  cursor: pointer
                }
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

                &.error {
                  filter: grayscale(100%) brightness(40%) sepia(100%) hue-rotate(-50deg) saturate(600%) contrast(0.8)
                }
              }
            }

            .error_msg {
              align-self: center
              margin-left: 10px

              .error {
                font-style: italic
                font-size: 12px
                font-weight: 400
              }
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
import inputField from './input-field.vue'
import textAreaField from './textarea-field.vue'
import pageTitle from './page-title.vue'
import btn from './button.vue'
import addImage from './add-image.vue'
import {common} from '../utils/consts.js'
import validators from '../utils/validators.js'

export default {
  components: {
    inputField, 
    textAreaField, 
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
      label_: '',
      website_: '',
      twitter_: '',
      instagram_: '',
      about_: '',
      banner_: undefined,
      avatar_: undefined
    }
  },

  computed: {
    in_set_artist() {
      return this.$state.set_artist_tx.length > 0
    },
    edit_self () {
      return !!(this.id && this.id.length && this.id === this.$state.my_artist_key)
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
      if (!value) return true
      return validators.url(value)
    },

    twitter_valid() {
      let value = this.twitter
      return !value || validators.twitter(value)
    },
    
    instagram_valid() {
      let value = this.instagram
      return !value || validators.instagram(value)
    },
    about_valid() {
      let value = this.about
      return !value || value.length <= 150
    },
    banner_valid() {
      return this.image_valid(this.banner)
    },
    avatar_valid() {
      return this.image_valid(this.avatar)
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
      return this.id ? this.$state.artists[this.id] : {}
    },
    label: {
      get () {
        return this.label_ || this.artist.label
      },
      set (val) {
        this.label_ = val
      }
    },
    website: {
      get () {
        return this.website_ || this.artist.website
      },
      set (val) {
        this.website_ = val
      }
    },
    twitter: {
      get () {
        return this.twitter_ || this.artist.twitter
      },
      set (val) {
        this.twitter_ = val
      }
    },
    instagram: {
      get () {
        return this.instagram_ || this.artist.instagram
      },
      set (val) {
        this.instagram_ = val
      }
    },
    about: {
      get () {
        console.log('about get', JSON.stringify(this.artist))
        return this.about_ || this.artist.about
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
        return this.banner_ || this.artist.banner
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
        return this.avatar_ || this.artist.avatar
      },
      set (val) {
        this.avatar_ = val
      }
    }
  },

  methods: {    
    image_valid (image) {
      if (!image) return true

      if (image.file) {
        return image.file.size <= common.MAX_IMAGE_SIZE
      }

      return image.ipfs_hash
    },

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
        about:     this.about
      }

      if (this.avatar) {
        data['avatar'] = this.avatar.file
      }

      if (this.banner) {
        data['banner'] = this.banner.file
      }

      await this.$store.setArtist(this.label, data)
    }
  }
}
</script>