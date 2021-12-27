#pragma once

#include <QObject>
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/base/cef_thread_checker.h"
#include <QDebug>
class SecondCefClientHandler : public QObject, public CefClient, public CefLifeSpanHandler
{
	Q_OBJECT

public:
	SecondCefClientHandler(QObject* parent);
	~SecondCefClientHandler();
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}
	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	void sendMessageToSecond(CefRefPtr<CefProcessMessage> message);
private:
	// List of existing browsers.
	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList browser_list_;
	base::ThreadChecker thread_checker_;
	bool is_closing_;

	IMPLEMENT_REFCOUNTING(SecondCefClientHandler);
};
