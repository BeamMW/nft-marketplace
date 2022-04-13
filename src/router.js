import userGallery from './components/user-gallery.vue'
import myPage from './components/my-page.vue'
import balance from './components/balance.vue'
import artworkDetails from './components/artwork-details.vue'
import artistDetails from './components/artist-details.vue'
import newCollection from './components/new-collection.vue'
import newNft from './components/new-nft.vue'
import {createRouter, createWebHistory} from 'vue-router'

// TODO:review routes
const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: userGallery, name: 'gallery'},
    {path: '/my-page', component: myPage, name: 'my-page'},
    {path: '/balance', component: balance, name: 'balance'},
    {path: '/artwork/:id', component: artworkDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/artist/:id?', component: artistDetails, name: 'artist', props: true},
    {path: '/new-collection', component: newCollection, name: 'new-collection'},
    {path: '/edit-collection/:id', component: newCollection, name: 'edit-collection', props: (route) => {return {id: Number(route.params.id)}}},
    {path:'/add-nft', component: newNft, name: 'add-nft'}
  ]
})

export default router