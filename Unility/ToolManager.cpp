#include "ToolManager.h"
#include <QCoreApplication>
#include "DataManager.h"
#include "ConfigManager.h"
#include "baseclass/pathmanager.h"
#include "ConfigSetting.h"
#include "CustomEventType.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <Shlobj.h>
#include <QApplication>
//#include "WebRtcLiveWdg.h"

using namespace vlive;
using namespace std;

ToolManager *ToolManager::mpInstance = nullptr;
std::mutex ToolManager::mInstanceMutex;

#define app_key     "207038a004fcc7012fec958ce992ef8a"
#define app_secret  "dda37cdf83997044539ad9c35ab87be1"

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
{
   if (mLibManager == nullptr) {
      mLibManager = new LibManager(this);
   }
   if (mWndManager == nullptr) {
      mWndManager = new WndManager;
   } 
   if (mDataManager == nullptr) {
       mDataManager = new DataManager;
   }
   if (mConfigManagerInterface == nullptr) {
       mConfigManagerInterface = new ConfigManager();
   }
   if (mpVHClassSDK == nullptr) {
       mpVHClassSDK = GetClassSDKInstance();
   }
}

ToolManager::~ToolManager()
{
   SaveRelease(mLibManager);
   SaveRelease(mWndManager);
   SaveRelease(mDataManager);
   SaveRelease(mConfigManagerInterface);
   DestoryClassSDKInstance();
}

ToolManager* ToolManager::GetInstance() {
   std::unique_lock<std::mutex> lock(mInstanceMutex);
   if (mpInstance == nullptr) {
      mpInstance = new ToolManager;  
   }
   return mpInstance;
}

void ToolManager::OnServiceMsg(std::string msgType, const std::wstring user_id, const std::wstring nickname, const std::string rolename, bool is_banned, int devType)
{
}

void ToolManager::CheckParam(int argc, char *argv[])
{
	GetDataManager()->WriteLog("%s startMode %d", __FUNCTION__, argc);

	if (argc == 1) { //桌面启动
		mIStartMode = eStartMode_desktop;
	}
	else if (argc == 2) //私有协议
	{
		mIStartMode = eStartMode_private;
		string cmd = argv[1];
		GetDataManager()->WriteLog("%s PrivateStartInfo %s", __FUNCTION__, argv[1]);

		QString strTemp = QString::fromStdString(cmd);
		int iPos =strTemp.indexOf('?');
		mPrivateStartInfo = strTemp.mid(iPos+1);
	}
   GetDataManager()->WriteLog("%s Leave", __FUNCTION__, argc);
	//return CRE_OK;
}

void ToolManager::Init() {

    TCHAR fullPath[MAX_PATH] = { 0 };
    char FilePath[MAX_PATH] = { 0 };

    const wchar_t *szPath = L"VhallRTC\\WebRtcSDK";
    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, fullPath);

    //QString confPath = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
	QString confPath = GetToolConfigPath();
	QString domainURL = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_DOMAIN, defaultDomainURL);
    QString debugaddr = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_DEBUG_ADDR, "");
    ToolManager::GetInstance()->GetDataManager()->Init(fullPath);
    if (mpVHClassSDK) {
        mpVHClassSDK->InitNetWorkProtocol(app_key, app_secret, domainURL.toStdString(), this, fullPath);
        if (!debugaddr.isEmpty()) {
            mpVHClassSDK->SetDebugLogAddr(debugaddr.toStdString().c_str());
        }
    }
}

QString ToolManager::GetConfigPath() {
	WCHAR tmp[1024 * 10];
	tmp[0] = 0;
	SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, tmp);
	wcscat_s(tmp, _MAX_PATH, L"\\VhallClass\\config.ini");
	return QString::fromWCharArray(tmp);
}

QString ToolManager::GetToolConfigPath() {
	WCHAR tmp[1024 * 10];
	tmp[0] = 0;
	SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, tmp);
	wcscat_s(tmp, _MAX_PATH, L"\\VhallClass\\vhall_class_config.ini");
	return QString::fromWCharArray(tmp);
}


void ToolManager::Release() {
    ToolManager::GetInstance()->GetWndManager()->DestoryAllWnd();
}

void ToolManager::RegisterQEventReciver(QObject* obj) {
    std::unique_lock<std::mutex> lock(mEventRecvMutex);
    bool bFind = false;
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        if (*iter == obj) {
            bFind = true;
            break;
        }
        iter++;
    }
    if (!bFind) {
        mEventReciver.push_back(obj);
    }
}

void ToolManager::RemoveQEventReciver(QObject* obj) {
    std::unique_lock<std::mutex> lock(mEventRecvMutex);
    bool bFind = false;
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        if (*iter == obj) {
            mEventReciver.erase(iter);
            break;
        }
        iter++;
    }
}

void ToolManager::PostEventToMainThread(QEvent* event) {
    std::list<QObject*>::iterator iter = mEventReciver.begin();
    while (iter != mEventReciver.end()) {
        QCoreApplication::postEvent(*iter, event);
        iter++;
    }
}

