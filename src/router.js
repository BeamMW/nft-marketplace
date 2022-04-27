import gallery from './components/gallery.vue'
import myPage from './components/my-page.vue'
import balance from './components/balance.vue'
import artworkDetails from './components/artwork-details.vue'
import collectionDetails from './components/collection-details.vue'
import artistDetails from './components/artist-details.vue'
import table from './components/table-presentation.vue'
import newCollection from './components/new-collection.vue'
import newNft from './components/new-nft.vue'
import {createRouter, createWebHistory} from 'vue-router'

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: gallery, name: 'gallery'},
    {path: '/my-page', component: myPage, name: 'my-page'},
    {path: '/balance', component: balance, name: 'balance'},
    {path: '/artwork/:id', component: artworkDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/collection-details/:id', component: collectionDetails, name: 'collection-details', 
      props: (route) => {return {id: Number(route.params.id)}}
    },
    {path: '/table', component: table, name: 'table'},
    {path: '/artist/:id?', component: artistDetails, name: 'artist', props: true},
    {path: '/new-collection', component: newCollection, name: 'add-collection'},
    {path:'/add-nft', component: newNft, name: 'add-nft'}
  ]
})
