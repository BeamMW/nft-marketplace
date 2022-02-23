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
        active_tab() {
          return this.$state.active_tab;
        },
        style() {
          return {
            "background-color": utils.getStyles().background_popup,
             "width": this.title === "Sort by" ? "200px" : "100px"
          }
        }
      },

    data() {
        return {
            selected_option: this.selected,
            show: false,
        };
    },
    emits: ['sort_by'],

    template: `
    <div class="sort-container">
    <span class="sort-text">{{title}}</span>
    <div class="custom-select" :tabindex="tabindex" @blur="show  = false">
        <div class="selected" :class="{ open: show }" @click="open">
            {{ options[selected_option].name }}
        </div>
        <div class="items" v-show="show"  :style="style">
            <div v-for="(option, i) of options || []" :key="i"
            @click="onSelected(i,option)">
                <span :class="{ highlightLine: selected_option === i}">{{ option.name }}</span>
            </div>
        </div>
      </div>
    </div>
    `,

    methods: {
        onSelected(idx,opt) {
          this.selected_option = idx;
          this.$emit('sort_by',opt)
          this.show = false;
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
