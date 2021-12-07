#pragma once
#include <qobject.h>
#include "include/cef_app.h"

class QyAppRenderer :public CefApp, public CefRenderProcessHandler {
public:
	QyAppRenderer();

	//重写CefApp 中的GetRenderProcessHandler方法
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{
		return this;
	}
	//实现 CefRenderProcessHandler 接口中的方法
	void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(QyAppRenderer);
};

