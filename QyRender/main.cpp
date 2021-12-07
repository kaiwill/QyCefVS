#include <QtCore/QCoreApplication>
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "QyAppRenderer.h"
int main(int argc, char *argv[])
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    //����������
    CefMainArgs main_args(hInstance);

    // Optional implementation of the CefApp interface.
    // ��ѡ���Ե�ʵ��CefApp�ӿ�
    CefRefPtr<QyAppRenderer> app(new QyAppRenderer);

    // Execute the sub-process logic. This will block until the sub-process should exit.
    // ִ���ӽ����߼�����ʱ�����ֱ���ӽ����˳���
    return CefExecuteProcess(main_args, app.get(),nullptr);
}
