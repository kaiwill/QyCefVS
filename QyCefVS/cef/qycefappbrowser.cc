// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "qycefappbrowser.h"
#include <string>
#include "include/cef_browser.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
QyCefAppBrowser::QyCefAppBrowser()
{
}

void QyCefAppBrowser::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();
	// 发出信号
	emit onCefOnctextInitialized();
}
void QyCefAppBrowser::OnBeforeCommandLineProcessing(
	const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line) {
	command_line->AppendSwitch("--disable-web-security");//关闭同源策略
}



