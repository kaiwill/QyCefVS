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
    //CEF 命令行参数
    CefMainArgs main_args(hInstance);
    
    // CefExecuteProcess 函数是用来创建进程的。
    // 本程序编译完成后的可执行文件是 QyCefVS.exe 首次启动的时候会创建主进程
    // exit_code是一个负数，可以用QT 的qDebug输出
    // CEF 需要创建子进程的时候，会再次调用 QyCefVS.exe 来启动进程，但是这一次会在调用QyCefVS.exe
    // 的时候传递参数，表明创建的是子进程，比如 “--type=renderer”
    // 这时CefExecuteProcess执行完毕之后，就会返回一个大于0的值，此时就会退出而不会继续向后面执行。
	int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    qDebug() << "exit_code:" << exit_code;

    if (exit_code >= 0) {
        // 如果exit_code 大于等于0， 表示创建的是子进程，直接退出了
        return exit_code;
    }
    // CEF 全局设置
    CefSettings settings;
    settings.no_sandbox = true;
    // 这个设置项将导致CEF在单独的线程上运行Browser的界面，而不是在主线程上。
    settings.multi_threaded_message_loop = true;
    
    // 注释 CEF 消息循环，使用QT 消息循环
//CefRunMessageLoop();
// 开启 QT 消息循环

    SimpleApp* cefApp=new SimpleApp;

    QApplication a(argc, argv);
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
