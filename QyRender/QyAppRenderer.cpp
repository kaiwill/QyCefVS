#pragma execution_character_set("UTF-8")
#include "QyAppRenderer.h"
#include <QDebug>
#include <QProcess>
#include "encrypt/qaesencryption.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
#include <include/wrapper/cef_helpers.h>

namespace {

}

QyAppRenderer::QyAppRenderer()
	:mScreenCommunicationV8Handler(new ScreenCommunicationV8Handler) {

}
void QyAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDictionaryValue> extra_info) {
	qDebug() << QString("=====OnBrowserCreated===FrameID:%1====browserID:%2")
		.arg(browser->GetMainFrame()->GetIdentifier())
		.arg(browser->GetIdentifier());
}

void QyAppRenderer::OnWebKitInitialized() {

}

// ==========master屏幕=========
// app.sendToSlaver(data);
// app.cleanSlaver();
// ==========slaver屏幕==========
// app.onReceiveFromMaster(function(data){  ...   });
// app.onCleanData();
void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	//// Retrieve the context's window object.
	CefRefPtr<CefV8Value> window = context->GetGlobal();
	//创建一个JavaScript 对象，全部为只读属性
	CefRefPtr<CefV8Value> appObject = CefV8Value::CreateObject(NULL, NULL);
	QString frameUrl = QString::fromStdString(frame.get()->GetURL());
	qDebug() << QString("=====OnContextCreated=====frameID:%1       URL:%2").arg(frame->GetIdentifier()).arg(frameUrl);
	if (frameUrl.endsWith("index.html")) {// 主屏窗口
		//qDebug() << QString("---创建：sendToSlaver/cleanSlaver  frameID:%1").arg(frame->GetIdentifier());
		CefRefPtr<CefV8Value> funcSendToSlaver = CefV8Value::CreateFunction("sendToSlaver", mScreenCommunicationV8Handler);
		CefRefPtr<CefV8Value> funcCleanSlaver = CefV8Value::CreateFunction("cleanSlaver", mScreenCommunicationV8Handler);
		appObject->SetValue("sendToSlaver", funcSendToSlaver, V8_PROPERTY_ATTRIBUTE_NONE);
		appObject->SetValue("cleanSlaver", funcCleanSlaver, V8_PROPERTY_ATTRIBUTE_NONE);
	}
	if (frameUrl.endsWith("slaver.html")) { // 副屏窗口
		//qDebug() << QString("---创建：onReceiveFromMaster/onCleanData  frameID:%1").arg(frame->GetIdentifier());
		CefRefPtr<CefV8Value> onReceiveDataFun = CefV8Value::CreateFunction("onReceiveFromMaster", mScreenCommunicationV8Handler);
		CefRefPtr<CefV8Value> funcCleanData = CefV8Value::CreateFunction("onCleanData", mScreenCommunicationV8Handler);
		appObject->SetValue("onReceiveFromMaster", onReceiveDataFun, V8_PROPERTY_ATTRIBUTE_NONE);
		appObject->SetValue("onCleanData", funcCleanData, V8_PROPERTY_ATTRIBUTE_NONE);
	}
	//绑定到window对象上,同样为只读属性
	window->SetValue("app", appObject, V8_PROPERTY_ATTRIBUTE_READONLY);
}




void QyAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	if (mScreenCommunicationV8Handler) {
		delete mScreenCommunicationV8Handler;
	}
}


bool QyAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	// 收到 browser进程发送的消息
	CEF_REQUIRE_RENDERER_THREAD();
	DCHECK_EQ(source_process, PID_BROWSER);

	qDebug() << QString("收到browser 进程消息 : %1").arg(frame->GetIdentifier());
	if (mScreenCommunicationV8Handler) {
		return mScreenCommunicationV8Handler->handleSlaverCallback(message);
	}
	return false;
}

