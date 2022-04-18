<template>
  <div class="nft-container">
    <pageTitle title="add nft"/>
    <div class="fields">
      <div class="col-first">
        <inputField v-model="name"
                    label="NFT Name*"
                    :valid="name_valid"
                    :max_length="100"
        />
        <textAreaField v-model="description"
                       label="Description"
                       :valid="description_valid"
                       :max_length="150"
        />
        <formSelect v-model="collection"
                    :options="collections"
        />
        <priceInput v-model="price"
                    label="Price"
                    :readonly="dontsell"
        />
        <switchInput v-model="dontsell" label="Not for sale"/>
      </div>
      <div class="col-second">
        <addImage v-model:image="image"
                  :error="image_valid ? '' : 'image cannot be larger than 250kb'"
                  title="Add NFT here<br>(any image)"
                  height="374px"
        />
      </div>
    </div>
  </div>
  <div class="actions">
    <btn text="cancel" @click="$router.go(-1)">
      <img src="~assets/icon-cancel.svg"/>
    </btn>
    <btn text="upload NFT" color="green" :disabled="!can_submit" @click="onUploadNFT">
      <img src="~assets/icon-create.svg"/>
    </btn>
  </div>
</template>

<style scoped lang="stylus">
  .nft-container {
    .fields {
      padding: 50px 30px 0px 30px
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
        margin-top: 30px
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
import priceInput from './price-input.vue'
import switchInput from './switch-input.vue'
import formSelect from './form-select.vue'
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'
import validators from 'utils/validators'
import router from 'router'

export default {
  components: {
    inputField, 
    textAreaField, 
    pageTitle,
    btn,
    addImage,
    priceInput,
    switchInput,
    formSelect
  },
  data () {
    return {
      name: '',
      website: '',
      twitter: '',
      instagram: '',
      description: '',
      image: undefined,
      price: '',
      dontsell: false,
      collection: 0,
    }
  },

  computed: {
    name_valid() {
      let value = this.name
      return !value || value.length <= 100
    },
    description_valid() {
      let value = this.description
      return !value || value.length <= 150
    },
    image_valid() {
      return !this.image || validators.image(this.image)
    },
    can_submit () {
      return this.name && this.name_valid &&
             this.description_valid &&
             this.image && this.image_valid
    },
    collections () {
      let colls = collsStore.artist_items
      let res = []
      for (let coll of colls) {
        res.push({
          name: coll.label,
          id: coll.id
        })
      }
      return res
    }
  },

  methods: {    
    async onUploadNFT() {
      let data = {
        description: this.description,
        image: this.image
      }
      let collID = this.collections[this.collection].id
      await artsStore.createNFT(collID, this.name, data)
      router.go(-1)
    }
  }
}
</script>