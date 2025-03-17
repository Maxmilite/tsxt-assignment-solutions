<script setup lang="ts">
import { i18nAssets } from '~/assets/constants';


const props = defineProps({
  loggedIn: Boolean,
});

const emit = defineEmits(['response']);

const startSession = (e: any) => {
  emit('response', e);
}

</script>

<template>
  <div style="padding-top: 20px">
    <h1>{{ i18nAssets.title }}</h1>

    <div class="my-2 text-center flex flex-wrap justify-center items-center">
      <Contests v-if="props.loggedIn" @response="(e) => {startSession(e);}"/>
      <p style="padding-top: 30vh" v-else>{{ i18nAssets.signinRequired }}</p>
    </div>
  </div>
</template>

<style>
.ep-button {
  margin: 4px;
}

.ep-button+.ep-button {
  margin-left: 0;
  margin: 4px;
}
</style>
