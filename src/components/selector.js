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
            "width": this.title === "Sort by" ? "200px" : "100px",
            "left": this.title === "Sort by" ? "-50px" : "-20px",
          }
      }
    },

    data() {
        return {
            show: false,
        };
    },

    emits: ['selected'],

    template: `
    <div class="selector-container">
      <span class="selector-title">{{title}}</span>
      <div class="custom-select" :tabindex="tabindex" @blur="show  = false">
        <div class="selected" :class="{ open: show }" @click="open">
            {{ options[selected].name }}
              <img src="./assets/icon-down.svg"  class="arrow" />
        </div>
        <div class="items" v-show="show"  :style="style">
            <div v-for="(option, i) of options || []" :key="i"
                @click="onSelected(option)">
                <span :class="{ highlightLine: selected === i}">{{ option.name }}</span>
            </div>
        </div>
      </div>
    </div>
    `,

    methods: {
      onSelected(opt) {
        this.$emit('selected',opt)
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
