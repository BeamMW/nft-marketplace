import html from '../utils/html.js';
import { tabs } from '../utils/consts.js';
import CustomSelect from './selector.js';

export default {
    computed: {
        active_tab () {
            return this.$state.active_tab
        },
        mine_tab_state () {
            return this.$state.artworks[tabs.MINE].length > 0
        },
        liked_tab_state () {
            return this.$state.artworks[tabs.LIKED].length > 0
        },
        sold_tab_state () {
            return this.$state.artworks[tabs.SOLD].length > 0
        },
        sale_tab_state () {
            return this.$state.artworks[tabs.SALE].length > 0
        },
    },

    components: {
        CustomSelect
    },

    render () {
        const selectorOptions = [
            "Added: Newest to Oldest",
            "Added: Oldest to Newest",
            "Price: Low to High",
            "Price: High to Low",
            "Likes: Low to High",
            "Likes: High to Low",
        ];

        //TODO: catch event from custom-select
        return html`
            <div class="actions-container">
                <div class="artworks-controls">
                    <div class="artworks-controls__tabs">
                        ${this.renderTab(tabs.ALL, 'ALL')}
                        ${this.mine_tab_state ? this.renderTab(tabs.MINE, 'MINE') : null}
                        ${this.sale_tab_state ? this.renderTab(tabs.SALE, 'SALE') : null}
                        ${this.sold_tab_state ? this.renderTab(tabs.SOLD, 'SOLD') : null}
                        ${this.liked_tab_state ? this.renderTab(tabs.LIKED, 'LIKED') : null}
                    </div>
                  <${CustomSelect}
                    options=${selectorOptions}
                    default="Added: Newest to Oldest"
                    class="select"
                    key="Sort"
                  />
                </div>
            </div>
        `;

        //enable after author load fix
        // <${CustomSelect}
        // options=${selectorOptions}
        // default="Sort by"
        // class="select"
        // />
    },

    methods: {
        onTabClicked(id) {
            if (this.active_tab !== id) {
                this.$store.setActiveTab(id);
            }
        },

        renderActiveLine(id) {
            if (id === this.active_tab) {
                return html`
                    <div class="tab-item__bottom-line"></div>
                `;
            }
        },

        renderTab(type, title) {
            return html`
                <span class="tab-item ${this.active_tab === type ? 'tab-active' : ''}" 
                onclick=${()=>{this.onTabClicked(type)}}>
                    <div class="tab-item__title">${title}</div>
                    ${this.renderActiveLine(type)}
                </span>
            `;
        }
    }
}
