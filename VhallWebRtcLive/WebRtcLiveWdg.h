#ifndef H_WEBRTCLIVEWDG_H
#define H_WEBRTCLIVEWDG_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QDateTime>
#include "ui_WebRtcLiveWdg.h"
#include "../Unility/Unility.h"
#include "VhallUI_define.h"
#include "MixStream\MixStreamLayout.h"
#include "LeftToolBarWdg.h"
#include "FullWebView.h"
#include "LibCefViewWdg.h"

#define CLASS_OVER_NOTICE_SHEET     QStringLiteral("border-image: url(:/widget/img/widget/class_not_start.jpg);")
#define KICK_OUT_CLASS_NOTICE_SHEET QStringLiteral("border-image: url(:/widget/img/widget/Big_kickOut_Class.jpg);")

class  CScreenShareToolWgd;
class VedioPlayUI;
class VideoChoiceUI;
class VHFadeOutTipWidget;
class VhallIALiveSettingDlg;
class VhallRenderWdg;
class VhallWaiting;
//class VHRoomMemberWdg;
class ApplyJoinWdg;
class InviteJoinWidget;
class AudioSliderUI;
class ByPassRoomWdg;
class RenderManager;
class WebEngineViewWdg;
//class ChatWebView;
class NoticeShowWdg;
class MoveFadeoutTipWidget;
class DoubleRenderWdg;  
class MultiClassFloatWnd;
class TeacherRenderWdg;
class CMemberListWdg;
class NoticeSendWdg;
class CTabItemDlg;
class CPaintLabel;
class StuSignInDlg;
class TimerWdg;
class FloatRender;
class VhallIALiveSettingDlg;
class VhallRenderMaskWdg;
class DeviceTestingWdg;

enum PAGE_INDEX {
   page_Doc = 0,
   page_Whiteboard,
   page_CenterRender,
   page_otherMediaWdg,
   page_ClassStatus,
   page_DesktopScreen,
   page_kickOut,
   page_MediaFilePlay,
   page_InvStateWdg,
   page_SoftwareSharing,
};

enum TOOL_PAGE_INDEX {
   page_memberList = 0,
   page_chat,
   page_notice,
};


//
//enum TOST_PAGE_INDEX {
//	message = 0,
//	success,
//	warning,
//	error,
//};

class WebRtcLiveWdg : public CBaseWnd, public MsgCallbackDelegate
{
   Q_OBJECT

public:
   WebRtcLiveWdg(QWidget *parent = Q_NULLPTR);
   ~WebRtcLiveWdg();
   virtual void Notice(const CSTDMapParam &);
   virtual void Notice(const CSTDMapParamPtr &);
   virtual void GetParam(CSTDMapParam &);
   CREATE_WND();

   virtual void OnRecvMsg(std::string msg_name, std::string msg);
   virtual void OnWebViewLoadEnd();
   virtual void OnWebViewDestoryed();

   bool IsExitFloatRender(const QString& join_id);
   bool IsExitRender();
   bool IsUserSpeaking( QString strId );

   void FadeOutTip(QString str, int bTostIndex, bool bLongTimeShow = false);
   int iFadeOutTipWidth();

