#pragma once
#include <string>
namespace Share {
	const std::string MASTER_BROWSER = "index.html"; //主屏浏览器标识
	const std::string SLAVER_BROWSER = "slaver.html";// 副屏浏览器标识
	const std::string UNKOWN_BROWSER = "unkown";//未命名浏览器标识
	//进程消息  发送消息给 副屏
	const std::string PROCESS_MESSAGE_TO_SLAVER_SCREEN = "PROCESS_MESSAGE_TO_SLAVER_SCREEN";
}