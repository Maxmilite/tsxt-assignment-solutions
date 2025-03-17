var queryList = new Array();
var answerList = new Array();

var unknownMessage = "对不起，我不知道你的问题。";

function initQueryList() {
    let add_options;
    for (let i = 0; i < queryList.length; ++i) {
        add_options += '<option value="' + queryList[i] + '">' + '</option>';
    }
    $("#text-list").append(add_options);
}

function initData() {
    let dataToAdd = [
        "为什么卢皇是神",
        "为什么恺皇是神",
        "为什么张皇是神",
        "为什么曹皇是神",
        "为什么任皇是神"
    ];
    let answerToAdd = [
        "就我个人来说，为什么卢皇是神对我的意义，不能不说非常重大。 苏轼曾经提到过，古之立大事者，不惟有超世之才，亦必有坚忍不拔之志。这启发了我， 既然如此， 问题的关键究竟为何？ 别林斯基在不经意间这样说过，好的书籍是最贵重的珍宝。带着这句话，我们还要更加慎重的审视这个问题： 莎士比亚在不经意间这样说过，本来无望的事，大胆尝试，往往能成功。这句话语虽然很短，但令我浮想联翩。 可是，即使是这样，为什么卢皇是神的出现仍然代表了一定的意义。 我们都知道，只要有意义，那么就必须慎重考虑。 为什么卢皇是神的发生，到底需要如何做到，不为什么卢皇是神的发生，又会如何产生。 所谓为什么卢皇是神，关键是为什么卢皇是神需要如何写。 我们一般认为，抓住了问题的关键，其他一切则会迎刃而解。 经过上述讨论， 我们一般认为，抓住了问题的关键，其他一切则会迎刃而解。",
    ];
    for (let i = 0; i < dataToAdd.length; ++i) {
        queryList[i] = dataToAdd[i];
    };
    for (let i = 0; i < answerToAdd.length; ++i) {
        answerList[i] = answerToAdd[i];
    }
}

function search() {
    let text = $("#input-bar")[0].value.replace(" ", "");
    if (text === undefined || text === null)
        return;
    for (var i = 0; i < queryList.length; ++i) {
        if (queryList[i] === text) {
            $("#content").html(
                answerList[i] === undefined ? "" : answerList[i]
            );
            Swal.fire({
                type: 'success',
                title: '查询成功',
                showConfirmButton: false,
                timer: 1000
            });
            return;
        }
    }
    Swal.fire({
        type: 'error',
        title: 'Oops...',
        text: '我不知道你的问题，请从下拉列表中选择一个问题。'
    });
    $("#content").html(unknownMessage);
}

$(document).ready(function () {
    $("#nav-icon")
        .css("width", $("#nav-icon")
            .css("height"));
    initData();
    initQueryList();
})