#include "VHMainThread.h"
#include "ConfigSetting.h"
#include <QJsonDocument>
#include "VHClassRoomInterface.h"
#include "../Unility/Unility.h"
#include <shlwapi.h>
#include <Shlobj.h>

#define app_key     "207038a004fcc7012fec958ce992ef8a"
#define app_secret  "dda37cdf83997044539ad9c35ab87be1"

ToolManager *globalToolManager = nullptr;
static MainThreadEventProcessor mMainThreadEventProcessor;

bool RegisterGlobalManager(void* reg) {
   if (reg == nullptr) {
      return false;
   }
   globalToolManager = (ToolManager*)reg;
   globalToolManager->RegisterQEventReciver(&mMainThreadEventProcessor);


}


MainThreadEventProcessor::MainThreadEventProcessor(QObject *parent)
   : QObject(parent)
{

}

MainThreadEventProcessor::~MainThreadEventProcessor(){

}

static QString GetToolConfigPath() {
	WCHAR tmp[1024 * 10];
	tmp[0] = 0;
	SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, tmp);
	wcscat_s(tmp, _MAX_PATH, L"\\VhallClass\\vhall_class_config.ini");
	return QString::fromWCharArray(tmp);
}

void MainThreadEventProcessor::customEvent(QEvent *event) {
	if (event == nullptr) {
		return;
	}
	int type = event->type();
	switch (type)
	{		
	case CustomEvent_InitUI: {
		TCHAR fullPath[MAX_PATH] = { 0 };
		char FilePath[MAX_PATH] = { 0 };

		const wchar_t *szPath = L"VhallRTC\\WebRtcSDK";
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, fullPath);
		//QString confPath = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
		QString confPath = GetToolConfigPath();
		QString domainURL = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_DOMAIN, defaultDomainURL);
		QString debugaddr = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_DEBUG_ADDR, "");
		globalToolManager->GetDataManager()->Init(fullPath);
		globalToolManager->SetClassSDK(GetClassSDKInstance());
		if (globalToolManager->GetClassSDK()) {
		//	globalToolManager->GetClassSDK()->InitSDK(app_key, app_secret, domainURL.toStdString(), globalToolManager, fullPath);
		    if (!debugaddr.isEmpty()) {
				globalToolManager->GetClassSDK()->SetDebugLogAddr(debugaddr.toStdString().c_str());
		    }
		}
			break;
		}
	case CustomEvent_DestoryWnd: {
		HandleDestoryWnd(event);
		break;
	}
	case CustomEvent_SafeToExitApp: {
		DestoryClassSDKInstance();
		globalToolManager->AppQuit();
		break;
	}
	case CustomEvent_TurnToLogin: {
		CBaseWnd* wnd1 = globalToolManager->GetWndManager()->FindWnd(WND_ID_LOGIN);
		wnd1->show();
		break;
	}
	default:
		break;
	}
}


void MainThreadEventProcessor::HandleDestoryWnd(QEvent* event) {
	if (event == nullptr) {
		return;
	}
	QEventDestoryWnd* destoryEvent = dynamic_cast<QEventDestoryWnd*>(event);
	if (destoryEvent) {
		globalToolManager->GetWndManager()->DestoryWnd(destoryEvent->mWndId);
	}
}