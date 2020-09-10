#pragma once
#include "vh_room_common.h"

#include <QEvent>
#include <QString>
#include <QList>
#define Str_PleaseStopMedia         QStringLiteral("���Ƚ����岥������")
//#define Str_PleaseStopQUESTION      QStringLiteral("���Ƚ������������")
#define Str_PleaseStopDesktop       QStringLiteral("���Ƚ������湲�������")
#define Str_PleaseStartClass        QStringLiteral("���ȿ�ʼ�Ͽ�")
#define Str_PleaseBackPenAuth QStringLiteral("�����ջػ���Ȩ��")

#define Str_IsExitVhallClassApp     QStringLiteral("�Ƿ��˳����ÿͻ���?")   
#define Str_StartAllMuteAudio       QStringLiteral("����ȫ�徲��")
#define Str_CancelAllMuteAudio      QStringLiteral("���ȫ�徲��")
#define Str_ClassNotStart           QStringLiteral("��û��ʼ�Ͽ�Ŷ~")
#define Str_ClassIsOver             QStringLiteral("�¿���,��Ϣһ�°�~")  
#define Str_KickOutClassNotice      QStringLiteral("�ѱ��߳�����")
#define Str_CancelKickOutUser       QStringLiteral("�ѱ�ȡ���߳�����")
#define Str_Your                    QStringLiteral("��")
#define CLASS_ROOM_ERR_NOTICE       QStringLiteral("�ÿγ��ѱ��رգ��������⣬����ϵ�γ̽�ʦ")
#define Str_RoomTeacherLeaveRoom    QStringLiteral("��ʦ��ʱ�뿪�˷��䣬���Ե�...")
#define Str_NoDeviceCannotPublish   QStringLiteral("û�п��õ�����Ƶ�豸���޷����𻥶�")
#define Str_AlreadyUnPublish        QStringLiteral("��������")
#define Str_AlreadyPublish          QStringLiteral("��������")
#define Str_SelfAlreadyDoubleVrtc QStringLiteral("���ѿ�ʼ˫��")
#define Str_SomeOneAlreadyDoubleVrtc QStringLiteral("��ʼ˫��")

#define Str_NoVrtcStudent       QStringLiteral("��⵽ѧԱû�ж����豸���޷�����˫�ƣ�")  

#define Str_NoScribeStudent_DoublePush      QStringLiteral("��������ѧԱ������ѧԱ��֧��˫�ƣ�")  
#define Str_NoScribeStudent_Deskop    QStringLiteral("��������ѧԱ������ѧԱ��֧�����湲��")  
#define Str_StudentMaybeCant_DoublePush QStringLiteral("û�ж��������豸��ѧԱ������֧��˫�ƣ�")  
#define Str_StudentMaybeCant_Deskop    QStringLiteral("�ֻ������ѧԱ�ݲ�֧�����湲��")  

#define Str_NoStudent_DoublePush      QStringLiteral("��ǰ����ѧԱ��֧��˫�ƣ��޷�����˫�ƣ�")  
//#define Str_NoStudent_Deskop    QStringLiteral("��ǰ����ѧԱ��֧�����湲���޷��������湲��")  

