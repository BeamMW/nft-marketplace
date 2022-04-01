<template>
  <hdr title="collection name"/>
  <div class="collection">
    <!-- banner -->
    <imagePreview :image="banner" width="100vw" height="200px"/>

    <div :class="!full_text ? 'block' : 'block--transformed'">
      <div class="left">
        <!-- artist info -->
        <div class="artist">
          <div class="image">
            <imagePreview :image="author_image" radius="36px 36px"/>
          </div>

          <div class="artist__info">
            <span class="artist__name">{{ author }}</span>
            <span class="artist__description">{{ author_description }}</span>
          </div>
        </div>

        <!-- collection description -->
        <div class="collection__description">
          <span ref="desc" :class="!full_text ? 'desc' : ''">{{ description }}</span>
          <span v-if="!full_text && is_long_text" class="button-more" @click="full_text = true">See more</span>
        </div>
      </div>

      <div class="right">
        <!-- "social" block -->
        <div class="social">
          <btn v-if="website" color="transparent" height="20px">
            <img src="~assets/glob-green.svg">
          </btn>

          <btn v-if="twitter" color="transparent" height="20px">
            <img src="~assets/twitter-green.svg">
          </btn>

          <btn v-if="instagram" color="transparent" height="20px">
            <img src="~assets/instagram-green.svg">
          </btn>

          <btn color="transparent" height="20px">
            <img src="~assets/icon-key-small.svg">
          </btn>
        </div>

        <!-- collection info -->
        <div v-if="!full_text" class="collection__details">
          <div>
            <span class="value">{{ items }}</span>
            <span>items</span>
          </div>

          <div>
            <span class="value">{{ owners }}</span>
            <span>owners</span>
          </div>

          <div>
            <span class="tooltip">{{ price }}</span>
            <div>
              <img src="~assets/icon-beam.svg">
              <span class="value">{{ formatted_price }}</span>
            </div>
            <span>price</span>
          </div>

          <div>
            <span class="tooltip">{{ volume_traded }}</span>
            <div>
              <img src="~assets/icon-beam.svg">
              <span class="value">{{ formatted_volume_traded }}</span>
            </div>
            <span>volume traded</span>
          </div>
        </div>
      </div>

      <!-- footer button -->
      <div v-if="full_text" class="footer">
        <btn text="See less"
             height="20px"
             color="transparent"
             :text_bold="false"
             @click="full_text = false"
        />
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.collection {
  display: grid
  grid-template-columns: 1fr 1fr
  grid-template-rows: repeat(3, minmax(min-content, max-content))
  border-radius: 10px
  overflow: hidden
}

.banner {
  min-height: 200px
  max-height: 300px
  min-width: 100%
  grid-column: 1 / 3
}

.block {
  grid-column: 1 / 3
  display: grid
  grid-template-columns: 1fr 1fr
  grid-template-rows: 1fr
  column-gap: 10px
  padding: 20px
  background-color: rgba(255, 255, 255, 0.05)
}

.block--transformed {
  grid-column: 1 / 3
  grid-row: 2 / 3
  display: grid
  padding: 20px
  background-color: rgba(255, 255, 255, 0.05)
}

.artist {
  display: flex
  margin-bottom: 20px

  .image {
    margin-right: 16px
    min-width: 72px
    min-height: 72px
    max-width: 72px
    max-height: 72px
  }

  &__info {
    display: flex
    flex-direction: column
    justify-content: center
    padding-right: 20px

    & > * {
      overflow: hidden
      text-overflow: ellipsis
      display: -moz-box
      -moz-box-orient: vertical
      display: -webkit-box
      -webkit-line-clamp: 1
      -webkit-box-orient: vertical
      line-clamp: 1
    }
  }

  &__name {
    font-weight: bold
    font-size: 16px
    margin-bottom: 4px
  }

  &__description {
    font-size: 14px
  }
}

