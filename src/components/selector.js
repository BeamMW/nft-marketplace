import { nextTick } from "vue";
import utils from "../utils/utils.js";
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
    name: {
      type: String,
      required: false,
      default: "Sort by",
    },
    tabindex: {
      type: Number,
      required: false,
      default: 0,
    },
  },
  computed: {
    active_tab() {
      return this.$state.active_tab;
    },
  },

  data: function () {
    return {
      selected: this.default
        ? this.default
        : this.options.length > 0
        ? this.options[0]
        : null,
      left: 0,
      top: 0,
      show: false,
    };
  },
  mounted() {
    this.$emit("input", this.selected);
    const { background_popup } = utils.getStyles();
    let items = document.querySelectorAll(".items");
    if (items) {
      items.forEach((item) => {
        item.style.backgroundColor = background_popup;
      });
    }
  },
  template: ` 
        <div class="sort-container" :id="name" >
        <span class="sort-text">{{name}}</span>
        <div class="custom-select" :tabindex="tabindex" @blur="show = false">
            <div class="selected" :class="{ open: show }"  @click="open">
                {{ selected }}
            </div>
            <div  class="items" v-show="show">
                <div v-for="(option, i) of options || []" :key="i"
                @click="selected = option; show = false; onSelected(i,option);">
                    {{ option }}
                </div>
            </div>
        </div>
    </div>
    `,

  methods: {
    onSelected(val, opt) {
      if (this.name === "Sort by") {
        this.$store.setSortBy(val);
      } else {
        this.$store.filterByAuthor(opt);
      }
    },
    close() {
      this.show = false;
      this.left = 0;
      this.top = 0;
    },
    open(ev) {
      this.show = true;

      nextTick(() => {
        const elementHeight = this.$el.offsetHeight;
        const largestHeight = window.innerHeight - elementHeight;
        const elementWidth = this.$el.offsetWidth;
        const largestWidth = window.innerWidth - elementWidth;
        let top = ev.clientY;
        if (top > largestHeight) {
          top = largestHeight;
        }
        let left = ev.clientX;
        if (left > largestWidth) {
          left = largestWidth;
        }
        this.left = left;
        this.top = top;
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
  },
};
