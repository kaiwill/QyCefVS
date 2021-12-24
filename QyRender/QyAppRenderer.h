#pragma once
#include <qobject.h>
#include "include/cef_app.h"

// 引入消息路由组件
#include "include/wrapper/cef_message_router.h"

#include "app_event_listener_v8_handler.h"
class QyAppRenderer :public CefApp, public CefRenderProcessHandler {
public:
	QyAppRenderer();
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
		return this;
	}
	void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;

	void OnWebKitInitialized() OVERRIDE;

	void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;

	void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;

	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message);

private:
	// 消息路由对象
	CefRefPtr<CefMessageRouterRendererSide> m_message_router;
	// 浏览器
	CefRefPtr<CefBrowser> m_mainBrowser;
	AppEventListenerV8Handler* m_appEventListenerV8Handler;
	// Include the default reference counting implementation.


	IMPLEMENT_REFCOUNTING(QyAppRenderer);
};

