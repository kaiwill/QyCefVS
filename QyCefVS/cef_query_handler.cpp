#pragma execution_character_set("UTF-8")
#include "cef_query_handler.h"
#include <QDebug>
namespace {
	const char kTestMessageName[] = "MSG_READ_TEXT_FILE";
}

//  ����Ⱦ����JavaScript �з��� window.cefQuery ���ã��յ���Ⱦ���̵���Ϣ����д���.
bool CefQueryHandler::OnQuery(CefRefPtr<CefBrowser> browser, //�ĸ������
	CefRefPtr<CefFrame> frame, //������е��ĸ�frame������
	int64 query_id, //��ϢID
	const CefString& request, // ��Ϣ����
	bool persistent,
	CefRefPtr<Callback> callback) {

	const std::string& message_name = request;
	if (message_name == kTestMessageName) {
		//�漰��IO����������ʹ���߳�����ļ����ݵĶ�ȡ
		//�߳�������ݶ�ȡ֮����Ҫ���� �ص�����������������Ҫ�� ����� callback������ʱ���浽map��
		//�Ա��߳�ִ�����֮���ܹ��ҵ�����
		m_pendings.insert(std::make_pair(query_id, callback));
		qDebug() << "========OnQuery========";
		emit  onReadFile(query_id);
		return true;
	}
	return false;
}

void CefQueryHandler::handleTextFileContent(int errorCode, QString fileContent, int64 query_id) {
	//��ȡ�ص�����
	CefRefPtr<Callback> callback = m_pendings[query_id];
	if (errorCode == -1) {
		//��ȡ�ļ�����
		callback->Failure(-1, QString("��ȡ�ļ�ʧ��").toStdString());
	}
	else {
		callback->Success(fileContent.toStdString());
	}
	// �����query_id ��callback��ӳ��
	auto it = m_pendings.find(query_id);
	m_pendings.erase(it);
}

// ȡ��
void CefQueryHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id)
{
	auto it = m_pendings.find(query_id);
	if (it != m_pendings.end())
	{
		it->second->Failure(-1, "canceled");
		m_pendings.erase(it);
	}
}
