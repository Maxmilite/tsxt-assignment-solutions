<script setup lang="ts">

const props = defineProps({
  msg: String,
  loggedIn: Boolean
});

const emit = defineEmits(['response']);

const startSession = () => {
  emit('response', 1);
}

</script>

<template>
  <div style="padding-top: 60px">
    <h1>{{ msg }}</h1>

    <p>
      A chat window that can help you solve problems.
    </p>

    <div class="my-2 text-center flex flex-wrap justify-center items-center" style="padding-top: 35vh">
      <el-button v-if="loggedIn" type="primary" plain size="large" @click="startSession()">
        Try it now
        <el-icon class="el-icon">
          <ArrowRight />
        </el-icon>
      </el-button>
      <p v-else>You need to sign in before using Mini-ChatGPT.</p>
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
