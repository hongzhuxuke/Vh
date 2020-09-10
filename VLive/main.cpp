//#include "VLive.h"
#include <QtWidgets/QApplication>
#include "../Unility/Unility.h"
#include "LoginWdg.h"
#include "./RelevantDlg/CAboutDlg.h"
#include "VLiveCustomEvent.h"
#include "ExceptionDump.h"
#include "pathManage.h"
#include <QDir>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSharedMemory> 
#include "ConfigSetting.h"
#include "TipsWdg.h"


std::wstring gCurrentVersion = L"0.0.0.0";
std::string gCurStreamID = "0";
std::wstring gCurClientType = L"vhallClass";

#include <comdef.h>
#include <Wbemidl.h>
#include <comdef.h>
#include <Wbemidl.h>
#include "AMDAdapterCheck.h"
#pragma comment(lib, "wbemuuid.lib")

bool IsNoticeUpdateGraphicsCardDriver(QString &cardName) {
    bool bNeedUpdate = false;

    HRESULT hres;
    // Obtain the initial locator to Windows Management
    // on a particular host computer.
    IWbemLocator *pLoc = 0;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID *)&pLoc);

    if (FAILED(hres)) {
        CoUninitialize();
        return false;       // Program has failed.
    }

    IWbemServices *pSvc = 0;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        NULL,                    // Security flags                 
        0,                       // Authority       
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    if (FAILED(hres)) {
        pLoc->Release();
        return false;                // Program has failed.
    }
    // Set the IWbemServices proxy so that impersonation
    // of the user (client) occurs.
    hres = CoSetProxyBlanket(
        pSvc,                         // the proxy to set
        RPC_C_AUTHN_WINNT,            // authentication service
        RPC_C_AUTHZ_NONE,             // authorization service
        NULL,                         // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,       // authentication level
        RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
        NULL,                         // client identity 
        EOAC_NONE                     // proxy capabilities     
    );

    if (FAILED(hres))
    {
        pSvc->Release();
        pLoc->Release();
        return false;               // Program has failed.
    }


    // Use the IWbemServices pointer to make requests of WMI. 
    // Make requests here:

    // For example, query for all the running processes
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_VideoController"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        cout << "Query for processes failed. "
            << "Error code = 0x"
            << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        return false;               // Program has failed.
    }
    else
    {
        IWbemClassObject *pclsObj;
        ULONG uReturn = 0;

        while (pEnumerator)
        {
            hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (0 == uReturn) {
                break;
            }
            VARIANT vtPropName;
            VARIANT vtPropVersion;

            // Get the value of the Name property
            hres = pclsObj->Get(L"Description", 0, &vtPropName, 0, 0);
            WCHAR* des;
            des = vtPropName.bstrVal;
            QList<QString> curVersonInfoList;
            QList<QString> configVersonInfoList;
            hres = pclsObj->Get(L"DriverVersion", 0, &vtPropVersion, 0, 0);
            if (QString::fromStdWString(vtPropName.bstrVal).contains("Intel(R) HD Graphics Family")) {
                QString curVersion = QString::fromStdWString(vtPropVersion.bstrVal);
                //此版本会出现崩溃问题
                if (QString("10.18.10.4242") == curVersion) {
                    bNeedUpdate = true;
                    cardName = QString::fromStdWString(vtPropName.bstrVal);
                }
            }
            else if (QString::fromStdWString(vtPropName.bstrVal).contains("Intel(R) HD Graphics 5500")) {
                QString curVersion = QString::fromStdWString(vtPropVersion.bstrVal);
                //此版本会出现崩溃问题
                if (QString("10.18.15.4271") == curVersion) {
                    bNeedUpdate = true;
                    cardName = QString::fromStdWString(vtPropName.bstrVal);
                }
            }
            if (bNeedUpdate) {
                VariantClear(&vtPropName);
                VariantClear(&vtPropVersion);
                break;
            }
        }
    }

    pSvc->Release();
    pLoc->Release();
    return bNeedUpdate;
}

