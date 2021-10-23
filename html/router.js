import Assets  from './components/assets.js'
import ErrView from './components/error.js'
import Loading from './components/loading.js'

export const router = VueRouter.createRouter({
    history: VueRouter.createWebHashHistory(),
    routes:  [
        {path: '/', component: Assets, name: "main"},
    ]
})
