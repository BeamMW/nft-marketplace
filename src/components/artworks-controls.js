import html from '../utils/html.js';
import { tabs, sort } from '../utils/consts.js';
import selector from './selector.js';

export default {
    computed: {
        active_tab () {
            return this.$state.active_tab
        },
        artists () {
            return this.$state.artists
        },
        active_sort_by() {
            return this.$state.sort_by;
        },
        tabs_sort_by() {
            const result = [{ name: "All", id:tabs.ALL }]

            if(this.$state.artworks[tabs.MINE].length > 0) {
                result.push({ name: "MINE", id: tabs.MINE })
            }

            if(this.$state.artworks[tabs.SALE].length > 0) {
                result.push({ name: "SALE", id: tabs.SALE })
            }

            if(this.$state.artworks[tabs.SOLD].length > 0) {
                result.push({ name: "SOLD", id: tabs.SOLD })
            }

            if(this.$state.artworks[tabs.LIKED].length > 0) {
                result.push({ name: "LIKED", id: tabs.LIKED })
            }

            return result
        },
        artist_options () {
            let result = [{name: "Everyone", key: 0}]
            for(let aid in this.artists) {
                let artist = this.artists[aid]
                result.push({name: artist.label, key: artist.key})
            } 
            return result
        },
        active_filter_by_artist () {
            let key = this.$state.filter_by_artist
            let artists = this.artist_options
            for (let idx = 0; idx <artists.length; ++idx) {
                if (key == artists[idx].key) {
                    return idx
                }
            }
            return 0
        }
    },

    components: {
        selector
    },

    data() {
        return {
            selector_options: [
                { name: "Added: Newest to Oldest", sort_type: sort.NEWEST_TO_OLDEST },
                { name: "Added: Oldest to Newest", sort_type: sort.OLDEST_TO_NEWEST },
                { name: "Price: Low to High", sort_type: sort.PRICE_ASC },
                { name: "Price: High to Low", sort_type: sort.PRICE_DESC },
                { name: "Likes: Low to High", sort_type: sort.LIKES_ASC },
                { name: "Likes: High to Low", sort_type: sort.LIKES_DESC }
             ],
        }
    },

    template: `
        <div class="actions-container">
            <div class="artworks-controls">
                <div class="artworks-controls__tabs">
                    <span v-for="(tab,i) of tabs_sort_by" class="tab-item" :class="{ 'tab-active': active_tab === tab.id}" @click="onTabClicked(tab.id)">
                        <div class="tab-item__title">{{tab.name}}</div>
                        <div v-if="active_tab === tab.id" class="tab-item__bottom-line"></div>
                    </span>
                </div>
                <div class="artwork-controls__selectors">
                    <selector
                        v-on:selected="onAuthor"
                        :options="artist_options"
                        :selected="active_filter_by_artist"
                        title="Author"
                    />
                    <selector
                        v-on:selected="onSortBy"
                        :options="selector_options"
                        :selected="active_sort_by"
                        title="Sort by"
                    />
                </div>
           </div>
        </div>
    `,

    methods: {
        onTabClicked(id) {
            if (this.active_tab !== id) {
                this.$store.setActiveTab(id)
            }
        },   
        onSortBy(opt) {
            this.$store.setSortBy(opt.sort_type)
            this.$parent.$refs.artslist.scrollTop = 0
        },
        onAuthor(opt) {
            this.$store.setFilterByArtist(opt.key)
            this.$parent.$refs.artslist.scrollTop = 0
        }
    }
}
