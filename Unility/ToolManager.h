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

	//������ʽ
	enum eStartMode
	{
		eStartMode_desktop = 0,	//��������
		eStartMode_private			//˽��Э��
	};

	//�ۿ�����
	enum eLoginMode
	{
		eLoginMode_Command = 0,	//�����¼
		eLoginMode_Free,         //���
		eLoginMode_WhiteList		//������
	};
   //PAAS SDK 
   //********************************************************   �ӿڵ��ûص��ɹ���ʧ��֪ͨ   ********************************************************//
/*
*   ����������ҵ����óɹ��¼�
*   code: RoomEvent �¼�����
*   userData: ������json����
*/
   virtual void OnSuccessedEvent(RoomEvent code, std::string userData = std::string());
   /*
*   ��������������HTTPҵ��API����ʧ���¼�
*   respCode: ������
*   msg��������Ϣ
*   userData: ������json����
*/
   virtual void OnFailedEvent(RoomEvent code, int respCode, const std::string& msg, std::string userData = std::string());
   /*
*   ��ȡ���������Ա�б�ص�
**/
   virtual void OnGetVHRoomMembers(const std::string& third_party_user_id, std::list<VHRoomMember>&);
   
   /*
*   ��ȡ���߳������������б�ص�
**/
   virtual void OnGetVHRoomKickOutMembers(std::list<VHRoomMember>&);
   /*
*   ����������Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ����ʾ������Ϣ
*/
   void OnRecvBaseAskforInavPublishMsg(const std::wstring& third_party_user_id);
   //********************************************************   �㲥��Ϣ�����¼�   ********************************************************//
/*
 *   �յ�����������Ϣ ,��ϢΪ�㲥��Ϣ���յ���Ϣ��ͨ�����뻥������ʱ��ȡ��Ȩ���б��ж��û��Ƿ������Ȩ��
 */
   void OnRecvBaseApplyInavPublishMsg(std::wstring& third_party_user_id);
   /*
*   ��������״̬����
*/
   virtual void OnRoomConnectState(RoomConnectState) ;
