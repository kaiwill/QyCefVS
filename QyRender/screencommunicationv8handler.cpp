#pragma execution_character_set("UTF-8")
#include "screencommunicationv8handler.h"
#include "include/cef_parser.h"
#include "qycefglobal.h"
#include <QDebug>
ScreenCommunicationV8Handler::~ScreenCommunicationV8Handler()
{
}
/// ==========master屏幕=========
///app.sendToSlaver(data);
// app.cleanSlaver();
// ==========slaver屏幕==========
// app.onReceiveFromMaster(function(data){  ...   });
// app.onCleanData();
bool ScreenCommunicationV8Handler::Execute(const CefString& name, //JavaScript 函数名
	CefRefPtr<CefV8Value> object,     //JavaScript函数持有者
	const CefV8ValueList& arguments,//JavaScript 参数
	CefRefPtr<CefV8Value>& retval,  // JavaScript返回值
	CefString& exception) {

	qDebug() << QString("ScreenCommunicationV8Handler::Execute    name:%1  mCallBackMapSize: %2")
		.arg(QString::fromStdString(name))
		.arg(mCallBackMap.size());
	// sendData 的第一个参数为一个JSON 字符串
	if (name == "sendToSlaver" || name == "cleanSlaver") {
		handleMaster(name, arguments, retval, exception);
		return true;
	}
	if ((name == "onReceiveFromMaster" || name == "onCleanData") && arguments[0]->IsFunction()) {
		handleSlaver(name, arguments, retval, exception);
		return true;
	}
	return false;
}
// 给Browser 进程发送消息
void ScreenCommunicationV8Handler::handleMaster(const CefString& name,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception
) {
	// 发送消息给browser 进程
	//创建消息
	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(Share::PROCESS_MESSAGE_TO_SLAVER_SCREEN);
	// 参数
	CefRefPtr<CefListValue> args = msg->GetArgumentList();
	//枚举转换为字符串
	qDebug() << "=====发送消息给Browser进程==function name:" << QString::fromStdString(name);
	if (name == "sendToSlaver") {
		args->SetSize(2); //2个参数
		args->SetString(0, name); //被调用的函数名称
		args->SetString(1, arguments[0]->GetStringValue());//参数内容

	}
	if (name == "cleanSlaver") {
		args->SetSize(1); //1个参数
		args->SetString(0, name); //被调用的函数名称
	}
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	// 发送消息
	context->GetFrame()->SendProcessMessage(PID_RENDERER, msg);

}

// 注册 JavaScript函数，保存回调函数
void ScreenCommunicationV8Handler::handleSlaver(const CefString& name,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception) {
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	JavaScriptCallBack callback = {
		name,context,context->GetFrame(),context->GetBrowser(),arguments[0]
	};

	// 回调函数加入到map中
	mCallBackMap.insert(name, callback);
	qDebug() << QString("ScreenCommunicationV8Handler::handleSlaver    name:%1  mCallBackMapSize: %2")
		.arg(QString::fromStdString(name)).arg(mCallBackMap.size());
}

// 调用注册的回调函数
bool ScreenCommunicationV8Handler::handleSlaverCallback(CefRefPtr<CefProcessMessage> message) {
	// 解析message
	CefString name = message->GetName();

	if (name != Share::PROCESS_MESSAGE_TO_SLAVER_SCREEN) {
		return false;
	}
	CefRefPtr<CefListValue> args = message->GetArgumentList();
	CefString functionName = args->GetString(0);

	qDebug() << QString("ScreenCommunicationV8Handler::handleSlaverCallback : %1    functionName:%2   callbackSize:%3     ::::::%4")
		.arg(QString::fromStdString(name))
		.arg(QString::fromStdString(functionName))
		.arg(mCallBackMap.size())
		.arg(functionName == "sendToSlaver");
	if (functionName == "sendToSlaver") {
		CefString callbackParam = args->GetString(1);
		// 解析成JSON
		//CefRefPtr<CefValue> jsonObject = CefParseJSON(callbackParam, JSON_PARSER_ALLOW_TRAILING_COMMAS);
		//onReceiveFromMaster
		qDebug() << QString("调用callback之前 : ********%1").arg(mCallBackMap.contains("onReceiveFromMaster"));
		if (mCallBackMap.contains("onReceiveFromMaster")) {
			JavaScriptCallBack callback = mCallBackMap["onReceiveFromMaster"];

			CefV8ValueList arguments;

			qDebug() << QString("调用callback之前 :   FrameID :%1   url:%2   callbackParam:%3")
				.arg(callback.frame->GetIdentifier())
				.arg(QString::fromStdString(callback.frame->GetURL()))
				.arg(QString::fromStdString(callbackParam));

			//回调函数参数
			arguments.push_back(CefV8Value::CreateString(callbackParam));
			// 执行回调
			callback.callbackFun->ExecuteFunctionWithContext(callback.context, NULL, arguments);

			return true;
		}
		return false;
	}
	if (functionName == "cleanSlaver") {
		//onCleanData
		if (mCallBackMap.contains("onCleanData")) {
			JavaScriptCallBack callback = mCallBackMap["onCleanData"];
			// 进入Contxt
			//callback.context->Enter();
			CefV8ValueList arguments;
			// 执行回调
			callback.callbackFun->ExecuteFunctionWithContext(callback.context, NULL, arguments);
			//callback.context->Exit();
			return true;
		}
		return false;
	}
	return false;
}