void ToolManager::PostEventToSelectWnd(QString wndId, QEvent* event) {
    if (mWndManager) {
        mWndManager->PostEventToWnd(wndId, event);
    }
}

void ToolManager::SafeToExitApp() {
    if (mWndManager) {
        mWndManager->DestoryAllWnd();
    }
    PostEventToMainThread(new QEvent(CustomEvent_SafeToExitApp));
}

void ToolManager::AppExec() {
    while (!mbExitApp) {
        mQuitLoop.exec();
    }
}
void ToolManager::AppQuit() {
    mbExitApp = true;
    mQuitLoop.quit();
    QCoreApplication::quit();
}

bool ToolManager::IsExist() {
    return mbExitApp;
}

void ToolManager::LogOut() {
    GetDataManager()->SetPublished(false);
    GetDataManager()->SetIsSpeaking(false);

    GetDataManager()->SetMuteCameraByTeacher(false);
    GetDataManager()->SetMuteMicByTeacher(false);
    GetClassSDK()->LeaveClassRoom();
    GetClassSDK()->ClearPassSdkInfo();
    GetWndManager()->DestoryAllWnd();
    PostEventToMainThread(new QEvent(CustomEvent_TurnToLogin));
}

void ToolManager::ReleaseInstance() {
   std::unique_lock<std::mutex> lock(mInstanceMutex);
   if (mpInstance != nullptr) {
      SaveRelease(mpInstance);
   }
}

int ToolManager::GetStartMode()
{
	return mIStartMode;
}
QString ToolManager::GetPrivateStartInfo()
{
	return mPrivateStartInfo;
}


LibManager* ToolManager::GetLibManager() {
   return mLibManager;
}

WndManager* ToolManager::GetWndManager() {
   return mWndManager;
}

DataManagerInterface* ToolManager::GetDataManager() {
   return mDataManager;
}

ConfigSettingInterface* ToolManager::GetConfigSettingInterface() {
    return mConfigManagerInterface;
}

VHClassRoomInterface* ToolManager::GetClassSDK() {

    return mpVHClassSDK;
}

void ToolManager::SetClassSDK(VHClassRoomInterface* class_sdk) {
	if (class_sdk) {
		mpVHClassSDK = class_sdk;
	}
}
    
void ToolManager::OnGetWebinarDetailInfo(int code, std::string respMsg) {
    PostEventToSelectWnd(WND_ID_LOGIN, new QEventRoomEvent(CustomEvent_GetWebinarDetailInfo, 0, "", 0, code, respMsg, ""));
}

void ToolManager::OnJoinClassRoom(int code, std::string respMsg, const int& iCurLabelNumber) {
      PostEventToSelectWnd(WND_ID_LOGIN, new QEventRoomEvent(CustomEvent_JoinClassRoom, 0, "", 0, code, respMsg, ""));
}

void ToolManager::OnInitQualityNumber(const int & iCurLabelNumber)
{
   if (iCurLabelNumber >= 0) {
      QString msConfPath = GetConfigPath();
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, iCurLabelNumber);
      GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iCurLabelNumber);
   }
}

void ToolManager::OnStartClass(int iPlayType, int code, std::string respMsg) {//300022
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartAndStopClass, 1, "", iPlayType, code, respMsg, ""));
}

void ToolManager::OnStopClass(int createDefaultRecord, int iPlayType, int code, std::string respMsg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartAndStopClass, 0, "", iPlayType, code, respMsg, ""));
}

void ToolManager::OnCallbackEvent(int event, int code /*= CALL_BACK_SUC*/, std::string msg/* = std::string()*/, std::string userData /*= std::string()*/) {
   QString id;
   int type;
   QJsonDocument doc = QJsonDocument::fromJson(QByteArray(userData.c_str(), userData.length()));
   if (doc.isObject()) {
      QJsonObject obj = doc.object();
      id = obj["process_id"].toString();
      type = obj["type"].toInt();
   }

   switch (event)
   {
   case RoomEvent_OnNetWork_Reconnecting:
      break;
   case RoomEvent_OnNetWork_Reconnect:
      break;
   case RoomEvent_OnNetWork_Close:
      PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_OnNetWork_Close, event, id, type, code, msg, userData));
      break;
   case RoomEvent_GetHandUpList:
   case RoomEvent_GetSilencedList:
   case RoomEvent_OnLineUserList:
   case RoomEvent_GetKickInavList:
      PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_GetVHMemberList, event, id, type, code, msg, userData));
      break;
   case RoomEvent_Start_MixStream:
      PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, event, id, type, code, msg, userData));
      break;
   case RoomEvent_Stop_PublishInavAnother:
      break;
   case RoomEvent_SetMainView:
      PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartSetMainStream, event, id, type, code, msg, userData));
      break;
   default:
      PostEventToMainThread(new QEventRoomEvent(CustomEvent_OnSuccessRoomEvent, event, id, type));
      break;
   }
}