#define Str_ClassOver               QStringLiteral("�¿���")
#define Str_ServerTimeOut  QStringLiteral("����������ӳ�ʱ����ˢ������")
#define Str_SignSuccess               QStringLiteral("ǩ���ɹ�")
//#define Str_ChatDisable             QStringLiteral("���ѱ�����")
#define Str_ChatForbided            QStringLiteral("�ѱ�����")
#define Str_IsInvitingUser          QStringLiteral("���������У����Ժ����")
#define Str_IsInvitedUser          QStringLiteral("��Ǹ����ѧԱ���������������ͬʱ�������������������")
#define Str_InviteTimeOut           QStringLiteral("���ڶԷ���ʱ��δ��Ӧ����������ʧ��")
#define Str_MuteCamera              QStringLiteral("���ѱ���ֹ����")
#define Str_CancelMuteCamera        QStringLiteral("���ѱ������ֹ����")
#define Str_MuteCameraByOther       QStringLiteral("�ѱ���ֹ����")
#define Str_CancelMuteCameraByOther    QStringLiteral("�ѱ������ֹ����")
#define Str_CannotOpearte           QStringLiteral("��ʱ��Ȩ����")
#define Str_YouCMuteMic             QStringLiteral("���Ѿ���")
#define Str_YouCancelMuteMic        QStringLiteral("���ѱ��������")
#define Str_LocalStreamPushErr      QStringLiteral("����ͷ�����쳣�����ڳ�����������...")
#define Str_DeskTopStreamPushErr    QStringLiteral("���湲�������쳣�����ڳ�����������...")
#define Str_SourceStreamPushErr     QStringLiteral("������������쳣�����ڳ�����������...")
#define Str_WebRtcRoomConnectErr    QStringLiteral("�����쳣�����Ժ�����")
#define Str_WebRtcRoomDisConnect    QStringLiteral("���������ѶϿ����ڳ������������Ժ�...")
#define Str_ConnectRoomErr          QStringLiteral("������������ʧ�ܣ��޷����л���")
#define Str_ChangeCamera            QStringLiteral("�л�����ͷ")
#define Str_MuteAudio               QStringLiteral("����")
#define Str_CancelMuteAudio         QStringLiteral("ȡ������")
#define Str_MuteVideo               QStringLiteral("��ֹ����")
#define Str_CancelMuteVideo         QStringLiteral("ȡ����ֹ����")
#define Str_AllUnPublish            QStringLiteral("ȫԱ����")
#define Str_AllMuteAudio            QStringLiteral("ȫԱ����")
#define Str_ChangeBigScreen         QString::fromWCharArray(L"�л�����")
#define Str_AllCancelMuteAudio      QStringLiteral("ȡ��ȫԱ����")    
#define Str_NotAllowDoublePush      QStringLiteral("��⵽ֻ��һ������ͷ���޷�����˫��")
#define Str_HaveNoCamereAllowDoublePush      QStringLiteral("��ⲻ������ͷ���޷�����˫��")
#define Str_OpenDoublePushCameraErr QStringLiteral("˫������ͷ��ʧ��")

