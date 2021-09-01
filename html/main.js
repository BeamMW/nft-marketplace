import App    from './components/app.js'
import Assets from './components/assets.js'
import utils  from './utils/utils.js'

utils.onLoad(beam => {
    const routes = [
        {path: '/assets', component: Assets}
    ]

    const router = new VueRouter({
        routes
    })

    const vueApp = Vue.createApp(App, {
        beam
    })
    
    return vueApp.mount('body')
})
