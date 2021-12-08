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

namespace {
	SimpleHandler* g_instance = nullptr;
}  // namespace

SimpleHandler::SimpleHandler(bool use_views)
	: use_views_(use_views), is_closing_(false) {
	DCHECK(!g_instance);
	g_instance = this;
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

	// Add to the list of existing browsers.
	browser_list_.push_back(browser);
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

	qDebug() << "收到:" << source_process << "进程消息: " << QString::fromStdString(message.get()->GetName());

	CefString title = message.get()->GetArgumentList().get()->GetString(0); //第一个参数 标题
	int width = message.get()->GetArgumentList().get()->GetInt(1); // 第二个参数宽度
	int height = message.get()->GetArgumentList().get()->GetInt(2);// 第三个参数高度
	//发送信号到 主窗口，让主窗口创建一个子窗口
	emit onReceiveRendererProccessMessasge(QString::fromStdString(title), width, height);
	return true;
}
