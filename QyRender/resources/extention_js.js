var app;
if(!app){
	app={};
    (function(){
        app.encrypt=function(originText){
            native function encrypt();
            return encrypt(originText);
        };
        app.decrypt=function(encryptText){
            native function decrypt();
            return decrypt(encryptText);
        };
        app.sayHello=function(callback) {
            native function sayHello();
            return sayHello(callback);
        };
    })();
}