.collection__description {
  color: #8897a8
  font-size: 14px
  display: flex
  flex-direction: column

  .desc {
    overflow: hidden
    text-overflow: ellipsis
    display: -moz-box
    -moz-box-orient: vertical
    display: -webkit-box
    -webkit-line-clamp: 3
    -webkit-box-orient: vertical
    line-clamp: 3
  }

  .button-more {
    cursor: pointer
    color: #00f6d2
    align-self: flex-end
    margin-right: 20px
  }
}

.social {
  height: 16px
  display: flex
  justify-content: flex-end
  align-items: center
  margin-bottom: 20px

  & > * {
    margin-left: 6px
  }
}

.collection__details {
  width: 100%
  height: 60px
  padding-top: 16px
  padding-bottom: 16px
  border-radius: 10px
  background-color: rgba(255, 255, 255, 0.05)
  display: grid
  grid-template-columns: 1fr 1fr 1fr 1fr

  & > div {
    display: grid
    grid-template-rows: 1fr 1fr
    align-items: center
    justify-items: center
    position: relative
    
    & > div {
      display: flex
      
      & img {
        margin-right: 5px
      }
    }
    
    & > span:not(.value) {
      font-size: 14px
    }

    & .value {
      display: flex
      font-size: 16px
    }

    &:hover .tooltip {
      visibility: visible
    }

    & .tooltip {
      position: absolute
      width: max-content
      padding: 7px 21px
      border-radius: 10px
      bottom: 120%
      color: black
      background-color: #0bccf7
      visibility: hidden
    }

    & .tooltip::after {
      content: ""
      position: absolute
      top: 100%
      left: 45%
      border-width: 5px
      border-style: solid
      border-color: #0bccf7 transparent transparent transparent
    }
  }

  & > *:not(:last-child) {
    border-right: 1px solid rgba(255, 255, 255, 0.1)
  }
}

.footer {
  grid-column: 1 / 3
  height: min-content
  display: flex
  justify-content: center
  align-items: center
  margin-top: 20px
  color: red
}
</style>

<script>
import hdr from './page-title.vue'
import btn from './button.vue'
import imagePreview from './image-preview.vue'

import {binary_search} from '../utils/search.js'
import formatter from '../utils/formatter.js'

export default {
  components: {
    hdr,
    btn,
    imagePreview
  },

  props: {
    id: {
      type: Number,
      required: true
    },
  },

  data() {
    return {
      full_text: false,
      is_long_text: false
    }
  },

  computed: {
    collection() {
      let all = this.$state.collections
      let idx = binary_search(all, a => a.id == this.id ? 0 : a.id < this.id ? 1 : -1)

      return all[idx]
    },
    author() {
      return this.collection.author
    },
    author_description() {
      return this.collection.author_description
    },
    author_image() {
      return this.collection.author_image
    },
    banner() {
      return this.collection.cover_image
    },
    description() {
      return this.collection.description
    },
    items() {
      return this.collection.items
    },
    owners() {
      return this.collection.owners
    },
    price() {
      return this.collection.price
    },
    volume_traded() {
      return this.collection.volume_traded
    },
    formatted_price() {
      return formatter(this.collection.price)
    },
    formatted_volume_traded() {
      return formatter(this.collection.volume_traded)
    },
    website() {
      return this.collection.website
    },
    twitter() {
      return this.collection.twitter
    },
    instagram() {
      return this.collection.instagram
    },
  },

  mounted() {
    const scrollHeight = this.$refs.desc.scrollHeight
    const offsetHeight = this.$refs.desc.offsetHeight

    if (scrollHeight > offsetHeight) {
      this.is_long_text = true
    }
  },

  methods: {
    toggleVisibleText() {
      !this.full_text ? this.full_text : !this.full_text
    },

    formatNumber(num) {
      if (num < 0.01) {
        return '< 0.01'
      }

      return num.toFixed(3)
    }
  },
}
</script>
