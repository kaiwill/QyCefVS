#include <QtCore/QCoreApplication>
#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "QyAppRenderer.h"
#include "log.h"
int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);
	qInstallMessageHandler(outputMessage);
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	//构造命令行
	CefMainArgs main_args(hInstance);

	// Optional implementation of the CefApp interface.
	// 可选择性地实现CefApp接口
	CefRefPtr<QyAppRenderer> app(new QyAppRenderer);

	// Execute the sub-process logic. This will block until the sub-process should exit.
	// 执行子进程逻辑，此时会堵塞直到子进程退出。
	return CefExecuteProcess(main_args, app.get(), nullptr);
}
