#pragma execution_character_set("UTF-8")
#include "screencommunicationv8handler.h"
#include "include/cef_parser.h"
#include "qycefglobal.h"
#include <QDebug>
ScreenCommunicationV8Handler::~ScreenCommunicationV8Handler()
{
}
/// ==========master��Ļ=========
///app.sendToSlaver(data);
// app.cleanSlaver();
// ==========slaver��Ļ==========
// app.onReceiveFromMaster(function(data){  ...   });
// app.onCleanData();
bool ScreenCommunicationV8Handler::Execute(const CefString& name, //JavaScript ������
	CefRefPtr<CefV8Value> object,     //JavaScript����������
	const CefV8ValueList& arguments,//JavaScript ����
	CefRefPtr<CefV8Value>& retval,  // JavaScript����ֵ
	CefString& exception) {

	qDebug() << QString("ScreenCommunicationV8Handler::Execute    name:%1  mCallBackMapSize: %2")
		.arg(QString::fromStdString(name))
		.arg(mCallBackMap.size());
	// sendData �ĵ�һ������Ϊһ��JSON �ַ���
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
// ��Browser ���̷�����Ϣ
void ScreenCommunicationV8Handler::handleMaster(const CefString& name,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception
) {
	// ������Ϣ��browser ����
	//������Ϣ
	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(Share::PROCESS_MESSAGE_TO_SLAVER_SCREEN);
	// ����
	CefRefPtr<CefListValue> args = msg->GetArgumentList();
	//ö��ת��Ϊ�ַ���
	qDebug() << "=====������Ϣ��Browser����==function name:" << QString::fromStdString(name);
	if (name == "sendToSlaver") {
		args->SetSize(2); //2������
		args->SetString(0, name); //�����õĺ�������
		args->SetString(1, arguments[0]->GetStringValue());//��������

	}
	if (name == "cleanSlaver") {
		args->SetSize(1); //1������
		args->SetString(0, name); //�����õĺ�������
	}
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	// ������Ϣ
	context->GetFrame()->SendProcessMessage(PID_RENDERER, msg);

}

// ע�� JavaScript����������ص�����
void ScreenCommunicationV8Handler::handleSlaver(const CefString& name,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception) {
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	JavaScriptCallBack callback = {
		name,context,context->GetFrame(),context->GetBrowser(),arguments[0]
	};

	// �ص��������뵽map��
	mCallBackMap.insert(name, callback);
	qDebug() << QString("ScreenCommunicationV8Handler::handleSlaver    name:%1  mCallBackMapSize: %2")
		.arg(QString::fromStdString(name)).arg(mCallBackMap.size());
}

// ����ע��Ļص�����
bool ScreenCommunicationV8Handler::handleSlaverCallback(CefRefPtr<CefProcessMessage> message) {
	// ����message
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
		// ������JSON
		//CefRefPtr<CefValue> jsonObject = CefParseJSON(callbackParam, JSON_PARSER_ALLOW_TRAILING_COMMAS);
		//onReceiveFromMaster
		qDebug() << QString("����callback֮ǰ : ********%1").arg(mCallBackMap.contains("onReceiveFromMaster"));
		if (mCallBackMap.contains("onReceiveFromMaster")) {
			JavaScriptCallBack callback = mCallBackMap["onReceiveFromMaster"];

			CefV8ValueList arguments;

			qDebug() << QString("����callback֮ǰ :   FrameID :%1   url:%2   callbackParam:%3")
				.arg(callback.frame->GetIdentifier())
				.arg(QString::fromStdString(callback.frame->GetURL()))
				.arg(QString::fromStdString(callbackParam));

			//�ص���������
			arguments.push_back(CefV8Value::CreateString(callbackParam));
			// ִ�лص�
			callback.callbackFun->ExecuteFunctionWithContext(callback.context, NULL, arguments);

			return true;
		}
		return false;
	}
	if (functionName == "cleanSlaver") {
		//onCleanData
		if (mCallBackMap.contains("onCleanData")) {
			JavaScriptCallBack callback = mCallBackMap["onCleanData"];
			// ����Contxt
			//callback.context->Enter();
			CefV8ValueList arguments;
			// ִ�лص�
			callback.callbackFun->ExecuteFunctionWithContext(callback.context, NULL, arguments);
			//callback.context->Exit();
			return true;
		}
		return false;
	}
	return false;
}