   DoubleRenderWdg*  GetRenderWdg();
   bool  IsExitAnserWebView();
   AnswerWebView*  AnswerWebViewDlg();
   VhallIALiveSettingDlg* GetPtrVhallIALiveSettingDlg();
   CMember WebRtcLiveWdg::GetMemberPersonalInfo(const QString& strUserId);
   int GetOnlineTotal();
   void NoticeShowNickName();
   bool IsDesktopShare();
   bool CheckAndStopScreenShare(const DoublePubStuChoiceUI::eChoiceType& type);
	bool IsTimerWdgVisable();
   bool isEixMediaScreen();
   bool isStudentSpeaking();
   int iRenderCount();
   bool  StartInvitationSpeech();
   bool HasPenAuth();// mstrIDPenAuth
   QString LocalStream();
   void deleteWhiteBoradWebView();
   void deleteDocWebView();
public slots:
   void showNormal();
   void slot_showInitPage();
protected:
   bool eventFilter(QObject *o, QEvent *e);
private slots:
   void slotChangeHandsUpState(const int& iState);
   void slot_LiveMixStreamWebFull();
   void slot_LiveMixStreamWebEnter();
   void slot_LiveMixStreamWebKeyEsc();
   void slot_changeMixStreamViewLayout();
   void slot_LiveMixStreamWebLeave();
	void slot_NoMicAndPlayer(const QString& msg);
   void slot_RestartSetLayout();
   void slot_ReSetMainStream();
protected:
   virtual void customEvent(QEvent* event);
   void closeEvent(QCloseEvent*);
   virtual void moveEvent(QMoveEvent *event);
   virtual void changeEvent(QEvent* event);
   void resizeEvent(QResizeEvent *event);

private:

   enum LAYOUT_POS {
      LAYOUT_POS_Normal = 0,
      LAYOUT_POS_RIGHT_TOP = 1,
      LAYOUT_POS_CENTER = 2,
   };


   void JsCallQtMsg(QString name, QString param);
   void HideVhallRenderMaskWdg();
   void recoveryOnlyVideoLayOut();
   void HideNickName();
	void setCurrentIndex(PAGE_INDEX index);
	
    void InitMicAndPlayDev();
    void InitLayOut();
    void InitToolWdg();
    void InitDocAndVideoRenderManager();
    void InitClassState();
    //void InitTabTool();
    void AddDocWebView();
    void AddWhiteBoradWebView();
    
    void AddAnswerWebView();
    void AddChatWebView();
    void AddRecordAndLiveStreamWebView();
	void HandleAuditInavPublish(QEvent* event);
	void HandlePublishFailed(QEvent* event);
	void HandleHandDelay(QEvent* event);
	void HandleStartCountdown(QEvent* event);
	void HandleStartAnswerTool(QEvent* event);
	void HandleAnswerDetail(QEvent* event);
	void HandleStartAnswer(QEvent* event);
	void HandleStartSign(QEvent* event);
	void HandleSignIn(QEvent* event);
	void HandleSignStop(QEvent* event);
	void HandleKickInavStream(QEvent* event);
	void HandleKickOutList(QEvent* event);
	void HandleForbiddenList(QEvent* event);
	void HandleHandUpList(QEvent* event);
	void HandleOnLineList(QEvent* event);
	void HandleOpenCaptureCallback(QEvent* event);
	void HandlePreOpenCaptureCallback(QEvent* event);
    void HandleSubScribStream(QEvent* event);
	void HandleRemoteStream(QEvent* event);
    void HandleOpenClassSubScribStream(QEvent* event);
	void HandleGetDesktopStream(QEventStream* msgEvent);
    void HandleSetLayoutCameraAuxiliary(QEvent* event);
    void HandleCloseFloatRender(QEvent* event);
    void HandleWebRtcRoomMixedReady(QEvent* event);
    void HandleShowNoticeMsg(QEvent* event);
    void HandleShowFadOutTipsMsg(QEvent* event);
    void HandleStartMixStream(QEvent* event);
    void HandleStartSetMainStream(QEvent* event);
    void HandleRecvFlashMsg(QEvent* event);
    void HandleStreamAdd(QEvent* event);
    void HandleRecvSetBrush(QEvent* event);
    void HandleRecvMediaMsg(QEvent* event);
    void HandleRecvAnswerMsg(QEvent* event);
    void HandleWebRtcRoomConnetSuc(QEvent* event);
    void HandleWebRtcRoomConnectErr();
    void HandleStartAndStopClass(QEvent* event);
    void HandleRecvAskForPublish(QEvent* event);
    void HandleSendApplyPublishMsg(QEvent* event);
    void HandleRecvDownAllUserPublishMsg();
    void HandleRecvApplyInavPublishMsg(QEvent* event);
	void HandleRecvSignUsers(QEvent* event);
    void HandleGetOnLineUserList(QEvent *event);
    void HandleCloseWnd(QEvent *event);
    void HandleEnableHandsUp(QEvent* event);
    void HandleSendAgreePublishMsg(QEvent* event);
    void HandleSendInvitePublishMsg(QEvent* event);
    void HandleUserPublishCallback(QEvent* event);
    void HandleUpdatePublishAbility(QEvent* event);
	 void HandleInviteDoubleVrtcCallback(QEvent* event);
    void HandleUserPublishFailedCallback(QEvent* event);
    void HandleOnRecvKickOutRoomMsg(QEvent* event);
    void HandleRecvOnUserPublishCallback(QEvent* event);
    void JustFadoutTips(QEvent* event);
	void HandleCanclePublish(QEvent* event);
    void HandleSendKickPublishMsg(QEvent* event);
    void HandleSendSetBrush(QEvent* event);
    void HandleSendForbiddenChat(QEvent* event);
    void HandleSendForbiddenAllChat(QEvent* event);
    void HandleOnRecvForbiddenChatMsg(QEvent* event);
    void HandleOnRecvForbiddenAllChatMsg(QEvent* event);
    void HandleOnRecvUpdatePublishAbility(QEvent* event);
    void HandleAllUnPublish(QEvent* event);
    void HandleMuteMic(QEvent* event);
    void HandleRecvMuteMic(QEvent* event);
    void HandleOperaMuteAllMic(QEvent* event);
    void HandleRecvAllMuteMic(QEvent* event);
    void HandleMuteVideo(QEvent* event);
    //void HandleMuteMicAll(QEvent* event);
    void HandleOnRecvOpenHandUpMsg(QEvent* event);
    void HandleUserLineState(QEvent* event);
    void HandleStartDoubleVrtc(QEvent* event);
    void HandleinviteDoubleVrtc(QEvent* event);
    void HandleInviteScreenSharing(QEvent* event);
    void  HandleStopDesktopSharingVrtc(QEvent* event);
	void  HandleWhiteListUserJoinRoom(QEvent* event);
    void  HandleEndDoubleVrtc(QEvent* event);
    void HandleDoubleVrtUsers(QEvent* event);
    void HandleGetDoubleVrtUsers(QEvent*vent);
    void HandleRequestStartDoubleVrtc(QEvent*vent);

