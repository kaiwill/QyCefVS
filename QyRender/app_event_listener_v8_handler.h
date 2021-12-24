#pragma once
#include "include/cef_v8.h"
#include <qdebug.h>

/// <summary>
/// 将回调函数和上下文包装在一起
/// </summary>
typedef struct _EventListener
{
	CefRefPtr<CefV8Value> callback_;
	CefRefPtr<CefV8Context> context_;
} EventListener;

typedef QMap < QString, QList<EventListener>> CallbackMap;

class AppEventListenerV8Handler :public CefV8Handler
{

public:
	bool Execute(const CefString& name, //JavaScript 函数名
		CefRefPtr<CefV8Value> object,     //JavaScript函数持有者
		const CefV8ValueList& arguments,//JavaScript 参数
		CefRefPtr<CefV8Value>& retval,  // JavaScript返回值
		CefString& exception) override;

	// 执行通知
	void executeNotify(QString eventType, QString content);
private:
	// 保存回调函数映射
	CallbackMap m_callbackMap;

	IMPLEMENT_REFCOUNTING(AppEventListenerV8Handler);
};

