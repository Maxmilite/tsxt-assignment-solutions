<script setup lang="tsx">
import { fail } from 'assert';
import { table } from 'console';
import { Table } from 'element-plus';
import { watch } from 'fs';
import { totalmem } from 'os';
import { onMounted, ref } from 'vue';
import { i18nAssets, timestampToDate } from '~/assets/constants';
import { getContestInfo, getRecords, getProblems } from '~/assets/telecom';

const props = defineProps({
  cid: Number,
  username: String
});

interface ContestItem {
  cid: number,
  name: string,
  startTime: number,
  endTime: number,
}

interface RecordItem {
  name: string,
  pid: number,
  time: number,
  score: number,
}

interface ProblemItem {
  pid: number,
  name: string,
  totalScore: number,
}

interface SimplifiedRecordItem {
  time: number,
  score: number,
  fb: boolean
}

interface TableItem {
  name: string,
  totalScore: number,
  totalTime: number,
  scores: SimplifiedRecordItem[]
}

const contestInfo = ref<ContestItem>();
const records = ref<RecordItem[]>();
const problems = ref<ProblemItem[]>();
const size = ref(0);

const tableData = ref<TableItem[]>([]);
const selfTableData = ref<TableItem>();
const selfRank = ref(0);

const successCounter = ref<number[]>([]);
const failureCounter = ref<number[]>([]);

const init = () => {
  if (records.value?.length === 0) return;
  selfTableData.value = {name: props.username || "", totalScore: 0, totalTime: 0, scores: 
      Array(size.value).fill({ time: 0, score: 0, fb: false })};
  successCounter.value = Array(size.value).fill(0);
  failureCounter.value = Array(size.value).fill(0);
  tableData.value = [];
  records.value?.forEach((e) => {
    let pos = -1;
    for (let i = 0; i < size.value; ++i) {
      if (problems.value![i].pid === e.pid) { pos = i; break; }
    };
    if (pos === -1) return;
    if (tableData.value?.filter((t) => t.name === e.name).length === 0) {
      tableData.value.push({name: e.name, totalScore: 0, totalTime: 0, scores: 
        Array(size.value).fill({ time: 0, score: 0, fb: false })});
    }
    let idx = -1;
    for (let i = 0; i < tableData.value.length; ++i) {
      if (tableData.value![i].name === e.name) { idx = i; break; }
    }

    if (e.score === problems.value![pos].totalScore) successCounter.value[pos]++;
    else failureCounter.value[pos]++;

    let cur = tableData.value[idx].scores[pos];
    if (cur!.score < e.score || cur!.time > e.time || (cur!.score === 0 && cur!.time === 0)) {
      tableData.value[idx].scores[pos] = { time: e.time, score: e.score, fb: false };
    }
  });
  for (let i = 0; i < size.value; ++i) {
    let idx = -1;
    for (let j = 0; j < tableData.value.length; ++j) {
      if (tableData.value[j].scores[i].score !== problems.value![i].totalScore) continue;
      if (idx === -1 || tableData.value[j].scores[i].time < tableData.value[idx].scores[i].time) idx = j;
    }
    if (idx === -1) continue;
    tableData.value[idx].scores[i].fb = true;
  }
  for (let j = 0; j < tableData.value.length; ++j) {
    for (let i = 0; i < size.value; ++i) { 
      tableData.value[j].totalScore += tableData.value[j].scores[i].score;
      if (tableData.value[j].scores[i].score) 
        tableData.value[j].totalTime += tableData.value[j].scores[i].time;
    }
  }
  tableData.value.sort((a: TableItem, b: TableItem) => { 
    return a.totalScore === b.totalScore ? a.totalTime - b.totalTime : b.totalScore - a.totalScore;
  });
  for (let j = 0; j < tableData.value.length; ++j) {
    if (tableData.value[j].name === props.username) { selfTableData.value = tableData.value[j]; selfRank.value = j + 1; break; }
  }
};

onMounted(() => {
    contestInfo.value = getContestInfo(props.cid!);
    records.value = getRecords(props.cid!);
    problems.value = getProblems(props.cid!);
    size.value = problems.value!.length;
    init();

    setInterval(() => {
      records.value = getRecords(props.cid!);
      init();
    }, 10000); // refreshing interval in milliseconds
})
</script>