const QEvent::Type  CustomEvent_InitUI = (QEvent::Type)5001;
const QEvent::Type  CustomEvent_ShowLogin = (QEvent::Type)5002;
const QEvent::Type  CustomEvent_WebRtcRoomConnetSuc = (QEvent::Type)7000;
const QEvent::Type  CustomEvent_WebRtcRoomConnetErr = (QEvent::Type)7001;
const QEvent::Type  CustomEvent_WebRtcRoomReConneting = (QEvent::Type)7002;
const QEvent::Type  CustomEvent_WebRtcRoomReConneted = (QEvent::Type)7003;
const QEvent::Type  CustomEvent_WebRtcRoomRecover = (QEvent::Type)7023;
const QEvent::Type  CustomEvent_WebRtcRoomNetWorkChanged = (QEvent::Type)7024;
const QEvent::Type  CustomEvent_WebRtcRoomNetWorkChangedCover = (QEvent::Type)7025;
const QEvent::Type  CustomEvent_WebRtcRoomDisConnected = (QEvent::Type)7026;
const QEvent::Type  CustomEvent_WebRtcRoomMixedReady = (QEvent::Type)7027;
const QEvent::Type  CustomEvent_OnRecvApplyPublishMsg = (QEvent::Type)7031;
const QEvent::Type  CustomEvent_OnRecvAuditInavPublishMsg = (QEvent::Type)7032;
const QEvent::Type  CustomEvent_OnRecvAskforInavPublishMsg = (QEvent::Type)7033;
const QEvent::Type  CustomEvent_OnRecvKickInavStreamMsg = (QEvent::Type)7034;
const QEvent::Type  CustomEvent_OnRecvKickOutRoomMsg = (QEvent::Type)7035;
const QEvent::Type  CustomEvent_OnRecvInavCloseMsg = (QEvent::Type)7037;
const QEvent::Type  CustomEvent_OnErrorRoomEvent = (QEvent::Type)7038;
const QEvent::Type  CustomEvent_OnSuccessRoomEvent = (QEvent::Type)7039;
const QEvent::Type  CustomEvent_GetLocalStreamSuc = (QEvent::Type)30003;
const QEvent::Type  CustomEvent_DevChanged = (QEvent::Type)30007;
const QEvent::Type  CustomEvent_SubScribSuc = (QEvent::Type)30010;
const QEvent::Type  CustomEvent_STREAM_REMOVED = (QEvent::Type)30013;
const QEvent::Type  CustomEvent_PushLocalStreamSuc = (QEvent::Type)30015;
const QEvent::Type  CustomEvent_PushLocalStreamError = (QEvent::Type)30016;
const QEvent::Type  CustomEvent_StopPushLocalStreamSuc = (QEvent::Type)30070;
const QEvent::Type  CustomEvent_DestoryWnd = (QEvent::Type)30071;
const QEvent::Type  CustomEvent_CHANGE_LAYOUT = (QEvent::Type)30073;
//const QEvent::Type  CustomEvent_User_OnLine = (QEvent::Type)30074;
const QEvent::Type  CustomEvent_StopPushLocalStreamErr = (QEvent::Type)30075;
const QEvent::Type  CustomEvent_User_OnLineList = (QEvent::Type)30076;
const QEvent::Type  CustomEvent_User_KickOutList = (QEvent::Type)30077;
const QEvent::Type  CustomEvent_User_ForbiddenList = (QEvent::Type)30078;
const QEvent::Type  CustomEvent_User_HandUpList = (QEvent::Type)30079;
//����ʹ����Ϣ
const QEvent::Type  CustomEvent_GetWebinarDetailInfo = (QEvent::Type)40000;
const QEvent::Type  CustomEvent_JoinClassRoom = (QEvent::Type)40001;
const QEvent::Type  CustomEvent_PreviewDev = (QEvent::Type)40002;
const QEvent::Type  CustomEvent_SafeToExitApp = (QEvent::Type)40003;
const QEvent::Type  CustomEvent_CloseFloatRender = (QEvent::Type)40004;
const QEvent::Type  CustomEvent_OnCaptureCallback = (QEvent::Type)40005;
const QEvent::Type  CustomEvent_ShowNoticeMsg = (QEvent::Type)40006;
const QEvent::Type  CustomEvent_StartMixStream = (QEvent::Type)40007;
const QEvent::Type  CustomEvent_StartSetMainStream = (QEvent::Type)40008;
const QEvent::Type  CustomEvent_TurnToLogin = (QEvent::Type)40009;
const QEvent::Type  CustomEvent_StartAndStopClass = (QEvent::Type)40010;
const QEvent::Type  CustomEvent_GetVHMemberList = (QEvent::Type)40011;
const QEvent::Type  CustomEvent_GetVHKickOutMemberList = (QEvent::Type)40012;
const QEvent::Type  CustomEvent_OpenHandsUp = (QEvent::Type)40013;
const QEvent::Type  CustomEvent_SendApplyPublish = (QEvent::Type)40014;
const QEvent::Type  CustomEvent_OnRecvApplyInavPublishMsg = (QEvent::Type)40015;
const QEvent::Type  CustomEvent_SendInvitePublishMsg = (QEvent::Type)40016;
const QEvent::Type  CustomEvent_SendAgreePublishMsg = (QEvent::Type)40017;
const QEvent::Type  CustomEvent_SendKickPublishMsg = (QEvent::Type)40018;
const QEvent::Type  CustomEvent_SendSetBrush = (QEvent::Type)40019;
const QEvent::Type  CustomEvent_SendForbiddenChat = (QEvent::Type)40020;
const QEvent::Type  CustomEvent_SendForbiddenAllChat = (QEvent::Type)40021;
const QEvent::Type  CustomEvent_OnRecvForbiddenChatMsg = (QEvent::Type)40022;
const QEvent::Type  CustomEvent_OnRecvForbiddenAllChatMsg = (QEvent::Type)40023;
const QEvent::Type  CustomEvent_MuteMic = (QEvent::Type)40024;
const QEvent::Type  CustomEvent_MuteVideo = (QEvent::Type)40025;
const QEvent::Type  CustomEvent_MuteMicAll = (QEvent::Type)40026;
const QEvent::Type  CustomEvent_UserPublishCallback = (QEvent::Type)40027;
const QEvent::Type  CustomEvent_OnUserPublishCallback = (QEvent::Type)40029;
const QEvent::Type  CustomEvent_UpdatePublishAbility = (QEvent::Type)40030;
const QEvent::Type  CustomEvent_OnRecvUpdatePublishAbility = (QEvent::Type)40031;
const QEvent::Type  CustomEvent_UserPublishFailedCallback = (QEvent::Type)40032;
const QEvent::Type  CustomEvent_OnRecvOpenHandUpMsg = (QEvent::Type)40033;
const QEvent::Type  CustomEvent_KickOut_Room = (QEvent::Type)40034;
const QEvent::Type  CustomEvent_ShowFadOutTipMsg = (QEvent::Type)40035;
const QEvent::Type  CustomEvent_HandleStartLocalCapture = (QEvent::Type)40036;
const QEvent::Type  CustomEvent_OnRecvDownAllUserPublishMsg = (QEvent::Type)40037;
const QEvent::Type  CustomEvent_GetSilencedList = (QEvent::Type)40038;
const QEvent::Type  CustomEvent_RecvPublishFailed = (QEvent::Type)40039;
const QEvent::Type  CustomEvent_ChangeToToolPage = (QEvent::Type)40040;
const QEvent::Type  CustomEvent_LocalStreamIdChanged = (QEvent::Type)40041;
const QEvent::Type  CustomEvent_RecvFlashMsg = (QEvent::Type)40042;
const QEvent::Type  CustomEvent_RecvAllMuteMic = (QEvent::Type)40043;
const QEvent::Type  CustomEvent_AllUnPublish = (QEvent::Type)40044;  
const QEvent::Type  CustomEvent_RecvMuteMic = (QEvent::Type)40045;
const QEvent::Type  CustomEvent_RecvMediaMsg = (QEvent::Type)40046;
const QEvent::Type  CustomEvent_RecvAnswerMsg = (QEvent::Type)40047;
const QEvent::Type  CustomEvent_RecvMuteVideo = (QEvent::Type)40048;
const QEvent::Type  CustomEvent_RecvSetBrush = (QEvent::Type)40049;
const QEvent::Type  CustomEvent_RecvHandDelay = (QEvent::Type)40050;
const QEvent::Type  CustomEvent_RecvUserOnLine = (QEvent::Type)40051;
const QEvent::Type  CustomEvent_RecvUserOffLIne = (QEvent::Type)40052;
const QEvent::Type  CustomEvent_RecvUploadDoc = (QEvent::Type)40053;
const QEvent::Type  CustomEvent_StartRenderCameraAuxiliary = (QEvent::Type)40054;
const QEvent::Type  CustomEvent_SetLayoutCameraAuxiliary = (QEvent::Type)40055;
const QEvent::Type  CustomEvent_CanclePublish = (QEvent::Type)40056;
const QEvent::Type  CustomEvent_RecvCancelHand = (QEvent::Type)40057;
const QEvent::Type  CustomEvent_RewardStu = (QEvent::Type)40058;
const QEvent::Type  CustomEvent_StartCountdown = (QEvent::Type)40059;
const QEvent::Type  CustomEvent_StartSign = (QEvent::Type)40060;
const QEvent::Type  CustomEvent_SignIn = (QEvent::Type)40061;
const QEvent::Type  CustomEvent_SignStop = (QEvent::Type)40062;
const QEvent::Type  CustomEvent_RecvSignUsers = (QEvent::Type)40063;
const QEvent::Type  CustomEvent_RewardError = (QEvent::Type)40064;
const QEvent::Type  CustomEvent_RemoteStreamAdd = (QEvent::Type)40065;
const QEvent::Type  CustomEvent_PreOnCaptureCallback = (QEvent::Type)40066;

