#pragma once
#include <qobject.h>
#include "include/cef_app.h"

class QyAppRenderer :public CefApp, public CefRenderProcessHandler {
public:
	QyAppRenderer();

	//重写CefApp 中的GetRenderProcessHandler方法
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
		return this;
	}
	//实现 CefRenderProcessHandler 接口中的方法
	void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;

	void OnWebKitInitialized() OVERRIDE;
	//
	void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;



	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message);

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(QyAppRenderer);
};

