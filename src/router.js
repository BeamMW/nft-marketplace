import gallery from './components/gallery.vue'
import artworkDetails from './components/artwork-details.vue'
import artistDetails from './components/become-artist.vue'
import {createRouter, createWebHistory} from 'vue-router'

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: gallery, name: 'gallery'},
    {path: '/artwork/:id', component: artworkDetails, name: 'artwork', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/artist', component: artistDetails, name: 'artist'}
  ]
})
