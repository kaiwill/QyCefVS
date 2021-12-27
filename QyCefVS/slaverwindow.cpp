#include "slaverwindow.h"
#include "cef/qycefclienthandler.h"
#include <QDir>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
SlaverWindow::SlaverWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//createBrowserWindow();
}

SlaverWindow::~SlaverWindow()
{
}
/// <summary>
/// ������������
/// </summary>
void SlaverWindow::createBrowserWindow() {
	//��ȫ��
	QDesktopWidget* desktop = QApplication::desktop();
	this->setGeometry(desktop->screenGeometry(1));
	this->showFullScreen();
	//��ȡʵ������
	CefRefPtr<QyCefClientHandler> handler(QyCefClientHandler::getInstance());
	// ��������ã�
	CefBrowserSettings browser_settings;
	//browser_settings.universal_access_from_file_urls = STATE_DISABLED;
	//����Ŀ¼
	QDir dir = QCoreApplication::applicationDirPath();
	QString uri = QDir::toNativeSeparators(dir.filePath("resources/slaver.html"));

	std::string url = uri.toStdString();
	// �����������Ϣ
	CefWindowInfo window_info;

	//window_info.SetAsPopup(NULL, "cefsimple");
	// ��ȡǶ�봰��ľ��


	CefWindowInfo cefWndInfo;
	RECT winRect;
	QRect qtRect = desktop->screenGeometry(1);
	winRect.left = qtRect.left();
	winRect.top = qtRect.top();
	winRect.right = qtRect.right();
	winRect.bottom = qtRect.bottom();
	HWND wnd = (HWND)this->winId();
	window_info.SetAsChild(wnd, winRect);

	uri = QDir::toNativeSeparators(dir.filePath("resources/slaver.html"));
	url = uri.toStdString();
	// �������������
	bool ret = CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, nullptr, nullptr);
	qDebug() << QString("SlaverWindow::createBrowserWindow:%1").arg(ret);
}
