<template>
  <div>
    <div class="form-container">
      <label class="label">{{ title }}</label>
    </div>
    <div class="custom-select" :class="{'readonly': readonly}" @blur="show = false">
      <div class="selected" :class="{'open focus': show}" @click="open">
        {{ options[modelValue].name }}
        <img src="~assets/arrow-down.svg" class="arrow"/>
      </div>
      <div v-show="show" class="items">
        <div v-for="(option, idx) in (options || [])" :key="option.id" @click="onSelected($event, idx)">
          <span :class="{ highlight: modelValue === idx}">{{ option.name }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.form-container {
  box-sizing: border-box

  .label {
    display: block
    margin-bottom:10px
    color: rgba(255,255,255,0.6)
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
  background-color: rgba(255, 255, 255, 0.08)
  border-radius: 10px
  cursor: pointer

  .selected {
    color: #fff
    width: 100%
    user-select: none
    font-size: 14px
    font-weight: normal
    letter-spacing: 0.47px
    padding: 12px 15px

    &.open {
      border: none
      border-radius: 10px
    }

    &.focus {
      background-color: rgba(255, 255, 255, 0.12)
    }
  }

  .arrow {
    width: 9px
    height: 5px
    position: absolute
    right: 20px 
    top:50%
  }

  /* matches the DEX dropdown: --color-dark-blue panel, no grey */
  .items {
    background-color: #042548
    color: #fff
    position: absolute
    border: 1px solid rgba(255, 255, 255, 0.08)
    border-radius: 10px
    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.5)
    font-size: 14px
    right: 0
    z-index: 1
    margin-top: 47px
    padding: 6px 0
    width: 100%
    max-height: 250px
    overflow-y: auto
    box-sizing: border-box

    div {
      color: #fff
      padding: 12px 15px
      white-space: nowrap
      line-height: 1
      cursor: pointer
      user-select: none

      &:hover {
        background-color: rgba(255, 255, 255, 0.07)
      }
    }

    .highlight {
      font-weight: bold
      color: #00f6d2
    }
  }
}

.readonly {
  background: rgba(255, 255, 255, 0.03)
  cursor: initial
}
</style>

<script>
import {nextTick} from 'vue'
 
export default {
  props: {
    title: {
      type: String,
      default: '',
      requred: true
    },
    options: {
      type: Array,
      required: true,
    },
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: [Number],
      required: true,
      default: 0,
    },
    readonly: {
      type: Boolean,
      default: false,
      required: false
    },
  },
  
  emits: [
    'update:modelValue'
  ],

  data () {
    return {
      show: false,
    }
  },
  
  methods: {
    onSelected(ev, idx) {
      this.$emit('update:modelValue', idx)
      nextTick(() => {
        this.show = false
      })
    },
    close() {
      this.show = false
    },
    open(ev) {
      if(this.readonly) {
        return 
      }
      if (this.show) {
        return
      }
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

