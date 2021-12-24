#pragma execution_character_set("UTF-8")
#include "mainwindow.h"
#include "cef/simple_handler.h"
#include "QDesktopWidget"
#include "QDockWidget"
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
MainWindow::MainWindow(SimpleApp* cefApp, QWidget* parent)
	: QMainWindow(parent), m_cefApp(cefApp),
	m_cef_query_handler(new CefQueryHandler),
	m_FileSystemWatcher(new FileSystemWatcher(this))
{
	ui.setupUi(this);
	// 当SimpleApp 中回调OnctextInitialized的时候，通知 主窗体创建浏览器窗口，并嵌入到主窗口中
	connect(m_cefApp, &SimpleApp::onCefOnctextInitialized, this, &MainWindow::createBrowserWindow);
	ui.mainToolBar->setVisible(false);

	QString watchPath = "E:\\tmp";
	m_FileSystemWatcher->addWatchPath(watchPath);
}

/// <summary>
/// 创建浏览器窗体
/// </summary>
void MainWindow::createBrowserWindow() {

	CefRefPtr<SimpleHandler> handler(new SimpleHandler(false, m_cef_query_handler));
	// 当文件发生变化时，通知到SimpleHandler
	connect(m_FileSystemWatcher, &FileSystemWatcher::onFileChangeEventTrigger, handler, &SimpleHandler::onFileChageEventTrigger);
	// 浏览器配置，
	CefBrowserSettings browser_settings;
	//browser_settings.universal_access_from_file_urls = STATE_DISABLED;
	//运行目录
	QDir dir = QCoreApplication::applicationDirPath();
	QString uri = QDir::toNativeSeparators(dir.filePath("resources/index.html"));
	// 要打开的网址
	//std::string url = "https://www.baidu.com";
	std::string url = uri.toStdString();
	// 浏览器窗口信息
	CefWindowInfo window_info;

	//window_info.SetAsPopup(NULL, "cefsimple");
	// 获取嵌入窗体的句柄


	CefWindowInfo cefWndInfo;
	RECT winRect;
	QRect qtRect = this->rect();
	winRect.left = qtRect.left();
	winRect.top = qtRect.top();
	winRect.right = qtRect.right();
	winRect.bottom = qtRect.bottom();
	HWND wnd = (HWND)centralWidget()->winId();
	window_info.SetAsChild(wnd, winRect);
	// Create the first browser window.
	CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
		nullptr, nullptr);

	connect(handler.get(), &SimpleHandler::onReceiveRendererProccessMessasge, this, &MainWindow::onReceiveRendererProccessMessasge);
	// 连接 CefQueryHandler中的onReadFile信号
	connect(m_cef_query_handler, &CefQueryHandler::onReadFile, this, &MainWindow::onReadFile);
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
	if (SimpleHandler::GetInstance()) {
		HWND wnd = SimpleHandler::GetInstance()->getBrowserWindowHandle();
		if (wnd) {
			QRect qtRect = this->centralWidget()->rect();
			::MoveWindow(wnd, qtRect.x(), qtRect.y(), qtRect.width(), qtRect.height(), true);
		}
	}
}

void MainWindow::onReceiveRendererProccessMessasge(QString title, int width, int height)
{
	/*QDialog* subWin = new QDialog(this);
	subWin->setWindowTitle(title);
	subWin->setFixedWidth(width);
	subWin->setFixedHeight(height);
	subWin->show();*/

}

//读取文件
void MainWindow::onReadFile(qint64 query_id) {
	qDebug() << "mainWindow========onReadFile";
	QString fileName = QFileDialog::getOpenFileName(NULL, "文件对话框", "F:", "文本文件(*txt)");
	QFile file(fileName);
	if (file.exists()) { // 如果文件存在
		// 读取文件内容
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QString content = file.readAll();
			file.close();
			//int errorCode, QString fileContent, int64 query_id
			//成功情况下回传数据，errorCode是 0
			m_cef_query_handler->handleTextFileContent(0, content, query_id);
			return;
		}
	}
	//失败情况下回传数据，errorCode是 -1
	m_cef_query_handler->handleTextFileContent(-1, "读取文件失败", query_id);
}
