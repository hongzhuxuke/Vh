#pragma once
#include <QString>
#include <QEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "vlive_def.h"

#define CLOSE_MIC                   QStringLiteral("关闭麦克风")
#define OPEN_MIC                    QStringLiteral("打开麦克风")
#define CLOSE_CAMERA                QStringLiteral("关闭摄像头")
#define OPEN_CAMERA                 QStringLiteral("打开摄像头")
#define INVITE_USER_JOIN            QStringLiteral("邀请上麦")
#define INVITE_USER_KICK_OFF        QStringLiteral("下麦")
#define ENABLE_SEND_MSG             QStringLiteral("允许发言")
#define DISABLE_SEND_MSG            QStringLiteral("禁言")
#define USER_KICK_OUT               QStringLiteral("踢出")
#define LOCK_USER_VIEW              QStringLiteral("选定画面")
#define UNLOCK_USER_VIEW            QStringLiteral("取消选定")
#define ALREADY_PLAY_MEDIA_FILE     QStringLiteral("正在播放视频文件，请稍后再试")
#define ALREADY_SCREEN_SHARE        QStringLiteral("当前正在桌面共享，请稍后再试")

//class VhallLiveDeviceInfo {
//public:
//	VhallLiveDeviceInfo(QString id, QString name, int index) {
//		devId = id;
//		devName = name;
//		devIndex = index;
//	}
//
//	QString devId;		//设备id
//	QString devName;  //名字
//	int devIndex;		//索引
//};

enum ResponseInviteType {
	ResponseInviteType_None = 0,
	ResponseInviteType_Accept = 1,
	ResponseInviteType_Refuse = 2,
	ResponseInviteType_Err = 3,
};


enum RenderViewState {
	RenderView_None = 0,
	RenderView_NoJoin = 1,
	RenderView_HasCamera = 2,
	RenderView_NoCamera = 3,
};

enum VhallSDKDevRequest {
	DevRequest_StartLive = 1,	//打开设置页面获取设备列表
	DevRequest_GetDevList = 2,	    //开播时获取设备列表
	DevRequest_InitSDK = 3,
};

//各个角色当前的演讲状态。
enum SpeakState {	 
	Host_InMainView = 1,
	Host_InListView = 2,
	Guest_InMainView = 3,
	Guest_InListView = 4,
	Other_InMainView = 5,
	Other_InListView = 6,
};


//enum BROADCAST_LAYOUT {
//	BROADCAST_LAYOUT_SPLIT_GRID = 4,   //
//	BROADCAST_LAYOUT_SPLIT_OVERLAP = 8,
//	BROADCAST_LAYOUT_SPLIT_TOPSIDE = 14,
//	BROADCAST_LAYOUT_SPLIT_BOTTOMSIDE = 13,
//	BROADCAST_LAYOUT_SPLIT_TOPSIDE_5 = 18,
//	BROADCAST_LAYOUT_SPLIT_BOTTOMSIDE_5 = 17,  //上层一个大  第二层五个小
//	BROADCAST_LAYOUT_SPLIT_ONLYONE = 0,   //一个大
//
//   //V2.0 http://wiki.vhallops.com/pages/viewpage.action?pageId=917942
//   // 旁路混流布局
//   CANVAS_LAYOUT_PATTERN_GRID_1 = 0,
//   CANVAS_LAYOUT_PATTERN_GRID_2_H = 1,
//   CANVAS_LAYOUT_PATTERN_GRID_3_E = 2,
//   CANVAS_LAYOUT_PATTERN_GRID_3_D = 3,
//   CANVAS_LAYOUT_PATTERN_GRID_4_M = 4,
//   CANVAS_LAYOUT_PATTERN_GRID_5_D = 5,
//   CANVAS_LAYOUT_PATTERN_GRID_6_E = 6,
//   CANVAS_LAYOUT_PATTERN_GRID_9_E = 7,
//   CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR = 8,
//   CANVAS_LAYOUT_PATTERN_FLOAT_2_1DL = 9,
//   CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL = 10,
//   CANVAS_LAYOUT_PATTERN_FLOAT_6_5D = 11,
//   CANVAS_LAYOUT_PATTERN_FLOAT_6_5T = 12,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1T4D = 13,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1D4T = 14,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1L4R = 15,
//   CANVAS_LAYOUT_PATTERN_TILED_5_1R4L = 16,
//   CANVAS_LAYOUT_PATTERN_TILED_6_1T5D = 17,
//   CANVAS_LAYOUT_PATTERN_TILED_6_1D5T = 18,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1L8R = 19,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1R8L = 20,
//   CANVAS_LAYOUT_PATTERN_TILED_13_1L12R = 21,
//   CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID = 22,
//   CANVAS_LAYOUT_PATTERN_TILED_9_1D8T = 23,
//   CANVAS_LAYOUT_PATTERN_TILED_13_1TL12GRID = 24,
//   CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID_E = 25,
//   CANVAS_LAYOUT_PATTERN_CUSTOM = 27
//};


