
window.onload = () => {
    console.log(window.app);
    if (window.app && window.app.addEventListener) {

        function handleFileChangeEvent(data) {
            // 将结果放入到div中
            var html = $("#msgContent").html() + "<br />" + data;
            $("#msgContent").html(html);
            console.log("=======>" + data);
        }

        //NEW, REMOVE, UPDATE_FILE, RENAME
        // 添加监听函数
        app.addEventListener("NEW", handleFileChangeEvent);
        app.addEventListener("REMOVE", handleFileChangeEvent);
        app.addEventListener("UPDATE_FILE", handleFileChangeEvent);
        app.addEventListener("RENAME", handleFileChangeEvent);
    }
}