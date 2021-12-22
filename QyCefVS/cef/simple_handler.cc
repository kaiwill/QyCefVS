// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#pragma execution_character_set("UTF-8")
#include "simple_handler.h"

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

namespace {
	SimpleHandler* g_instance = nullptr;
}  // namespace

SimpleHandler::SimpleHandler(bool use_views, CefQueryHandler* cef_query_handler)
	: use_views_(use_views), is_closing_(false) {
	DCHECK(!g_instance);
	g_instance = this;
	m_message_handler.reset(cef_query_handler);
}

SimpleHandler::~SimpleHandler() {
	g_instance = nullptr;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
	return g_instance;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	qDebug() << "=========OnAfterCreated=======";
	// 当浏览器创建以后，保存到浏览器列表中
	browser_list_.push_back(browser);

	if (!m_message_router) {
		// browser进程侧 消息路由配置, renderer进程中JavaScript发出消息和取消消息使用 
		// window.cefQuery 和 window.cefQueryCancel , 如果不想使用这两个名字，可以在这里配置
		CefMessageRouterConfig config;
		//    config.js_query_function = "cefQuery";
		//    config.js_cancel_function = "cefQueryCancel";

		m_message_router = CefMessageRouterBrowserSide::Create(config);

		// 为消息路由设置 消息处理器，即从 render进程发送过来的消息交给谁处理。
		//m_message_handler.reset(new CefQueryHandler);
		m_message_router->AddHandler(m_message_handler.get(), false);
	}
}

/// <summary>
/// CefRequestHandler 接口中的方法
/// 当浏览器发出请求资源之前调用。 
/// 返回false 浏览器继续发出请求 返回true，浏览器停止导航。
/// </summary>
/// <param name="browser"></param>
/// <param name="frame"></param>
/// <param name="request"></param>
/// <param name="user_gesture"></param>
/// <param name="is_redirect"></param>
/// <returns></returns>
bool SimpleHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool user_gesture,
	bool is_redirect) {
	m_message_router->OnBeforeBrowse(browser, frame);
	return false;
}




bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (browser_list_.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		is_closing_ = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();

	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}

	if (browser_list_.empty()) {
		// All browser windows have closed. Quit the application message loop.
		CefQuitMessageLoop();
		// 当最后一个浏览器已经关闭，释放消息路由.
		m_message_router->RemoveHandler(m_message_handler.get());
		m_message_handler.reset();
		m_message_router = nullptr;
	}
}

bool SimpleHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut)
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

bool SimpleHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {

	CEF_REQUIRE_UI_THREAD();

	return m_message_router->OnProcessMessageReceived(browser, frame,
		source_process, message);
}