/*收到申请上麦消息*/
void ToolManager::OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id, const std::wstring& nickname) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvApplyInavPublishMsg, QString::fromStdWString(third_party_user_id), QString::fromStdWString(nickname)));
}

void ToolManager::OnRecvBaseApplyInavPublishMsg(std::wstring & third_party_user_id)
{
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvApplyInavPublishMsg, QString::fromStdWString(third_party_user_id), QString::fromStdWString(third_party_user_id)));

}

void ToolManager::OnRoomConnectState(RoomConnectState code)
{
   std::string respMsg;
      switch (code)
      {
      case   RoomConnectState_Connected :     //房间链接成功
         respMsg = "The network is Connected";
         break;
      case     RoomConnectState_Reconnecting://网络正在重连
         respMsg = "The network is Reconnecting";
         break;
      case     RoomConnectState_DisConnect: //网络链接已断开 
         respMsg = "The network is DisConnect";
         break;
      case     RoomConnectState_NetWorkDisConnect: //网络被关闭 
         respMsg = "The network is shut down";
         break;
      default:
         break;
      }

   OnJoinClassRoom(code, respMsg);
}

//void ToolManager::OnRecvAuditInavPublishMsg(const std::wstring & third_party_user_id, AuditPublish PublishType)
//{
//   OnRecvAuditInavPublishMsg(third_party_user_id, PublishType)
//}

void ToolManager::OnRecvFlahsMsg(std::string flashMsg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvFlashMsg, QString::fromStdString(flashMsg),""));
}

void ToolManager::OnSignUsersMsg(std::list<std::string> stlistsignUsers, std::list<std::string> stlistNoSignUsers) {
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventSignUsers(CustomEvent_RecvSignUsers, stlistsignUsers, stlistNoSignUsers));
}


void ToolManager::OnRecvSetBrushMsg(std::string data, std::string msg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvSetBrush, QString::fromStdString(data), QString::fromStdString(msg)));
}

void ToolManager::OnRecvDocUploadMsg(std::string msg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvUploadDoc, QString::fromStdString(msg), ""));
}

void ToolManager::OnRemoteStreamAdd(const std::string& user_id, const std::string& stream_id, vlive::VHStreamType streamType) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RemoteStreamAdd, QString::fromStdString(user_id), QString::fromStdString(stream_id), streamType));
}

void ToolManager::OnSubScribeStreamErr(const std::string& user_id, const std::string& msg, int errorCode) {

}

void ToolManager::OnRecvMediaMsg(std::string mediaMsg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvMediaMsg, QString::fromStdString(mediaMsg), ""));
}

void ToolManager::OnRecvAnswerMsg(std::string msg) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_RecvAnswerMsg, QString::fromStdString(msg), ""));
}

void ToolManager::OnRecvForbiddenChat(int forbid, std::string user_id, std::string data) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvForbiddenChatMsg, QString::fromStdString(user_id), QString::fromStdString(data), forbid) );
}

void ToolManager::OnRecvForbiddenAllChat(int forbid, std::string data) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvForbiddenAllChatMsg, "", QString::fromStdString(data), forbid));
}

void ToolManager::OnRecvUpdatePublishAbility(const std::string& join_uid, int statue) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvUpdatePublishAbility, QString::fromStdString(join_uid), "", statue));
}

/*收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作*/
void ToolManager::OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish event) {
   
	if ( QString::fromStdString(GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id   ).compare(QString::fromStdWString(third_party_user_id))==0 )
		PostEventToMainThread(new QEventVHMember(CustomEvent_OnRecvAuditInavPublishMsg, QString::fromStdWString(third_party_user_id), "", event));
}

void ToolManager::OnRecvKickInavMsg(const std::wstring & third_party_user_id)
{
   OnRecvKickOutRoomMsg(QString::fromWCharArray( third_party_user_id.c_str()).toStdString(), 1);
}

void ToolManager::OnUserPublishCallback(const std::wstring & third_party_user_id, const std::string & stream_id, PushStreamEvent event)
{
   //*type	int	否	1 上麦(默认) 2 下麦 3 拒绝上麦

   ClassPushStreamEvent streamEvent = PushStreamEvent_UnPublish;
      switch (event)
      {
      case PushStreamEvent_Upper: streamEvent = PushStreamEvent_Publish; break;
      case  PushStreamEvent_Lower: streamEvent = PushStreamEvent_UnPublish; break;
      case  PushStreamEvent::PushStreamEvent_Refused: streamEvent = ClassPushStreamEvent::PushStreamEvent_Refused; break;
      default:
         break;
      }
   std::string thirdId = QString::fromWCharArray(third_party_user_id.c_str()).toStdString();
   OnRecvUserPublishCallback(thirdId, thirdId, streamEvent, JOIN_ROLE_Student, 0, 0, 0);


}

/*邀请上麦消息*/
void ToolManager::OnRecvAskforInavPublishMsg(const std::wstring& join_uid, const std::wstring& target_uid, int status) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEventRecvMsg(CustomEvent_OnRecvAskforInavPublishMsg, QString::fromStdWString(join_uid), QString::fromStdWString(target_uid), status));
}

