#include "app_event_listener_v8_handler.h"
#include <QDebug>
bool AppEventListenerV8Handler::Execute(
	const CefString& name, //JavaScript ������
	CefRefPtr<CefV8Value> object,     //JavaScript����������
	const CefV8ValueList& arguments,//JavaScript ����
	CefRefPtr<CefV8Value>& retval,  // JavaScript����ֵ
	CefString& exception) {

	QString funcName = QString::fromStdString(name);

	if (name == "addEventListener" && arguments.size() == 2 && arguments[1]->IsFunction()) {
		// �¼�����
		CefRefPtr<CefV8Value> eventNameValue = arguments[0];
		QString eventName = QString::fromStdString(eventNameValue.get()->GetStringValue());
		// ��ȡ�ص�����
		CefRefPtr<CefV8Value> callback = arguments[1];

		EventListener eventListener = { callback ,CefV8Context::GetCurrentContext() };
		// ����ص�����
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
	// �ļ��иı�
	if (m_callbackMap.contains(eventType)) {
		QList<EventListener> callbackList = m_callbackMap[eventType];
		qDebug() << QString("AppEventListenerV8Handler::executeNotify====>%1===>%2").arg(eventType).arg(callbackList.size());
		for (auto it = callbackList.begin(); it != callbackList.end(); ++it) {
			// �ص���������ֵ
			CefRefPtr<CefV8Value> callbackFunctionParam = CefV8Value::CreateString(content.toStdString());
			CefV8ValueList arguments;
			arguments.push_back(callbackFunctionParam);
			// ִ��JavaScript�ص��������������ݸ�����������һ��CefV8ValueList
			CefRefPtr<CefV8Context> context = (*it).context_;
			CefRefPtr<CefV8Value> callback = (*it).callback_;
			context.get()->Enter();//����������
			callback->ExecuteFunction(NULL, arguments);
			context.get()->Exit();//�˳�������
		}
	}
}
