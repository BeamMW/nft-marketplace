<template>
<div class="selector-container">
      <span class="selector-title">{{title}}</span>
      <div class="custom-select" :tabindex="tabindex" @blur="show  = false">
        <div class="selected" :class="{ open: show }" @click="open">
            {{ options[selected].name }}
              <img src="../assets/icon-down.svg"  class="arrow" />
        </div>
        <div class="items" v-show="show"  :style="style">
            <div v-for="(option, i) of options || []" :key="i"
                @click="onSelected(option,i)">
                <span :class="{ highlightLine: selected === i}">{{ option.name }}</span>
            </div>
        </div>
      </div>
    </div>

</template>


<style scoped lang="stylus">
.selector-container {
    margin-left: auto;
    display: flex;
    justify-content: center;
}
.selector-title{
    align-self: center;
    color: #fff;
    opacity: 0.5; 
}

.custom-select {
    position: relative;
    display: flex;
    text-align: left;
    outline: none;
    height: 47px;
    line-height: 47px;
    min-width: 0px;
    margin-left: auto;
}

.custom-select .selected {
    color: #fff;
    padding-left: 1em;
    cursor: pointer;
    user-select: none;
    opacity: 0.7;
    font-size: 14px;
    font-weight: bold;
    letter-spacing: 0.47px;
}

.custom-select .selected.open {
    border: none;
}

.custom-select .arrow {
    width:8px;
    height:8px;
    margin-left:3px;
}

.custom-select .items {
    color: #fff;
    position: absolute;
    border:none;
    border-radius: 4px;
    font-size: 16px;
    right: 0;
    z-index: 1;
    margin-top: 40px;
    padding: 0 20px;
}
  
.custom-select .items div {
    color: #fff;
    padding: 15px 0 15px 0;
    white-space: nowrap;
    line-height: 1;
    cursor: pointer;
    user-select: none;
}
  
.custom-select .items div:hover {
    color: #00f6d2;
}
</style>
<script>
import { nextTick } from 'vue';
import utils from '../utils/utils.js';

export default {
    props: {
      options: {
        type: Array,
        required: true,
      },
      selected: {
        type: Number,
        required: false,
        default: 0,
      },
      tabindex: {
        type: Number,
        required: false,
        default: 0,
      },
      title: {
          type: String,
          required: false,
          default: "",
      },
    },

    computed: {
      style() {
        return {
            "background-color": utils.getStyles().background_popup,
          }
      }
    },

    data() {
        return {
            show: false,
        };
    },

    emits: ['selected'],


    methods: {
      onSelected(opt,i) {
        this.$emit('selected',opt,i)
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
              document.removeEventListener("mousedown", downAway, true)
              this.close()
            }
          }
          let clickAway = (evc) => {
            if (ev != evc) {
              document.removeEventListener("click", clickAway, true)
              this.close()
            }
          }
          let scrollAway = (evc) => {
            document.removeEventListener("scroll", scrollAway, scroll)
            this.close()
          }
          document.addEventListener("mousedown", downAway, true)
          document.addEventListener("click", clickAway, true)
          document.addEventListener("scroll", scrollAway, true)
        })
      },
    }
}
</script>