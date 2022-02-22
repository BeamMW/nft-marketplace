import { nextTick } from 'vue';
import utils from '../utils/utils.js';

export default {
    props: {
        options: {
          type: Array,
          required: true,
        },
        default: {
          type: String,
          required: false,
          default: null,
        },
        tabindex: {
          type: Number,
          required: false,
          default: 0,
        },
        name: {
            type: String,
            required: false,
            default: "Sort by",
        },
      },

      computed: {
        active_tab() {
          return this.$state.active_tab;
        },
        style() {
          return {
            "background-color": utils.getStyles().background_popup,
          }
        }
      },

      },
    data() {
        return {
            selected: this.default
            ? this.default
            : this.options.length > 0
            ? this.options[0]
            : null,
            show: false,
        };
    },

    mounted() {
        this.$emit("input", this.selected);
        const { background_popup } = utils.getStyles();
            
        document.getElementById('selector-items').style.backgroundColor = background_popup;
    },

    template: `
    <div class="sort-container" :id="name">
    <span class="sort-text">{{name}}</span>
    <div class="custom-select" :tabindex="tabindex" @blur="show  = false">
        <div class="selected" :class="{ open: show }" @click="open">
            {{ selected }}
        </div>
        <div class="items" v-show="show"  :style="style">
            <div v-for="(option, i) of options || []" :key="i"
            @click="selected = option; show = false; onSelected(i, option);">
                {{ option }}
            </div>
        </div>
      </div>
    </div>
    `,

    methods: {
        onSelected(val) {
            this.$store.setSortBy(val);
          },
          close() {
            this.show = false;
          },
          open(ev) {
            this.show = true;
      
            nextTick(() => {
              let downAway = (evc) => {
                if (!this.$el.contains(evc.target)) {
                  document.removeEventListener("mousedown", downAway, true);
                  this.close();
                }
              };
              let clickAway = (evc) => {
                if (ev != evc) {
                  document.removeEventListener("click", clickAway, true);
                  this.close();
                }
              };
              let scrollAway = (evc) => {
                document.removeEventListener("scroll", scrollAway, scroll);
                this.close();
              };
              document.addEventListener("mousedown", downAway, true);
              document.addEventListener("click", clickAway, true);
              document.addEventListener("scroll", scrollAway, true);
            });
          },
        }
    }
}
