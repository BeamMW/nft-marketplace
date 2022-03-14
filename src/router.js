import gallery from './components/gallery.js'
import details from './components/artwork-details.vue'
import {createRouter, createWebHashHistory} from 'vue-router'

export const router = createRouter({
    history: createWebHashHistory(),
    routes:  [
        {path: '/', component: gallery, name: "gallery"},
        {path: '/details', component: details, name: "details"}
    ]
})
