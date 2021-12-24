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

QyAppRenderer::QyAppRenderer() {

}
void QyAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDictionaryValue> extra_info) {
	//保存浏览器对象
	m_mainBrowser = browser;
}

void QyAppRenderer::OnWebKitInitialized() {
	if (m_message_router == NULL) {
		// Create the renderer-side router for query handling.
		CefMessageRouterConfig config;
		//创建 渲染进程侧消息路由对象
		m_message_router = CefMessageRouterRendererSide::Create(config);
	}
}

void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	// 注册 window.cefQuery
	//注册监听器
	//// Retrieve the context's window object.
	CefRefPtr<CefV8Value> window = context->GetGlobal();

	//创建一个JavaScript 对象，全部为只读属性
	CefRefPtr<CefV8Value> appObject = CefV8Value::CreateObject(NULL, NULL);

	// JavaScript 函数就需要 CefV8Handler 来处理。这里使用了前面定义的 AppNativeV8Handler
	m_appEventListenerV8Handler = new AppEventListenerV8Handler();
	CefRefPtr<CefV8Value> funcAddEventListener = CefV8Value::CreateFunction("addEventListener", m_appEventListenerV8Handler);
	CefRefPtr<CefV8Value> funcRemoveEventListener = CefV8Value::CreateFunction("removeEventListener", m_appEventListenerV8Handler);
	appObject->SetValue("addEventListener", funcAddEventListener, V8_PROPERTY_ATTRIBUTE_NONE);
	appObject->SetValue("removeEventListener", funcRemoveEventListener, V8_PROPERTY_ATTRIBUTE_NONE);

	//绑定到window对象上,同样为只读属性
	window->SetValue("app", appObject, V8_PROPERTY_ATTRIBUTE_READONLY);

	m_message_router->OnContextCreated(browser, frame, context);
}

void QyAppRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {

	// 释放消息路由
	m_message_router->OnContextReleased(browser, frame, context);
}


bool QyAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	// 收到 browser进程发送的消息
	CEF_REQUIRE_RENDERER_THREAD();
	DCHECK_EQ(source_process, PID_BROWSER);

	// 取得消息的名字
	QString msgName = QString::fromStdString(message.get()->GetName());
	if (msgName == "FILE_CHAGE_EVENT_TRIGGER_NOTIFY") {
		//取出内容
		CefString eventType = message.get()->GetArgumentList().get()->GetString(0); //第0个参数
		CefString content = message.get()->GetArgumentList().get()->GetString(1); //第1个参数

		qDebug() << "收到browser 进程消息:" << QString::fromStdString(content);
		// 调用JS 回调
		m_appEventListenerV8Handler->executeNotify(QString::fromStdString(eventType), QString::fromStdString(content));
		return true;
	}

	return m_message_router->OnProcessMessageReceived(browser, frame, source_process, message);
}

