<template>
  <div class="nft-container">
    <pageTitle title="Add NFT"/>
    <loading v-if="collections === undefined" text="Loading collections"/>
    <notFound v-else-if="collections == null" text="Failed to load collections" error/>
    <template v-else>
      <p class="description">
        <i>After NFT is created it would not be visible<br>
          until reviewed by a moderator.</i>
      </p>
      <div class="fields">
        <div class="col-first">
          <inputField v-model="name"
                      label="NFT Name*"
                      :valid="name_valid"
                      :max_length="35"
          />
          <textAreaField v-model="description"
                         label="Description"
                         :valid="description_valid"
                         :max_length="300"
          />
          <formSelect v-model="collection"
                      :options="collections"
          />
          <priceInput v-model="price"
                      label="Price"
          />
          <switchInput v-model="dontsell" label="Not for sale"/>
        </div>
        <div class="col-second">
          <addImage v-model:image="image"
                    :error="image_valid ? '' : 'image cannot be larger than 250kb'"
                    accept="image/*"
                    title="Add NFT here<br>(any image, including animated)"
                    height="374px"
          />
        </div>
      </div>
    </template>  
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
    .description {
      font-size: 14px
      text-align: center
      color: #fff
      margin: 10px 0px 20px 0px
      font-family: 'SFProDisplay', sans-serif

      & > i {
        opacity: 0.7
        display: block
        margin-top: 6px
      }
    }

    .fields {
      padding: 0px 30px 0px 30px
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
import notFound from './not-found.vue'
import btn from './button.vue'
import addImage from './add-image.vue'
import priceInput from './price-input.vue'
import switchInput from './switch-input.vue'
import formSelect from './form-select.vue'
import collsStore from 'stores/collections'
import artsStore from 'stores/artworks'
import validators from 'utils/validators'
import router from 'router'
import {common} from 'utils/consts'
import {useObservable} from '@vueuse/rxjs'
import {computed} from 'vue'
import loading from './loading.vue'

export default {
  components: {
    inputField, 
    textAreaField, 
    pageTitle,
    btn,
    addImage,
    priceInput,
    switchInput,
    formSelect,
    loading,
    notFound
  },

  setup () {
    const collsObservable = computed(() => useObservable(collsStore.getLazyAllItems('artist')))
    const collections = computed(() => {
      let colls = collsObservable.value.value
      if (!colls) {
        return undefined
      }
      let res = []
      for (let coll of colls) {
        res.push({
          name: coll.label,
          id: coll.id
        })
      }
      return res
    })
    return {
      collections
    }
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
      dontsell: true,
      collection: 0,
    }
  },

  computed: {
    name_valid() {
      let value = this.name
      return !value || value.length <= 35
    },
    description_valid() {
      let value = this.description
      return !value || value.length <= 300
    },
    image_valid() {
      return !this.image || validators.image(this.image)
    },
    can_submit () {
      return this.name && this.name_valid &&
             this.description_valid &&
             this.image && this.image_valid
    }
  },

  watch: {
    dontsell(newval) {
      if(newval) {
        this.price = ''
      }
    },
    price(newval) {
      this.dontsell = !newval || parseFloat(newval) === 0.0
    }
  },

  methods: {    
    async onUploadNFT() {
      let data = {
        description: this.description,
        image: this.image
      }
      let collID = this.collections[this.collection].id
      let price  = parseFloat(this.price) * common.GROTHS_IN_BEAM
      await artsStore.createNFT(collID, this.name, data, price)
      router.go(-1)
    }
  }
}
</script>