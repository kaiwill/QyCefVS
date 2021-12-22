#pragma execution_character_set("UTF-8")
#include "cef_query_handler.h"
#include <QDebug>
namespace {
	const char kTestMessageName[] = "MSG_READ_TEXT_FILE";
}

//  从渲染进程JavaScript 中发出 window.cefQuery 调用，收到渲染进程的消息后进行处理.
bool CefQueryHandler::OnQuery(CefRefPtr<CefBrowser> browser, //哪个浏览器
	CefRefPtr<CefFrame> frame, //浏览器中的哪个frame发出的
	int64 query_id, //消息ID
	const CefString& request, // 消息名称
	bool persistent,
	CefRefPtr<Callback> callback) {

	const std::string& message_name = request;
	if (message_name == kTestMessageName) {
		//涉及到IO操作，这里使用线程完成文件内容的读取
		//线程完成内容读取之后，需要调用 回调方法，所以这里需要将 对象和 callback对象暂时保存到map中
		//以便线程执行完毕之后，能够找到它们
		m_pendings.insert(std::make_pair(query_id, callback));
		qDebug() << "========OnQuery========";
		emit  onReadFile(query_id);
		return true;
	}
	return false;
}

void CefQueryHandler::handleTextFileContent(int errorCode, QString fileContent, int64 query_id) {
	//获取回调对象
	CefRefPtr<Callback> callback = m_pendings[query_id];
	if (errorCode == -1) {
		//读取文件出错
		callback->Failure(-1, QString("读取文件失败").toStdString());
	}
	else {
		callback->Success(fileContent.toStdString());
	}
	// 清除掉query_id 与callback的映射
	auto it = m_pendings.find(query_id);
	m_pendings.erase(it);
}

// 取消
void CefQueryHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id)
{
	auto it = m_pendings.find(query_id);
	if (it != m_pendings.end())
	{
		it->second->Failure(-1, "canceled");
		m_pendings.erase(it);
	}
}