const QEvent::Type  CustomEvent_StartDoubleVrtcCallback = (QEvent::Type)40067;
const QEvent::Type  CustomEvent_EndDoubleVrtcCallback = (QEvent::Type)40068;
const QEvent::Type  CustomEvent_DoubleVrtUsersCallback = (QEvent::Type)40069;
const QEvent::Type  CustomEvent_DoubleVrtUsersList = (QEvent::Type)40070;
const QEvent::Type  CustomEvent_inviteDoubleVrtc = (QEvent::Type)40071;
const QEvent::Type  CustomEvent_InviteScreenSharing = (QEvent::Type)40072;

const QEvent::Type  CustomEvent_StartAnswerTool = (QEvent::Type)40073;
const QEvent::Type  CustomEvent_AnswerDetail = (QEvent::Type)40074;
const QEvent::Type  CustomEvent_StartAnswer = (QEvent::Type)40075;

const QEvent::Type  CustomEvent_ChangeBigScreen = (QEvent::Type)40076;
const QEvent::Type  CustomEvent_InviteDoubleVrtcCallback = (QEvent::Type)40077;
const QEvent::Type  CustomEvent_EndDesktopSharingCallback = (QEvent::Type)40078;
const QEvent::Type CustomEvent_RequestStartDoubleVrtcCallback = (QEvent::Type)40079;
const QEvent::Type CustomEvent_OnNetWork_Close = (QEvent::Type)40080;
const QEvent::Type CustomEvent_FloatType = (QEvent::Type)40081;
const QEvent::Type CustomEvent_FullType = (QEvent::Type)40082;
const QEvent::Type CustomEvent_ReciveBmpData = (QEvent::Type)40083;
const QEvent::Type CustomEvent_CloseTest = (QEvent::Type)40083;
const QEvent::Type  CustomEvent_LibCefLoadFinished = (QEvent::Type)40084;
const QEvent::Type  CustomEvent_LibCefMsg = (QEvent::Type)40085;
const QEvent::Type  CustomEvent_Examination = (QEvent::Type)40086;
const QEvent::Type  CustomEvent_TitleChanged = (QEvent::Type)40087;
const QEvent::Type  CustomEvent_WhiteListUserJoinRoom = (QEvent::Type)40088;
const QEvent::Type CustomEvent_FloatType_Renders = (QEvent::Type)40089;
const QEvent::Type CustomEvent_ChangeRrlayType = (QEvent::Type)40090;