<template>
  <div style="padding-top: 20px;">
    <h1>{{ i18nAssets.title + " - " + contestInfo?.name }}</h1>
    <ProgressBar :detailed="true" :start-time="contestInfo?.startTime" :end-time="contestInfo?.endTime" style="width: calc(100% - 100px); margin-left: 50px;"/>
  </div>
  <el-auto-resizer v-if="size" style="width: calc(100% - 60px); margin: 30px 30px;">
    <template #default="{ height, width }">
      <el-row justify="center">
        <el-col style="font-weight:bolder;" :span="2"><div class="grid-content grid-title-content"><p>{{ i18nAssets.scoreboard.rank }}</p></div></el-col>
        <el-col style="font-weight:bolder;" :span="4"><div class="grid-content grid-title-content"><p>{{ i18nAssets.scoreboard.name }}</p></div></el-col>
        <el-col style="font-weight:bolder;" :span="2"><div class="grid-content grid-title-content"><p>{{ i18nAssets.scoreboard.totalScore }}</p></div></el-col>
        <el-col style="font-weight:bolder;" :span="1" v-for="(problem, i) in problems">
          <el-tooltip placement="top">
            <template #content>{{ i18nAssets.scoreboard.problemName + problem.name }}<br />{{ i18nAssets.scoreboard.problemTotalScore + problem.totalScore }}</template>
            <div class="grid-content grid-title-content">
              <p>{{ String.fromCharCode(i + 65) }}</p>
            </div>
          </el-tooltip>
        </el-col>
      </el-row>
      <el-row justify="center" v-if="selfRank !== 0">
          <el-col style="font-weight:bolder;" :span="2"><div class="grid-content grid-normal-content"><p>{{ selfRank }}</p></div></el-col>
          <el-col :span="4"><div class="grid-content grid-normal-content"><p>{{ selfTableData!.name }}</p></div></el-col>
          <el-col :span="2"><div class="grid-content grid-normal-content"><p>{{ selfTableData!.totalScore }}</p></div></el-col>
          <el-col :span="1" v-for="(score, i) in selfTableData?.scores">
            <el-tooltip v-if="score.time !== 0" placement="top">
            <template v-if="score.time !== 0" #content>{{ i18nAssets.scoreboard.score + score.score }}<br />{{ i18nAssets.scoreboard.time + timestampToDate(score.time) }}</template>
            <div v-if="score.fb" class="grid-content grid-fb-content">
              <p>{{ score.score }}</p>
            </div>
            <div v-else-if="score.score === problems![i].totalScore" class="grid-content grid-normal-content">
              <p>{{ score.score }}</p>
            </div>
            <div v-else class="grid-content grid-failed-content">
              <p>{{ score.score }}</p>
            </div>
            
            </el-tooltip>
            <div v-else class="grid-content" style="min-height: 52.8px;">
            </div>
          </el-col>
      </el-row>
      <el-row justify="center" v-for="(data, i) in tableData">
          <el-col style="font-weight:bolder;" :span="2"><div class="grid-content grid-normal-content"><p>{{ i + 1 }}</p></div></el-col>
          <el-col :span="4"><div class="grid-content grid-normal-content"><p>{{ data.name }}</p></div></el-col>
          <el-col :span="2"><div class="grid-content grid-normal-content"><p>{{ data.totalScore }}</p></div></el-col>
          <el-col :span="1" v-for="(score, i) in data.scores">
            <el-tooltip v-if="score.time !== 0" placement="top">
            <template v-if="score.time !== 0" #content>{{ i18nAssets.scoreboard.score + score.score }}<br />{{ i18nAssets.scoreboard.time + timestampToDate(score.time) }}</template>
            <div v-if="score.fb" class="grid-content grid-fb-content">
              <p>{{ score.score }}</p>
            </div>
            <div v-else-if="score.score === problems![i].totalScore" class="grid-content grid-normal-content">
              <p>{{ score.score }}</p>
            </div>
            <div v-else class="grid-content grid-failed-content">
              <p>{{ score.score }}</p>
            </div>
            
            </el-tooltip>
            <div v-else class="grid-content" style="min-height: 52.8px;">
            </div>
          </el-col>
      </el-row>
      <el-row justify="center">
        <el-col :span="8"><div class="grid-content grid-title-content" style="min-height: 72px;"/><p>&nbsp;</p></el-col>
        <el-col :span="1" v-for="(problem, i) in problems">
          <div class="grid-content grid-title-content" style="display: flex; flex-direction: column; min-height: 72px; align-items: center; justify-content: center;">
            <div style="display: flex; flex-direction: row;">
              <el-icon><Check /></el-icon> &nbsp;
              <el-text>{{ successCounter[i] }}</el-text>
            </div>
            <div style="display: flex; flex-direction: row;">
              <el-icon><Close /></el-icon> &nbsp;
              <el-text>{{ failureCounter[i] }}</el-text>
            </div>
          </div>
        </el-col>
      </el-row>
    </template>
  </el-auto-resizer>
  <el-empty v-else description="No Data" />
</template>

<style lang="scss">
.ep-button {
  margin: 4px;
}

.ep-button+.ep-button {
  margin-left: 0;
  margin: 4px;
}

.el-row {
  margin-bottom: 20px;
}
.el-row:last-child {
  margin-bottom: 0;
}

.grid-content {
  height: 54px;
  text-align: center;
  display: flex;
  justify-content: center;
  align-items: center;
  border: 1px solid;
  border-color: var(--el-border-color);
}

.grid-title-content {
  background-color: #409EFFA0;
}

.grid-normal-content {
  background-color: #0cfc1833;
}

.grid-fb-content {
  background-color: #04af29d7;
}

.grid-failed-content {
  background-color: #ff330041;
}

</style>