   void HandlePlayCameraAuxiliary();
   void HandlePushLocalStreamError(QEvent* event);
   void HandleLocalStreamIdChanged(QEvent *event);
   void HandleStartLocalCapture();
   void HandlePushLocalStreamSuc(QEvent* event);
   void HandlePushCameraStreamSuc(QEvent* event);
   void HandlePushAuxiliaryCameraStreamSuc(QEvent* event);
   //void HandlePushMediaFileStreamSuc(QEvent* event);
   void HandlePushDesktopStreamSuc(QEvent* event);
   void CreateScreenShareWgd();
   void HandleGetSoftwareStream(QEventStream* msgEvent);
   void HandleGetMediaFileStream(QEventStream* msgEvent);
   void HandleChangeToolPage(QEvent* msgEvent);
   void HandleKickOut(QEvent* msgEvent);
   void HandleCancelHand(QEvent* msgEvent);
   void HandleRewardStu(QEvent*event);
   void HandleChangeBigScreen(QEvent* event);
   bool StartPushStream(int & iState);
   void HandleStopLiving();
   void RemoveSamllRenderFromTeacherWnd();
   void ChangePublishState(bool publish);
   void handleAVCapture(QEventStream* msgEvent);
   void handleOtherMedia(QEventStream* msgEvent);
   void handleOtherMediaRemote(QEventStream* msgEvent);
   //void handleRemoveRemoteStream(QEventStream* msgEvent);
   void RenderSmallClass(QEventStream *msgEvent);

