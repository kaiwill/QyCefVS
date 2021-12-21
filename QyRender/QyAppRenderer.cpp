#pragma execution_character_set("UTF-8")
#include "QyAppRenderer.h"
#include <QDebug>
#include <QProcess>
#include "encrypt/qaesencryption.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>
namespace {

	// 获取cpu名称
	const QString CPU_NAME_CMD = "wmic cpu get name";
	// 查询cpu序列号
	const QString CPU_ID_CMD = "wmic cpu get processorid";
	// 查看硬盘序列号
	const QString DISK_NUM_CMD = "wmic diskdrive where index=0 get serialnumber";
	// 查询网卡和IP地址
	const QString IP_MAC_ADDRESS_CMD = "wmic PATH Win32_NetworkAdapterConfiguration WHERE \"IPEnabled = True and not MACAddress like '00:%'\" get Ipaddress,MACAddress";
	// 内存大小
	const QString MEM_SIZE_CMD = "wmic ComputerSystem get TotalPhysicalMemory";

	//IP地址正则表达式
	QString IP_REG_PATTERN = "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)";
	//MAC地址正则表达式
	QString MAC_REG_PATTERN = "([0-9a-fA-F]{2})(([/\\s:][0-9a-fA-F]{2}){5})";

	// 应用程序环境
	typedef struct
	{
		QString cpuSn;			//cpu序列号
		QString cpuName;	// cpu名称
		QString ipAddr;			//IP地址
		QString macAddr;		//网卡mac地址
		QString hddSn;			// 硬盘序列号
		qint64 memory;				//内存大小，单位 字节
	} AppEnv;

	QString getWMIC(const QString& cmd)
	{
		QProcess p;
		p.start(cmd);
		p.waitForFinished();
		QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
		QStringList list = cmd.split(" ");
		result = result.remove(list.last(), Qt::CaseInsensitive);
		result = result.replace("\r", "");
		result = result.replace("\n", "");
		result = result.simplified();
		return result;
	}


	// 获取应用程序运行环境
	static AppEnv getAppEnv() {
		AppEnv appEnv;
		appEnv.cpuName = getWMIC(CPU_NAME_CMD); // CPU名称
		appEnv.cpuSn = getWMIC(CPU_ID_CMD); // CPU序列号

		QString	ipMac = getWMIC(IP_MAC_ADDRESS_CMD); //IP地址和mac地址
		QRegExp ipRx(IP_REG_PATTERN); //IP正则
		if (ipMac.indexOf(ipRx) >= 0) {
			appEnv.ipAddr = ipRx.cap(0);
		}

		QRegExp macRx(MAC_REG_PATTERN);//MAC正则
		if (ipMac.indexOf(macRx) >= 0) {
			appEnv.macAddr = macRx.cap(0);
		}
		appEnv.hddSn = getWMIC(DISK_NUM_CMD); //硬盘

		appEnv.memory = getWMIC(MEM_SIZE_CMD).toLongLong();

		return appEnv;
	}


	class AppNativeV8Handler : public CefV8Handler {
	public:
		AppNativeV8Handler() {}
		bool Execute(const CefString& name, //JavaScript 函数名
			CefRefPtr<CefV8Value> object,     //JavaScript函数持有者
			const CefV8ValueList& arguments,//JavaScript 参数
			CefRefPtr<CefV8Value>& retval,  // JavaScript返回值
			CefString& exception) override {

			qDebug() << "====1====Execute=====================";
			// 加密
			if (name == "encrypt") {
				std::string originText = arguments.at(0).get()->GetStringValue();
				QString	 rtText = encrypt(QString::fromStdString(originText));
				//返回值交给 retval
				retval = CefV8Value::CreateString(rtText.toStdString());
				qDebug() << "====2====Execute=====================";
				return true;
			}
			// 解密
			if (name == "decrypt") {
				std::string encryptText = arguments.at(0).get()->GetStringValue();
				QString	 rtText = decrypt(QString::fromStdString(encryptText));
				retval = CefV8Value::CreateString(rtText.toStdString());
				qDebug() << "====3====Execute=====================";
				return true;
			}
			// sayHello 带回调的JavaScript 函数
			// 名称为 sayHello, 参数数量为1，而且类型是一个函数
			if (name == "sayHello" && arguments.size() == 1 && arguments[0]->IsFunction()) {
				// 获取回调函数
				CefRefPtr<CefV8Value> callback = arguments[0];
				QDateTime now = QDateTime::currentDateTime();//获取系统当前时间
				time_t nowTime = now.toTime_t();
				//转换成CefDate 类型
				CefTime cefNow(nowTime);
				CefRefPtr<CefV8Value> callbackFunctionParam = CefV8Value::CreateDate(cefNow);

				//CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
				//context->Enter();
				CefV8ValueList arguments;
				arguments.push_back(callbackFunctionParam);


				// 执行JavaScript回调，并将参数传递给它，参数是一个CefV8ValueList
				callback->ExecuteFunction(NULL, arguments);
				//context->Exit();
				qDebug() << "====4====Execute=====================";
				return true;
			}
			return false;
		}

