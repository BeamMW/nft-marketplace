import userGallery from './components/user-gallery.vue'
import myPage from './components/my-page.vue'
import balance from './components/balance.vue'
import artworkDetails from './components/artwork-details.vue'
import editArtist from './components/edit-artist'
import editCollection from './components/edit-collection'
import newNft from './components/new-nft.vue'
import collectionDetails from './components/collection-details.vue'
import {createRouter, createWebHistory} from 'vue-router'

// TODO:review routes
const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: userGallery, name: 'gallery'},
    {path: '/my-page', component: myPage, name: 'my-page'},
    {path: '/balance', component: balance, name: 'balance'},
    {path: '/artwork/:id', component: artworkDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/edit-artist/:id', component: editArtist, name: 'edit-artist', props: true},
    {path: '/become-artist', component: editArtist, name: 'become-artist'},
    {path: '/edit-collection/:id', component: editCollection, name: 'edit-collection', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/new-collection', component: editCollection, name: 'new-collection'},
    {path: '/new-nft', component: newNft, name: 'new-nft'},
    {path: '/collection-details', component: collectionDetails, name: 'collection-details'}
  ]
})

export default router