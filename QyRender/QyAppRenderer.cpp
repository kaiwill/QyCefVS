#include "QyAppRenderer.h"

/// <summary>
/// 构造方法空实现
/// </summary>
QyAppRenderer::QyAppRenderer() {

}
/// <summary>
/// 当CefBrowser对象已经创建的时候回调,将来JS与c++通信的时候会用到，现在只做空实现
/// </summary>
/// <param name="browser"></param>
/// <param name="extra_info"></param>
void QyAppRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDictionaryValue> extra_info) {

}