//enum VhallLiveStreamType {
//	VhallLiveStreamType_Video = 2,
//	VHStreamType_Desktop = 3,
//	VHStreamType_MediaFile = 4,
//};

class IPlayMediaFileObj {
public:
   virtual bool IsPlayMediaFile() = 0;
   virtual bool IsPlayDesktopShare() = 0;
};

class DeviceChangedListener {
public:
	virtual void OnDevChanged(const QString &cameraDevId, const int cameraIndex, const QString &micDevId, const int micIndex, const QString &playerDevId, const int playerIndex, const int iLayoutMode) = 0;
};

class CustomDeleteLiveMsgEvent : public QEvent {
public:
   CustomDeleteLiveMsgEvent(QEvent::Type type, bool bExit, int reaSon ) :
		QEvent(type) {
        mbExit = bExit;
        mReason = reaSon;
	};
   bool mbExit;
   int mReason;
};


class CustomOnHttpResMsgEvent : public QEvent {
public:
   CustomOnHttpResMsgEvent(QEvent::Type type, QString url, QString msg, int code, QString uid, bool bNoticeMsg) :
      QEvent(type) {
      mUrl = url;
      mMsg = msg;
      mCode = code;
      mUid = uid;
      mbNoticeMsg = bNoticeMsg;
   };
   QString mUrl;
   QString mMsg;
   int mCode;
   QString mUid;
   bool mbNoticeMsg;
};

class CustomReConnectEvent : public QEvent {
public:
   CustomReConnectEvent(QEvent::Type type, void* ptr) :
      QEvent(type) {
      msg = ptr;
   };
   void* msg;
};

class CustomDevChangedEvent : public QEvent {
public:
	CustomDevChangedEvent(QEvent::Type type, QString cameraDevId, int cameraIndex, QString micDevId, int micIndex, QString playerDevId, int playerIndex) :
		QEvent(type) {
		this->cameraDevId = cameraDevId;
		this->cameraIndex = cameraIndex;
		this->micDevId = micDevId;
		this->micIndex = micIndex;
		this->playerDevId = playerDevId;
		this->playerIndex = playerIndex;
	};

	QString cameraDevId;
	int cameraIndex;
	QString micDevId;
	int micIndex;
	QString playerDevId;
	int playerIndex;
};


class VhallActiveJoinParam {
public:
	VhallActiveJoinParam() {
		bExitToLivelist = true;
		bIsLiving = false;
	}
	void Clear() {
		bExitToLivelist = true;
		device_audio = 0;
		device_video = 0;
		handsUp = 0;
		currPresenter.clear();
		bizRole = 0;
		reportUrl.clear();
		mainShow.clear();
		speaker_list.clear();
		invite_list.clear();
		against_url.clear();
		room_token.clear();
	}
	bool bExitToLivelist;
	bool bIsLiving;			//互动是否已经开始直播 true:已经开始
	int  device_audio;		//麦克风状态1开启，0关闭
	int  device_video;		//摄像头状态1开启，0关闭
	int  handsUp;           //是否能举手1可以，0不可以
	int  bizRole;			//sdk参数
	QString currPresenter;	//当前主讲人参会ID
	QString mainShow;			//当前主画面参会ID
	QString mainShowRole;	//当前主画面参会成员的角色
	QString speaker_list;	//上麦参会列表
	QString invite_list;		//已发邀请列表
	QString join_uid;			//当前用户id
	QString role_name;		//当前用户角色  当前参会角色host主持人，guest嘉宾，assistant助理，user观众
	QString against_url;		//sdk参数
	QString room_token;		//sdk参数
	QString reportUrl;		//sdk参数
	QString chatUrl;			//聊天 url
	QString thirdPushStreamUrl;
	QString hostNickName;
	QString hostJoinUid;
	QString msgToken;
	QString userNickName;
	QString imageUrl;		 //头像url
	QString streamName;	 //活动ID	：366476354
	QString webinarName;	 //活动名称：例如 Test直播互动
	QString pluginUrl;	 //文档插件url
   QString watchLayout; //画面布局。
};

class SpeakUser {
public:
	SpeakUser(){}
	SpeakUser(QString id, /*QString role, QString name,*/int audio = 1, int video = 1) {
		userId = id;
		//roleName = role;
		//nickName = name;
      nAudio = audio;
      nVideo = video;
	}
	QString userId;
	//QString roleName;
	//QString nickName;
   int nAudio;
   int nVideo;
	bool bOffLine = false;
	unsigned long long offLineTime;
};

