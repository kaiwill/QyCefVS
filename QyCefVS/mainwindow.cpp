#pragma execution_character_set("UTF-8")
#include "mainwindow.h"
#include "cef/qycefclienthandler.h"
#include "QDesktopWidget"
#include "QDockWidget"
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QyCefAppBrowser* cefApp, QWidget* parent)
	: QMainWindow(parent), m_cefApp(cefApp)
{
	ui.setupUi(this);
	// 当QyCefAppBrowser 中回调OnctextInitialized的时候，通知 主窗体创建浏览器窗口，并嵌入到主窗口中
	connect(m_cefApp, &QyCefAppBrowser::onCefOnctextInitialized, this, &MainWindow::createBrowserWindow);
	ui.mainToolBar->setVisible(false);
	this->activateWindow();//让主窗口处于活动窗口
}

// 创建浏览器对象
bool MainWindow::createBrowser(QString url, QWidget* parentWin) {
	//获取实例对象
	CefRefPtr<QyCefClientHandler> handler(QyCefClientHandler::getInstance());
	// 浏览器配置，
	CefBrowserSettings browser_settings;
	//browser_settings.universal_access_from_file_urls = STATE_DISABLED;
	CefWindowInfo window_info;
	RECT winRect;
	QRect qtRect = parentWin->rect();
	winRect.left = qtRect.left();
	winRect.top = qtRect.top();
	winRect.right = qtRect.right();
	winRect.bottom = qtRect.bottom();
	HWND wnd = (HWND)parentWin->winId();
	window_info.SetAsChild(wnd, winRect);
	// Create the  browser window.
	return CefBrowserHost::CreateBrowser(window_info, handler, url.toStdString(), browser_settings,
		nullptr, nullptr);
}

/// <summary>
/// 创建浏览器窗体,并与QT 窗体集成
/// </summary>
void MainWindow::createBrowserWindow() {
	//运行目录
	QDir dir = QCoreApplication::applicationDirPath();
	// 要打开的网址
	QString uriMaster = QDir::toNativeSeparators(dir.filePath("resources/index.html"));
	// 创建MasterBrowser
	createBrowser(uriMaster, this->centralWidget());

	// 操作系统桌面
	QDesktopWidget* desktop = QApplication::desktop();
	if (desktop->screenCount() <= 1) {
		qDebug() << QString("无第二屏幕:%1").arg(desktop->screenCount());
		return;
	}
	QString uriSlaver = QDir::toNativeSeparators(dir.filePath("resources/slaver.html"));
	m_slaverWin = new SlaverWindow(NULL);
	m_slaverWin->setGeometry(desktop->screenGeometry(1));
	// 创建浏览器窗口
	bool ret = createBrowser(uriSlaver, m_slaverWin);
	m_slaverWin->show();
	m_slaverWin->showFullScreen();
	//qDebug() << QString("SlaverWindow::createBrowserWindow======:%1").arg(ret);

}
void MainWindow::resizeEvent(QResizeEvent* event)
{
	if (QyCefClientHandler::getInstance()) {
		HWND wnd = QyCefClientHandler::getInstance()->getMasterBrowserWindowHandle();
		if (wnd) {
			QRect qtRect = this->centralWidget()->rect();
			::MoveWindow(wnd, qtRect.x(), qtRect.y(), qtRect.width(), qtRect.height(), true);
		}
	}
}
void MainWindow::closeEvent(QCloseEvent* e) {
	//qDebug() << QString("MainWindow::closeEvent");
	if (m_slaverWin) {
		m_slaverWin->hide();
		m_slaverWin->close();
		delete m_slaverWin;
	}
	//释放QyCefClientHandler 实例对象
	QyCefClientHandler::releaseInstance();
}