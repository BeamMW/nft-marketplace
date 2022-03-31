<template>
  <div class="container">
    <pageTitle title="Become an Artist"/>
    <p class="description">
      To become a publisher you need to set up a username.<br>
      Registration will allow you to publish and manage your NFTs.
    </p>
    <div class="fields">
      <div class="col-first">
        <inputField v-model="name"
                    label="Artist Name*"
                    :valid="name_valid"
                    :max_length="100"
                    style="margin-bottom:60px;margin-top:0"
        />
        <inputField v-model="website"
                    label="Website"
                    placeholder="https://website.name/"
                    img="glob"
                    :max_length="250"
                    :valid="website_valid"
        />
        <inputField v-model="twitter"
                    label="Twitter"
                    placeholder="@twitter"
                    img="twitter"
                    :max_length="15"
                    :valid="twitter_valid"
        />
        <inputField v-model="instagram"
                    label="Instagram"
                    placeholder="@instagram"
                    img="instagram"
                    :max_length="30"
                    :valid="instagram_valid"
        />
      </div>
      <div class="col-second">
        <textAreaField v-model="about"
                       label="About me"
                       :valid="about_valid"
                       :max_length="150"
                       :show_counter="true"
        />
        <div class="banner">
          <addImage :banner="banner.data"
                    :valid="banner_valid"
                    title="Add an artist banner"
                    :accepts="inputAccepts"
                    @remove="onRemoveBanner"
                    @upload="onUploadBanner"
          />
        </div>
        <div class="container-avatar">
          <div class="avatar" :style="avatarStyles">
            <div v-if="avatar" class="remove">
              <img src="~/assets/remove.svg" alt="remove avatar" @click="onRemoveAvatar"/>
            </div>
            <img v-if="avatar" :src="avatar.data" alt="avatar" class="image" :class="{'error': !avatar_valid}"/>
            <label v-if="!avatar" class="text" for="avatar">Add an artist image</label>
            <input id="avatar"
                   ref="avatarUploader"
                   type="file"      
                   accept="image/apng, image/avif, image/gif, image/jpeg, image/png, image/svg+xml, image/webp"
                   class="files"
                   @click="resetImageUploader"
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
  <div class="actions">
    <btn text="cancel" @click="$router.go(-1)">
      <img src="~assets/icon-cancel.svg"/>
    </btn>
    <btn text="create account" color="blue" :disabled="!can_submit" @click="onCreate">
      <img src="~assets/icon-create.svg"/>
    </btn>
  </div>
</template>

<style scoped lang="stylus">
  .container {
    .description {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 30px 0px
      font-family: 'SFProDisplay', sans-serif
    }

    .fields {
      padding: 0 30px
      display: flex

      .col-first {
        flex-basis: 50%

        & > * {
          margin-bottom: 20px
        }
      }

      .col-second {
        flex-basis: 50%
        margin-left: 30px

        .banner {
          height: 135px
          margin-top: 33px
          margin-bottom: 20px
        }

        .container-avatar {
          display: flex
          
          .avatar {
            display: flex
            align-items: center
            justify-content: center
            height: 120px
            width: 120px
            background-color: rgba(26, 246, 214, 0.1)
            border-radius: 9999px
            position: relative

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
    margin-top: 50px

    & > *:not(:first-child) {
      margin-left: 30px
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

export default {
  components: {
    inputField, 
    textAreaField, 
    pageTitle,
    btn,
    addImage
  },
  data () {
    return {
      name: '',
      website: '',
      twitter: '',
      instagram: '',
      about: '',
      avatar: undefined,
      banner: {
        data:'',
        size:0
      },
      inputAccepts: ['image/apng', 'image/avif', 'image/gif', 'image/jpeg','image/png','image/svg+xml', 'image/webp']
    }
  },

  computed: {
    avatarStyles() {
      return {
        'border' :  this.avatar ? '1px dashed transparent' : '1px dashed #1AF6D6',
      }
    },

    name_valid() {
      let value = this.name
      return !value || value.length <= 100
    },

    website_valid() {
      let value = this.website
      if (!value) return true
      let url 
      try {
        url = new URL(this.website)
      }
      catch(_) {
        return false
      }
      return value.length <= 250 && 
             (url.protocol === 'http:' || url.protocol === 'https:') &&
             (url.toString() === value || url.toString() === value + '/')
    },

    twitter_valid() {
      let value = this.twitter
      return !value || /^[@][a-zA-Z0-9_]{1,15}$/.test(value)
    },

    instagram_valid() {
      let value = this.instagram
      return !value || (/^(?!.*[..]{2})[@][a-zA-Z0-9_.]{1,30}$/.test(value))
    },

    about_valid() {
      let value = this.about
      return !value || value.length <= 150
    },

    banner_valid() {
      return !this.banner || this.banner.size <= common.MAX_IMAGE_SIZE
    },
    
    avatar_valid() {
      return !this.avatar || this.avatar.size <= common.MAX_IMAGE_SIZE
    },

    can_submit () {
      return this.name && this.name_valid &&
             this.website_valid &&
             this.twitter_valid &&
             this.instagram_valid &&
             this.about_valid &&
             this.banner_valid &&
             this.avatar_valid
    }
  },

  methods: {    
    loadImage(e, cback) {
      let file = e.target.files[0]
      let reader = new FileReader()
      reader.readAsDataURL(file)
      reader.onload = (e) => {
        cback(e.target.result, file.size)
      }
    },

    onUploadBanner(e) {
      this.loadImage(e, (data, size) => {
        this.banner = {data, size}
      })
    },

    onUploadAvatar(e) {
      this.loadImage(e, (data, size) => {
        this.avatar = {data, size}
      })
    },

    onRemoveBanner() {
      this.banner = {
        data: '',
        size: 0
      }
    },
    
    onRemoveAvatar() {
      this.avatar = undefined
    },

    resetImageUploader() {
      this.$refs.avatarUploader.value = ''
    },
  }
}
</script>