import App      from './components/app.js'
import {store}  from './store.js'
import {router} from './router.js'
import utils    from './utils/utils.js'

utils.initialize({
    "appname": "BEAM Gallery",
    "apiResultHandler": (...args) => store.onApiResult(...args)
}, (err) => {
    const vueApp = Vue.createApp(App)
    vueApp.config.globalProperties.$store = store
    vueApp.config.globalProperties.$state = store.state
    vueApp.use(router)
    vueApp.mount('body')
    
    if (err) {
        return store.setError(err)
    }

    store.start()
})
