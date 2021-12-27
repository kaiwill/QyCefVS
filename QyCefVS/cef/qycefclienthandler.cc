// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#pragma execution_character_set("UTF-8")
#include "qycefclienthandler.h"

#include <sstream>
#include <string>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMetaEnum>

namespace {
	// 单例实例对象
	QyCefClientHandler* g_instance = nullptr;
}  // namespace

QyCefClientHandler::QyCefClientHandler() {
	DCHECK(!g_instance);
	g_instance = this;
}

QyCefClientHandler::~QyCefClientHandler() {
	g_instance = nullptr;
}

// static
QyCefClientHandler* QyCefClientHandler::getInstance() {
	if (g_instance) {
		return g_instance;
	}
	return new QyCefClientHandler;
}
// static
void QyCefClientHandler::releaseInstance() {
	if (g_instance) {
		delete g_instance;
	}
}
/// <summary>
/// 每弹出一个浏览器窗体，就会有一个新的browser
/// </summary>
/// <param name="browser"></param>
void QyCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	//const std::string type = getBrowserType(browser);
	//保存浏览器对象
	if (browserMap.size() == 0) {
		masterBrowser = browser;
	}
	else {
		slaverBrowser = browser;
	}
	browserMap[browser->GetIdentifier()] = browser;

	//qDebug() << QString("OnAfterCreated=======>frameID:%1 ,  total:  %2    browserID: %3")
	//	.arg(browser->GetMainFrame()->GetIdentifier())
	//	.arg(browserMap.size())
	//	.arg(browser->GetIdentifier());
}

bool QyCefClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	// 允许关闭
	return false;
}

void QyCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	// 从map中移除
	browserMap.remove(browser->GetIdentifier());
}

bool QyCefClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
{

	if (event.type == KEYEVENT_RAWKEYDOWN) {
		switch (event.windows_key_code) {
		case VK_F5: //刷新页面
			browser->Reload();
			return true;
		case VK_F12: // 打开浏览器开发者工具
			CefWindowInfo windowInfo;
			CefBrowserSettings settings;
			windowInfo.SetAsPopup(NULL, "Dev Tools");
			browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
			return true;
		}
	}
	return false;
}
// 收到渲染进程消息
bool QyCefClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {

	// 使用slaver browser对象转发消息
	if (slaverBrowser.get()) {
		slaverBrowser->GetMainFrame()->SendProcessMessage(PID_RENDERER, message);
		return true;
	}

	return false;
}

/// <summary>
/// 获取窗口句柄
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
HWND QyCefClientHandler::getMasterBrowserWindowHandle() {
	if (masterBrowser) {
		return masterBrowser->GetHost()->GetWindowHandle();
	}
	return NULL;
}

/// <summary>
/// 控制是否弹出窗体  
/// false 则弹出新的窗体，
///		会创建一个新的browser 对象，
///		会回调 OnAfterCreated 方法
/// true 则不弹出新的窗体，此时需要在这个方法中通过 browser 中的 frame加载新的地址
bool QyCefClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, //浏览器对象
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
	bool* no_javascript_access) {

	CEF_REQUIRE_UI_THREAD();
	qDebug() << QString("OnBeforePopup=======target_url:%1 ,  target_frame_name:%2     user_gesture:%3")
		.arg(QString::fromStdString(target_url))
		.arg(QString::fromStdString(target_frame_name))
		.arg(user_gesture);
	//if (!target_url.empty())
	//{
	//	//获取浏览器对象中的 主frame对象，然后加载url
	//	browser->GetMainFrame()->LoadURL(target_url);
	//	return true;
	//}
	return false;
}

//CefLoadHandler
void QyCefClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward) {

}
void QyCefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	TransitionType transition_type) {
	qDebug() << QString("OnLoadStart======");
}
// 加载结束的时候
void QyCefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {

	qDebug() << QString("OnLoadEnd ===> url:%1    httpStatusCode:%2       browserID:%3")
		.arg(QString::fromStdString(frame->GetURL()))
		.arg(httpStatusCode)
		.arg(browser->GetIdentifier());
}
void  QyCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl) {

}
// CefRequestHandler methods
bool   QyCefClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool user_gesture,
	bool is_redirect) {
	qDebug() << QString("OnBeforeBrowse ===> url:%1")
		.arg(QString::fromStdString(request->GetURL()));
	return false;
}