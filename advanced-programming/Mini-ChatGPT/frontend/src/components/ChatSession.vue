<script setup lang="ts">
import { ElButton, ElMessage, genFileId, UploadProps, UploadRawFile } from 'element-plus';
import "~/styles/index.scss";
import 'uno.css'
import "element-plus/theme-chalk/src/message.scss"
import "~/styles/chat-session.scss"

import { onBeforeUnmount, onMounted, ref } from 'vue'
import { ElScrollbar } from 'element-plus'
import { UploadFilled } from '@element-plus/icons-vue'
import { UploadInstance } from 'element-plus/es/components/upload';
import { fdatasync, read } from 'fs';

const props = defineProps({
  submitFunction: Function,
  autoCompleteListFunction: Function
});

interface WordItem {
  value: string
}

const search_state = ref('')

const words = ref<WordItem[]>([])
const querySearch = (queryString: string, cb: any) => {
  const results = queryString
    ? words.value.filter(createFilter(queryString))
    : words.value
  cb(results)
}
const createFilter = (queryString: string) => {
  return (word: WordItem) => {
    return (
      word.value.toLowerCase().indexOf(queryString.toLowerCase()) !== -1
    )
  }
}
const loadAll = () => {
  let res = props.autoCompleteListFunction === undefined ? [] : props.autoCompleteListFunction();
  return res;
}

const messageList = ref([
  { id: 1, message: "I\'m a robot. You can chat with me freely now." },
]);

const innerChatBox = ref<HTMLDivElement>();
const chatBoxRef = ref<InstanceType<typeof ElScrollbar>>();

const submitLoading = ref(false);

function submit() {

  if (documentType.value == "File") {
    submitFile();
    return;
  }

  if (props.submitFunction === undefined)
    return;
  let submitButton = document.getElementById("submit-button");
  if (submitButton === null)
    return;
  let message = search_state.value;
  search_state.value = "";


  if (!message) {
    ElMessage.info("You need to type some words.");
    return;
  }

  submitLoading.value = true;
  messageList.value.push({ id: 0, message: message });
  updateChatBoxHeight();

  props.submitFunction(message, receiveMessage);

}

const receiveMessage = (result: string) => {
  messageList.value.push({ id: 1, message: result });
  updateChatBoxHeight();
  submitLoading.value = false;
}

const receiveFileRequest = (response: any) => {
  console.log(response);
}

const updateChatBoxHeight = () => {
  setTimeout(() => chatBoxRef.value!.setScrollTop(innerChatBox.value!.clientHeight));
}

function registerSession() {
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", "/api/session/register", false);
  HTTPRequest.withCredentials = true;
  try {
    HTTPRequest.send();
    let returnMessage = JSON.parse(HTTPRequest.responseText);
    if (returnMessage.code == 200) {
      return 1;
    } else {
      ElMessage.error("Error: " + returnMessage.msg);
      return 0;
    }
  } catch (error) {
    ElMessage.error("Error: Network Error");
    return 0;
  }
}

function renewSession() {
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", "/api/session/renew", false);
  HTTPRequest.withCredentials = true;
  try {
    HTTPRequest.send();
    let returnMessage = JSON.parse(HTTPRequest.responseText);
    if (returnMessage.code == 200) {
      return 1;
    } else {
      ElMessage.error("Error: " + returnMessage.msg);
      return 0;
    }
  } catch (error) {
    ElMessage.error("Error: Network Error");
    return 0;
  }
}

function removeSession() {
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", "/api/session/remove", false);
  HTTPRequest.withCredentials = true;
  try {
    HTTPRequest.send();
    let returnMessage = JSON.parse(HTTPRequest.responseText);
    if (returnMessage.code == 200) {
      return 1;
    } else {
      ElMessage.error("Error: " + returnMessage.msg);
      return 0;
    }
  } catch (error) {
    ElMessage.error("Error: Network Error");
    return 0;
  }
}

const emit = defineEmits(['response']);
function exit(e: number) {
  emit('response', e);
}

var schedule: any;
const registered = ref(false);

const documentType = ref('Text');
const options = [
  {
    value: 'Text',
    label: 'Text',
  },
  {
    value: 'Article',
    label: 'Article',
  },
  {
    value: 'File',
    label: 'File',
  }
]

const upload = ref<UploadInstance>();

