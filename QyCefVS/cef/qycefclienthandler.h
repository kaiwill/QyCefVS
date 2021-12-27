// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include <QMap>
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"
#include "QObject"
#include "../qycefglobal.h"
class QyCefClientHandler :public QObject, public CefClient
	, public CefLifeSpanHandler
	, public CefKeyboardHandler
	, public CefLoadHandler
	, public CefRequestHandler {
	Q_OBJECT
public:
	explicit QyCefClientHandler();
	~QyCefClientHandler();

	// Provide access to the single global instance of this object.
	static QyCefClientHandler* getInstance();
	static void releaseInstance();

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, //浏览器对象
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		CefRefPtr<CefDictionaryValue>& extra_info,
		bool* no_javascript_access) OVERRIDE;

	HWND getMasterBrowserWindowHandle();


	//CefKeyboardHandler
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE
	{
		return this;
	}
	virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event,
		CefEventHandle os_event,
		bool* is_keyboard_shortcut);

	//CefLoadHandler
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) override;
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		TransitionType transition_type) override;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) override;

	// CefRequestHandler methods
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool user_gesture,
		bool is_redirect) override;


	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;



public slots:

signals:

private:
	typedef QMap<int, CefRefPtr<CefBrowser>> BrowserMap;
	//保存主屏，副屏浏览器
	BrowserMap browserMap;
	bool is_closing_ = false;
	CefRefPtr<CefBrowser> masterBrowser = NULL;
	CefRefPtr<CefBrowser> slaverBrowser = NULL;

	IMPLEMENT_REFCOUNTING(QyCefClientHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
