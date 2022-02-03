import Gallery from "./components/gallery.js";
import { createRouter, createWebHashHistory } from "vue-router";

export const router = createRouter({
  history: createWebHashHistory(),
  routes: [{ path: "/", component: Gallery, name: "gallery" }],
});
