<template>
  <div class="container">
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
                       :show_counter="false"
        />
        <div class="container">
          <label class="label">
            Collection
          </label>
        </div>
        <div class="custom-select" @blur="show = false">
          <div class="selected" :class="{ open: show }" @click="open">
            {{ selector_options[selected].name }}
            <img src="~assets/icon-down.svg" class="arrow"/>
          </div>
          <div v-show="show" class="items" :style="style">
            <div v-for="(option, i) of selector_options" :key="i" @click="selectedValue(i)">
              <span>{{ option.name }}</span>
            </div>
          </div>
        </div>
        <priceInput v-model="price" color="#fff" placeholder="0" @trigger-key="onKey" @trigger-paste="onPaste"/>
        <span class="price">{{ price }} USD</span>
        <div class="switch_container">
          <label class="switch">
            <input type="checkbox">
            <span class="slider round"></span>
          </label>
          <label class="switch_text">Not for sale</label>
        </div>
      </div>
      <div class="col-second">
        <div class="banner">
          <addImage :banner="banner.data"
                    :valid="banner_valid"
                    title="Add NFT here"
                    :accepts="inputAccepts"
                    @remove="onRemoveBanner"
                    @upload="onUploadBanner"
          >
          </addimage>
          <div class="accept">
            <span v-if="!banner" class="accept_text">({{ inputAccepts[0] }} or {{ inputAccepts[1] }}) </span> 
          </div>
        </div>
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
  .container {
    
    .fields {
      padding: 0 30px
      display: flex
      margin-top: 77px
      
      .col-first {
        flex-basis: 50%
        & > *:not(:last-child) {
          margin-bottom: 20px
        }
        .container {
          box-sizing: border-box

          .label {
            display: block
            margin-bottom:10px
            color: #8da1ad
            font-family: 'SFProDisplay', sans-serif
            font-size: 14px

            &.error {
              color: rgba(255, 98, 92, 0.7)
            }
          }
        }

        .custom-select {
          position: relative
          display: flex
          outline: none
          background: rgba(255,255,255,0.05)
          border-radius: 10px
          padding: 12px 8px

          .selected {
            color: #fff
            padding-left: 1em
            cursor: pointer
            user-select: none
            opacity: 0.7
            font-size: 14px
            font-weight: bold
            letter-spacing: 0.47px

            &.open {
              border: none
            }
          }

          .arrow {
            width: 9px
            height: 5px
            position: absolute
            right: 20px 
            top:50%
          }

          .items {
            color: #fff
            position: absolute
            border:none
            border-radius: 4px
            font-size: 16px
            right: 0
            z-index: 1
            margin-top: 40px
            width: 100%
            height: 300px
            overflow-y: scroll

            div {
              color: #fff
              padding: 15px 0px 15px 15px
              white-space: nowrap
              line-height: 1
              cursor: pointer
              user-select: none
              
              &:hover {
                color: #00f6d2
              }
            }
      
            .highlight {
              font-size: 14px
              font-weight: bold
              color: #00f6d2
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

        .switch_container {
          display: flex
          align-items: flex-end

          .switch {
            position: relative
            display: inline-block
            width: 29px
            height: 19px
                      
            .slider {
              position: absolute
              cursor: pointer
              top: 0
              left: 0
              right: 0
              bottom: 0
              background-color: rgba(255,255,255,0.05)
              transition: 0.4s
              border: 1px solid #b4bec8

              &::before {
                position: absolute
                content: ""
                height: 17px
                width: 16px
                left: 0px
                bottom: 0px
                background-color: #b4bec8
                transition: 0.4s
              }
            }
            .round {
              border-radius: 34px

              &:before {
                border-radius: 50%
              }
            }

            input {
              opacity: 0
              width: 0
              height: 0
              
              &:checked + .slider {
                background-color: #2196f3
              }

              &:focus + .slider {
                box-shadow: 0 0 1px #2196f3
              }

              &:checked + .slider:before {
                transform: translateX(11px)
              }
            }
          }
        }

        .switch_text {
          margin-left:10px
        }
      }

      .col-second {
        flex-basis: 50%
        margin-left: 30px
        margin-top: 30px

        .banner {
          height: 84%
          position: relative

          .accept {
            display: flex
            flex-direction: row
            justify-content: center
            width: 100%
            position: absolute
            top: 55%
            color: #1af6d6

            .accept_text {
              font-size: 14px
              color: #1af6d6
              cursor: pointer
            }
          }
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
import {nextTick} from 'vue'
import inputField from './input-field.vue'
import textAreaField from './textarea-field.vue'
import pageTitle from './page-title.vue'
import btn from './button.vue'
import addImage from './add-image.vue'
import priceInput from './price-input.vue'
import {common} from '../utils/consts.js'
import utils from '../utils/utils.js'

export default {
  components: {
    inputField, 
    textAreaField, 
    pageTitle,
    btn,
    addImage,
    priceInput
  },
  data () {
    return {
      name: '',
      website: '',
      twitter: '',
      instagram: '',
      description: '',
      price: '',
      show: false,
      selected: 0,
      banner: {
        data:'',
        size:0
      },
      inputAccepts: ['.gif', '.jpeg', '.jpg'],
      selector_options: [
        {name: 'Collection 1'},
        {name: 'Collection 2'},
        {name: 'Collection 3'},
        {name: 'Collection 4'},
        {name: 'Collection 5'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},
        {name: 'Collection 6'},

      ],
    }
  },

  computed: {
    bannerStyles() {
      return {
        'border' :  this.banner ? '1px dashed transparent' : '1px dashed #1AF6D6',
      }
    },
    style() {
      return {
        'background-color': utils.getStyles().background_popup,
      }
    },
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
    
    can_submit () {
      return this.name && this.name_valid &&
             this.website_valid &&
             this.twitter_valid &&
             this.instagram_valid &&
             this.description_valid &&
             this.banner_valid 
    }
  },

  methods: {    
    selectedValue(i) {
      console.log('called')
      this.selected = i
      this.show = false
    },

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
      })
    },

    onRemoveBanner() {
      this.banner = {
        data: '',
        size: 0
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
        let clickAway = (evc) => {
          if (ev != evc) {
            document.removeEventListener('click', clickAway, true)
            this.close()
          }
        }
        document.addEventListener('click', clickAway, true)
      })
    },
  }
}
</script>