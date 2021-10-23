import App      from './components/app.js'
import {store}  from './store.js'
import {router} from './router.js'
import utils    from './utils/utils.js'

utils.onLoad(beam => {
    const vueApp = Vue.createApp(App, {beam})
    
    vueApp.config.globalProperties.$store = store
    vueApp.config.globalProperties.$state = store.state
    vueApp.use(router)
    vueApp.mount('body')
    
    return {
        start () {
            const style = document.createElement('style');
            style.innerHTML = `.error {color: ${beam.style.validator_error};}`
            document.head.appendChild(style)
            document.body.style.color = beam.style.content_main
            store.start()
        },

        onApiResult(...args) {
            store.onApiResult(...args)
        }
    }
})
