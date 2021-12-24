#include "app_event_listener_v8_handler.h"
#include <QDebug>
bool AppEventListenerV8Handler::Execute(
	const CefString& name, //JavaScript 函数名
	CefRefPtr<CefV8Value> object,     //JavaScript函数持有者
	const CefV8ValueList& arguments,//JavaScript 参数
	CefRefPtr<CefV8Value>& retval,  // JavaScript返回值
	CefString& exception) {

	QString funcName = QString::fromStdString(name);

	if (name == "addEventListener" && arguments.size() == 2 && arguments[1]->IsFunction()) {
		// 事件名称
		CefRefPtr<CefV8Value> eventNameValue = arguments[0];
		QString eventName = QString::fromStdString(eventNameValue.get()->GetStringValue());
		// 获取回调函数
		CefRefPtr<CefV8Value> callback = arguments[1];

		EventListener eventListener = { callback ,CefV8Context::GetCurrentContext() };
		// 保存回调函数
		m_callbackMap[eventName].append(eventListener);
		qDebug() << QString("AppEventListenerV8Handler::Execute====>addEventListener===>%0").arg(m_callbackMap.size());
		return true;
	}
	if (name == "removeEventListener" && arguments.size() == 1) {
		CefRefPtr<CefV8Value> eventNameValue = arguments[0];
		CefString eventName = eventNameValue.get()->GetStringValue();
		m_callbackMap.remove(QString::fromStdString(eventName));
		return true;
	}

	return false;
}


void AppEventListenerV8Handler::executeNotify(QString eventType, QString content) {
	// 文件夹改变
	if (m_callbackMap.contains(eventType)) {
		QList<EventListener> callbackList = m_callbackMap[eventType];
		qDebug() << QString("AppEventListenerV8Handler::executeNotify====>%1===>%2").arg(eventType).arg(callbackList.size());
		for (auto it = callbackList.begin(); it != callbackList.end(); ++it) {
			// 回调函数参数值
			CefRefPtr<CefV8Value> callbackFunctionParam = CefV8Value::CreateString(content.toStdString());
			CefV8ValueList arguments;
			arguments.push_back(callbackFunctionParam);
			// 执行JavaScript回调，并将参数传递给它，参数是一个CefV8ValueList
			CefRefPtr<CefV8Context> context = (*it).context_;
			CefRefPtr<CefV8Value> callback = (*it).callback_;
			context.get()->Enter();//进入上下文
			callback->ExecuteFunction(NULL, arguments);
			context.get()->Exit();//退出上下文
		}
	}
}
