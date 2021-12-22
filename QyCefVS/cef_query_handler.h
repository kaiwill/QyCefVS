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

	//  ����Ⱦ����JavaScript �з��� window.cefQuery ���ã��յ���Ⱦ���̵���Ϣ����д���.
	bool OnQuery(CefRefPtr<CefBrowser> browser, //�ĸ������
		CefRefPtr<CefFrame> frame, //������е��ĸ�frame������
		int64 query_id, //��ϢID
		const CefString& request, // ��Ϣ����
		bool persistent,
		CefRefPtr<Callback> callback) override;
	// ȡ��
	void CefQueryHandler::OnQueryCanceled(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id) override;

	// �����̷߳����źţ�����ѡ���ļ��Ի���Ȼ���ȡ�ļ�
	// ��ȡ�����󣬵��ø÷��������ļ���ȡ�Ľ�����ش�����Ⱦ����
	void handleTextFileContent(int errorCode, QString fileContent, int64 query_id);

	//�ź�
signals:
	// ��ȡ�ļ�
	void onReadFile(qint64 query_id);

private:
	// �������� query_id �� callback֮���ӳ���ϵ
	std::map<int64, CefRefPtr<Callback>> m_pendings;
};

