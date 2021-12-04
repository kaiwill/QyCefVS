// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include <list>
#include "include/wrapper/cef_helpers.h"
class SimpleHandler : public CefClient,
                      public CefLifeSpanHandler {
 public:
  explicit SimpleHandler(bool use_views);
  ~SimpleHandler();

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();

  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
      return this;
  }

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  // 重写它的目的是为了在点击链接的时候在同一个窗口打开连接。
  // 默认返回的是 false，这里在主frame中加载地址，然后返回true
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, //浏览器对象
      CefRefPtr<CefFrame> frame,
      const CefString& target_url, //要打开的地址
      const CefString& target_frame_name,
      WindowOpenDisposition target_disposition,
      bool user_gesture,
      const CefPopupFeatures& popupFeatures,
      CefWindowInfo& windowInfo,
      CefRefPtr<CefClient>& client,
      CefBrowserSettings& settings,
      CefRefPtr<CefDictionaryValue>& extra_info,
      bool* no_javascript_access) {
      
      CEF_REQUIRE_UI_THREAD();

      if (!target_url.empty())
      {
          //获取浏览器对象中的 主frame对象，然后加载url
          browser->GetMainFrame()->LoadURL(target_url);
          return true;
      }
      return false;
  }


 private:
  const bool use_views_;

  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;

  bool is_closing_;

  IMPLEMENT_REFCOUNTING(SimpleHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