class QEventStream : public QEvent {
public:
    QEventStream(QEvent::Type type, QString userid, std::string streamId, int mediaType, int iVhCapture, bool hasVideo, bool hasAudio,std::string attr = std::string()) :
        QEvent(type) {
        strUser = userid;
        strStream = streamId;
        mStreamType = mediaType;
		mIVHCapture = iVhCapture;
        mbHasVideo = hasVideo;
        mbHasAudio = hasAudio;
        mStreamAttributes = attr;
    };
    QString strUser;
    std::string strStream;
    std::string mStreamAttributes;
    int mStreamType;    
	int mIVHCapture;//VHCapture
    bool mbHasVideo;        //�����Ƿ������Ƶ true��
    bool mbHasAudio;
};

class CMember
{
public:
	int status;            // ����״̬��Ĭ��Ϊ0�����ߣ�1Ϊ����
	int role_type;
	int no_mic;           //�Ƿ������������  0 �������� 1 ������
	int is_joinmic; //1  ���Ѿ������ (������) 			0  ����δ����ģ����֣�
	QString user_id;  //�û�ID
	QString nick_name;
};

class QEventOnLineList : public QEvent {
public:
	QEventOnLineList(QEvent::Type type,const vlive::ClassOnLineUserInfo &users) :
		QEvent(type) {
		total = users.total;
		total_page = users.total_page;
		curr_page = users.curr_page;

		std::list<vlive::ClassMember>::const_iterator iter = users.curr_page_users.begin();
		for (; iter != users.curr_page_users.end(); iter++)
		{
			CMember mem;
			mem.status = iter->status;            // ����״̬��Ĭ��Ϊ0�����ߣ�1Ϊ����
			mem.role_type = iter->role_type;
			mem.no_mic = iter->no_mic;           //�Ƿ������������  0 �������� 1 ������
			mem.is_joinmic = iter->is_joinmic;//1  ���Ѿ������ (������) 			0  ����δ����ģ����֣�
			mem.user_id = QString::fromStdString(iter->user_id);  //�û�ID
			mem.nick_name = QString::fromStdString(iter->nick_name);

			mUsers.append(mem);
		}

		//mUsers = users;
	};
	int total;//������
	int total_page;//��ҳ��
	int curr_page; //�뵱ǰҳ��

	QList<CMember> mUsers;
};

class CDoubleVrtcUsers
{
public:
   QString mstrId;         //�û�ID
   QString mstrNickName;
   int miNoDoubleVrtc;  //��֧��˫·���� 0:����˫�� 1:������
};

class DoubleVrtcUsersEvent : public QEvent {
public:
   DoubleVrtcUsersEvent(QEvent::Type type, std::list<vlive::ClassDoubleVrtcUsers> users) :
      QEvent(type) {

      std::list<vlive::ClassDoubleVrtcUsers>::const_iterator iter = users.begin();
      for (; iter != users.end(); iter++)
      {
         CDoubleVrtcUsers mem;
         mem.mstrId = QString::fromStdString(iter->user_id);  //�û�ID
         mem.mstrNickName = QString::fromStdString(iter->nick_name);
         mem.miNoDoubleVrtc = iter->no_double_vrtc;
         mUsers.append(mem);
      }
   };
   QList<CDoubleVrtcUsers> mUsers;
};



class QEventStreamError : public QEvent {
public:
	QEventStreamError(QEvent::Type type, std::string streamId, int streamType, const int codeErr, const std::string& msg) :
		QEvent(type) {
		iCodeErr = codeErr;
		mStreamType = streamType;
		strMsg = msg;
      mstreamId = streamId;
	};
	int iCodeErr;
	int mStreamType;
	std::string strMsg;
   std::string mstreamId;
};


class QEventVHPublishFailed : public QEvent {
public:
	QEventVHPublishFailed(QEvent::Type type, const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type) :
		QEvent(type) {
		mstrUid = QString::fromStdString(join_uid);
		miStatus = status;
		mstrNick = QString::fromStdString(nick_name);
		miRoleType = role_type;
		miAppType = app_type;
	};

	QString mstrUid;
	int miStatus;
	QString mstrNick;
	int miRoleType;
	int miAppType;
};

class QEventStartSign : public QEvent {
public:
	QEventStartSign(QEvent::Type type, const std::string& join_uid, int time, const std::string& strwebinar) :
		QEvent(type) {
		mstrUid = QString::fromStdString(join_uid);
		miTime = time;
		mstrwebinar = QString::fromStdString(strwebinar);
	};

	QString mstrUid;
	int miTime;
	QString mstrwebinar;
};

class QEventSignIn : public QEvent {
public:
	QEventSignIn(QEvent::Type type, const std::string& strWebinar, const std::string& sign_id,
		const std::string& signer, const std::string& signerNickname) :
		QEvent(type) {
		mstrAddSignid = QString::fromStdString(sign_id);
		mstrwebinar = QString::fromStdString(strWebinar);
		mstrSigner = QString::fromStdString(signer);
		msignerNickname = QString::fromStdString(signerNickname);
	};

	QString mstrAddSignid;
	QString mstrSigner;
	QString msignerNickname;
	QString mstrwebinar;
};

