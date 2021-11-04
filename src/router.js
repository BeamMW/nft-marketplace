import Gallery  from './components/gallery.js'

export const router = VueRouter.createRouter({
    history: VueRouter.createWebHashHistory(),
    routes:  [
        {path: '/', component: Gallery, name: "gallery"},
    ]
})
