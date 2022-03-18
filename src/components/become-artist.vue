<template>
  <div class="container">
    <h1 class="title">Become an Artist</h1>
    <p class="description">
      To become a publisher you need to set up a username.<br>
      Registration will allow you to publish and manage your NFTs.
    </p>
    <div class="fields">
      <div class="col-first">
        <inputField id="name"
                    v-model="name"
                    label="Artist Name*"
                    placeholder=""
                    padding-left="15"        
                    :is-valid="error.name"
                    type="text"
                    style="margin-bottom:44px;margin-top:0"
        />
        <inputField id="website"
                    v-model="website"
                    label="Website"
                    placeholder="https://website.name/"
                    img-name="glob"
                    padding-left="44"
                    :is-valid="error.website"
                    type="text"
        />
        <inputField id="twitter"
                    v-model="twitter"
                    label="Twitter"
                    placeholder="@twitter"
                    img-name="twitter"
                    padding-left="44"
                    :is-valid="error.twitter"
                    type="text"
        />
        <inputField id="instagram"
                    v-model="instagram"
                    label="Instagram"
                    placeholder="@instagram"
                    img-name="instagram"
                    padding-left="44"
                    :is-valid="error.instagram"
                    type="text"
        />
      </div>
      <div class="col-second">
        <textAreaField id="aboutMe"
                       v-model="aboutMe"
                       label="About me"
                       placeholder=""
                       :is-valid="error.aboutMe"
                       max-length="150"
                       type="text"
        />
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
    padding: 0 30px

    .title {
      font-weight: 700
      font-size: 14px
      line-height: 17px
      text-align: center
      text-transform: uppercase
      color: #FFFFFF
    }

    .description {
    font-size: 14px
    line-height: 17px
    text-align: center
    color: #FFFFFF
    margin-top: 30px
    }

    .fields {
      display: flex

      .col-first {
        flex-basis: 50%
      }
      .col-second {
        flex-basis: 50%
        margin-left: 30px
      }
    }
  }

  .actions {
    display:flex
    justify-content: center
    margin-top: 50px

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

export default {
  
  components: {
    inputField, textAreaField
  },
  emits:['close-become-artist'],
  data () {
    return {
      name: '',
      website: '',
      twitter: '',
      instagram: '',
      aboutMe: '',
      error: {
        name: false,
        website: false,
        twitter: false,
        instagram: false,
        aboutMe: false,
      }
    }
  },
  watch: {
    name(value){
      this.name = value
      this.validateName(value)
    },

    website(value){
      this.website = value
      this.validdateURL(value)
    },

    twitter(value){
      this.twitter = value
      this.validateTwitterNickname(value)
    },

    instagram(value){
      this.instagram = value
      this.validateInstagramickname(value)
    },

    aboutMe(value){
      this.aboutMe = value
      this.validateAboutMe(value)
    }
  },
  methods: {
    close() {
      this.$emit('close-become-artist')
    },
    validateName(value) {
      return this.error.name = value.length > 0 
    },

    validdateURL(str) {
      let pattern = new RegExp('^(https?:\\/\\/)?'+ // protocol
        '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|'+ // domain name
        '((\\d{1,3}\\.){3}\\d{1,3}))'+ // OR ip (v4) address
        '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*'+ // port and path
        '(\\?[;&a-z\\d%_.~+=-]*)?'+ // query string
        '(\\#[-a-z\\d_]*)?$','i') // fragment locator
      this.error.website =!!pattern.test(str)
      return this.error.website
    },

    validateTwitterNickname(value) {
      return this.error.twitter = value.startsWith('@') && value.length < 16
    },

    validateInstagramickname(value) {
      return this.error.instagram = value.startsWith('@') && value.length < 15
    },

    validateAboutMe(value) {
      return this.error.aboutMe =  value.length < 150
    }
  }

}
</script>