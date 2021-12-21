


if (window.appEnv) {
    console.log(window.appEnv);
}

console.log(app);
window.onload = () => {
    /* $("#btnEncrypt").click(() => {
         var result = appEnv.encrypt($("#msg").val());
         $("#msgEncryptResult").val(result);
     });
 
     $("#btnDncrypt").click(() => {
         var result = appEnv.decrypt($("#msgEncryptResult").val());
         $("#dncryptInfo").html("解密结果:" + result);
     });
 */

    $("#btnEncrypt").click(() => {
        var result = app.encrypt($("#msg").val());
        $("#msgEncryptResult").val(result);
    });

    $("#btnDncrypt").click(() => {
        var result = app.decrypt($("#msgEncryptResult").val());
        $("#dncryptInfo").html("解密结果:" + result);
    });

    $("#btnTestSayHello").click(() => {
        app.sayHello((data) => {
            console.log(data);
        });
    });


}