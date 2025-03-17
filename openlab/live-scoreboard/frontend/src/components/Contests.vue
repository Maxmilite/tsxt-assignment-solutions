<script setup lang="ts">

import { ref, onMounted } from "vue";
import { i18nAssets, timestampToDate } from "~/assets/constants";
import { getContestList } from "~/assets/telecom";

interface ContestItem {
  cid: number,
  name: string,
  startTime: number,
  endTime: number,
}

const props = defineProps({});

const tableData = ref<ContestItem[]>([]);

const emit = defineEmits(['response']);

const cb = (e: any) => {
  emit('response', e);
}



onMounted(() => {
  tableData.value = getContestList();
});

</script>

<template>
  <div style="height: 99%; width: 90%; border: 1px solid var(--ep-border-color); border-radius: 3px;">
    <div id="main">

      <el-table align="center" :data="tableData" stripe style="height: 90%;" :border="true">
        <el-table-column align="center" prop="name" :label="i18nAssets.contestList.contest_name" />
        <el-table-column align="center" :label="i18nAssets.contestList.duration" width="350">
          <template #default="scope">
            <p>{{ timestampToDate(tableData[scope.$index].startTime) + " ~ " + timestampToDate(tableData[scope.$index].endTime) }}</p>
          </template>
        </el-table-column>
        <el-table-column align="center" :label="i18nAssets.contestList.progress">
          <template #default="scope">
            <ProgressBar :start-time="tableData[scope.$index].startTime" :end-time="tableData[scope.$index].endTime"/>
          </template>
        </el-table-column>
        
        <el-table-column align="center" :label="i18nAssets.contestList.operations" width="100">
          <template #default="scope">
            <el-button link @click="cb(tableData[scope.$index].cid)">{{ i18nAssets.view }}</el-button>
          </template>
        </el-table-column>
      </el-table>
    </div>
  </div>
  <div style="width: 100%; height: 60px;"></div>
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
