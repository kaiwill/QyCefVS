#pragma once
#include "include/cef_v8.h"
#include <qdebug.h>

/// <summary>
/// ���ص������������İ�װ��һ��
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
	bool Execute(const CefString& name, //JavaScript ������
		CefRefPtr<CefV8Value> object,     //JavaScript����������
		const CefV8ValueList& arguments,//JavaScript ����
		CefRefPtr<CefV8Value>& retval,  // JavaScript����ֵ
		CefString& exception) override;

	// ִ��֪ͨ
	void executeNotify(QString eventType, QString content);
private:
	// ����ص�����ӳ��
	CallbackMap m_callbackMap;

	IMPLEMENT_REFCOUNTING(AppEventListenerV8Handler);
};

