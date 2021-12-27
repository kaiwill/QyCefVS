#pragma once
#include <qobject.h>
#include "include/cef_app.h"
#include "screencommunicationv8handler.h"
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
	ScreenCommunicationV8Handler* mScreenCommunicationV8Handler = NULL;
	// Include the default reference counting implementation.

	IMPLEMENT_REFCOUNTING(QyAppRenderer);
};