void ToolManager::OnSuccessedEvent(RoomEvent code, std::string userData)
{
   OnJoinClassRoom(code, userData);
}

void ToolManager::OnFailedEvent(RoomEvent code, int respCode, const std::string & msg, std::string userData)
{
   RewardError(respCode, msg);
}

void ToolManager::OnGetVHRoomMembers(const std::string & third_party_user_id, std::list<VHRoomMember>& members)
{

   ClassOnLineUserInfo onLineUserInfo;
   onLineUserInfo.total = members.size();
   onLineUserInfo.total_page = 1;
   onLineUserInfo.curr_page = 1;

   list<VHRoomMember>::iterator it;
   for (it = members.begin(); it != members.end(); it++)
   {
      VHRoomMember tem = *it;
      ClassMember member;
      //member.status;            // 在线状态，默认为0不在线，1为在线
      //member.role_type;
      //member.no_mic;           //是否具有上麦能力  0 有能力。 1 无能力
      //member.is_joinmic; //1  是已经上麦的 (连麦中) 			0  举手未上麦的（举手）
      member.user_id =  QString::fromWCharArray( tem.thirdPartyUserId.c_str()).toStdString();  //用户ID
      member.nick_name = member.user_id;
      onLineUserInfo.curr_page_users.push_back(member);
   }


   OnGetOnLineUserRoomMembers(onLineUserInfo);

}

void ToolManager::OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&members)
{
   ClassOnLineUserInfo onLineUserInfo;
   onLineUserInfo.total = members.size();
   onLineUserInfo.total_page = 1;
   onLineUserInfo.curr_page = 1;

   list<VHRoomMember>::iterator it;
   for (it = members.begin(); it != members.end(); it++)
   {
      VHRoomMember tem = *it;
      ClassMember member;
      //member.status;            // 在线状态，默认为0不在线，1为在线
      //member.role_type;
      //member.no_mic;           //是否具有上麦能力  0 有能力。 1 无能力
      //member.is_joinmic; //1  是已经上麦的 (连麦中) 			0  举手未上麦的（举手）
      member.user_id = QString::fromWCharArray(tem.thirdPartyUserId.c_str()).toStdString();  //用户ID
      member.nick_name = member.user_id;
      onLineUserInfo.curr_page_users.push_back(member);
   }


   OnGetKickOutRoomMembers(onLineUserInfo);
}

void ToolManager::OnRecvBaseAskforInavPublishMsg(const std::wstring & third_party_user_id)
{
   OnRecvAskforInavPublishMsg(third_party_user_id, L"", 1);
}


/*踢出流消息  消息为广播消息，收到消息后，执行踢出流*/
void ToolManager::OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) {
    PostEventToMainThread(new QEventRecvMsg(CustomEvent_OnRecvKickInavStreamMsg, QString::fromStdWString(third_party_user_id)));
}

/*踢出互动房间 , 消息为广播消息，收到消息后，判断是当前用户后，执行踢出房间操作*/
void ToolManager::OnRecvKickOutRoomMsg(const std::string& third_party_user_id,int status) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRecvMsg(CustomEvent_OnRecvKickOutRoomMsg, QString::fromStdString(third_party_user_id), QString::number(status)));
}

void ToolManager::OnRecvOpenHandMsg(int status) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_OnRecvOpenHandUpMsg, QString::number(status)));
}

/*上/下/拒绝上麦消息 消息为广播消息*/
void ToolManager::OnRecvUserPublishCallback(const std::string& third_party_user_id, const std::string& nickname, vlive::ClassPushStreamEvent event, 
   int role_type, int device_type, int iRewardNum, const int& iNoDoubleVrtc){
    QEventUserPublish *eventMsg = new QEventUserPublish(CustomEvent_OnUserPublishCallback, QString::fromStdString(third_party_user_id), nickname, iRewardNum, event);
    eventMsg->role_type = role_type;
    eventMsg->device_type = device_type;
	eventMsg->miRewardNum = iRewardNum;
   eventMsg->miNoDoubleVrtc = iNoDoubleVrtc;
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, eventMsg);
}

/*互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间*/
void ToolManager::OnRecvInavCloseMsg() {
    PostEventToMainThread(new QEvent(CustomEvent_OnRecvInavCloseMsg));
}

void ToolManager::OnUserOnLineState(bool online, const std::string user_id, int role_type) {
    //PostEventToMainThread(new QEventVHMember(CustomEvent_User_OnLine, QString::fromStdString(user_id), role_type));
	//不在此处处理，在聊天socketIo中进行接收处理
}

void ToolManager::OnChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHSimulCastType type, int code, std::string msg) {
	//不需要做什么处理，暂不处理
}

void ToolManager::OnRecvPublishFailedMsg(const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type) {
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventVHPublishFailed(CustomEvent_RecvPublishFailed, join_uid, status, nick_name, role_type, app_type));
}

void  ToolManager::OnRecvHandDelayMsg(const std::string& target_id)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventVHPublishFailed(CustomEvent_RecvHandDelay, target_id, 0, "", 0, 0));
}

