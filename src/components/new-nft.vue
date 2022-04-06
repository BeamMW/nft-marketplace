<template>
  <div class="nft-container">
    <pageTitle title="add nft"/>
    <div class="fields">
      <div class="col-first">
        <inputField v-model="name"
                    label="Name*"
                    :valid="name_valid"
                    :max_length="100"
        />
        <textAreaField v-model="description"
                       label="Description"
                       :valid="description_valid"
                       :max_length="150"
        />
        <formSelect :options="selector_options"
                    @selected="selectCollection"
        />
        <div class="price-container">
          <div class="label">Price</div>
          <priceInput v-model:price="price" color="rgba(255,255,255,0.7)"/>
        </div>
        <switchInput v-model:checked="sale"/>
      </div>
      <div class="col-second">
        <addImage v-model:image="image"
                  :valid="image_valid"
                  title="Add NFT here<br>(.jpg, .png, .gif)"
                  height="374px"
        />
      </div>
    </div>
  </div>
  <div class="actions">
    <btn text="cancel" @click="$router.go(-1)">
      <img src="~assets/icon-cancel.svg"/>
    </btn>
    <btn text="create account" color="blue" :disabled="!can_submit">
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

        .price-container {

          .label {
            margin-bottom: -10px
            color: rgba(255, 255, 255, 0.6)
            font-family: 'SFProDisplay', sans-serif
            font-size: 14px
          }
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
import {common} from '../utils/consts.js'

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
      show: false,
      sale: false,
      selector_options: [
        {name: 'Collection 0', id: 0},
        {name: 'Collection 1', id: 1},
        {name: 'Collection 2', id: 2},
        {name: 'Collection 3', id: 3},
        {name: 'Collection 4', id: 4},
        {name: 'Collection 5', id: 5},
        {name: 'Collection 6', id: 6},
        {name: 'Collection 7', id: 7}

      ],
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
      return !this.image || this.image.size <= common.MAX_IMAGE_SIZE
    },
    can_submit () {
      return this.name && this.name_valid &&
             this.description_valid &&
             this.image_valid
    },
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
    selectCollection(opt) {
      console.log(opt)
    },
  }
}
</script>