const handleExceed: UploadProps['onExceed'] = (files) => {
  upload.value!.clearFiles();
  const file = files[0] as UploadRawFile;
  file.uid = genFileId();
  upload.value!.handleStart(file);
}
const submitFile = () => {
  upload.value!.submit();
  upload.value!.clearFiles();
}
const handleUpload = (e: any) => {
  const reader = new FileReader();
  reader.readAsText(e.file);
  let HTTPRequest = new XMLHttpRequest();
  reader.onload = () => {
    submitLoading.value = true;
    messageList.value.push({ id: 0, message: "Message from file: " + reader.result?.toString() || "" });
    updateChatBoxHeight();
    HTTPRequest.open("GET", "api/message?message=" + reader.result, true);
    HTTPRequest.onreadystatechange = () => {
      if (HTTPRequest.readyState == 4) {
        if (HTTPRequest.status == 200) {
          receiveMessage(HTTPRequest.responseText);
        }
        else if (HTTPRequest.status == 401) {
          receiveMessage("You need to log in before using this ChatBot.");
        } else if (HTTPRequest.status == 403) {
          receiveMessage("Server is full currently, you are kicked out.");
        } else {
          receiveMessage("An error occurred while corresponding with the chatbot.");
        }
      }
    }
    HTTPRequest.send();
  }
  return HTTPRequest;
}

onMounted(() => {
  words.value = loadAll();
  updateChatBoxHeight();
  if (registerSession()) {
    ElMessage.success("A new chat session created.");
    schedule = setInterval(renewSession, 60000);
    registered.value = true;
  } else {
    exit(0);
  }
})

onBeforeUnmount(() => {
  if (registered.value) {
    removeSession();
    window.clearInterval(schedule);
  }
})


</script>

<template>
  <el-container style="height: calc(100vh - 60px);">
    <el-main id="chatbox-main">
      <el-scrollbar ref="chatBoxRef">
        <div ref="innerChatBox">
          <div v-for="item in messageList">
            <span v-if="item.id === 0" class="scrollbar-self">{{ item.message }}</span>
            <span v-else class="scrollbar-chatbot">{{ item.message }}</span>
          </div>
        </div>
      </el-scrollbar>
    </el-main>
    <el-footer height="max-content"
      style="min-height: 60px; padding: 0 0; display: flex; align-items: center; justify-content: center; border: 1px solid var(--ep-border-color); border-radius: 3px; height: 120px margin-bottom: 0">
      <el-row
        style="display: flex; width: 100%; height: 100%; margin: 0 0; flex-direction: column; justify-content: center; align-items: center;">
        <el-col style="display: flex; justify-content: center; align-items: center;">
          <el-select v-model="documentType" placeholder="Select" style="width: 100px">
            <el-option v-for="item in options" :key="item.value" :label="item.label" :value="item.value" />
          </el-select>
        </el-col>
        <el-col v-if="documentType == 'Text'"
          style="display: flex; justify-content: center; align-items: center; height: 100%;">
          <el-autocomplete style="margin: 0 0;" v-model="search_state" :fetch-suggestions="querySearch" clearable
            type="input" class="inline-input search-bar w-50vw" size="large"
            placeholder="Type any words freely. Use Ctrl + Enter to submit." :disabled="submitLoading"
            @keyup.ctrl.enter="submit" />
        </el-col>
        <el-col v-else-if="documentType == 'Article'"
          style="display: flex; justify-content: center; align-items: center; height: 100%;">
          <el-input type="textarea" style="margin: 0 0; width: 50vw;" input-style="padding: 20px 20px;" resize="none"
            :rows="15" v-model="search_state" size="large" placeholder="Type articles freely. Use Ctrl + Enter to submit."
            @keyup.ctrl.enter="submit">
          </el-input>
        </el-col>
        <el-col v-else-if="documentType == 'File'"
          style="display: flex; justify-content: center; align-items: center; height: 100%;">
          <el-upload accept=".txt" action="/api/message/file" :http-request="handleUpload" ref="upload"
            :auto-upload="false" drag :limit="1" :on-exceed="handleExceed" style="width: 400px;">
            <el-icon><upload-filled /></el-icon>
            <div>
              Drop file here or click to upload
            </div>
          </el-upload>
        </el-col>
        <el-col style="display: flex; justify-content: center; align-items: center;">
          <el-button id="submit-button" :loading="submitLoading" style="margin: 0 0;" type="primary" plain
            @click="submit">Submit</el-button>
        </el-col>
      </el-row>
    </el-footer>
  </el-container>
</template>