   //VideoProfileIndex GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType = -1);
private slots:
   void slot_NoticeShowNickName();
   void slot_NoticeShowMsgTimeout();
   bool slot_CloseClicked();
   void slot_MinClicked();
   void slot_TurnToLogin();
   void slot_MaxClicked();
   //void slot_BtnScreenClicked();
   void slot_btnDoc();
   void slot_Whiteboard(bool);
   void slot_Answer();
   void slot_BtnStopScreenClicked();
   void slot_closeMultiFloat(QString strId);
   void slot_closeFloat(QString strId);
   void slot_DesktopSharing();
   void slot_interactionTool();
   void slot_CameraDoublePush(bool);
   void slot_StudentDoublePush();
   void slot_Media();
   void slot_DoublePush();

   void slot_interactionItemClicked(const int& iOpType);
   void slotStopReDoublePush();
   void slot_DocLoadFinished();

   void slot_tabDrag(int index, QPoint point);  //响应拖动动作的槽函数
   void slot_closeTab(int);                    //关闭tab的槽函数
   void slot_tabBarDoubleClicked();            //响应双击弹回的槽函数
   void slot_changed(int index);
   void slot_DragChat(const QPoint& posShow);
   void slot_DragMember(const QPoint& posShow);
   void slot_OpMediaType(const int& iOptype, const QPoint& posShow);
   void slot_OnClickedCamera(bool bOpen);
   void slot_JsCallQtMsg(const int& membercount);
   void slot_StartStuDoublePush(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);
	void slot_StopDoublePush();
   void slotCheckUserIsPublish();

   void slot_KickUserPublish(QString strId);
   void slot_AuditPublish(QString strId);
   void slot_AskforPublish(QString strId);
   void slot_ApplyPublishTimeOut();
private:
   void SetDesktopMeidaMixStreamLayout(VHStreamType streamType);
   void LiveTypeChangeToLogout();

   void DisConnectRoom(std::string userId);
   //void addRemoteStream(std::string strStreamId);
   //void removeRemoteStream(std::string strStreamId);

   void removeApplyPublishUser(QString strUser);
   bool addApplyPublishUser(QString strUser);
   void TestWidgetLayout();
   void WritWidgetInfo(QWidget* pWidget);

