import myPage from 'components/my-page'
import userPage from 'components/user-page'
import balancePage from 'components/balance-page'
import collectionPage from 'components/collection-page'
import nftDetails from 'components/nft-details'
import editArtist from 'components/edit-artist'
import editCollection from 'components/edit-collection'
import editNFT from 'components/edit-nft'
import moderatorPage from 'components/moderator-page'
import {createRouter, createWebHistory} from 'vue-router'

// TODO:review routes
const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: userPage, name: 'gallery'},
    {path: '/my-page', component: myPage, name: 'my-page'},
    {path: '/balance', component: balancePage, name: 'balance'},
    {path: '/nft/:id', component: nftDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/edit-artist/:id', component: editArtist, name: 'edit-artist', props: true},
    {path: '/become-artist', component: editArtist, name: 'become-artist'},
    {path: '/edit-collection/:id', component: editCollection, name: 'edit-collection', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/new-collection', component: editCollection, name: 'new-collection'},
    {path: '/edit-nft', component: editNFT, name: 'edit-nft'},
    {path: '/collection/:id', component: collectionPage, name: 'collection', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/moderator', component: moderatorPage, name: 'moderator'} 
  ]
})

export default router