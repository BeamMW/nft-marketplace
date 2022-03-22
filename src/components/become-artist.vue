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
                    style="margin-bottom:30px;margin-top:0"
        />
        <inputField v-model="website"
                    label="Website"
                    placeholder="https://website.name/"
                    img="glob"
                    :valid="website_valid"
        />
        <inputField v-model="twitter"
                    label="Twitter"
                    placeholder="@twitter"
                    img="twitter"
                    :valid="twitter_valid"
        />
        <inputField v-model="instagram"
                    label="Instagram"
                    placeholder="@instagram"
                    img="instagram"
                    :valid="instagram_valid"
        />
      </div>
      <div class="col-second">
        <textAreaField v-model="about"
                       label="About me"
                       :valid="about_valid"
                       :max-length="150"
        />
        <div class="banner" :style="bannerStyles">
          <label v-if="!banner" class="text" for="banner">Add an artist banner</label>
          <input id="banner"
                 type="file"
                 accept="image/apng, image/avif, image/gif, image/jpeg, image/png, image/svg+xml, image/webp"      
                 class="files"
                 @change="onUploadBanner"
          />
        </div>
        <div class="image" :style="avatarStyles">
          <label v-if="!avatar" class="text" for="avatar">Add an artist image</label>
          <input id="avatar"
                 type="file"
                 accept="image/apng, image/avif, image/gif, image/jpeg, image/png, image/svg+xml, image/webp"      
                 class="files"
                 @change="onUploadAvatar"
          />
        </div>
      </div>
    </div>
  </div>
  <div class="actions">
    <div class="button close" @click="close">
      <img src="~assets/icon-cancel.svg"/>
      <span class="text">cancel</span>
    </div>
    <div class="button create">
      <img src="~assets/icon-create.svg"/>
      <span class="text">create account</span>
    </div>
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
        
        .files {
          visibility:hidden
          width: 0
        }
      }
    }

    .banner {
      display: flex
      align-items: center
      justify-content: center
      height: 135px
      margin-top: 33px
      margin-bottom: 20px
      background-color: rgba(26, 246, 214, 0.1)
      background-repeat: no-repeat
      border-radius: 10px
    }

    .image {
      display: flex
      align-items: center
      justify-content: center
      height: 120px
      width: 120px
      background-color: rgba(26, 246, 214, 0.1)
      border-radius: 9999px
    }

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
  }

  .actions {
    display:flex
    justify-content: center
    margin-top: 15px

    .button {
      padding: 11px 25px
      border-radius: 50px
      display: flex
      align-items: center
      color: #032e49
      font-size: 14px
      font-weight: bold
      cursor: pointer

      .text {
        margin-left: 5px
        line-height: 1
      }
    }

    .create {
      background-color: #0bccf7
      margin-left: 20px
    }

    .close {
      background-color: rgba(255, 255, 255, 0.1)
      color: #fff
    }
  }
</style>

<script>
import inputField from './input-field.vue'
import textAreaField from './textarea-field.vue'
import pageTitle from './page-title.vue'

export default {
  
  components: {
    inputField, 
    textAreaField, 
    pageTitle
  },

  emits:['close-become-artist'],
  
  data () {
    return {
      name: '',
      website: '',
      twitter: '',
      instagram: '',
      about: '',
      banner:'',
      avatar:''
    }
  },

  computed: {
    avatarStyles() {
      return {
        'background': this.avatar ? `url(${this.avatar}) no-repeat center` : '',
        'border' :  this.avatar ? '' : '1px dashed #1AF6D6',
      }
    },
    bannerStyles() {
      return {
        'background': this.banner ? `url(${this.banner}) no-repeat center` : '',
        'border' :  this.banner ? '' : '1px dashed #1AF6D6',
      }
    },
    name_valid() {
      let value = this.name
      return !value || value.length <= 120
    },
    website_valid() {
      let pattern = new RegExp('^(https?:\\/\\/)?'+ // protocol
        '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|'+ // domain name
        '((\\d{1,3}\\.){3}\\d{1,3}))'+ // OR ip (v4) address
        '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*'+ // port and path
        '(\\?[;&a-z\\d%_.~+=-]*)?'+ // query string
        '(\\#[-a-z\\d_]*)?$','i') // fragment locator
      let value = this.website
      return !value || !!pattern.test(value)
    },
    twitter_valid() {
      // TODO: real validation, not all chars are allowed
      let value = this.twitter
      return !value || (value.startsWith('@') && value.length < 16)
    },
    instagram_valid() {
      // TODO: real validation, not all chars are allowed
      let value = this.instagram
      return !value || (value.startsWith('@') && value.length <= 30)
    },
    about_valid() {
      let value = this.about
      return !value || value.length <= 150
    }
    // TODO: add images validation, not more than 250kb
    // if image is larger that 250kb, apply red filter to image and
    //   - for banner just write on banner itself 'image cannot be larger than 250kb'
    //   - for avatar, ame at the right of avatar
  },

  methods: {
    close() {
      this.$emit('close-become-artist')
    },
    
    loadImage(e, cback) {
      let file = e.target.files[0]
      let reader = new FileReader()
      reader.readAsDataURL(file)
      reader.onload = (e) => {
        cback(e.target.result)
      }
    },

    onUploadBanner(e) {
      this.loadImage(e, banner => {
        this.banner = banner
      })
    },

    onUploadAvatar(e) {
      this.loadImage(e, avatar => {
        this.avatar = avatar
      })
    }
  }
}
</script>