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
      },
    data() {
        return {
            selected: this.default
            ? this.default
            : this.options.length > 0
            ? this.options[0]
            : null,
            open: false,
        };
    },

    mounted() {
        this.$emit("input", this.selected);
        const { background_popup } = utils.getStyles();
            
        document.getElementById('selector-items').style.backgroundColor = background_popup;
    },

    template: `
        <div class="sort-container">
        <span class="sort-text">Sort by</span>
        <div class="custom-select" :tabindex="tabindex" @blur="open = false">
            <div class="selected" :class="{ open: open }" @click="open = !open">
                {{ selected }}
            </div>
            <div id="selector-items" class="items" :class="{ selectHide: !open }">
                <div v-for="(option, i) of options" :key="i"
                @click="selected = option; open = false; onSelected(i);">
                    {{ option }}
                </div>
            </div>
        </div>
    </div>
    `,

    methods: {
        onSelected(val) {
            this.$store.setSortBy(val);
        }
    }
}