class QEventSignStop : public QEvent {
public:
	QEventSignStop(QEvent::Type type, const std::string& join_uid, const std::string& strwebinar) :
		QEvent(type) {
		mstrUid = QString::fromStdString(join_uid);
		mstrwebinar = QString::fromStdString(strwebinar);
	};

	QString mstrUid;
	QString mstrwebinar;
};

class QEventStartAnswerTool : public QEvent {
public:
	QEventStartAnswerTool(QEvent::Type type, const std::string& msg) :
		QEvent(type) {
		mstrItems = QString::fromStdString(msg);

	};
	QString mstrItems;
};

class QEventStartAnswerDetail : public QEvent {
public:
	QEventStartAnswerDetail(QEvent::Type type, const std::string& msg, int code = -1) :
		QEvent(type) {
		mstrItems = QString::fromStdString(msg);
		mcode = code;
	};
	int mcode;
	QString mstrItems;
};

class QEventStartAnswer : public QEvent {
public:
	QEventStartAnswer(QEvent::Type type, const std::string& msg, int code = -1) :
		QEvent(type) {
		mstrItems = QString::fromStdString(msg);
		mcode = code;
	};
	int mcode;
	QString mstrItems;
};

class QEventAnswerToolSubmit : public QEvent {
public:
	QEventAnswerToolSubmit(QEvent::Type type, const std::string& msg) :
		QEvent(type) {
		mstrItems = QString::fromStdString(msg);
	};

	QString mstrItems;
};

class QEventVHMember : public QEvent {
public:
    QEventVHMember(QEvent::Type type, QString userid, QString nick,int status = -1) :
        QEvent(type) {
        thirdPartyUserId = userid;
		strNick = nick;
        memberStatusType = status;
    };
	QString strNick;
    QString thirdPartyUserId;
    int memberStatusType;
};

class QEventStartCountdown : public QEvent {
public:
	QEventStartCountdown(QEvent::Type type, QString userid, const int& iplayEndSound = -1, const int& iTime=-1) :
		QEvent(type) {
		thirdPartyUserId = userid;
		miplayEndSound = iplayEndSound;
		miTime = iTime;
	};

	QString thirdPartyUserId;
	int miplayEndSound;
	int miTime;
};

class QEventUserPublish : public QEvent {
public:
    QEventUserPublish(QEvent::Type type, QString userid, std::string stream_id, 
       int rewardnum ,int status = -1, const int& iNoDoubleVrtc = -1) :
        QEvent(type) {
        thirdPartyUserId = userid;
        strStreamId = stream_id;
        memberStatusType = status;
		miRewardNum = rewardnum;
      miNoDoubleVrtc = iNoDoubleVrtc;
    };
    QString thirdPartyUserId;
    std::string strStreamId;
    int memberStatusType;
    int role_type = -1;
    int device_type = 1;
	int miRewardNum = -1;
   int miNoDoubleVrtc;
};

class QEventDestoryWnd : public QEvent {
public:
    QEventDestoryWnd(QEvent::Type type, QString wndId) :
        QEvent(type) {
        mWndId = wndId;
    };
    QString mWndId;
};

class QEventRoomEvent : public QEvent {
public:
    QEventRoomEvent(QEvent::Type type, int cbType, 
		QString userId = QString(), int processType = 0, 
		int respCode = 0, std::string resp = std::string(), 
		std::string userData = std::string()) :
        QEvent(type) {
        mRespCode = respCode;
        mRespMsg = resp;
        mCallBackCodeType = cbType;
        mUserId = userId;
        mProcessType = processType;
        mUserData = QString::fromStdString(userData);
    };

    std::string mRespMsg;//������Ӧ��Ϣ
    int mCallBackCodeType;
    int mRespCode;
    int mProcessType;
    QString mUserId;
    QString mUserData;
};

class QEventConfigBroadCast : public QEvent {
public:
    QEventConfigBroadCast(QEvent::Type type, std::string msg) :
        QEvent(type) {
        strMsg = msg;
    };
    std::string strMsg;
};

