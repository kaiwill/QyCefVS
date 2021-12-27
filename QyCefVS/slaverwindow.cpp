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
/// 创建副屏窗口
/// </summary>
void SlaverWindow::createBrowserWindow() {
	//先全屏
	QDesktopWidget* desktop = QApplication::desktop();
	this->setGeometry(desktop->screenGeometry(1));
	this->showFullScreen();
	//获取实例对象
	CefRefPtr<QyCefClientHandler> handler(QyCefClientHandler::getInstance());
	// 浏览器配置，
	CefBrowserSettings browser_settings;
	//browser_settings.universal_access_from_file_urls = STATE_DISABLED;
	//运行目录
	QDir dir = QCoreApplication::applicationDirPath();
	QString uri = QDir::toNativeSeparators(dir.filePath("resources/slaver.html"));

	std::string url = uri.toStdString();
	// 浏览器窗口信息
	CefWindowInfo window_info;

	//window_info.SetAsPopup(NULL, "cefsimple");
	// 获取嵌入窗体的句柄


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
	// 创建浏览器窗口
	bool ret = CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, nullptr, nullptr);
	qDebug() << QString("SlaverWindow::createBrowserWindow:%1").arg(ret);
}
