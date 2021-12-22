
window.onload = () => {
    console.log(window.cefQuery);
    $("#btnReadTextFile").click(() => {
        window.cefQuery({
            request: 'MSG_READ_TEXT_FILE',
            onSuccess: function (response) {
                $("#textFileContent").text(response);
            },
            onFailure: function (error_code, error_message) {
                $("#textFileContent").text("错误:" + error_code + "  " + error_message);
            }
        });
    });
}