

window.onload = () => {
    $("#ajaxTest").click(() => {
        $.get("http://127.0.0.1/user.json", function (data) {
            let info = "姓名：" + data.user + " 年龄:" + data.age;
            $("#info").html(info);
        })
    })
}