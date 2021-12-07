#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "cef/simple_app.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SimpleApp* cefApp,QWidget* parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void createBrowserWindow();
    void toogleDevToolsWindow(CefBrowser* browser);
private:
     SimpleApp*  m_cefApp=NULL;
    Ui::MainWindowClass ui;
};