class QEventRecvMsg : public QEvent {
public:
    QEventRecvMsg(QEvent::Type type, QString user_id,QString userData = QString(),int Status = 0) :
        QEvent(type) {
        mUserId = user_id;
        mUserData = userData;
        mStatus = Status;
    };
    QString mUserId;
    QString mUserData;
    int mStatus;
    int miNoDoubleVrtc;// 0 ����˫�� 1 ����˫��//0 ����˫�� 1 ����˫��
};

class QEventChangeUser : public QEvent {
public:
	QEventChangeUser(QEvent::Type type, const std::string& msg, int code) :
		QEvent(type) {
		strMsg = QString::fromStdString(msg);
		iCodeErr = code;
	};
	QString strMsg;
	int iCodeErr = -1;
};


class QEventSignUsers : public QEvent {
public:
	QEventSignUsers(QEvent::Type type, std::list<std::string> listSignIn, std::list<std::string> listNo) :
		QEvent(type) {
		mstlistsignUsers = listSignIn;
		mstlistNoSignUsers = listNo;
	};

	std::list<std::string> mstlistsignUsers;
	std::list<std::string> mstlistNoSignUsers;
};

class CustomEventType : public QEvent {
public:
    CustomEventType(QEvent::Type type, QString data, bool bState = true);
public:
    QString msg;
    bool mbState;
};

class CustomChangeToolPage : public QEvent {
public:
	CustomChangeToolPage(QEvent::Type type, int iPage)
		:QEvent(type)
	{
		miPage = iPage;
	}
public:
	int miPage;
};

class DoubleVrtcUserEvent : public QEvent {
public:
   DoubleVrtcUserEvent(QEvent::Type type,  QString id,  QString strNickName)
      :QEvent(type)
   {
      userId = id;
      nickName = strNickName;
   }
public:
   QString userId;
   QString nickName;
};

class BmpEvent : public QEvent {
public:
   BmpEvent(QEvent::Type type, int iLen, int iWidth = 0, int iHeight = 0)
      :QEvent(type)
   {
      
      mIlen = iLen;
      mStrData = new unsigned char[iLen + 1];
      memset(mStrData, 0, iLen + 1);

      miWidth = iWidth;
      miHeight = iHeight;
   }

   ~BmpEvent()
   {
      int a = 0;
      delete[]mStrData;
	  mStrData = nullptr;
   }

public:
   unsigned char* mStrData = nullptr;
   int mIlen;
   int miWidth = 0;
   int miHeight = 0;
};

class CustomRecvMsgEvent : public QEvent {
public:
   CustomRecvMsgEvent(QEvent::Type type, QString data, bool hasVideo = true) :
      QEvent(type) {
      msg = data;
      mbHasVideo = hasVideo;
   };
   QString msg;
   bool mbHasVideo;
};

class CustomRecvCEFJSMsgEvent : public QEvent {
public:
   CustomRecvCEFJSMsgEvent(QEvent::Type type, QString data = QString(), bool hasVideo = true) :
      QEvent(type) {
      msg = data;
      mbHasVideo = hasVideo;
   };
   QString msg;
   QString mStreamId;
   QString mJoinUserId;
   int mode;
   bool mbHasVideo;
   int mStreamType;
};

class CefMsgEvent : public QEvent {
public:
   CefMsgEvent(QEvent::Type type, QString fun, QString msg) :
      QEvent(type) {
      mFunName = fun;
      mCefMsgData = msg;
   };
   QString mFunName;
   QString mCefMsgData;
};

class ExaminationEvent : public QEvent {
public:
   ExaminationEvent(QEvent::Type type) :
      QEvent(type) {
   };

};

class WhiteListUserJoinRoom : public QEvent {
public:
	WhiteListUserJoinRoom(QEvent::Type type, QString phone, QString class_token) :
		QEvent(type) {
		mPhone = phone;
		mClass_Token = class_token;
	};
	QString mPhone;
	QString mClass_Token;

};