void ToolManager::OnRecvStopDesktopSharing()
{
   //PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_EndDoubleVrtcCallback, QString::fromStdString(target_id), QString::fromStdString(nickName)));
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_EndDesktopSharingCallback, "", ""));

}

void  ToolManager::OnRecvStartSign(const std::string&strWebinar, const std::string& sign_id, const int& signShowTime)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartSign(CustomEvent_StartSign, sign_id, signShowTime, strWebinar));
}

void  ToolManager::OnRecvSignIn(const std::string& strWebinar, const std::string& sign_id, const std::string& signer, const std::string& signerNickname)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventSignIn(CustomEvent_SignIn, strWebinar, sign_id, signer, signerNickname));

}

void  ToolManager::OnRecvSignStop(const std::string& strWebinar, const std::string& sign_id)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventSignStop(CustomEvent_SignStop, strWebinar, sign_id));
}

void  ToolManager::OnRecvStartCountdown(const std::string& id, const int& iplayEndSound, const int& iTime)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartCountdown(CustomEvent_StartCountdown, QString::fromStdString(id), iplayEndSound,iTime));
}

void  ToolManager::OnRecvCancelCountdown(const std::string& id)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartCountdown(CustomEvent_StartCountdown, QString::fromStdString(id)));
}

void  ToolManager::OnRecvAnswerToolMsg(const std::string& msg)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartAnswerTool(CustomEvent_StartAnswerTool, msg));
}

void  ToolManager::OnRecvAnswerDetailMsg(int code, const std::string& msg)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartAnswerDetail(CustomEvent_AnswerDetail, msg, code));
}

void  ToolManager::OnRecvStartAnswerMsg(int code, const std::string& msg)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStartAnswer(CustomEvent_StartAnswer, msg, code));
}

void  ToolManager::OnRecvCancelHand(std::string strJoinId, int iRoleType, std::string strNickName, std::string strTime)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventVHPublishFailed(CustomEvent_RecvCancelHand, strJoinId, 0, strNickName, iRoleType, 0) );
}

void ToolManager::OnRecvHandRewardStu(std::string target_id, std::string nickName, const int& iFavorNum)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventVHMember(CustomEvent_RewardStu, QString::fromStdString(target_id), QString::fromStdString(nickName),iFavorNum));
}

void ToolManager::OnRecvMuteMicMsg(const std::string& join_uid, int status) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_RecvMuteMic, 0, QString::fromStdString(join_uid), status, 200, "", QString::number(status).toStdString()));
}

void ToolManager::OnRecvDownAllUserPublishMsg() {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_OnRecvDownAllUserPublishMsg));
}

void ToolManager::OnRecvAllMuteMicMsg(int status) {
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_RecvAllMuteMic, 0, QString(""), 0, 200, "", QString::number(status).toStdString()));
}

void ToolManager::OnRecvMuteVideoMsg(const std::string& join_uid, int status) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_RecvMuteVideo, 0, QString::fromStdString(join_uid), status, 200, "", QString::number(status).toStdString()));
}

void ToolManager::OnRemoveRemoteUserLiveStream(const std::wstring & user, const std::string & stream, const vlive::VHStreamType type)
{
}

void ToolManager::OnStreamMixed(const std::string & user, const std::string & stream, int type)
{
}

void ToolManager::OnRecvStartDoubleVrtc(const std::string& join_uid, const std::string& nickName)
{
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_StartDoubleVrtcCallback,  QString::fromStdString(join_uid), QString::fromStdString(nickName)));
}

void ToolManager::OnRecvEndDoubleVrtc(const std::string& target_id, const std::string& nickName)
{
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_EndDoubleVrtcCallback, QString::fromStdString(target_id),  QString::fromStdString(nickName)));
}

void ToolManager::OnRecvExamination()
{
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new ExaminationEvent(CustomEvent_Examination) );
}

void  ToolManager::OnRecvWhiteListUserJoinRoom(const std::string& phone, const std::string& class_token)
{
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new WhiteListUserJoinRoom(CustomEvent_WhiteListUserJoinRoom, QString::fromStdString(phone), QString::fromStdString(class_token)));
}


void ToolManager::OnRecvSetBigScreen(const std::string& target_id) {
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ChangeBigScreen, QString::fromStdString(target_id)));
}

void ToolManager::OnRecvInviteDoubleVrtc(std::string target_id)
{
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_inviteDoubleVrtc, QString::fromStdString(target_id), ""));

}

void ToolManager::OnRecvInviteScreenSharing(std::string target_id)
{
   //if (nullptr != mpVHClassSDK && mpVHClassSDK->GetClassRoomBaseInfo().mUserInfo.join_id == target_id) {
      PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUserEvent(CustomEvent_InviteScreenSharing, QString::fromStdString(target_id), ""));
   //}
}


