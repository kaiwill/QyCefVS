
window.onload = () => {
    console.log(window.secondScreen);

    if(window.app && window.app.sendToSlaver){
        $("#btnSendSecondScreen").click(()=>{
            // 发送数据 sendData 发送的是字符串
            app.sendToSlaver(JSON.stringify({
                time:new Date(),
                content:$("#txtMessage").val()
            }));    
        });

        $("#btnCleanSecondScreen").click(()=>{
            // 清屏
            window.app.cleanSlaver();
        });
    }
    
}