int main(int argc, char *argv[])
{

   //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   //qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");


    QApplication a(argc, argv);

	bool bNeedUpdate = false;

    //加载qss
    //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
	QString class_conf = ToolManager::GetInstance()->GetToolConfigPath();
    int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
    if (hideLogo == 1) {
       a.setWindowIcon(QIcon(":/ico/class.ico"));
    }
    else {
       a.setWindowIcon(QIcon(":/ico/VhallClassRoom.ico"));
    }

    QSharedMemory mem("VHALLCLASSROOM");// 全局对象名   
    if (!mem.create(1))// 如果全局对象以存在则退出   
    {
       //CBaseWnd* tips = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_TIPS_WND, "close");
       TipsWdg* tips = new TipsWdg(/*this*/);
       if (tips) {
          CSTDMapParam param;
          //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
          int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
          if (hideLogo) {
             param[TipsMsg] = QStringLiteral("课堂客户端已经启动！");
          }
          else {
             param[TipsMsg] = QStringLiteral("微吼课堂已经启动！");
          }
          param[ShowCancelButton] = PARAM_FALSE;
          tips->Notice(param);
          int accept = tips->exec();
          if (accept == QDialog::Accepted) {
             ToolManager::GetInstance()->GetDataManager()->WriteLog("%s", __FUNCTION__);
             ToolManager::GetInstance()->SafeToExitApp();
             return 0;
          }
       }
    }

	//自适应屏幕
	QString msConfPath = ToolManager::GetInstance()->GetConfigPath();
	QString confadaptive = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, SCREEN_ADAPTIVE, "");
	if (confadaptive == "1") {
		qputenv("QT_ENABLE_HIGHDPI_SCALING", "1");
		QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	}


    //解决pepflashplayer.dll在no-sanbox模式下弹窗cmd黑框
    std::wstring wpath = GetAppPath() + L"cmd.exe";
    SetEnvironmentVariableW(L"ComSpec", wpath.c_str());

    QTranslator trans;
    trans.load(":/zh/img/qt_zh_CN.qm");
    QCoreApplication::installTranslator(&trans);

    qputenv("QT_BEARER_POLL_TIMEOUT", QByteArray::number(-1));
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9223");

	CExceptionDump g_oDmpFile;
    VLiveCustomEvent eventReciver;
    ToolManager::GetInstance()->Init();     
	if (!ToolManager::GetInstance()->GetLibManager()->InitDll()) {
		int err = GetLastError();
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s  InitDll is error %d", __FUNCTION__, err);
	}

    ToolManager::GetInstance()->RegisterQEventReciver(&eventReciver);
	//ToolManager::GetInstance()->PostEventToMainThread(new CustomEventType(CustomEvent_InitUI, ""));
	ToolManager::GetInstance()->GetWndManager()->RegisterWndFactory(WND_ABOUT, REGISTER_WND(CAboutDlg));
    ToolManager::GetInstance()->GetWndManager()->RegisterWndFactory(WND_ID_LOGIN, REGISTER_WND(LoginWdg));
	//ToolManager::GetInstance()->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_SETTING, REGISTER_WND(VhallIALiveSettingDlg));

	QFile qss(":/Qss/Style.qss");
	if (qss.open(QFile::ReadOnly)) {
		a.setStyleSheet(qss.readAll());
		qss.close();
	}
	else
	{
		//CBaseWnd* tips = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_TIPS_WND, "close");
		TipsWdg* tips = new TipsWdg(/*this*/);
		if (tips) {
			CSTDMapParam param;
			param[TipsMsg] = QStringLiteral("皮肤配置文件加载失败，程序界面可能会存在异常显示！");
			param[ShowCancelButton] = PARAM_FALSE;
			tips->Notice(param);
			int accept = tips->exec();
		}
	}

	

	//char *targv[] = {
 //  "C:\\Users\\vhall\\Documents\\VhallLive2.0\\VhallClassRoom.exe",
 //  "VHALLCLASSROOM:\\vhallclassroom?auth_token=3a9b8517fca197606d6f066cad44071d"
	//};
	//argc = sizeof(targv) / sizeof(char *);
	//argv = targv;


	//int iFlag = 0;
	//while (1) {
	//	char* a = new char[1024 * 1024 * 10];
	//	ToolManager::GetInstance()->GetDataManager()->WriteLog("%s %d ", __FUNCTION__, iFlag++);
	//}

	ToolManager::GetInstance()->CheckParam(argc, argv);
	CBaseWnd* aboutWnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ABOUT);
	CSTDMapParam param;
	aboutWnd->Notice(param);

    while (!ToolManager::GetInstance()->IsExist()) {
        a.exec();
    }
    //ToolManager::GetInstance()->GetClassSDK()->des
	ToolManager::GetInstance()->GetDataManager()->WriteLog("MAIN RETUN");
    //ToolManager::GetInstance()->AppExec();
    ToolManager::GetInstance()->Release();
    ToolManager::ReleaseInstance();
	ToolManager::GetInstance()->GetDataManager()->WriteLog("MAIN 00000");
    return 0;
}
