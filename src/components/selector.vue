<template>
  <div class="selector-container">
    <span class="title">{{ title }}</span>
    <div class="custom-select" :tabindex="tabindex" @blur="show = false">
      <div ref="selected" class="selected" :class="{ open: show }" @click="open">
        {{ options[selected].name }}
        <img src="~assets/icon-down.svg" class="arrow"/>
      </div>
      <div v-show="show" class="items" :style="style">
        <div v-for="option of options || []" :key="option.id" @click="onSelected(option.id)">
          <span :class="{ highlight: selected === option.id}">{{ option.name }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .selector-container {
    display: flex
    align-items: center
    user-select: none

    .title {
      align-self: center
      color: #fff
      opacity: 0.5
      padding-bottom: 3px
    }

    .custom-select {
      outline: none
      position: relative

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
        width:8px
        height:8px
        margin-left:3px
      }

      .items {
        color: #fff
        position: absolute
        border: none
        border-radius: 4px
        font-size: 16px

        z-index: 1
        padding: 0 20px
        margin-top: 5px
        right: 0
          
        div {
          color: #fff
          padding: 15px 0 15px 0
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
  }    
</style>

<script>
import {nextTick} from 'vue'
import utils from 'utils/utils.js'

export default {
  props: {
    options: {
      type: Array,
      required: true,
    },
    selected: {
      type: Number,
      required: true,
    },
    tabindex: {
      type: Number,
      required: false,
      default: 0,
    },
    title: {
      type: String,
      required: false,
      default: '',
    },
  },

  emits: ['update:selected'],

  data() {
    return {
      show: false,
    }
  },

  computed: {
    style() {
      return {
        'background-color': utils.getStyles().background_popup,
      }
    }
  },

  methods: {
    onSelected(opt) {
      this.$emit('update:selected', opt)
      this.show = false
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