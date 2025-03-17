import { createApp } from "vue";
import App from "./App.vue";

import "~/styles/index.scss";
import 'uno.css'

import "element-plus/theme-chalk/src/message.scss";

import * as ElementPlusIconsVue from '@element-plus/icons-vue';

const app = createApp(App);
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component)
}

app.mount("#app");