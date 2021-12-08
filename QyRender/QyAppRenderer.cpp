#pragma execution_character_set("UTF-8")
#include "QyAppRenderer.h"
#include <QDebug>
/// <summary>
/// 构造方法空实现
/// </summary>
QyAppRenderer::QyAppRenderer() {

}
/// <summary>
/// 当CefBrowser对象已经创建的时候回调,将来JS与c++通信的时候会用到，现在只做空实现
/// </summary>
/// <param name="browser"></param>
/// <param name="extra_info"></param>
void QyAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDictionaryValue> extra_info) {
	qDebug() << "=====OnBrowserCreated=======";
}

void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {

	// 发送消息给 Browser进程
	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("showSubWindow");
	CefRefPtr<CefListValue> args = msg->GetArgumentList();
	CefString title("我是一个子窗口");
	int width = 400, height = 300; //窗体的宽和高
	args->SetSize(2); //两个参数
	args->SetString(0, title);
	args->SetInt(1, width);
	args->SetInt(2, height);
	qDebug() << "=====发送消息给Browser进程=======";
	// 发送消息给Browser进程
	frame->SendProcessMessage(PID_BROWSER, msg);
}

// 收到其它进程发送过来的消息
bool QyAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	qDebug() << "收到进程:" << source_process << "的消息, 消息名称:"
		<< QString::fromStdString(message.get()->GetName());
	return true;
}