//   /*
//*   �յ����������Ϣ ,��ϢΪ�㲥��Ϣ���յ�ͬ�������ִ���������
//*/
//   virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
   /*
   *   �߳��������� , ��ϢΪ�㲥��Ϣ���յ���Ϣ���ж��ǵ�ǰ�û���ִ���߳��������
   */
   virtual void OnRecvKickInavMsg(const std::wstring& third_party_user_id) ;

   /*
   *   ��/��/�ܾ�������Ϣ ��ϢΪ�㲥��Ϣ
   */
   virtual void OnUserPublishCallback(const std::wstring& third_party_user_id, const std::string& stream_id, PushStreamEvent event);

   /*
   *   �û�������֪ͨ
   *   online�� �û�����/ �û����� / ���� / ȡ������
   *   user_id�� �û�id
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
    //�˳�Ӧ��
    void SafeToExitApp();
    //�˳�����½ҳ��
    void LogOut();
	QString GetConfigPath();
	QString GetToolConfigPath();
    /*�������������ɹ�֮������������ID�����ı�֪ͨ**/
    virtual void OnRePublishStreamIDChanged(VHStreamType streamType, const std::wstring& user_id, const std::string& old_streamid, const std::string& new_streamid);
    /*�յ�����������Ϣ*/
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id, const std::wstring& nickname) override;
    virtual void OnRecvFlahsMsg(std::string flashMsg);
    virtual void OnRecvSetBrushMsg(std::string data,std::string msg);
    virtual void OnRecvDocUploadMsg(std::string msg);

    /**
*   ���յ�Զ������
*/
    virtual void OnRemoteStreamAdd(const std::string& user_id, const std::string& stream_id, vlive::VHStreamType streamType);
    /**
*   ������ʧ�ܡ�
*/
    virtual void OnSubScribeStreamErr(const std::string& user_id, const std::string& msg, int errorCode);
    /****/
    virtual void OnRecvMediaMsg(std::string mediaMsg);

    virtual void OnRecvAnswerMsg(std::string msg);
    
    /*���Ժ�ȡ������*/
    virtual void OnRecvForbiddenChat(int forbid, std::string user_id, std::string data);
    /*���Ժ�ȡ������*/
    void OnRecvForbiddenAllChat(int forbid, std::string data);
    /*������������*/
    void OnRecvUpdatePublishAbility(const std::string& join_uid, int statue);
    /*�յ����������Ϣ ,��ϢΪ�㲥��Ϣ���յ�ͬ�������ִ���������*/
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish);
    /*����������Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ����ʾ������Ϣ*/
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& join_uid, const std::wstring& target_uid, int status) override;
    /*�߳�����Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ��ִ���߳���*/
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id);
    /*�߳���������*/
    virtual void OnRecvKickOutRoomMsg(const std::string& third_party_user_id,int status);
    /*
    *  ���յ����ֿ�����Ϣ
    *  status ״̬��1����0�أ�
    */
    virtual void OnRecvOpenHandMsg(int status);
    /*��/��/�ܾ�������Ϣ ��ϢΪ�㲥��Ϣ*/
    virtual void OnRecvUserPublishCallback(const std::string& third_party_user_id, const std::string& nickname, 
       vlive::ClassPushStreamEvent event, int role_type, int device_type, int iRewardNum, const int& iNoDoubleVrtc);

    /*��������ر���Ϣ  ���ܵ�����Ϣ�������ڻ����������Ա��ȫ�������˳�����*/
    virtual void OnRecvInavCloseMsg();
    /* �û�������֪ͨ*/
    virtual void OnUserOnLineState(bool online, const std::string user_id, int role_type);
    /* �յ����� ����ʧ����Ϣ*/
    virtual void OnRecvPublishFailedMsg(const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type);
	/*
	*   ���յ����ֳ�ʱ��Ϣ
	*/
	virtual void  OnRecvHandDelayMsg(const std::string& target_id);
   /*
   *   ���յ��������湲����Ϣ
   */
   virtual void OnRecvStopDesktopSharing();
	/*�յ���ʼǩ����Ϣ*/
	virtual void  OnRecvStartSign(const std::string&strWebinar, const std::string& sign_id, const int& signShowTime);
	virtual void  OnRecvSignIn(const std::string& strWebinar, const std::string& sign_id, const std::string& signer, const std::string& signerNickname);
	virtual void OnSignUsersMsg(std::list<std::string> stlistsignUsers, std::list<std::string> stlistNoSignUsers);
	virtual void  OnRecvSignStop(const std::string& strWebinar, const std::string& sign_id);
	//�����˷��𵹼�ʱ �� ������ȡ������ʱ 
	virtual void  OnRecvStartCountdown(const std::string& id, const int& iplayEndSound, const int& iTime);
	virtual void  OnRecvCancelCountdown(const std::string& id);
	/*�յ���ʼ��������Ϣ*/
	virtual void  OnRecvAnswerToolMsg(const std::string& msg);
	virtual void  OnRecvAnswerDetailMsg(int code, const std::string& msg);
	virtual void  OnRecvStartAnswerMsg(int code, const std::string& msg);
	/*
	*   ����ȡ��������Ϣ
	*/
	virtual void  OnRecvCancelHand(std::string strJoinId, int iRoleType, std::string strNickName, std::string strTime);
	/*
	*   ���ս�������������Ϣ
	*/
	virtual void OnRecvHandRewardStu(std::string target_id, std::string nickName,const int& iFavorNum);
	/*
    *  ���յ�������Ϣ
    *  status ״̬��1������0ȡ��������
    */
    virtual void OnRecvMuteMicMsg(const std::string& join_uid, int status);
    /*�������������û���Ϣ*/
    virtual void OnRecvDownAllUserPublishMsg();
    /**
    *   ���յ�ȫԱ������Ϣ
    *   status ״̬��1������0ȡ��������
    */
    virtual void OnRecvAllMuteMicMsg(int status) ;
    /*
    *  ���յ�������Ϣ
    *  status ״̬��1������0ȡ��������
    */
    virtual void OnRecvMuteVideoMsg(const std::string& join_uid, int status);

    /*
*   Զ�˵�ý�����˳���
*/
    virtual void OnRemoveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const vlive::VHStreamType type);

    /*
    *
    */
    virtual void OnStreamMixed(const std::string& user, const std::string& stream, int type);
    /*
   *  ���յ���ʼ˫����Ϣ
   */
    virtual void OnRecvStartDoubleVrtc(const std::string& join_uid, const std::string& nickName);
    /*
   *  ���յ�����˫����Ϣ
   */
    virtual void OnRecvEndDoubleVrtc(const std::string& target_id, const std::string& nickName);
    /*
   *  ���տ�ʼ������Ϣ��Ϣ
   */
    virtual void  OnRecvExamination();
	/*
   *  ���հ�������¼��Ϣ
   */
	virtual void  OnRecvWhiteListUserJoinRoom(const std::string& phone, const std::string& class_token);
   /*
   *  ���յ����������ô��洦��
   */
    virtual void OnRecvSetBigScreen(const std::string& target_id);
    /*
   *  ���ձ�����˫����Ϣ
   */
    void OnRecvInviteDoubleVrtc(std::string target_id);
    /*
   *  ���ձ��������湲����Ϣ
   */
    void OnRecvInviteScreenSharing(std::string target_id);
    /*���Ĵ�С���ص�*/
    virtual void OnChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHSimulCastType type, int code, std::string msg);
    /*���������¼�*/
    virtual void OnWebRtcRoomConnetEventCallback(const VHRoomConnectEnum, const std::string& id = "");
    /*���ش򿪲ɼ��豸,��������ͷ�����湲����ʼ�岥��Ƶ�ص�*/
    virtual void OnOpenCaptureCallback(vlive::VHStreamType streamType, vlive::VHCapture code, bool hasVideo, bool hasAudio);
    /*��ʼ�����ɹ��ص�*/
    virtual void OnPushStreamSuc(VHStreamType streamType, std::string& streamid, bool hasVideo, bool hasAudio, std::string& attributes);
    /*����ʧ�ܻص�*/
    virtual void OnPushStreamError(std::string streamId, VHStreamType streamType, const int codeErr = 0, const std::string& msg = std::string());
    /*ֹͣ�����ɹ��ص�*/
    virtual void OnStopPushStreamCallback(VHStreamType streamType, int code, const std::string& msg);
    /*���յ�Զ�˵�ý����  hasVideo ���Ƿ������Ƶ*/
    virtual void OnReciveRemoteUserLiveStream(const std::wstring& user, const std::string& stream, const VHStreamType type, bool hasVideo, bool hasAudio);
    /*Զ�˵�ý�����˳�*/
    virtual void OnRemoteUserLiveStreamRemoved(const std::wstring& user, const std::string& stream, const VHStreamType type);
    /*���չ���ص�*/
    virtual void OnRecvNoticeMsg(const std::string msg);
    /*�Ͽλص�*/
    virtual void OnRecvStartClassMsg(const std::wstring& join_uid, const std::wstring& nickname, const int& iCurLabelNumber, const int& iStartPlayType);
    /**�յ��¿���Ϣ**/
    virtual void OnRecvStopClassMsg(int iPlayType);

    /*
    *   �ӿ�˵������ȡ�γ�����
    */
    virtual void OnGetWebinarDetailInfo(int code, std::string respMsg);
    /**
    *  �ӿ�˵����������÷��䡣
    **/
    virtual void OnJoinClassRoom(int code, std::string respMsg, const int& iCurLabelNumber = -1);
    
    /**
*  �ӿ�˵����������÷��䡣
**/
    virtual void OnInitQualityNumber( const int& iCurLabelNumber = 1);


    /*
    *   �ӿ�˵������ʦ������ʼ�ϿΡ�
    *   ����ֵ��ERROR_CODE
    *   �ص�������RoomEvent
    **/
    virtual void OnStartClass(int iPlayType, int code, std::string respMsg);
    /*
    *   �ӿ�˵��: ��ʦ�����¿�
    *        createDefaultRecord: �Ƿ�����Ĭ�ϻط� 1�� 0��
    *   ����ֵ��ERROR_CODE
    *   �ص�������RoomEvent
    **/
    virtual void OnStopClass(int createDefaultRecord, int iPlayType, int code, std::string respMsg);
    /*
    *   ��������������HTTPҵ��API���óɹ��¼�
    *   event: RoomEvent/ClassRoomEvent �¼�����
    *   code: ������
    *   msg: ��Ӧ��Ϣ
    *   userData: ������json����
    */
    virtual void OnCallbackEvent(int event, int code = 200, std::string msg = std::string(), std::string userData = std::string());
    /***
    *   �ɹ���ȡ���߳�Ա�б�ص�
    **/
    virtual void OnGetOnLineUserRoomMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   �ɹ���ȡ�߳���Ա�б�ص�
    **/
    virtual void OnGetKickOutRoomMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   �ɹ���ȡ���Գ�Ա�б�ص�
    **/
    virtual void OnGetSilencedMembers(const vlive::ClassOnLineUserInfo& users);
    /***
    *   �ɹ���ȡ�����б�ص�
    **/
    virtual void OnGetHandUpsMembers(const vlive::ClassOnLineUserInfo& users);
    /*�ɹ���ȡ����˫���û��б�*/
    virtual void OnGetDoubleVrtcUsers(std::list<ClassDoubleVrtcUsers> users);
    /*
    *   ����/�ر� ���ֳɹ���ʧ�ܻص�
    */
    virtual void OnOpenHandsUp(bool open, int code, std::string respMsg);
    /*
    *   ��ʦͬ���û�����
    */
    virtual void OnAuditPublish(std::string uid, int code, std::string respMsg);
    /**
    *  �ӿ�˵������ʦ����ָ���û������������ʹ�á�
    **/
    virtual void OnKickUserPublish(const std::string& kick_user_id, int code, std::string respMsg);
    /**
    *  �ӿڲ�������ʦ���������û�
    **/
    virtual void OnKickAllUnPublish(int code, std::string respMsg);
    /**
    *  �ӿ�˵������ʦ����ѧԱ���α��������������ʹ�á�
    **/
    virtual void OnAskforPublish(const std::string& audit_user_id, int code, std::string respMsg);
    /**
    *  �ӿ�˵����ѧԱ���α����֣���������
    *  �ص������¼���RoomEvent::RoomEvent_Apply_Push
    *  ����ֵ��VhallLiveErrCode
    */
    virtual void OnApplyPublish(int code, std::string respMsg);
    /**
   *  �ӿ�˵��������û��Ƿ������лص���
   *  ������user_id ������û�id.
   *         code: ������
   *         respMsg: ������Ϣ
   */
    virtual void OnCheckUserIsPublish(const std::string& user_id, int code, std::string respMsg);
	/**
	*  �ӿ�˵��������ѧԱ����
	*/
	virtual void RewardError(int code, std::string respMsg);
	/**
	*  �ӿ�˵����ѧԱ���α����֣�ȡ�����֡�
	*  �ص������¼���RoomEvent::RoomEvent_Apply_Push
	*  ����ֵ��VhallLiveErrCode
	*/
	virtual void CanclePublish(int code, std::string respMsg);
    /**
    *  �ӿ�˵������ʦ��ѧԱ�߳����÷���/ȡ���߳����÷���
    **/
    virtual void OnKickOutClassRoom(const std::string& kick_user_id, int type, int code, std::string respMsg);
    /*
    *   �ӿ�˵������ʦ��Ȩ�û�����
    */
    virtual void OnChangeUserPenAuth(std::string userId, bool auth, int code, std::string respMsg);
    /*
    *  �ӿ�˵������ʦ���ԡ�ȡ������ѧԱ���α�
    */
    virtual void OnForbiddenChat(std::string userId, bool forbid, int code, std::string respMsg);
    /*
    *  �ӿ�˵������ʦȫ����ԡ�ȡ������
    */
    virtual void OnForbiddenChatAll(bool forbid, int code, std::string respMsg);
    /*
    *   �ӿ�˵������ʦ����ѧԱ��α�
    */
    virtual void OnOperateUserMic(std::string userId, bool close, int code, std::string respMsg);
    /*
    *   �ӿ�˵������ʦ��ʦ����ȫ���û�
    *       close: true �رգ� false ��
    */
    virtual void OnOperateAllUserMic(int mute, int code, std::string respMsg);
    /*
    *   �ӿ�˵������ʦ�رա����û�����ͷ
    *   ����˵����
    *       userId���û�id
    *       close: true �رգ� false ��
    */
    virtual void OnOperateUserCamera(std::string userId, bool close, int code, std::string respMsg);
    /**
    *  �ӿ�˵����ѧԱ���α���/��/�ܾ�����״̬��ִ��ѧԱ���α��ȷ����������ʹ�á�
    *  ����˵����
    *       stream_id: ��������ID
    *       type:      PushStreamEvent
    *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishCallback(vlive::ClassPushStreamEvent type, std::string uid, int code, std::string respMsg);
    /**
    *  �ӿ�˵����ѧԱ���α���/����ʧ��֪ͨ��ѧԱ���α��ȷ����������ʹ�á�
    *  ����˵����
    *       stream_id: ��������ID
    *       type:      PushStreamEvent
    *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishFailedCallback(vlive::ClassPushStreamEvent type, int code, std::string respMsg);
    /*
    *  �ӿ�˵����ѧԱ��α��ϱ�����������ͨ����ȡ��˷�������豸�б��鿴�Ƿ����豸��������豸�ϱ�״̬�������˲�������˳�Ա����
    *  ����˵����
    *      bool enablePublish;            true ������false ��������
    *      bool enablePublishDouble;   true ��˫�ƣ�false ����˫��
    */
    virtual void OnUpdatePublishAbility(int enablePublish, int enablePublishDouble, int code, std::string respMsg);
   //////////////////////////////////////////////////////////
    /*
    *  �ӿ�˵�����ϱ������豸��Ϣ���������Ӧ
    *  ����˵����
    */
    virtual void OnSendMicsDeviceCallback( int code, std::string respMsg);
    /*
   *  �ӿ�˵��������ѧԱ˫��������Ӧ
   *  ����˵����
   */
    virtual void OnInviteDoubleVrtcCallback(int code, std::string respMsg);

    /*
*  �ӿ�˵�������뿪ʼ˫��������Ӧ
*  ����˵����
*/
    virtual void OnStartDoubleVrtcCallback(int code, std::string respMsg);
    /*
*  �ӿ�˵��������˫��/���湲�� ������Ӧ
*  ����˵����
*/
    virtual void OnStopDoubleVrtcCallback(int code, std::string respMsg);
    /*
*  �ӿ�˵�������� ѧԱ˫���û��б� ������Ӧ
*  ����˵����
*/
    virtual void OnDoubleVrtcUsersCallback(int code, std::string respMsg);

	virtual void PostDestoryTestPaperDlg();
   //////////////////////////////////////////////////////////

    /**
    *  ����˵������ʦ�л���������ǩ
    **/
    virtual void OnChangeToToolPage(vlive::ToolPage pageIndex, int code, std::string respMsg);

    virtual void OnSetConfigBroadEvent(int layoutType, int nCode, const std::string &msg);
    virtual void OnChangeRrlayType();
    //void GetVideoProfileByConfig(QString def, int& videoDef, int& layoutDef);
    enum eVideoProFileType {
       eVideoProFileTypeVideo = 0,                     //������ֱ���
       eVideoProFileTypeBroadcast,              //��·����ֱ���
       eVideoProFileTypeInteraction,            //��������ֱ���
       eVideoProFileTypeDoublePush,           //˫�ƻ���ֱ���
       eVideoProFileTypeDesktop,                //���湲����ֱ���
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
    LibManager* mLibManager = nullptr;    //��̬��/lib��������
    WndManager* mWndManager = nullptr;    //���ڿ�������
    DataManagerInterface* mDataManager = nullptr;  //���ݿ�������
    ConfigSettingInterface* mConfigManagerInterface = nullptr;
    VHClassRoomInterface* mpVHClassSDK = nullptr;
    std::mutex mEventRecvMutex;
    std::list<QObject*> mEventReciver;
    QEventLoop mQuitLoop;
    std::atomic_bool mbExitApp = false;
	int mIStartMode;			//������ʽ
	QString mPrivateStartInfo;
};  


