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
    // ͨ��GetModuleHandle ��ȡ HINSTANCE
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    //CEF �����в���
    CefMainArgs main_args(hInstance);
    
    // CefExecuteProcess �����������������̵ġ�
    // �����������ɺ�Ŀ�ִ���ļ��� QyCefVS.exe �״�������ʱ��ᴴ��������
    // exit_code��һ��������������QT ��qDebug���
    // CEF ��Ҫ�����ӽ��̵�ʱ�򣬻��ٴε��� QyCefVS.exe ���������̣�������һ�λ��ڵ���QyCefVS.exe
    // ��ʱ�򴫵ݲ������������������ӽ��̣����� ��--type=renderer��
    // ��ʱCefExecuteProcessִ�����֮�󣬾ͻ᷵��һ������0��ֵ����ʱ�ͻ��˳���������������ִ�С�
	int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    qDebug() << "exit_code:" << exit_code;

    if (exit_code >= 0) {
        // ���exit_code ���ڵ���0�� ��ʾ���������ӽ��̣�ֱ���˳���
        return exit_code;
    }
    // CEF ȫ������
    CefSettings settings;
    settings.no_sandbox = true;
    // ������������CEF�ڵ������߳�������Browser�Ľ��棬�����������߳��ϡ�
    settings.multi_threaded_message_loop = true;
    
    // ע�� CEF ��Ϣѭ����ʹ��QT ��Ϣѭ��
//CefRunMessageLoop();
// ���� QT ��Ϣѭ��

    SimpleApp* cefApp=new SimpleApp;

    QApplication a(argc, argv);
    MainWindow w(cefApp, nullptr);
    w.show();
    CefRefPtr<SimpleApp> app(cefApp);
    // ��ʼ��CEF
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
