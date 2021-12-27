// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "QObject"
// Implement application-level callbacks for the browser process.
class QyCefAppBrowser : public QObject, public CefApp, public CefBrowserProcessHandler {
	Q_OBJECT
public:
	QyCefAppBrowser();

	// CefApp methods:
	CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
		return this;
	}

	// CefBrowserProcessHandler methods:
	void OnContextInitialized() OVERRIDE;

	void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line);
signals:
	void onCefOnctextInitialized();

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(QyCefAppBrowser);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
