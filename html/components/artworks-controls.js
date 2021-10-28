import html from '../utils/html.js';
import { tabs } from '../utils/consts.js';

export default {
    computed: {
        active_tab () {
            return this.$state.active_tab;
        }
    },

    render () {
        return html`
            <div class="artworks-controls">
                <div class="artworks-controls__tabs">
                    <span class="tab-item ${this.active_tab === tabs.ALL ? 'tab-active' : ''}" 
                    onclick=${()=>{this.onTabClicked(tabs.ALL)}}>
                        <div class="tab-item__title">ALL</div>
                        ${this.renderActiveLine(tabs.ALL)}
                    </span>
                    <span class="tab-item ${this.active_tab === tabs.MINE ? 'tab-active' : ''}" 
                    onclick=${()=>{this.onTabClicked(tabs.MINE)}}>
                        <div class="tab-item__title">MINE</div>
                        ${this.renderActiveLine(tabs.MINE)}
                    </span>
                    <span class="tab-item ${this.active_tab === tabs.SALE ? 'tab-active' : ''}" 
                    onclick=${()=>{this.onTabClicked(tabs.SALE)}}>
                        <div class="tab-item__title">ON SALE</div>
                        ${this.renderActiveLine(tabs.SALE)}
                    </span>
                    <span class="tab-item ${this.active_tab === tabs.LIKED ? 'tab-active' : ''}" 
                    onclick=${()=>{this.onTabClicked(tabs.LIKED)}}>
                        <div class="tab-item__title">LIKED</div>
                        ${this.renderActiveLine(tabs.LIKED)}
                    </span>
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
                return html`
                    <div class="tab-item__bottom-line"></div>
                `;
            }
        }
    }
}
