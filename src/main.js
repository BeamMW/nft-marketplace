import './styles/fonts.css'
import './styles/global.css'

import App from 'components/app'
import router from 'router'
import utils from 'utils/utils'
import {createApp} from 'vue/dist/vue.esm-bundler.js'
import store from 'stores/global'
import nft from 'components/nft'
import collection from 'components/collection'
import createCollection from 'components/create-collection'
import approveCollection from 'components/approve-collection'
import approveArtist from 'components/approve-artist'
import createNFT from 'components/create-nft'
import approveNFT from 'components/approve-nft'
import ErrorEx from 'utils/errorex'

utils.initialize(
  {
    'appname': 'BEAM Gallery',
    'min_api_version': '6.2',
    'headless': true,
    'apiResultHandler': (...args) => store.onApiResult(...args)
  },
  (err) => {
    const vueApp = createApp(App)

    //
    // Handle uncaught vue errors & promise/async rejections
    //
    vueApp.config.errorHandler = (err, instance, info) => {store.setError(err)}
    window.addEventListener('unhandledrejection', ev => store.setError(ev.reason))

    vueApp.config.globalProperties.$store = store
    vueApp.config.globalProperties.$state = store.state
    vueApp.component('nft', nft)
    vueApp.component('collection', collection)
    vueApp.component('create-collection', createCollection)
    vueApp.component('approve-collection', approveCollection)
    vueApp.component('approve-artist', approveArtist)
    vueApp.component('create-nft', createNFT)
    vueApp.component('approve-nft', approveNFT)
    vueApp.use(router)
    vueApp.mount('body')

    const {validator_error,
      content_main,
      appsGradientTop,
      appsGradientOffset,
      background_main_top,
      background_main
    } = utils.getStyles()

    if (utils.isWeb()) {
      document.body.style.padding = '30px 10px 20px 30px'
    }

    const topColor = [appsGradientOffset, 'px,'].join('')
    const mainColor = [appsGradientTop, 'px,'].join('')
    const style = document.createElement('style')
    style.innerHTML = `.error {color: ${validator_error};}`
    document.head.appendChild(style)
    document.body.style.color = content_main

    if (!utils.isDesktop()) {
      document.body.style.backgroundImage = [
        'linear-gradient(to bottom,',
        background_main_top, topColor,
        background_main, mainColor,
        background_main
      ].join(' ')
    }

    if (err) {
      return store.setError(new ErrorEx('Failed to initialize application', err), true)
    }

    store.start()
  }
)
