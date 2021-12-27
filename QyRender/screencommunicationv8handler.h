#pragma once
#include "include/cef_v8.h"
#include <qdebug.h>
#include <QMap>

typedef struct _JavaScriptCallBack {
	// callback ��Ӧ��JavaScript������
	CefString name;
	// JavaScript������
	CefRefPtr<CefV8Context> context;
	// callback��Ӧ��frame
	CefRefPtr<CefFrame> frame;
	// callback ��Ӧ�� browser
	CefRefPtr<CefBrowser> browser;
	// callback ����
	CefRefPtr<CefV8Value> callbackFun;
}JavaScriptCallBack;

class ScreenCommunicationV8Handler :public CefV8Handler
{
public:
	ScreenCommunicationV8Handler() {

	}
	~ScreenCommunicationV8Handler();

	bool Execute(const CefString& name, //JavaScript ������
		CefRefPtr<CefV8Value> object,     //JavaScript����������
		const CefV8ValueList& arguments,//JavaScript ����
		CefRefPtr<CefV8Value>& retval,  // JavaScript����ֵ
		CefString& exception) override;

	// ���� salver�Ļص�
	bool handleSlaverCallback(CefRefPtr<CefProcessMessage> message);
private:
	// ����ص�����
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
