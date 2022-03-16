import gallery from './components/gallery.js'
import details from './components/artwork-details.vue'
import becomeArtist from './components/become-artist.vue'
import {createRouter, createWebHashHistory} from 'vue-router'

export const router = createRouter({
    history: createWebHashHistory(),
    routes:  [
        {path: '/', component: gallery, name: "gallery"},
        {path: '/details', component: details, name: "details"},
        {path:'/become-artist', component: becomeArtist, name: "becomeArtist"}
    ]
})
