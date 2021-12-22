#pragma once
#pragma execution_character_set("UTF-8")
#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "cef/simple_app.h"
#include "cef_query_handler.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(SimpleApp* cefApp, QWidget* parent = Q_NULLPTR);
	~MainWindow() {
		if (m_cef_query_handler != NULL) {
			delete m_cef_query_handler;
		}
	};

protected:
	void resizeEvent(QResizeEvent* event);

private slots:
	void createBrowserWindow();
	// 渲染进程发出信号
	void onReadFile(qint64 query_id);
	void onReceiveRendererProccessMessasge(QString title, int width, int height);
private:
	SimpleApp* m_cefApp = NULL;

	CefQueryHandler* m_cef_query_handler;
	Ui::MainWindowClass ui;
};
