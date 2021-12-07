#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "cef/simple_app.h"
#include <qdebug.h>
int main(int argc, char *argv[])
{
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();
    // 通过GetModuleHandle 获取 HINSTANCE
    HINSTANCE hInstance = GetModuleHandle(nullptr);
   
// 开启 QT 消息循环
    SimpleApp* cefApp=new SimpleApp;
    QApplication a(argc, argv);
    //CEF 命令行参数
    CefMainArgs main_args(hInstance);
    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = true;

    //分离的执行体
    QString executerPath = QApplication::applicationDirPath().append("\\QyRender.exe");
    CefString(&settings.browser_subprocess_path).FromASCII(executerPath.toStdString().c_str());

    MainWindow w(cefApp, nullptr);
    w.show();
    CefRefPtr<SimpleApp> app(cefApp);
    // 初始化CEF
    CefInitialize(main_args, settings, app.get(), nullptr);
    int ret = a.exec();
    // Shut down CEF.
    CefShutdown();
    return ret;
}


//QApplication a(argc, argv);
//MainWindow w;
//w.show();
//return a.exec();
