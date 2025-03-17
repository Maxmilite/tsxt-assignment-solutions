<script setup lang="ts">
;
import { ref, onMounted, reactive, computed } from "vue";
import { ElMessage } from 'element-plus';

interface WordItem {
  question: string,
  answer: string,
  date: Date,
  category: string,
  id: number
}

const dialogFormVisible = ref(false);
const form = reactive({
  id: 0,
  category: '',
})

const props = defineProps({
  historyFunction: Function,
  setCategoryFunction: Function,
});

const tableData = ref<WordItem[]>([]);

onMounted(() => {
  if (props.historyFunction !== undefined) {
    tableData.value = props.historyFunction();
  }
});

const submit = () => {
  if (form.category == "") {
    ElMessage.error("Category cannot be empty.");
    return;
  }
  if (props.setCategoryFunction!(form.id, form.category)) {
    tableData.value.at(tableData.value.length - form.id)!.category = form.category;
    dialogFormVisible.value = false;
  }
}

const openDialog = (id: number) => {
  form.id = tableData.value.at(id)!.id;
  form.category = tableData.value.at(id)!.category;
  dialogFormVisible.value = true;
}

const search = ref('')
const filterTableData = computed(() =>
  tableData.value.filter(
    (data) =>
      !search.value ||
      data.category.toLowerCase().includes(search.value.toLowerCase())
  )
)
</script>

<template>
  <div style="height: 99%; border: 1px solid var(--ep-border-color); border-radius: 3px;">
    <h2>Mini-ChatGPT Chat History</h2>
    <div id="main" style="flex-direction: column;">
      <el-input v-model="search" size="small" placeholder="Type to search category"
        style="width: 200px; align-self: end; margin: 10px 100px" />
      <el-table :data="filterTableData" stripe style="width: 90%; height: 90%;" :border="true">
        <el-table-column sortable align="center" prop="date" label="Date" width="240" />
        <el-table-column sortable align="center" prop="question" label="Question" />
        <el-table-column sortable align="center" prop="answer" label="Answer" />
        <el-table-column sortable align="center" prop="category" label="Category" width="120" />
        <el-table-column align="center" fixed="right" label="Options" width="80">
          <template #default="scope">
            <el-button link type="primary" size="small" @click="openDialog(scope.$index)">Edit</el-button>
          </template>
        </el-table-column>
      </el-table>
    </div>
    <el-dialog draggable destroy-on-close v-model="dialogFormVisible" title="Options" :center="true"
      width="calc(max(500px, 30%))">
      <el-form :model="form" style="display: flex; flex-direction: column; align-items: center;">
        <el-form-item label="Category">
          <el-input style="width: 300px; margin-left: 0; margin-right: 0" v-model="form.category" autocomplete="off" />
        </el-form-item>
      </el-form>
      <template #footer>
        <span class="dialog-footer">
          <el-button @click="() => { dialogFormVisible = false; }">Cancel</el-button>
          <el-button type="primary" @click="submit()">
            Confirm
          </el-button>
        </span>
      </template>
    </el-dialog>
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
