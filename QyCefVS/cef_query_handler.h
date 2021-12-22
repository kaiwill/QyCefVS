#pragma once
#include "QObject"
#include "include/wrapper/cef_message_router.h"
class CefQueryHandler : public QObject,
	public CefMessageRouterBrowserSide::Handler
{
	Q_OBJECT
public:
	explicit CefQueryHandler() {
	}

	//  从渲染进程JavaScript 中发出 window.cefQuery 调用，收到渲染进程的消息后进行处理.
	bool OnQuery(CefRefPtr<CefBrowser> browser, //哪个浏览器
		CefRefPtr<CefFrame> frame, //浏览器中的哪个frame发出的
		int64 query_id, //消息ID
		const CefString& request, // 消息名称
		bool persistent,
		CefRefPtr<Callback> callback) override;
	// 取消
	void CefQueryHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id) override;

	// 向主线程发送信号，弹出选择文件对话框，然后读取文件
	// 读取结束后，调用该方法处理文件读取的结果，回传到渲染进程
	void handleTextFileContent(int errorCode, QString fileContent, int64 query_id);

	//信号
signals:
	// 读取文件
	void onReadFile(qint64 query_id);

private:
	// 用来保存 query_id 和 callback之间的映射关系
	std::map<int64, CefRefPtr<Callback>> m_pendings;
};