/*房间链接事件*/
 void ToolManager::OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum type, const std::string& joinId) {
     switch (type)
     {
     case VHRoomConnect_ROOM_CONNECTED:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEvent(CustomEvent_WebRtcRoomConnetSuc));
         break;
     case VHRoomConnect_ROOM_ERROR:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomConnetErr));
         break;
     case VHRoomConnect_ROOM_RECONNECTING:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomReConneting));
         break;
     case VHRoomConnect_RE_CONNECTED:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomReConneted));
         break;
     case VHRoomConnect_ROOM_DISCONNECTED:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomDisConnected));
         break;
     case VHRoomConnect_ROOM_RECOVER:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomRecover));
         break;
     case VHRoomConnect_ROOM_NETWORKCHANGED:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomNetWorkChanged));
         break;
     case VHRoomConnect_ROOM_NETWORKRECOVER:
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_WebRtcRoomNetWorkChangedCover));
         break;
     case VHRoomConnect_ROOM_MIXED_STREAM_READY: 
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventVHMember(CustomEvent_WebRtcRoomMixedReady, QString::fromStdString(joinId), ""));
         break;
     default:
         break;
     }
 }

/*本地打开采集设备,包括摄像头、桌面共享、开始插播视频回调*/
 void ToolManager::OnOpenCaptureCallback(vlive::VHStreamType streamType, vlive::VHCapture code, bool hasVideo, bool hasAudio) {
	 if (nullptr != mWndManager){
         if (streamType == VHStreamType_Preview_Video) {
			 //PostEventToSelectWnd(WND_ID_WEBRTC_SETTING, new QEventStream(CustomEvent_PreOnCaptureCallback, "", "", streamType, code, hasVideo, hasAudio));
			 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_PreOnCaptureCallback, "", "", streamType, code, hasVideo, hasAudio));
			 //PostEventToSelectWnd(WND_ID_DECICE_TESTING, new QEventStream(CustomEvent_PreOnCaptureCallback, "", "", streamType, code, hasVideo, hasAudio));
         }
         //else if (streamType == VHStreamType_Auxiliary_CamerCapture) {
         //    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_Preview_Auxiliary_Camera, "", "", streamType, code, hasVideo, hasAudio));
         //}
         else {
             PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_OnCaptureCallback, "", "", streamType, code, hasVideo, hasAudio));
         }
	 }
	 
 }
/*推流成功回调*/
 void ToolManager::OnPushStreamSuc(VHStreamType streamType, std::string& streamid, bool hasVideo, bool hasAudio, std::string& attributes) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_PushLocalStreamSuc, "", streamid, streamType, 0, hasVideo, hasAudio, attributes));
 }
/*推流失败回调*/
 void ToolManager::OnPushStreamError(std::string streamId, VHStreamType streamType, const int codeErr, const std::string& msg ) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStreamError(CustomEvent_PushLocalStreamError, streamId, streamType, codeErr, msg));
 }
/*停止推流回调*/
 void ToolManager::OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg) {
     if (vlive::VhallLive_OK == code){
         GetDataManager()->WriteLog("%s :succeced streamType=%d msg=%s", __FUNCTION__, streamType, msg.c_str());
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_StopPushLocalStreamSuc, "", "", streamType, VHCapture_OK, true, true));
     }
     else{
         GetDataManager()->WriteLog("%s :faild streamType=%d msg=%s", __FUNCTION__, streamType, msg.c_str());
         PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_StopPushLocalStreamErr, "", "", streamType, VHCapture_OK, true, true));
     }
 }

/*接收到远端的媒体流  hasVideo ：是否包含视频*/
 void ToolManager::OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_SubScribSuc, QString::fromStdWString(user), stream, type, 0, hasVideo, hasAudio));
 }

/*远端的媒体流退出了*/
 void ToolManager::OnRemoteUserLiveStreamRemoved(const std::wstring& user, const std::string& stream, const VHStreamType type) {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventStream(CustomEvent_STREAM_REMOVED, QString::fromStdWString(user), stream, type, 0, true, true));
 }

 void ToolManager::OnGetOnLineUserRoomMembers(const vlive::ClassOnLineUserInfo &users) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventOnLineList(CustomEvent_User_OnLineList, users));
 }

 void ToolManager::OnGetKickOutRoomMembers(const vlive::ClassOnLineUserInfo& users) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventOnLineList(CustomEvent_User_KickOutList, users));
 }

 void ToolManager::OnGetSilencedMembers(const vlive::ClassOnLineUserInfo& users) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventOnLineList(CustomEvent_User_ForbiddenList, users));
 }

void ToolManager::OnGetHandUpsMembers(const vlive::ClassOnLineUserInfo& users) {
	PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventOnLineList(CustomEvent_User_HandUpList, users));
}

void ToolManager::OnGetDoubleVrtcUsers(std::list<ClassDoubleVrtcUsers> users) {
   PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new DoubleVrtcUsersEvent(CustomEvent_DoubleVrtUsersList, users));
}

