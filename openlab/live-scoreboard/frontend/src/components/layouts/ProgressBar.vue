<script lang="ts" setup>
import { time, timeStamp } from 'console';
import { computed, onMounted, ref, watch } from 'vue'
import { i18nAssets } from '~/assets/constants';

const currentTime = ref<number>(0);
const percentage = ref<number>(0);
const duration = computed(() => Math.floor(percentage.value / 5));
const durationString = ref("");
const elapsedString = ref("");

const timestampToDate = (timestamp: number) => {
  let now = new Date(timestamp * 1000),
  y = now.getFullYear(),
  m = now.getMonth() + 1,
  d = now.getDate();
  return y + "-" + (m < 10 ? "0" + m : m) + "-" + (d < 10 ? "0" + d : d) + " " + now.toTimeString().substr(0, 8);
}

const props = defineProps({
  startTime: Number,
  endTime: Number,
  detailed: Boolean
})

watch(props, async (e, t) => {
  durationString.value = timestampToDate(props.startTime!) + " ~ " + timestampToDate(props.endTime!);
})

onMounted(() => {
  setInterval(() => {
    currentTime.value = new Date().getTime() / 1000;
    percentage.value = Math.floor((currentTime.value - props.startTime!) / (props.endTime! - props.startTime!) * 100);
    if (percentage.value > 100) percentage.value = 100;
    if (percentage.value < 0) percentage.value = 0;
    if (percentage.value == 0) elapsedString.value = i18nAssets.value.pending;
    else if (percentage.value == 100) elapsedString.value = i18nAssets.value.finished;
    else elapsedString.value = i18nAssets.value.time_elapse + Math.floor((Math.floor(new Date().getTime() / 1000) - props.startTime!) / 3600) + ":" +
       Math.floor(Math.floor((Math.floor(new Date().getTime() / 1000) - props.startTime!) % 3600) / 60) + ":" +
       Math.floor((Math.floor(new Date().getTime() / 1000) - props.startTime!) % 60);
  }, 500);
});

</script>

<template>
  <div class="progress">
    <p style="width: 100%" v-if="props.detailed">
      {{ durationString }}
    </p>
    <p style="width: 100%" v-if="props.detailed">
      {{ elapsedString }}
    </p>
      <el-progress v-if="percentage == 100"
        :percentage="100"
        :stroke-width="15"
        status="success"
        striped
        striped-flow
        :duration="10"
        style="margin: auto auto;"
      />
      <el-progress v-else
        :percentage="percentage"
        :stroke-width="15"
        striped striped-flow
        :duration="duration"

      >
      </el-progress>
  </div>
</template>

<style scoped>
.progress .el-progress--line {
  margin-bottom: 15px;
  width: 100%;
}
</style>