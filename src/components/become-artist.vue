<template>
  <div class="container">
    <h1 class="title">Become an Artist</h1>
    <p class="description">
      To become a publisher you need to set up a username.<br>
      Registration will allow you to publish and manage your NFTs.
    </p>
    <div class="fields">
      <div class="col-first">
     <inputField
        v-model="name"
        label="Artist Name*"
        id="name"
        placeholder=""
        paddingLeft="5"        
        width="424"
        :isValid="error.name"
        type="text"
      />
      <inputField
        v-model="website"
        label="Website"
        id="website"
        placeholder="https://website.name/"
        imgName="glob"
        paddingLeft="50"
        width="374"
        :isValid="error.website"
        type="text"
      />
      <inputField
        v-model="twitter"
        label="Twitter"
        id="twitter"
        placeholder="@twitter"
        imgName="twitter"
        paddingLeft="50"
        width="374"
        :isValid="error.twitter"
        type="text"
      />
      <inputField
        v-model="instagram"
        label="Instagram"
        id="instagram"
        placeholder="@instagram"
        imgName="instagram"
        paddingLeft="50"
        width="374"
        :isValid="error.instagram"
        type="text"
      />
      </div>
      <div>
        <inputField
        v-model="aboutMe"
        label="About me"
        id="aboutMe"
        paddingLeft="5"
        width="424"
        height="79"
        :isValid="error.aboutMe"
        type="text"
      />
      </div>
    </div>
  </div>
  <div class="actions">
    <div class="button close">
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
        margin-right: 30px
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

export default {
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
      this.name = value;
      this.validateName(value);
    },

    website(value){
      this.website = value;
      this.validdateURL(value);
    },

    twitter(value){
      this.twitter = value;
      this.validateTwitterNickname(value);
    },

    instagram(value){
      this.instagram = value;
      this.validateInstagramickname(value);
    },

    aboutMe(value){
      this.aboutMe = value;
      this.validateAboutMe(value);
    }
  },
  
  components: {
    inputField
  },

  methods: {
    validateName(value) {
      return this.error.name = value.length > 0 
    },

    validdateURL(str) {
      let pattern = new RegExp('^(https?:\\/\\/)?'+ // protocol
        '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|'+ // domain name
        '((\\d{1,3}\\.){3}\\d{1,3}))'+ // OR ip (v4) address
        '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*'+ // port and path
        '(\\?[;&a-z\\d%_.~+=-]*)?'+ // query string
        '(\\#[-a-z\\d_]*)?$','i'); // fragment locator
        this.error.website =!!pattern.test(str);
        return this.error.website
    },

    validateTwitterNickname(value) {
      return this.error.twitter = value.startsWith('@') && value.length < 16;
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