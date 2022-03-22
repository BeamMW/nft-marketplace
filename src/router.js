import gallery from './components/gallery.vue'
import details from './components/artwork-details.vue'
import myPage from './components/my-page.vue'
import {createRouter, createWebHistory} from 'vue-router'

export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {path: '/', component: gallery, name: 'gallery'},
    {path: '/details/:id', component: details, name: 'details', props: (route) => {return {id: Number(route.params.id)}}},
    {path: '/my-page', component: myPage, name: 'my-page'}
  ]
})
