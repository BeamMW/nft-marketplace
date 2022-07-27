import myPage from 'components/my-page'
import userPage from 'components/user-page'
import balancePage from 'components/balance-page'
import collectionPage from 'components/collection-page'
import nftPage from 'components/nft-page'
import editArtist from 'components/edit-artist'
import editCollection from 'components/edit-collection'
import editNFT from 'components/edit-nft'
import moderatorPage from 'components/moderator-page'
import {createRouter, createWebHashHistory} from 'vue-router'

const router = createRouter({
  history: createWebHashHistory(window.location.pathname),
  routes: [
    {path: '/', component: userPage, name: 'gallery'},
    {path: '/my', component: myPage, name: 'my'},
    {path: '/balance', component: balancePage, name: 'balance'},
    {path: '/edit-artist/:id', component: editArtist, name: 'edit-artist', props: true},
    {path: '/become-artist', component: editArtist, name: 'become-artist'},
    {path: '/edit-collection/:id', component: editCollection, name: 'edit-collection', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/new-collection', component: editCollection, name: 'new-collection'},
    {path: '/new-nft', component: editNFT, name: 'new-nft', 
      props: (route) => {
        return route.params.collid ? {collid: Number(route.params.collid)} : {}
      }
    },
    {
      path: '/collection/:id', 
      component: collectionPage, 
      name: 'collection', 
      props: (route) => {
        return {
          id: Number(route.params.id),
          mode: route.query.mode
        }
      }
    },
    {
      path: '/nft/:id', 
      component: nftPage, 
      name: 'nft',
      props: (route) => {
        return {
          id: Number(route.params.id),
          mode: route.query.mode
        }
      },
    },
    {path: '/moderator', component: moderatorPage, name: 'moderator'} 
  ]
})

export default router