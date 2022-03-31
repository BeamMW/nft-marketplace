<template>
  <div class="container">
    <pageTitle title="add new collection"/>
    <p class="description">
      Before you can add any NFT you need to create a Collection
    </p>
    <div class="fields">
      <div class="col-first">
        <inputField v-model="name"
                    label="Gallery Name*"
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
        <textAreaField v-model="description"
                       label="Description"
                       :valid="description_valid"
                       :max_length="150"
                       :show_counter="false"
        />
        <div class="banner">
          <addImage :banner="banner"
                    :valid="banner_valid"
                    title="Add Gallery image"
                    :accepts="inputAccepts"
                    @remove="onRemoveBanner"
                    @upload="onUploadBanner"
          />
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
      }
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
      description: '',
      banner: undefined,
      avatar: undefined,
      inputAccepts: ['image/apng', 'image/avif', 'image/gif', 'image/jpeg','image/png','image/svg+xml', 'image/webp']
    }
  },

  computed: {
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

    description_valid() {
      let value = this.description
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
             this.description_valid &&
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
        console.log('called',this.banner)
      })
    },

    onUploadAvatar(e) {
      this.loadImage(e, (data, size) => {
        this.avatar = {data, size}
      })
    },

    onRemoveBanner() {
      this.banner = undefined
    },
    
    onRemoveAvatar() {
      this.avatar = undefined
    },
  }
}
</script>