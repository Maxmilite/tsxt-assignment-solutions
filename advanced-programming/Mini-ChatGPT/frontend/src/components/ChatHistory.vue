<script setup lang="ts">
import "~/styles/index.scss";
import 'uno.css'
import "element-plus/theme-chalk/src/message.scss"
import "~/styles/chat-session.scss"

import { onMounted, ref } from 'vue'
import { ElScrollbar } from 'element-plus'

const props = defineProps({
  historyFunction: Function,
  category: String
});

interface WordItem {
  question: string,
  answer: string,
  date: Date,
  category: string,
  id: number
}

const words = ref<WordItem[]>([])
const loadAll = () => {
  let res = props.historyFunction!(props.category);
  return res;
}

const innerChatBox = ref<HTMLDivElement>();
const chatBoxRef = ref<InstanceType<typeof ElScrollbar>>();


onMounted(() => {
  words.value = loadAll();
})

</script>

<template>
  <el-container style="height: calc(100vh - 60px);">
    <el-main id="chatbox-main" style="height: inherit; margin: 0 0; padding: 0 0;">
      <el-scrollbar ref="chatBoxRef">
        <p>Message in {{ category == undefined ? "All Categories" : "Category " + category }}</p>
        <div ref="innerChatBox">
          <div v-for="item in words">
            <p class="scrollbar-self" style="margin-right: 20px;">{{ item.question }}</p>
            <p class="scrollbar-chatbot" style="margin-left: 20px;">{{ item.answer }}</p>
          </div>
        </div>
      </el-scrollbar>
    </el-main>
  </el-container>
</template>
