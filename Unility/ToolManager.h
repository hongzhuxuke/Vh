#pragma once

#include <QObject>
#include <mutex>  
#include <QEvent>
#include <QEventLoop>
#include "WndManager.h"
#include "LibManager.h"
#include "DataManagerInterface.h"
#include "ConfigManagerInterface.h"
#include "VHClassRoomInterface.h"
#include "class_callback_monitor.h"
#include <mutex>

#ifndef MAX_NICKNAME_LEN
#define MAX_NICKNAME_LEN  20
#endif

#include <fstream>
#include <time.h>

using namespace std;
void add_log(const char *logfile, bool bN, const char *fmt, ...);

using namespace vlive;
#define SaveRelease(a)\
if(a){            \
   delete a;      \
   a = nullptr;   \
}                 \

class ToolManager : public QObject,public VHRoomMonitor
{
    Q_OBJECT

public:
    static ToolManager* GetInstance();
    static void ReleaseInstance();
	int GetStartMode();
	QString GetPrivateStartInfo();

	//启动方式
	enum eStartMode
	{
		eStartMode_desktop = 0,	//桌面启动
		eStartMode_private			//私有协议
	};

	//观看限制
	enum eLoginMode
	{
		eLoginMode_Command = 0,	//口令登录
		eLoginMode_Free,         //免费
		eLoginMode_WhiteList		//白名单
	};
   //PAAS SDK 
   //********************************************************   接口调用回调成功与失败通知   ********************************************************//
/*
*   监听房间内业务调用成功事件
*   code: RoomEvent 事件类型
*   userData: 附带的json数据
*/
   virtual void OnSuccessedEvent(RoomEvent code, std::string userData = std::string());
   /*
*   监听互动房间内HTTP业务API调用失败事件
*   respCode: 错误码
*   msg：错误信息
*   userData: 附带的json数据
*/
   virtual void OnFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string());
   /*
*   获取互动房间成员列表回调
**/
   virtual void OnGetVHRoomMembers(const std::string& third_party_user_id, std::list<VHRoomMember>&);
   
   /*
*   获取被踢出互动房间人列表回调
**/
   virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&);
   /*
*   邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息
*/
   void OnRecvBaseAskforInavPublishMsg(const std::wstring& third_party_user_id);
   //********************************************************   广播消息监听事件   ********************************************************//
/*
 *   收到申请上麦消息 ,消息为广播消息，收到消息后，通过进入互动房间时获取的权限列表，判断用户是否有审核权限
 */
   void OnRecvBaseApplyInavPublishMsg(std::wstring& third_party_user_id);
   /*
*   房间网络状态连接
*/
   virtual void OnRoomConnectState(RoomConnectState) ;
