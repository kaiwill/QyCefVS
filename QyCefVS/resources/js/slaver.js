
window.onload=function(){
    
    // alert("sssssssssssssss");
    // alert(window.app);
    // alert(window.app.onReceiveFromMaster);
    if(window.app && window.app.onReceiveFromMaster){

        // 接收主屏发送数据 data是字符串
        window.app.onReceiveFromMaster((data)=>{
            var htmlData=$("#receivedData").html();
            htmlData+="<br />"+data;
            $("#receivedData").html(htmlData);
        }); 

        //清除数据
        window.app.onCleanData(()=>{
            $("#receivedData").html("");
        }); 
    }

}