	private:
		// aes加密算法密钥
		QString _aes_secret_key = "a0123456789";

		/*
		* AES算法字符串加密
		*/
		QString encrypt(QString originText) {

			QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
			QByteArray hashKey = QCryptographicHash::hash(_aes_secret_key.toUtf8(), QCryptographicHash::Md5);
			QByteArray encodedText = encryption.encode(originText.toUtf8(), hashKey);
			QString str_encode_text = QString::fromLatin1(encodedText.toBase64());
			qDebug() << "encrypt:" << originText << "    result:" << str_encode_text;
			return str_encode_text;
		}

		/*
		* AES算法字符串解密
		*/
		QString decrypt(QString encryptText) {
			QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
			QByteArray hashKey = QCryptographicHash::hash(_aes_secret_key.toUtf8(), QCryptographicHash::Md5);
			QByteArray decodedText = encryption.decode(QByteArray::fromBase64(encryptText.toLatin1()), hashKey);
			qDebug() << "decrypt:" << encryptText << "    result:" << QString::fromUtf8(decodedText);
			return QString::fromUtf8(decodedText);
		}
		IMPLEMENT_REFCOUNTING(AppNativeV8Handler);
	};

}

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
	//qDebug() << "=====OnBrowserCreated=======";
}

void QyAppRenderer::OnWebKitInitialized() {
	//1. 从资源文件中获取要扩展的JavaScript代码
	QFile jsFile(":/extention_js.js");
	jsFile.open(QIODevice::ReadOnly);
	QByteArray jsFileData = jsFile.readAll();
	//2.  JavaScript 字符串
	QString jsCode(jsFileData);

	//	std::string jsCode = R"(
	//	var app="12345";
	//)";

	//qDebug() << "jsCode:" << QString::fromStdString(jsCode);
	// 3. Register app extension module
	// JavaScript里调用函数时，就会去通过CefRegisterExtension注册的CefV8Handler列表里查找
	// 找到"v8/app"对应的CefV8HandlerImpl，就调用它的Execute方法
	CefRefPtr<CefV8Handler> v8Handler = new AppNativeV8Handler();

	CefRegisterExtension("v8/app", jsCode.toStdString(), v8Handler);

}

void QyAppRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	//qDebug() << "=====OnContextCreated=======";
	// 获取app运行环境硬件信息
	AppEnv appEnv = ::getAppEnv();

	//// Retrieve the context's window object.
	CefRefPtr<CefV8Value> window = context->GetGlobal();

	////// 创建一个JavaScript 对象，全部为只读属性例如：
	//// window.appEnv={cpuSn:"BFEBFBFF000506E3",cpuName:"Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz",ipAddr:"192.168.1.91",macAddr:"38:D5:47:79:B8:39",hddSn:"W4X0NVY7",memory:17054674944}
	CefRefPtr<CefV8Value> appEnvObject = CefV8Value::CreateObject(NULL, NULL);
	/*AppEnv appEnv = {
		"BFEBFBFF000506E3",
		"Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz",
		"192.168.1.91",
		"38:D5:47:79:B8:39",
		"W4X0NVY7",
		17054674944
	};*/

	appEnvObject->SetValue("cpuSn", CefV8Value::CreateString(appEnv.cpuSn.toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	appEnvObject->SetValue("cpuName", CefV8Value::CreateString(appEnv.cpuName.toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	appEnvObject->SetValue("ipAddr", CefV8Value::CreateString(appEnv.ipAddr.toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	appEnvObject->SetValue("macAddr", CefV8Value::CreateString(appEnv.macAddr.toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	appEnvObject->SetValue("hddSn", CefV8Value::CreateString(appEnv.hddSn.toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	appEnvObject->SetValue("memory", CefV8Value::CreateString(QString::number(appEnv.memory).toStdString()), V8_PROPERTY_ATTRIBUTE_READONLY);
	// JavaScript 函数调用后，执行的本地处理器
	CefRefPtr<CefV8Handler> handler = new AppNativeV8Handler();
	CefRefPtr<CefV8Value> funcEncrypt = CefV8Value::CreateFunction("encrypt", handler);
	CefRefPtr<CefV8Value> funcDecrypt = CefV8Value::CreateFunction("decrypt", handler);
	appEnvObject->SetValue("encrypt", funcEncrypt, V8_PROPERTY_ATTRIBUTE_NONE);
	appEnvObject->SetValue("decrypt", funcDecrypt, V8_PROPERTY_ATTRIBUTE_NONE);

	//绑定到window对象上,同样为只读属性
	window->SetValue("appEnv", appEnvObject, V8_PROPERTY_ATTRIBUTE_READONLY);
}

// 收到其它进程发送过来的消息
bool QyAppRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	qDebug() << "收到进程:" << source_process << "的消息, 消息名称:"
		<< QString::fromStdString(message.get()->GetName());
	return true;
}

