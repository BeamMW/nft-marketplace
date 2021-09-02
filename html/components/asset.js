import html    from '../utils/html.js'
import utils   from '../utils/utils.js'
import loading from './loading.js'

export default {
    props: {
        id: {
            type: Number,
            required: true
        },
        artist: {
            type: String,
            required: true
        },
        title: {
            type: String,
            required: true,
            default: "Loading..."
        },
        bytes: {
            type: Object,
            default: undefined
        }
    },

    render (...args) {
        return html`
            <div class="item">
                ${this.renderPreview()}
                <div class="first-row">
                    <span class="artist">${this.artist}</span>
                    <span class="cprice">Price</span>
                </div>
                <div class="second-row">
                    <span class="title">${this.title}</span>
                    <span class="price">${this.id * 2} BEAM</span>
                </div>
            </div>
        `
    },

    methods: {
        renderPreview() {
            if (this.bytes) {
                let image = URL.createObjectURL(new Blob([this.bytes], {type: 'image/jpeg'}))
                return html`
                    <div class="preview-container">
                        <img src=${image}/>
                    </div>
                `
            } else {
                return html`
                    <div class="preview-container">
                        <${loading}/>
                    </div>
                `
            }
        }
    }
}

