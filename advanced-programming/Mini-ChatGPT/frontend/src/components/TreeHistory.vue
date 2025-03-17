<script setup lang="ts">
;
import { ref, onMounted, reactive } from "vue";
import "~/styles/index.scss";
import 'uno.css'
import "element-plus/theme-chalk/src/message.scss"
import "~/styles/chat-session.scss"

interface WordItem {
  question: string,
  answer: string,
  date: Date,
  category: string,
  id: number
}

interface Tree {
  label: string
  children?: Tree[]
}

const dialogFormVisible = ref(false);
const form = reactive({
  id: 0,
  category: '',
})
const treeData = ref<Tree[]>([]);

const props = defineProps({
  historyFunction: Function,
  setCategoryFunction: Function,
  categoriesFunction: Function
});

const tableData = ref<WordItem[]>([]);
const groupList = ref<String[]>([]);

const defaultProps = {
  children: 'children',
  label: 'label',
}

onMounted(() => {
  if (props.historyFunction !== undefined) {
    tableData.value = props.historyFunction();
    groupList.value = props.categoriesFunction!();
    groupList.value.forEach((e: String) => {
      treeData.value.push({ label: e.toString(), children: [] })
    })
    tableData.value.forEach((e) => {
      for (let i in treeData.value) {
        if (treeData.value[i].label == e.category) {
          treeData.value[i].children!.push({ label: `Question: ${e.question} | Answer: ${e.answer} | Date: ${e.date}`, children: [] });
        }
      }
    })

  }
});

</script>

<template>
  <div style="height: 99%; border: 1px solid var(--ep-border-color); border-radius: 3px;">
    <h2>Mini-ChatGPT Tree History</h2>
    <div id="main" style="flex-direction: column;">
      
      <el-scrollbar style="border: 2px solid var(--ep-border-color); padding: 20px 20px;">
        <el-tree :data="treeData" :props="defaultProps" accordion
          style="width: 70vw;" />
      </el-scrollbar>

    </div>
  </div>
</template>

<style>
#main {
  width: 100%;
  height: 80%;
  display: flex;
  align-items: center;
  justify-content: center;
}
</style>
