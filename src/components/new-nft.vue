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
        <div class="collection-container">
          <div class="label">Collection</div>
          <div class="select-container">
            <img src="~assets/icon-down.svg" alt="icon"/>
            <select v-model="selected" class="select">
              <option v-for="option in selector_options" :key="option.name" :value="option.name">
                {{ option.name }}
              </option>
            </select>
          </div>
        </div>
        <priceInput v-model="price" color="#fff" @trigger-key="onKey" @trigger-paste="onPaste"/>
        <span class="price">{{ price }} USD</span>
        <switchInput v-model:checked="sale"/>
      </div>
      <div class="col-second">
        <addImage v-model:image="image"
                  :valid="image_valid"
                  title="Add NFT here<br>(.jpg, .png, .gif)"
                  height="344px"
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

        & > .collection-container {
          .label {
            margin-bottom:10px
            color: rgba(255, 255, 255, 0.6)
            font-family: 'SFProDisplay', sans-serif
            font-size: 14px
          }

          .select-container {
            display: flex
            position: relative

            & > img {
              position: absolute
              width: 9px
              height: 5px
              right:20px
              top:50%
            }

            .select {
              -moz-appearance:none
              -webkit-appearance:none
              appearance:none
              font-family: 'SFProDisplay', sans-serif
              background-color: rgba(255, 255, 255, 0.05)
              border: none
              outline-width: 0
              font-size: 14px
              color: white
              border-radius: 10px
              padding: 12px 8px
              width: 100%
            }
          }
        }

        .price {
          display: block
          margin-top:-14px
          margin-bottom: 20px
          font-size: 12px
          color: #fff
          opacity: 0.7
        }
      }

      .col-second {
        flex-basis: 50%
        margin-left: 30px
        margin-top: 30px
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
import {nextTick} from 'vue'
import inputField from './input-field.vue'
import textAreaField from './textarea-field.vue'
import pageTitle from './page-title.vue'
import btn from './button.vue'
import addImage from './add-image.vue'
import priceInput from './price-input.vue'
import switchInput from './switch-input.vue'
import {common} from '../utils/consts.js'
import utils from '../utils/utils.js'

export default {
  components: {
    inputField, 
    textAreaField, 
    pageTitle,
    btn,
    addImage,
    priceInput,
    switchInput
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
      selected: 0,
      sale: null,
      selector_options: [
        {name: 'Collection 1'},
        {name: 'Collection 2'},
        {name: 'Collection 3'},
        {name: 'Collection 4'},
        {name: 'Collection 5'},
        {name: 'Collection 6'}
      ],
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

    image_valid() {
      return !this.image || this.image.size <= common.MAX_IMAGE_SIZE
    },
    
    can_submit () {
      return this.name && this.name_valid &&
             this.website_valid &&
             this.twitter_valid &&
             this.instagram_valid &&
             this.description_valid &&
             this.image_valid
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

    onKey(ev) {
      if (ev.isComposing || ev.keyCode === 229 || ev.ctrlKey || ev.altKey || ev.metaKey) {
        return
      }

      const specialKeys = [
        'Backspace', 'Tab', 'ArrowDown', 'ArrowLeft', 'ArrowRight', 'ArrowUp',
        'Control', 'Delete', 'F5'
      ]

      if (specialKeys.indexOf(ev.key) !== -1) {
        return
      }

      const current = this.price
      const next = current.concat(ev.key)

      if (!utils.handleString(next)) {
        ev.preventDefault()
      }
    },

    onPaste(ev) {
      if (ev.clipboardData != undefined) {
        const text = ev.clipboardData.getData('text')
        if (!utils.handleString(text)) {
          ev.preventDefault()
        }
      }
    },
    
    close() {
      this.show = false
    },

    open(ev) {
      this.show = true
      nextTick(() => {
        let downAway = (evc) => {
          if (!this.$el.contains(evc.target)) {
            document.removeEventListener('mousedown', downAway, true)
            this.close()
          }
        }
        
        let clickAway = (evc) => {
          if (ev != evc) {
            document.removeEventListener('click', clickAway, true)
            this.close()
          }
        }
        
        let scrollAway = (evc) => {
          document.removeEventListener('scroll', scrollAway, scroll)
          this.close()
        }
        
        document.addEventListener('mousedown', downAway, true)
        document.addEventListener('click', clickAway, true)
        document.addEventListener('scroll', scrollAway, true)
      })
    },
  }
}
</script>