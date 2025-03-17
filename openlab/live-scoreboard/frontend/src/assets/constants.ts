import { ref } from "vue";

const timestampToDate = (timestamp: number) => {
  let now = new Date(timestamp * 1000),
  y = now.getFullYear(),
  m = now.getMonth() + 1,
  d = now.getDate();
  return y + "-" + (m < 10 ? "0" + m : m) + "-" + (d < 10 ? "0" + d : d) + " " + now.toTimeString().substr(0, 8);
}

const zh_CN = {
  __language__: "zh_CN",
  signinRequired: "查询信息需要登录。",
  title: "比赛榜单",
  error: "查询时发生错误。",
  scoreboard: {
    rank: "排名",
    name: "姓名",
    totalScore: "总得分",
    problemName: "题目名称：",
    problemTotalScore: "题目总分：",
    score: "得分：",
    time: "时间：",
  },
  noData: "没有数据",
  view: "查看",
  pending: "等待开始",
  finished: "已完成",
  time_elapse: "已用时：",
  contestList: {
    contest_name: "比赛名称",
    duration: "持续时间",
    progress: "进度",
    operations: "操作"
  }
};

const en_US = {
  __language__: "en_US",
  signinRequired: "You need to sign in before querying.",
  title: "Live Scoreboard",
  error: "An error occurred while querying.",
  scoreboard: {
    rank: "Rank",
    name: "Name",
    totalScore: "Score",
    problemName: "Name: ",
    problemTotalScore: "Total Score: ",
    score: "Score: ",
    time: "Time: ",
  },
  noData: "No Data",
  view: "View",
  pending: "Pending",
  finished: "Finished",
  time_elapse: "Time Elapsed: ",
  contestList: {
    contest_name: "Contest Name",
    duration: "Duration",
    progress: "Progress",
    operations: "Operations"
  }
};

const i18nAssets =  ref(en_US);

const switchLanguage = () => {
  if (i18nAssets.value.__language__ == "zh_CN") i18nAssets.value = en_US;
  else i18nAssets.value = zh_CN;
}

export {i18nAssets, switchLanguage, timestampToDate};