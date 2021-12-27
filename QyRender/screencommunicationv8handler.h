#pragma once
#include "include/cef_v8.h"
#include <qdebug.h>
#include <QMap>

typedef struct _JavaScriptCallBack {
	// callback 对应的JavaScript函数名
	CefString name;
	// JavaScript上下文
	CefRefPtr<CefV8Context> context;
	// callback对应的frame
	CefRefPtr<CefFrame> frame;
	// callback 对应的 browser
	CefRefPtr<CefBrowser> browser;
	// callback 函数
	CefRefPtr<CefV8Value> callbackFun;
}JavaScriptCallBack;

class ScreenCommunicationV8Handler :public CefV8Handler
{
public:
	ScreenCommunicationV8Handler() {

	}
	~ScreenCommunicationV8Handler();

	bool Execute(const CefString& name, //JavaScript 函数名
		CefRefPtr<CefV8Value> object,     //JavaScript函数持有者
		const CefV8ValueList& arguments,//JavaScript 参数
		CefRefPtr<CefV8Value>& retval,  // JavaScript返回值
		CefString& exception) override;

	// 处理 salver的回调
	bool handleSlaverCallback(CefRefPtr<CefProcessMessage> message);
private:
	// 保存回调函数
	QMap<std::string, JavaScriptCallBack> mCallBackMap;

	void handleMaster(const CefString& name,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception
	);
	void handleSlaver(const CefString& name,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception);

	IMPLEMENT_REFCOUNTING(ScreenCommunicationV8Handler);
};
