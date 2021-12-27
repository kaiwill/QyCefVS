#pragma once
#pragma execution_character_set("UTF-8")
#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "cef/qycefappbrowser.h"
#include "slaverwindow.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QyCefAppBrowser* cefApp, QWidget* parent = Q_NULLPTR);
	~MainWindow() {

		if (m_slaverWin) {
			delete m_slaverWin;
		}
	};

protected:
	void resizeEvent(QResizeEvent* event);
	void closeEvent(QCloseEvent* e);
private slots:
	void createBrowserWindow();
private:
	bool createBrowser(QString url, QWidget* parentWin);
	QyCefAppBrowser* m_cefApp = NULL;
	Ui::MainWindowClass ui;
	SlaverWindow* m_slaverWin = NULL;


};
