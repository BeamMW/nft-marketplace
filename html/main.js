import App      from './components/app.js'
import Loading  from './components/loading.js'
import Assets   from './components/assets.js'
import Error    from './components/error.js'
import utils    from './utils/utils.js'

utils.onLoad(beam => {
    const routes = [
        {path: '/',       name: "root",   component: Loading},
        {path: '/error',  name: "error",  component: Error, props: true},
        {path: '/assets', name: "assets", component: Assets}
    ]

    const router = VueRouter.createRouter({
        history: VueRouter.createWebHashHistory(),
        routes
    })

    const vueApp = Vue.createApp(App, {
        beam
    })

    vueApp.use(router)
    return vueApp.mount('body')
})
