import { i18nAssets } from "./constants";

const serverAddress = "/api";

const getContestList = () => {
  let returnMessage = "";
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", serverAddress + "/contest/list", false);
  try {
    HTTPRequest.send();
    returnMessage = HTTPRequest.responseText;
    return JSON.parse(returnMessage);
  } catch (error) {
    return i18nAssets.value.error;
  }
}

const getContestInfo = (id: number) => {
  let returnMessage = "";
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", serverAddress + "/contest/info?id=" + id, false);
  try {
    HTTPRequest.send();
    returnMessage = HTTPRequest.responseText;
    return JSON.parse(returnMessage);
  } catch (error) {
    return i18nAssets.value.error;
  }
}

const getRecords = (id: number) => {
  let returnMessage = "";
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", serverAddress + "/record?id=" + id, false);
  try {
    HTTPRequest.send();
    returnMessage = HTTPRequest.responseText;
    return JSON.parse(returnMessage);
  } catch (error) {
    return i18nAssets.value.error;
  }
}

const getProblems = (id: number) => {
  let returnMessage = "";
  let HTTPRequest = new XMLHttpRequest();
  HTTPRequest.open("GET", serverAddress + "/contest/problems?id=" + id, false);
  try {
    HTTPRequest.send();
    returnMessage = HTTPRequest.responseText;
    return JSON.parse(returnMessage);
  } catch (error) {
    return i18nAssets.value.error;
  }
}

export { getContestList, getContestInfo, getRecords, getProblems };