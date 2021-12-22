#pragma execution_character_set("UTF-8")
#include "QyAppRenderer.h"
#include <QDebug>
#include <QProcess>
#include "encrypt/qaesencryption.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
namespace {

}

QyAppRenderer::QyAppRenderer() {

}
void QyAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDictionaryValue> extra_info) {

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
	return m_message_router->OnProcessMessageReceived(browser, frame, source_process, message);
}