//   /*
//*   收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作
//*/
//   virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
   /*
   *   踢出互动房间 , 消息为广播消息，收到消息后，判断是当前用户后，执行踢出房间操作
   */
   virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id) ;

   /*
   *   上/下/拒绝上麦消息 消息为广播消息
   */
   virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event);

   /*
   *   用户上下线通知
   *   online： 用户上线/ 用户下线 / 禁言 / 取消禁言
   *   user_id： 用户id
   */
   virtual void OnServiceMsg(std::string msgType, const std::wstring user_id, const std::wstring nickname, const std::string rolename, bool is_banned, int devType);
   //PAAS SDK        **************************** END  



	void CheckParam(int argc, char *argv[]);
    void Init();
    void Release();
    void RegisterQEventReciver(QObject* obj);
    void RemoveQEventReciver(QObject* obj);
    void PostEventToMainThread(QEvent* event);
    void PostEventToSelectWnd(QString wndId, QEvent* event);
    //退出应用
    void SafeToExitApp();
    //退出到登陆页面
    void LogOut();
	QString GetConfigPath();
	QString GetToolConfigPath();
    /*本地网络重连成功之后重推流，流ID发生改变通知**/
    virtual void OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid);
    /*收到申请上麦消息*/
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id, const std::wstring& nickname) override;
    virtual void OnRecvFlahsMsg(std::string flashMsg);
    virtual void OnRecvSetBrushMsg(std::string data,std::string msg);
    virtual void OnRecvDocUploadMsg(std::string msg);

    /**
*   接收到远端推流
*/
    virtual void OnRemoteStreamAdd(const std::string& user_id, const std::string& stream_id, vlive::VHStreamType streamType);
    /**
*   订阅流失败。
*/
    virtual void OnSubScribeStreamErr(const std::string& user_id, const std::string& msg, int errorCode);
    /****/
    virtual void OnRecvMediaMsg(std::string mediaMsg);

    virtual void OnRecvAnswerMsg(std::string msg);
    
    /*禁言和取消禁言*/
    virtual void OnRecvForbiddenChat(int forbid, std::string user_id, std::string data);
    /*禁言和取消禁言*/
    void OnRecvForbiddenAllChat(int forbid, std::string data);
    /*更新上麦能力*/
    void OnRecvUpdatePublishAbility(const std::string& join_uid, int statue);
    /*收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作*/
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
    /*邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息*/
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& join_uid, const std::wstring& target_uid, int status) override;
    /*踢出流消息  消息为广播消息，收到消息后，执行踢出流*/
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id);
    /*踢出互动房间*/
    virtual void OnRecvKickOutRoomMsg(const std::string& third_party_user_id,int status);
    /*
    *  接收到举手开关消息
    *  status 状态（1开、0关）
    */
    virtual void OnRecvOpenHandMsg(int status);
    /*上/下/拒绝上麦消息 消息为广播消息*/
    virtual void OnRecvUserPublishCallback(const std::string& third_party_user_id, const std::string& nickname, 
       vlive::ClassPushStreamEvent event, int role_type, int device_type, int iRewardNum, const int& iNoDoubleVrtc);

    /*互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间*/
    virtual void OnRecvInavCloseMsg();
    /* 用户上下线通知*/
    virtual void OnUserOnLineState(bool online, const std::string user_id, int role_type);
    /* 收到上麦 下麦失败消息*/
    virtual void OnRecvPublishFailedMsg(const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type);
	/*
	*   接收到举手超时消息
	*/
	virtual void  OnRecvHandDelayMsg(const std::string& target_id);
   /*
   *   接收到结束桌面共享消息
   */
   virtual void OnRecvStopDesktopSharing();
	/*收到开始签到消息*/
	virtual void  OnRecvStartSign(const std::string&strWebinar, const std::string& sign_id, const int& signShowTime);
	virtual void  OnRecvSignIn(const std::string& strWebinar, const std::string& sign_id, const std::string& signer, const std::string& signerNickname);
	virtual void OnSignUsersMsg(std::list<std::string> stlistsignUsers, std::list<std::string> stlistNoSignUsers);
	virtual void  OnRecvSignStop(const std::string& strWebinar, const std::string& sign_id);
	//主讲人发起倒计时 、 主讲人取消倒计时 
	virtual void  OnRecvStartCountdown(const std::string& id, const int& iplayEndSound, const int& iTime);
	virtual void  OnRecvCancelCountdown(const std::string& id);
	/*收到开始答题器消息*/
	virtual void  OnRecvAnswerToolMsg(const std::string& msg);
	virtual void  OnRecvAnswerDetailMsg(int code, const std::string& msg);
	virtual void  OnRecvStartAnswerMsg(int code, const std::string& msg);
	/*
	*   接收取消举手消息
	*/
	virtual void  OnRecvCancelHand(std::string strJoinId, int iRoleType, std::string strNickName, std::string strTime);
	/*
	*   接收奖杯数量增加消息
	*/
	virtual void OnRecvHandRewardStu(std::string target_id, std::string nickName,const int& iFavorNum);
	/*
    *  接收到静音消息
    *  status 状态（1静音、0取消静音）
    */
    virtual void OnRecvMuteMicMsg(const std::string& join_uid, int status);
    /*接收下麦所有用户消息*/
    virtual void OnRecvDownAllUserPublishMsg();
    /**
    *   接收到全员静音消息
    *   status 状态（1静音、0取消静音）
    */
    virtual void OnRecvAllMuteMicMsg(int status) ;
    /*
    *  接收到禁画消息
    *  status 状态（1禁画、0取消禁画）
    */
    virtual void OnRecvMuteVideoMsg(const std::string& join_uid, int status);

    /*
*   远端的媒体流退出了
*/
    virtual void OnRemoveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const vlive::VHStreamType type);

    /*
    *
    */
    virtual void OnStreamMixed(const std::string& user, const std::string& stream, int type);
    /*
   *  接收到开始双推消息
   */
    virtual void OnRecvStartDoubleVrtc(const std::string& join_uid, const std::string& nickName);
    /*
   *  接收到结束双推消息
   */
    virtual void OnRecvEndDoubleVrtc(const std::string& target_id, const std::string& nickName);
    /*
   *  接收开始考试消息消息
   */
    virtual void  OnRecvExamination();
	/*
   *  接收白名单登录消息
   */
	virtual void  OnRecvWhiteListUserJoinRoom(const std::string& phone, const std::string& class_token);
   /*
   *  接收到主讲人设置大画面处理
   */
    virtual void OnRecvSetBigScreen(const std::string& target_id);
    /*
   *  接收被邀请双推消息
   */
    void OnRecvInviteDoubleVrtc(std::string target_id);
    /*
   *  接收被邀请桌面共享消息
   */
    void OnRecvInviteScreenSharing(std::string target_id);
    /*订阅大小流回调*/
    virtual void OnChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHSimulCastType type, int code, std::string msg);
    /*房间链接事件*/
    virtual void OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum, const std::string& id = "");
    /*本地打开采集设备,包括摄像头、桌面共享、开始插播视频回调*/
    virtual void OnOpenCaptureCallback(vlive::VHStreamType streamType, vlive::VHCapture code, bool hasVideo, bool hasAudio);
    /*开始推流成功回调*/
    virtual void OnPushStreamSuc(VHStreamType streamType, std::string& streamid, bool hasVideo, bool hasAudio, std::string& attributes);
    /*推流失败回调*/
    virtual void OnPushStreamError(std::string streamId, VHStreamType streamType, const int codeErr = 0, const std::string& msg = std::string());
    /*停止推流成功回调*/
    virtual void OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg);
    /*接收到远端的媒体流  hasVideo ：是否包含视频*/
    virtual void OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio);
    /*远端的媒体流退出*/
    virtual void OnRemoteUserLiveStreamRemoved(const std::wstring& user, const std::string& stream, const VHStreamType type);
    /*接收公告回调*/
    virtual void OnRecvNoticeMsg(const std::string msg);
    /*上课回调*/
    virtual void OnRecvStartClassMsg(const std::wstring& join_uid, const std::wstring& nickname, const int& iCurLabelNumber, const int& iStartPlayType);
    /**收到下课消息**/
    virtual void OnRecvStopClassMsg(int iPlayType);

    /*
    *   接口说明：获取课程详情
    */
    virtual void OnGetWebinarDetailInfo(int code, std::string respMsg);
    /**
    *  接口说明：进入课堂房间。
    **/
    virtual void OnJoinClassRoom(int code, std::string respMsg, const int& iCurLabelNumber = -1);
    
    /**
*  接口说明：进入课堂房间。
**/
    virtual void OnInitQualityNumber( const int& iCurLabelNumber = 1);


    /*
    *   接口说明：讲师操作开始上课。
    *   返回值：ERROR_CODE
    *   回调监听：RoomEvent
    **/
    virtual void OnStartClass(int iPlayType, int code, std::string respMsg);
    /*
    *   接口说明: 讲师操作下课
    *        createDefaultRecord: 是否生成默认回放 1是 0否
    *   返回值：ERROR_CODE
    *   回调监听：RoomEvent
    **/
    virtual void OnStopClass(int createDefaultRecord, int iPlayType, int code, std::string respMsg);
    /*
    *   监听互动房间内HTTP业务API调用成功事件
    *   event: RoomEvent/ClassRoomEvent 事件类型
    *   code: 错误码
    *   msg: 响应消息
    *   userData: 附带的json数据
    */
    virtual void OnCallbackEvent(int event, int code = 200, std::string msg = std::string(), std::string userData = std::string());
    /***
    *   成功获取在线成员列表回调
    **/
    virtual void OnGetOnLineUserRoomMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   成功获取踢出成员列表回调
    **/
    virtual void OnGetKickOutRoomMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   成功获取禁言成员列表回调
    **/
    virtual void OnGetSilencedMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   成功获取举手列表回调
    **/
    virtual void OnGetHandUpsMembers(const vlive::ClassOnLineUserInfo& users);
    /*成功获取可以双推用户列表*/
    virtual void OnGetDoubleVrtcUsers(std::list<ClassDoubleVrtcUsers> users);
    /*
    *   开启/关闭 举手成功或失败回调
    */
    virtual void OnOpenHandsUp(bool open, int code, std::string respMsg);
    /*
    *   讲师同意用户上麦
    */
    virtual void OnAuditPublish(std::string uid, int code, std::string respMsg);
    /**
    *  接口说明：讲师下麦指定用户，主讲人身份使用。
    **/
    virtual void OnKickUserPublish(const std::string& kick_user_id, int code, std::string respMsg);
    /**
    *  接口参数：讲师下麦所有用户
    **/
    virtual void OnKickAllUnPublish(int code, std::string respMsg);
    /**
    *  接口说明：讲师邀请学员、嘉宾上麦，主讲人身份使用。
    **/
    virtual void OnAskforPublish(const std::string& audit_user_id, int code, std::string respMsg);
    /**
    *  接口说明：学员、嘉宾举手，申请上麦。
    *  回调监听事件：RoomEvent::RoomEvent_Apply_Push
    *  返回值：VhallLiveErrCode
    */
    virtual void OnApplyPublish(int code, std::string respMsg);
    /**
   *  接口说明：检测用户是否上麦中回调。
   *  参数：user_id 被检测用户id.
   *         code: 错误码
   *         respMsg: 错误信息
   */
    virtual void OnCheckUserIsPublish(const std::string& user_id, int code, std::string respMsg);
	/**
	*  接口说明：奖励学员出错。
	*/
	virtual void RewardError(int code, std::string respMsg);
	/**
	*  接口说明：学员、嘉宾举手，取消举手。
	*  回调监听事件：RoomEvent::RoomEvent_Apply_Push
	*  返回值：VhallLiveErrCode
	*/
	virtual void CanclePublish(int code, std::string respMsg);
    /**
    *  接口说明：讲师将学员踢出课堂房间/取消踢出课堂房间
    **/
    virtual void OnKickOutClassRoom(const std::string& kick_user_id, int type, int code, std::string respMsg);
    /*
    *   接口说明：讲师授权用户画笔
    */
    virtual void OnChangeUserPenAuth(std::string userId, bool auth, int code, std::string respMsg);
    /*
    *  接口说明：讲师禁言、取消禁言学员、嘉宾
    */
    virtual void OnForbiddenChat(std::string userId, bool forbid, int code, std::string respMsg);
    /*
    *  接口说明：讲师全体禁言、取消禁言
    */
    virtual void OnForbiddenChatAll(bool forbid, int code, std::string respMsg);
    /*
    *   接口说明：讲师静音学员或嘉宾
    */
    virtual void OnOperateUserMic(std::string userId, bool close, int code, std::string respMsg);
    /*
    *   接口说明：讲师讲师静音全部用户
    *       close: true 关闭， false 打开
    */
    virtual void OnOperateAllUserMic(int mute, int code, std::string respMsg);
    /*
    *   接口说明：讲师关闭、打开用户摄像头
    *   参数说明：
    *       userId：用户id
    *       close: true 关闭， false 打开
    */
    virtual void OnOperateUserCamera(std::string userId, bool close, int code, std::string respMsg);
    /**
    *  接口说明：学员、嘉宾上/下/拒绝上麦状态回执。学员、嘉宾等非主讲人身份使用。
    *  参数说明：
    *       stream_id: 操作的流ID
    *       type:      PushStreamEvent
    *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishCallback(vlive::ClassPushStreamEvent type, std::string uid, int code, std::string respMsg);
    /**
    *  接口说明：学员、嘉宾上/下麦失败通知。学员、嘉宾等非主讲人身份使用。
    *  参数说明：
    *       stream_id: 操作的流ID
    *       type:      PushStreamEvent
    *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishFailedCallback(vlive::ClassPushStreamEvent type, int code, std::string respMsg);
    /*
    *  接口说明：学员或嘉宾上报上麦能力。通过获取麦克风和摄像设备列表，查看是否有设备，如果有设备上报状态，主讲人才能邀请此成员上麦。
    *  参数说明：
    *      bool enablePublish;            true 能上麦，false 不能上麦
    *      bool enablePublishDouble;   true 能双推，false 不能双推
    */
    virtual void OnUpdatePublishAbility(int enablePublish, int enablePublishDouble, int code, std::string respMsg);
   //////////////////////////////////////////////////////////
    /*
    *  接口说明：上报推流设备信息结果请求响应
    *  参数说明：
    */
    virtual void OnSendMicsDeviceCallback( int code, std::string respMsg);
    /*
   *  接口说明：邀请学员双推请求响应
   *  参数说明：
   */
    virtual void OnInviteDoubleVrtcCallback(int code, std::string respMsg);

    /*
*  接口说明：邀请开始双推请求响应
*  参数说明：
*/
    virtual void OnStartDoubleVrtcCallback(int code, std::string respMsg);
    /*
*  接口说明：结束双推/桌面共享 请求响应
*  参数说明：
*/
    virtual void OnStopDoubleVrtcCallback(int code, std::string respMsg);
    /*
*  接口说明：请求 学员双推用户列表 请求响应
*  参数说明：
*/
    virtual void OnDoubleVrtcUsersCallback(int code, std::string respMsg);

	virtual void PostDestoryTestPaperDlg();
   //////////////////////////////////////////////////////////

    /**
    *  接收说明：讲师切换工具栏标签
    **/
    virtual void OnChangeToToolPage(vlive::ToolPage pageIndex, int code, std::string respMsg);

    virtual void OnSetConfigBroadEvent(int layoutType, int nCode, const std::string &msg);
    virtual void OnChangeRrlayType();
    //void GetVideoProfileByConfig(QString def, int& videoDef, int& layoutDef);
    enum eVideoProFileType {
       eVideoProFileTypeVideo = 0,                     //主画面分辨率
       eVideoProFileTypeBroadcast,              //旁路画面分辨率
       eVideoProFileTypeInteraction,            //互动画面分辨率
       eVideoProFileTypeDoublePush,           //双推画面分辨率
       eVideoProFileTypeDesktop,                //桌面共享画面分辨率
    };

    void GetVideoProfileByConfig(int labelNumber, int& videoDef, const eVideoProFileType& eType);

    void AppExec();
    void AppQuit();
    bool IsExist();
    LibManager* GetLibManager();
    WndManager* GetWndManager();
    DataManagerInterface* GetDataManager();
    ConfigSettingInterface* GetConfigSettingInterface();
    VHClassRoomInterface* GetClassSDK();
	void SetClassSDK(VHClassRoomInterface* class_sdk);
private:
    ToolManager(QObject *parent = nullptr);
    ~ToolManager();

    static ToolManager* mpInstance;
    static std::mutex mInstanceMutex;
    LibManager* mLibManager = nullptr;    //动态库/lib库管理对象
    WndManager* mWndManager = nullptr;    //窗口库管理对象
    DataManagerInterface* mDataManager = nullptr;  //数据库管理对象
    ConfigSettingInterface* mConfigManagerInterface = nullptr;
    VHClassRoomInterface* mpVHClassSDK = nullptr;
    std::mutex mEventRecvMutex;
    std::list<QObject*> mEventReciver;
    QEventLoop mQuitLoop;
    std::atomic_bool mbExitApp = false;
	int mIStartMode;			//启动方式
	QString mPrivateStartInfo;
};  


