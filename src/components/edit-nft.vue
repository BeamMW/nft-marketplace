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
      <!-- TODO: center form, not scale -->
      <div class="fields">
        <div class="col-first">
          <formInput v-model="name"
                     label="NFT Name*"
                     :valid="name_valid"
                     :max_length="35"
          />
          <textArea v-model="description"
                    label="Description"
                    :valid="description_valid"
                    :max_length="300"
          />
          <formSelect v-model="collection"
                      title="Collection"
                      :options="collections"
          />
          <priceInput v-model="price"
                      label="Price"
          />
          <switchInput v-model="dontsell" label="Not for sale"/>
        </div>
        <div class="col-second">
          <addImage v-model="image"
                    v-model:error="image_error"
                    class="add-image"
                    accept="image/apng;image/gif;image/jpeg;image/png;image/svg+xml;image/webp"
                    title="Add NFT image here<br>(*.apng, *.gif, *.jpeg, *.png, *.svg, *.webp)"
                    height="390px"
                    width="390px"
                    :min_width="360"
                    :min_height="360"
                    contain
          />
        </div>
      </div>
    </template>  
  </div>
  <div class="actions">
    <btn text="cancel" @click="$router.go(-1)">
      <img src="~assets/cancel.svg"/>
    </btn>
    <btn text="upload NFT" color="green" :disabled="!can_submit" @click="onUploadNFT">
      <img src="~assets/create.svg"/>
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
        flex: 1

        & > *:not(:last-child) {
          margin-bottom: 20px
        }
      }

      .col-second {
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
import formInput from 'controls/form-input'
import textArea from 'controls/textarea'
import pageTitle from 'controls/page-title'
import notFound from 'controls/not-found'
import btn from 'controls/button'
import addImage from 'controls/add-image'
import priceInput from 'controls/price-input'
import switchInput from 'controls/switch-input'
import formSelect from 'controls/form-select'
import loading from 'controls/loading'
import collsStore from 'stores/collections'
import nftsStore from 'stores/nfts'
import router from 'router'
import {common} from 'utils/consts'
import {computed} from 'vue'

export default {
  components: {
    formInput, 
    textArea, 
    pageTitle,
    btn,
    addImage,
    priceInput,
    switchInput,
    formSelect,
    loading,
    notFound
  },

  props: {
    collid: {
      type: Number,
      default: undefined,
      required: false
    }
  },

  setup (props) {
    const lazyColls = collsStore.getLazyAllItems('artist')
    const collections = computed(() => {
      let colls = lazyColls.value
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
      description: '',
      image: undefined,
      image_error: undefined,
      price: '',
      dontsell: true,
      collection: 0
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
    can_submit () {
      return this.name && this.name_valid &&
             this.description_valid &&
             this.image && !this.image_error
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

  created() {
    let unwatch = this.$watch('collections', (newval) => {
      if (this.collid !== undefined && newval.length) {
        for(let idx = 0; idx < newval.length; idx++) {
          if(newval[idx].id === this.collid) {
            this.collection = idx
            break
          }
        }
        unwatch()
      }
    })
  },

  methods: {    
    async onUploadNFT() {
      let data = {
        description: this.description,
        image: this.image
      }
      let collID = this.collections[this.collection].id
      let price  = parseFloat(this.price) * common.GROTHS_IN_BEAM
      await nftsStore.createNFT(collID, this.name, data, price)
      router.go(-1)
    }
  }
}
</script>