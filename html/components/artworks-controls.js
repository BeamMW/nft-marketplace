import html from '../utils/html.js';
import { common } from '../utils/consts.js';

export default {
    computed: {

    },

    render () {
        return html`
            <div class="artworks-controls">
                <div class="artworks-controls__tabs">
                    <span class="tab-item tab-active">
                        <div class="tab-item__title">ALL</div>
                        <div class="tab-item__bottom-line"></div>
                    </span>
                    <span class="tab-item">
                        <div class="tab-item__title">MINE</div>
                        
                    </span>
                    <span class="tab-item">
                        <div class="tab-item__title">ON SALE</div>

                    </span>
                    <span class="tab-item">
                        <div class="tab-item__title">LIKED</div>
                        
                    </span>
                </div>
            </div>
        `
    },

    methods: {

    }
}
