import html from "../utils/html.js";
import { tabs } from "../utils/consts.js";
import CustomSelect from "./selector.js";

export default {
  computed: {
    active_tab() {
      return this.$state.active_tab;
    },
  },

  components: {
    CustomSelect,
  },

  render() {
    const selectorOptions = [
      "Added: Newest to Oldest",
      "Added: Oldest to Newest",
      "Sold: Newest to Oldest",
      "Sold: Oldest to Oldest",
      "Price: Low to High",
      "Price: High to Low",
      "Likes: Low to High",
      "Likes: High to Low",
    ];

    //TODO: catch event from custom-select
    return html`
      <div class="actions-container">
        <div class="artworks-controls w-full">
          <div class="artworks-controls__tabs">
            ${this.renderTab(tabs.ALL, "ALL")}
            ${this.renderTab(tabs.MINE, "MINE")}
            ${this.renderTab(tabs.SALE, "SALE")}
            ${this.renderTab(tabs.SOLD, "SOLD")}
            ${this.renderTab(tabs.LIKED, "LIKED")}
          </div>
          <${CustomSelect}
            options=${selectorOptions}
            default="Added: Newest to Oldest"
            class="select"
          />
        </div>
      </div>
    `;
  },

  methods: {
    onTabClicked(id) {
      if (this.active_tab !== id) {
        this.$store.setActiveTab(id);
      }
    },

    renderActiveLine(id) {
      if (id === this.active_tab) {
        return html` <div class="tab-item__bottom-line"></div> `;
      }
    },

    renderTab(type, title) {
      if (
        type === tabs.ALL ||
        (this.$state.artworks[type] !== undefined &&
          this.$state.artworks[type].length > 0)
      ) {
        return html`
          <span
            class="tab-item ${this.active_tab === type ? "tab-active" : ""}"
            onclick=${() => {
              this.onTabClicked(type);
            }}
          >
            <div class="tab-item__title">${title}</div>
            ${this.renderActiveLine(type)}
          </span>
        `;
      }
    },
  },
};
