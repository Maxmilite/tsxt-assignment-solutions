<script lang="ts" setup>
import { nextTick, onMounted, ref } from 'vue'
import {
  Location,
  Document,
  Menu as IconMenu,
  Setting,
} from '@element-plus/icons-vue'
import { ElButton, ElInput } from 'element-plus';

const props = defineProps({
  state: Number,
  categoriesFunction: Function,
  setQueryCategoryFunction: Function
})

const emit = defineEmits(['response']);
function exit(e: number) {
  emit('response', e);
}

const isCollapse = ref(true)

// const inputVisible = ref(false);
// const InputRef = ref<InstanceType<typeof ElInput>>();
// const inputValue = ref('')

// const showInput = () => {
//   inputVisible.value = true
//   nextTick(() => {
//     InputRef.value!.input!.focus()
//   })
// }

// const handleInputConfirm = () => {
//   if (inputValue.value) {
//     groupList.value.push({
//       id: groupList.value.length + 1,
//       text: inputValue.value,
//     });
//     // console.log(groupList)
//   }
//   inputVisible.value = false;
//   inputValue.value = "";
// }

// const SessionItem = { id: 0, text: "Session" }
// const GroupItem = { id: 0, text: "Default", child: [SessionItem] }


// const handleGroupClose = (key: typeof GroupItem) => {
//   groupList.value.splice(groupList.value.indexOf(key), 1);
// }

// const handleItemClose = (father: typeof GroupItem, key: typeof SessionItem) => {
//   father.child.splice(father.child.indexOf(key), 1);
// }

const groupList = ref<String[]>([]);

onMounted(() => {
  setTimeout(() => { groupList.value = props.categoriesFunction!(); }, 100)
});

</script>

<template>
  <el-menu default-active="2" class="el-menu-vertical-demo" :collapse="isCollapse">
  <el-menu-item index="0" @click="isCollapse = !isCollapse">
      <el-icon v-if="isCollapse">
        <ArrowRightBold />
      </el-icon>
      <el-icon v-else>
        <ArrowLeftBold />
      </el-icon>
      <template #title>History</template>
    </el-menu-item>
    <el-menu-item @click="setQueryCategoryFunction!(); exit(4);">
      <el-icon>
        <MessageBox />
      </el-icon>
      <span>All Categories</span>
    </el-menu-item>
    <el-menu-item v-for="group in groupList" :index="(groupList.indexOf(group) + 1).toString()"
      @click="setQueryCategoryFunction!(group); exit(4);">
      <el-icon>
        <MessageBox />
      </el-icon>
      <template #title>
        {{ group }}
      </template>
    </el-menu-item>


    <!-- <el-menu-item v-if="state === 1" >
        <el-icon>
          <Document />
        </el-icon>
        <span>Current</span>
      </el-menu-item>
      <el-sub-menu 
        v-for="group in groupList" :allow-drop="true" :allow-drag="true"
        :index="(groupList.indexOf(group) + 1).toString()" draggable="true">
        <template #title>
          <el-icon>
            <MessageBox />
          </el-icon><span>{{ group.text }}</span>
          <el-icon v-if="!isCollapse" style="margin-left: 10px" @click="handleGroupClose(group)">
            <Close />
          </el-icon>
        </template>
        <el-menu-item-group>
          <template #title v-if="isCollapse"><span>{{ group.text }}</span></template>
          <el-menu-item draggable="true" v-for="item in group.child" :index="(groupList.indexOf(group) + 1) + '-' + (group.child.indexOf(item) + 1)">
            <el-icon>
              <Document />
            </el-icon>{{ item.text }}
            <el-icon style="margin-left: 10px" @click="handleItemClose(group, item)">
              <Close />
            </el-icon>
          </el-menu-item>
        </el-menu-item-group>
      </el-sub-menu> -->

    <!-- <div v-if="!isCollapse">
        <el-button v-if="!inputVisible" style="margin-top: 10px;" type="default" size="default" class="w-30"
          @click="showInput">
          + New Group
        </el-button>
        <el-input v-else style="margin-top: 10px;" type="default" size="default" class="w-30" ref="InputRef"
          v-model="inputValue" @keyup.enter="handleInputConfirm" @blur="handleInputConfirm">
        </el-input>
      </div> -->

  </el-menu>
</template>
