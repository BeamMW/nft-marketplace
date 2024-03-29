<template>
  <messageModal ref="messageModal"/>
  <div class="collection-container">
    <template v-if="edit_mode">
      <pageTitle title="Edit collection"/>
      <p v-if="collection" class="description">
        After collection is changed it would not be visible until reviewed by a moderator.<br> 
        NFTs from collection would still appear in gallery.
      </p>
    </template>
    <template v-else>
      <pageTitle title="Add new collection"/>
      <p class="description">
        Before you can add any NFT you need to create a collection<br>
        <i>After your collection is created it would not be visible until reviewed by a moderator.</i>
      </p>
    </template>
    <loading v-if="edit_mode && collection === undefined" text="Loading Collection"/>
    <notFound v-else-if="edit_mode && collection == null" text="Collection Not Found" error/>
    <div v-else class="scrollable">
      <div class="fields">
        <div class="col-first">
          <formInput v-model="label"
                     label="Collection Name*"
                     :valid="label_valid"
                     :max_length="40"
                     style="margin-bottom:55px;margin-top:0"
          />
          <formInput v-model="website"
                     label="Website"
                     placeholder="https://website.name/"
                     img="globe"
                     :max_length="150"
                     :valid="website_valid"
          />
          <formInput v-model="twitter"
                     label="Twitter"
                     placeholder="twitter"
                     img="twitter"
                     :max_length="15"
                     :valid="twitter_valid"
                     :counter="false"
                     :allowed="twitter_allowed"
          />
          <formInput v-model="instagram"
                     label="Instagram"
                     placeholder="instagram"
                     img="instagram"
                     :max_length="30"
                     :valid="instagram_valid"
                     :counter="false"
                     :allowed="instagram_allowed"
          />
        </div>
        <div class="col-second">
          <textArea v-model="description"
                    label="Description"
                    height="198px"
                    :valid="description_valid"
                    :max_length="1000"
          />
          <addImage v-model="cover"
                    v-model:error="cover_error"
                    title="Add collection image<br>(*.jpg, *.png, *.svg)"
                    accept="image/jpeg,image/png,image/svg+xml"
                    height="135px"
                    :min_width="1400"
                    :min_height="260"
                    cover
          />
        </div>
      </div>
      <div class="actions">
        <btn text="cancel" @click="$store.toBack()">
          <img src="~assets/cancel.svg"/>
        </btn>
        <btn :text="edit_mode ? 'update collection' : 'create collection'" 
             color="green" 
             :disabled="!can_submit" 
             @click="onSetCollection"
        >
          <img src="~assets/create.svg"/>
        </btn>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .collection-container {
    width: 100%
    height: 100%
    display: flex
    flex-direction: column

    .description {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 10px 0px 20px 0px
      font-family: 'SFProDisplay', sans-serif

      & > i {
        opacity: 0.7
        line-height: 29px
      }
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
          margin-left: 30px

          & > *:not(:last-child) {
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
      display: flex
      justify-content: center
      margin-top: 35px
      margin-bottom: 10px

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
import loading from 'controls/loading'
import notFound from 'controls/not-found'
import collsStore from 'stores/collections'
import validators from 'utils/validators'
import messageModal from 'components/message-modal'

export default {
  components: {
    messageModal,
    formInput, 
    textArea, 
    pageTitle,
    btn,
    addImage,
    loading,
    notFound
  },

  props: {
    id: {
      type: Number,
      required: false,
      default: undefined
    },
  },

  setup(props) {
    let collection = props.id ? collsStore.getLazyItem(props.id) : undefined
    return {
      collection
    }
  },

  data () {
    return {
      label_: undefined,
      website_: undefined,
      twitter_: undefined,
      instagram_: undefined,
      description_: undefined,
      cover_: undefined,
      cover_error: undefined,
    }
  },

  computed: {
    edit_mode () {
      return this.id !== undefined
    },
    label: {
      get () {
        return this.label_ != undefined ? this.label_ : (this.collection || {}).label
      },
      set (val) {
        this.label_ = val
      }
    },
    label_valid() {
      let value = this.label
      return !value || value.length <= 40
    },
    website: {
      get () {
        return this.website_ != undefined ? this.website_ : (this.collection || {}).website
      },
      set (val) {
        this.website_ = val
      }
    },
    website_valid() {
      let value = this.website
      if (!value) return true
      return value.length <= 150 && validators.url(value)
    },
    twitter: {
      get () {
        return this.twitter_ != undefined ? this.twitter_ : (this.collection || {}).twitter
      },
      set (val) {
        this.twitter_ = val
      }
    },
    twitter_valid() {
      let value = this.twitter
      return !value || validators.twitter(value)
    },
    twitter_allowed() {
      return validators.twitter_allowed()
    },
    instagram: {
      get () {
        return this.instagram_ != undefined ? this.instagram_ : (this.collection || {}).instagram
      },
      set (val) {
        this.instagram_ = val
      }
    },
    instagram_valid() {
      let value = this.instagram
      return !value || validators.instagram(value)
    },
    instagram_allowed() {
      return validators.instagram_allowed()
    },
    description: {
      get () {
        return this.description_ != undefined ? this.description_ : (this.collection || {}).description
      },
      set (val) {
        this.description_ = val
      }
    },
    description_valid() {
      let value = this.description
      return !value || value.length <= 1000
    },
    cover: {
      get () {
        if (this.cover_ === null) {
          return undefined
        }
        return this.cover_ || (this.collection || {}).cover
      },
      set (val) {
        this.cover_ = val
      }
    },
    cover_valid() {
      return !this.cover || !this.cover_error
    },
    can_submit () {
      return this.label && this.label_valid &&
             this.website_valid &&
             this.twitter_valid &&
             this.instagram_valid &&
             this.description_valid &&
             this.cover_valid
    }
  },

  methods: {   
    async onSetCollection() {
      if (this.$state.is_headless) {
        return this.$store.switchToHeaded()  
      } 
      
      try {
        let data = {
          website:     this.website,
          twitter:     this.twitter,
          instagram:   this.instagram,
          description: this.description,
          cover:       this.cover
        }
        let txid = await collsStore.setCollection(this.id, this.label, data)
        if (!txid) {
          // user cancelled
          return
        }
      }
      catch(err) {
        if (err.error === 'label already exists') {
          this.$refs.messageModal.open(
            'Duplicate collection name', 
            'You already have a collection with the same name.<br>Please choose another name and retry.',
            true
          )
          return
        }
        throw err
      }
      this.$store.toBack()
    }
  }
}
</script>