void ToolManager::OnOpenHandsUp(bool open, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_OpenHandsUp, 0, "", 0, code, respMsg, open  == true ? "1" : "0"));
}

 void ToolManager::OnAuditPublish(std::string uid, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendAgreePublishMsg, 0, QString::fromStdString(uid), 0, code, respMsg, ""));
 }

 void ToolManager::OnKickUserPublish(const std::string& kick_user_id, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendKickPublishMsg, 0, QString::fromStdString(kick_user_id), 0, code, respMsg, ""));
 }

 void ToolManager::OnKickAllUnPublish(int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_AllUnPublish, 0, "", 0, code, respMsg, ""));
 }
                                 
 void ToolManager::OnAskforPublish(const std::string& audit_user_id, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendInvitePublishMsg, 0, QString::fromStdString(audit_user_id), 0, code, respMsg, ""));
 }

 void ToolManager::OnApplyPublish(int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendApplyPublish, 0, "", 0, code, respMsg, ""));
 }

 void ToolManager::OnCheckUserIsPublish(const std::string& user_id, int code, std::string respMsg) {
   if (USER_NOT_PUBLISH == code) {
      GetDataManager()->WriteLog("%s KickUserPublish %s", __FUNCTION__, user_id.c_str());
      GetClassSDK()->KickUserPublish(user_id);
   }
 }

 void ToolManager::RewardError(int code, std::string respMsg)
 {
	 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_RewardError, 0, "", 0, code, respMsg, ""));
 }

 void ToolManager::CanclePublish(int code, std::string respMsg)
 {
	 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_CanclePublish, 0, "", 0, code, respMsg, ""));
 }

 void ToolManager::OnKickOutClassRoom(const std::string& kick_user_id, int type, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_KickOut_Room, 0, QString::fromStdString(kick_user_id), type, code, respMsg, QString::number(type).toStdString()));
 }

 void ToolManager::OnChangeUserPenAuth(std::string userId, bool auth, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendSetBrush, 0, QString::fromStdString(userId), 0, code, respMsg, auth == true ? "1" : "0"));
 }

 void ToolManager::OnForbiddenChat(std::string userId, bool forbid, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendForbiddenChat, 0, QString::fromStdString(userId), 0, code, respMsg, forbid == true ? "1" : "0"));
 }

 void ToolManager::OnForbiddenChatAll(bool forbid, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_SendForbiddenAllChat, 0, "", 0, code, respMsg, forbid == true ? "1" : "0"));
 }                                                                                                                                              

 void ToolManager::OnOperateUserMic(std::string userId, bool close, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_MuteMic, 0, 
		 QString::fromStdString(userId), 0, code, respMsg,  close == true ? "1" :"0"));
 }

 void ToolManager::OnOperateAllUserMic(int mute, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_MuteMicAll, 0, "", 0, code, respMsg, QString::number(mute).toStdString()));
 }

 void ToolManager::OnOperateUserCamera(std::string userId, bool close, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_MuteVideo, 0, QString::fromStdString(userId), 0, code, respMsg, close == true ? "1" : "0"));
 }

 void ToolManager::OnUserPublishCallback(vlive::ClassPushStreamEvent type, std::string uid, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_UserPublishCallback, 0, QString::fromStdString(uid), 0, code, respMsg, QString::number(type).toStdString()));
 }

 void ToolManager::OnUserPublishFailedCallback(vlive::ClassPushStreamEvent type, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_UserPublishFailedCallback, 0, "", 0, code, respMsg, QString::number(type).toStdString()));
 }

 void ToolManager::OnUpdatePublishAbility(int enablePublish, int enablePublishDouble, int code, std::string respMsg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_UpdatePublishAbility, 0, "", 0, code, respMsg, QString::number(enablePublish).toStdString()));
 }

 ///////////////////////////////////////////////////////////////////////////////////////////////
 void ToolManager::OnSendMicsDeviceCallback(int code, std::string respMsg)
 {
    //RewardError(code, respMsg);
 }


 void ToolManager::OnStopDoubleVrtcCallback(int code, std::string respMsg) {
	 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventChangeUser(CustomEvent_InviteDoubleVrtcCallback, respMsg, code));
 }


 void ToolManager::OnInviteDoubleVrtcCallback(int code, std::string respMsg)
 {
    RewardError(code, respMsg);
 }

 void ToolManager::OnStartDoubleVrtcCallback(int code, std::string respMsg)
 {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventChangeUser(CustomEvent_RequestStartDoubleVrtcCallback, respMsg, code));
 }

 void ToolManager::OnDoubleVrtcUsersCallback(int code, std::string respMsg)
 {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_DoubleVrtUsersCallback, 0, "", 0, code, respMsg));
 }

 void ToolManager::PostDestoryTestPaperDlg()
 {
	 ToolManager::GetInstance()->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, CHOOSE_TEST_PAPER_DLG));//1994
	 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_CloseTest, ""));

 }

 ///////////////////////////////////////////////////////////////////////////////////////////////

 void ToolManager::OnChangeToToolPage(vlive::ToolPage pageIndex, int code, std::string respMsg) {
	 PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomChangeToolPage(CustomEvent_ChangeToToolPage, pageIndex));
 }

 void ToolManager::OnSetConfigBroadEvent(int layoutType, int nCode, const std::string &msg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, nCode, "", layoutType, nCode, msg, ""));
 }

 void ToolManager::OnChangeRrlayType()
 {
    PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ChangeRrlayType, ""));
 }

 void ToolManager::OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_LocalStreamIdChanged,0,"", streamType,0, old_streamid, new_streamid));
 }

 void ToolManager::OnRecvNoticeMsg(const std::string msg) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowNoticeMsg, QString::fromStdString(msg)));
 }

 void ToolManager::OnRecvStartClassMsg(const std::wstring& join_uid, const std::wstring& nickname, const int& iCurLabelNumber, const int& iStartPlayType) {
    QString msConfPath = GetConfigPath();
    GetDataManager()->WriteLog("%s iQualityId %d", __FUNCTION__, iCurLabelNumber);
    ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, iCurLabelNumber);

     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartAndStopClass, 
        1, 
        QString::fromStdWString(join_uid),
        iStartPlayType,200, QString::fromStdWString(nickname).toStdString()));
 }

 void ToolManager::OnRecvStopClassMsg(int iPlayType) {
     PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartAndStopClass, 0, "", iPlayType, 200));
 }

 //void ToolManager::GetVideoProfileByConfig(QString def, int& videoDef, int& layoutDef) {
 //    int interactiveVideoProfile = 360;
 //    int mixLayoutProfile = 720;
 //    if (mpVHClassSDK) {
 //        ClassRoomBaseInfo baseInfo = mpVHClassSDK->GetClassRoomBaseInfo();
 //        std::list<UserSettings>::iterator iter = baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.begin();
 //        while (iter != baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.end()) {
 //            if (def.toStdString() == iter->desc) {
 //                interactiveVideoProfile = iter->video_profile;
 //                mixLayoutProfile = iter->broadcast_video_profile;
 //                break;
 //            }
 //            iter++;
 //        }
 //    }
 //    videoDef = interactiveVideoProfile;
 //    layoutDef = mixLayoutProfile;
 //}

 void ToolManager::GetVideoProfileByConfig(int labelNumber, int& videoDef, const eVideoProFileType & eType)
 {
    add_log("Test.text", true, "********start*************");
    if (mpVHClassSDK) {
       ClassRoomBaseInfo baseInfo = mpVHClassSDK->GetClassRoomBaseInfo();
       std::list<UserSettings>::iterator iter = baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.begin();
       while (iter != baseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.end()) {
          if (labelNumber == iter->label_number) {
             char* temp = new char[20];
             memset(temp, 0, 20);
             itoa(labelNumber, temp, 10);

             add_log("Test.text", false, temp);
             add_log("Test.text", false,  iter->desc.c_str());
            // char* temp = new char[20];
             memset(temp, 0, 20);
             itoa(eType, temp, 10);
             add_log("Test.text", false, temp);

             switch (eType)
             {
             case eVideoProFileTypeVideo:                     //主画面分辨率
                if (JOIN_ROLE_Teacher == GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type) {
                   videoDef = iter->video_profile;
                   add_log("Test.text", false, "主画面");
                }
                else {
                   videoDef = iter->Interaction_profile;
                   add_log("Test.text", false, "互动");
                }
                break;
            case  eVideoProFileTypeBroadcast:             //旁路画面分辨率
               videoDef = iter->broadcast_video_profile;
               break;
            case eVideoProFileTypeInteraction:           //互动画面分辨率
               videoDef = iter->Interaction_profile;
               break;
            case eVideoProFileTypeDoublePush:           //双推画面分辨率
               videoDef = iter->DoublePush_profile;
               break;
            case eVideoProFileTypeDesktop:                //桌面共享画面分辨率
               videoDef = iter->Desktop_profile;
               break;
             default:
                break;
             }
          }
          iter++;
       }
    }

    add_log("Test.text", true, "###########END###########");
 }

 void add_log(const char *logfile, bool bN , const char *fmt, ...)
 {
    //int fd;
    //va_list ap;
    //char msg[1024];
    //struct tm *nowp, now;
    //ofstream ofs;  //提供写文件的功能


    ////char tmpFile[100];
    ////memset(tmpFile, 0, 100);
    ////strftime(tmpFile, sizeof(tmpFile), "%Y%m%d", localtime(&t));
    ////strcat(tmpFile, logfile);
    ////strcat(tmpFile, ".log");
    //ofs.open(logfile, ios::app);

    //if (bN) {
    //   time_t t = time(0);
    //   char tmp[64];
    //   memset(tmp, 0, 64);
    //   strftime(tmp, sizeof(tmp), "%Y/%m/%d %X ", localtime(&t));

    //   ofs << tmp;
    //}
    //

    //memset(msg, 0, sizeof(msg));
    //va_start(ap, fmt);
    //vsnprintf(msg, sizeof(msg), fmt, ap);
    //va_end(ap);

    //if (bN) {
    //   ofs << msg << "        \n";
    //}
    //else
    //{
    //   ofs << msg << "        ";
    //}

    //ofs.close();
 }