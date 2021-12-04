// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"
#include <string>
#include "include/cef_browser.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "simple_handler.h"

SimpleApp::SimpleApp() {}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();
  CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));
  // 浏览器配置，
  CefBrowserSettings browser_settings;
  // 要打开的网址
  std::string url= "https://www.baidu.com";
  // 浏览器窗口信息
  CefWindowInfo window_info;
  window_info.SetAsPopup(NULL, "cefsimple");
    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                                  nullptr, nullptr);
}
