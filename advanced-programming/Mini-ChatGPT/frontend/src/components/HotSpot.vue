<script setup lang="ts">

import { ref, onMounted } from "vue";

interface WordItem {
  question: string,
  answer: string,
  popularity: number,
  rank: number
}

const props = defineProps({
  hotSpotFunction: Function
});

const tableData = ref<WordItem[]>([]);

onMounted(() => {
  if (props.hotSpotFunction !== undefined) {
    tableData.value = props.hotSpotFunction();
  }
});

</script>

<template>
  <div style="height: 99%; border: 1px solid var(--ep-border-color); border-radius: 3px;">
    <h1>Mini ChatGPT Search HotSpot List</h1>
    <div id="main">
      <el-table align="center" :data="tableData" stripe style="width: 80%; height: 90%;" :border="true">
        <el-table-column sortable align="center" prop="rank" label="Rank" width="100" />
        <el-table-column sortable align="center" prop="question" label="Question" />
        <el-table-column sortable align="center" prop="answer" label="Answer" />
        <el-table-column sortable align="center" prop="popularity" label="Popularity" width="120" />
      </el-table>
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
