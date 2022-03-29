import gallery from './components/gallery.vue'
import details from './components/artwork-details.vue'
import myPage from './components/my-page.vue'
import balance from './components/balance.vue'
import artworkDetails from './components/artwork-details.vue'
import artistDetails from './components/become-artist.vue'
import collectionDetails from './components/collection-details.vue'
import {createRouter, createWebHistory} from 'vue-router'

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: gallery, name: 'gallery'},
    {path: '/details/:id', component: details, name: 'details', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/my-page', component: myPage, name: 'my-page'},
    {path: '/balance', component: balance, name: 'balance'},
    {path: '/artwork/:id', component: artworkDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/artist', component: artistDetails, name: 'artist'},
    {path: '/collection-details', component: collectionDetails, name: 'collection-details'}
  ]
})
