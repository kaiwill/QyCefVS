#include "mainwindow.h"
#include "cef/simple_handler.h"
#include "QDesktopWidget"
#include "QDockWidget"
MainWindow::MainWindow(SimpleApp* cefApp,QWidget *parent)
    : QMainWindow(parent),m_cefApp(cefApp)
{
    ui.setupUi(this);
    // ��SimpleApp �лص�OnctextInitialized��ʱ��֪ͨ �����崴����������ڣ���Ƕ�뵽��������
    connect(m_cefApp, &SimpleApp::onCefOnctextInitialized, this, &MainWindow::createBrowserWindow);
}

/// <summary>
/// �������������
/// </summary>
void MainWindow::createBrowserWindow() {

    CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));
  // ��������ã�
  CefBrowserSettings browser_settings;
  // Ҫ�򿪵���ַ
  std::string url= "https://www.baidu.com";
  // �����������Ϣ
  CefWindowInfo window_info;
  
  //window_info.SetAsPopup(NULL, "cefsimple");
  // ��ȡǶ�봰��ľ��
    

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

void MainWindow::toogleDevToolsWindow(CefBrowser* browser)
{
    //m_dev_tools->show();
   
}
