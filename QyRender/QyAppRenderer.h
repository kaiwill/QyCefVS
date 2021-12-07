#pragma once
#include <qobject.h>
#include "include/cef_app.h"

class QyAppRenderer :public CefApp, public CefRenderProcessHandler {
public:
	QyAppRenderer();

	//��дCefApp �е�GetRenderProcessHandler����
	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{
		return this;
	}
	//ʵ�� CefRenderProcessHandler �ӿ��еķ���
	void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDictionaryValue> extra_info) OVERRIDE;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(QyAppRenderer);
};

