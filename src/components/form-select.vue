<template>
  <div>
    <div class="form-container">
      <label class="label">
        Collection
      </label>
    </div>
    <div class="custom-select" @blur="show = false">
      <div class="selected" :class="{'open': show}" @click="open">
        {{ options[selected_collection].name }}
        <img src="~assets/icon-down.svg" class="arrow"/>
      </div>
      <div v-show="show" class="items">
        <div v-for="option of options || []" :key="option.id" @click="onSelected($event, option.id)">
          <span :class="{ highlight: selected_collection === option.id}">{{ option.name }}</span>
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
  background: rgba(255,255,255,0.05)
  border-radius: 10px

  .selected {
    color: #fff
    width: 100%
    user-select: none
    font-size: 14px
    font-weight: normal
    letter-spacing: 0.47px
    cursor: pointer
    padding: 12px 15px

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
    background-color: rgb(50, 50, 50)
    color: #fff
    position: absolute
    border:none
    border-radius: 4px
    font-size: 16px
    right: 0
    z-index: 1
    margin-top: 47px
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
</style>

<script>
import {nextTick} from 'vue'
 
export default {
  props: {
    options: {
      type: Array,
      required: true,
    },
  },
  emits: ['selected'],
  data () {
    return {
      show: false,
      selected_collection: 1
    }
  },
  methods: {
    onSelected(ev, opt) {
      this.selected_collection = opt
      this.$emit('selected', opt)
      nextTick(() => {this.show = false})
    },
    close() {
      this.show = false
    },
    open(ev) {
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