	void QuitLiveWdg();
   void initLeftToolBarOptype();
   void AuxiliaryShowTeacherWdg();
	void showTimeCountDown();
   void showAuxiliaryWdg(QEventStream* msgEvent, DoublePubStuChoiceUI::eChoiceType type);
	void createTimerWdg();
	void ShowDragDlg(QWidget *draged, QPoint posShow);
	void clostEjectTabItem(CTabItemDlg *widget);
	void creatTab(const bool& showMember, const bool& showNotic);
	void createTabLabel();
	std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
	void showMediaFilePlayWdg(const bool&  bShow);
	//void SetPlayDeskTopAndMediaFileState(bool playing);
	void StopShowMediaVideo();
	int GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos);
	void ChangeNoticeShow(bool bCurTool = true);


   enum eToolPage {
      eToolPage_Doc = 0,
      eToolPage_Board = 1,
      eToolPage_Answer_Open,
      eToolPage_Answer_Close
   };

   Ui::WebRtcLiveWdg ui;
   RenderManager* mRenderManager = nullptr;

   //WebEngineViewWdg* mDocWebEngineViewWdg = nullptr;
   //WebEngineViewWdg* mWhiteBoardWebEngineViewWdg = nullptr;
   //WebEngineViewWdg* mRecordAndLiveStreamWebView = nullptr;//回放
   //WebEngineViewWdg* mAnswerWebEngineViewWdg = nullptr;
   ///////////////////////////Tab 聊天、成员、公告/////////////////////////
   //WebEngineViewWdg* mChatWebEngineViewWdg = nullptr;

   LibCefViewWdg* mDocCefViewWdg = nullptr;
   LibCefViewWdg* mWhiteBoardCefViewWdg = nullptr;
   LibCefViewWdg* mRecordAndLiveStreamCefViewWdg = nullptr;//回放 / 旁路（混流）
   LibCefViewWdg* mAnswerCefViewWdg = nullptr;
   /////////////////////////Tab 聊天、成员、公告/////////////////////////
   LibCefViewWdg* mChatCefViewWdg = nullptr;

   NoticeSendWdg* mNoticeSendWdg = nullptr;
   CMemberListWdg *mMemberListWdg = nullptr;

   CPaintLabel* mpPaintLable = nullptr;
   CPaintLabel* mpPaintLableL = nullptr;
   CTabItemDlg* mpEjectTabItem = nullptr;
   bool mbEjectNoticeSend = false;
   //CTabItemWgd* mpNoticeTabItem = nullptr;
   //CTabItemWgd* mpMemberListTabItem = nullptr;
   /////////////////////////Tab 聊天、成员、公告/////////////////////////
   NoticeShowWdg* mNoticeShowWdg = nullptr;
   QTimer mNoticeShowMsgTimer;

   MixStreamLayout mMixStreamLayout;
   LeftToolBarWdg* mLeftToolBarWdg = nullptr;

   VHFadeOutTipWidget *mpFadeOutTipWidget[3];// nullptr;
   MoveFadeoutTipWidget* mpMoveFadeOutTipWdg = nullptr;
   DoubleRenderWdg* mDoubleRenderWdg = nullptr;
   CScreenShareToolWgd* m_pScreenShareToolWgd = NULL;
   bool mbFirstConnectWebRtcServer;
   VhallIALiveSettingDlg* mVhallIALiveSettingDlg = nullptr;
   QString mTeacherJoinId;   //小班课一直为老师的id，公开课为主讲人的id
   QString mTeacherJoinNickName;

   TeacherRenderWdg* mTeacherRender = nullptr;  //大画面窗体

   QMutex mStreamMutexCS;
   QString mLocalStream;

   QString mLocalAuxiliaryStreamId;
   QString mDeskTopStream;//桌面共享流id、软件共享流id、插播流id
  //QString mMediaStream;
	QString mDoublePushNickName;
   bool mMixedReady = false;
   FullWebView* mpFullWebView = nullptr;
   bool mbClickedWhiteBoard = false;

   bool mbIsMaxSize = false;
   QRect mNormalRect;
   StuSignInDlg* mpStuSignInDlg = nullptr;
   TimerWdg* mpTimerWdg = nullptr;
   int miOldStackedTab = 0;
   int mTotal = 0;         //总人数
   QList<QWidget*> mSunWndList;
   //QMap<QString, FloatRender*> m_pMapFloatRender;

   //VhallRenderMaskWdg *mpVhallRenderMaskWdg = nullptr;
   //PAGE_INDEX mOldIndex = page_Doc;
   //QTimer* mUserNickNameShowTimer = nullptr;
   QStringList mRecivePubIdList;
   ChangeView* mChangeBroadCastLayout = nullptr;

   MultiClassFloatWnd* mMultiClassFloatWnd = nullptr;
   MultiClassFloatWnd* mFloatWnd = nullptr;

   //QMap<QString, MultiClassFloatWnd*> mapMultiClassFloatWnd;
   bool mbQuitAfterUpPublish = false;
   int mRestartSetLayoutType;
   bool mbRestartSetLayout = false;
   QString mStrStreamId = "";
   //bool mDecTestResult = false;  //检测结果 通过true
   QTimer* mpReSetMainStreamTimer = nullptr;

   QTimer* mpApplyPublishTime = nullptr; //邀请上麦用户列表定时管理器
   QMap<QString, QDateTime> mstrApplyPublishUsers;//邀请上麦用户列表定时管理器
   DeviceTestingWdg* mpDeviceTestWdg = nullptr; 
   QString mstrIDPenAuth = "";//当前拥有画笔权限id

   enum eMultiClassFloatType {
      eMultiClassFloatType_Multi = 0,
      eMultiClassFloatType_Float,
      eMultiClassFloatType_Null
   };

   eMultiClassFloatType mFloatType = eMultiClassFloatType_Null;
};
#endif // !H_WEBRTCLIVEWDG_H