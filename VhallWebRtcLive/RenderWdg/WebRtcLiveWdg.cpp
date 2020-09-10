#include "WebRtcLiveWdg.h"
#include "WebRtcLiveControl.h"
#include "./Setting/VhallIALiveSettingDlg.h"
#include "CScreenShareToolWgd.h"
#include "vhfadeouttipwidget.h"
#include "VhallWebRtcLive.h"
#include "../Unility/CustomEventType.h"
#include "./RenderWdg/VhallRenderWdg.h"
#include "../Unility/Unility.h"
#include "./InviteJoinWdg/ApplyJoinWdg.h"
#include "./InviteJoinWdg/InviteJoinWidget.h"
#include "LiveEventInterface.h"
#include "vh_room_common.h"
#include "RenderWdg/RenderManager.h"
//#include "WebView/WebEngineViewWdg.h"
//#include "WebView/ChatWebView.h"
#include "RenderWdg/DoubleRenderWdg.h"
#include "TeacherRenderWdg.h"
#include <QDateTime>
#include <QApplication> 
#include <QDesktopWidget> 
#include <QScreen> 
#include <QFileDialog> 
#include <QScrollBar> 
#include <QPoint>
#include <QDebug>  
#include "pathmanager.h"
#include "class.pub.Const.h"
#include "ConfigSetting.h"
#include "..\memberList\CMemberListWdg.h"
#include "..\Notice\NoticeSendWdg.h"
#include "./RenderWdg/FloatRender.h"
#include "./Notice/NoticeShowWdg.h"
#include "movefadeouttipwidget.h"
#include "..\TabWidget\CTabItemDlg.h"
#include "..\TabWidget\CPaintLabel.h"
#include "VhallIALiveSettingDlg.h"
#include "TimerWdg.h"
#include "MultiClassFloatWnd.h"
#include "VideoChoiceUI.h"
#include "StuSignInDlg.h"
#include "TipsWdg.h"
#include "MediaPlayWebView.h"
#include "AnswerWebView.h"
#include "FloatRender.h"
#include "DeviceTestingWdg.h"

#define SELECT_STYLE    "QPushButton{font: 14px \"΢���ź�\";color:rgb(82, 204, 144);border:0px; background-color:rgb(64, 66, 72); border-top-color:#52CC90;border-top-width: 2px;border-style:solid;}"
#define DEFAULT_STYLE   "QPushButton{font: 14px \"΢���ź�\";color:rgb(141, 142, 143);border:0px;background-color:rgb(52, 54, 58);}"

extern ToolManager *globalToolManager;

#define PLAYUI_PLAYSTATE
#define PLAYUI_PLAYSTATE_STOPPING  -2
#define PLAYUI_PLAYSTATE_FAIL      -1
#define PLAYUI_PLAYSTATE_NOTHING   0
#define PLAYUI_PLAYSTATE_OPENING   1
#define PLAYUI_PLAYSTATE_BUFFERING 2
#define PLAYUI_PLAYSTATE_PALYERING 3
#define PLAYUI_PLAYSTATE_PAUSED    4
#define PLAYUI_PLAYSTATE_STOP      5
#define PLAYUI_PLAYSTATE_END       6
#define PLAYUI_PLAYSTATE_ERROR     7
#define ENABLESPEAKTIME 5000

#define STRCHAT QStringLiteral("����")
#define STRMEMBER QStringLiteral("��Ա")
#define STRNOTICE QStringLiteral("����")

/*������*/
CREATE_WND_FUNCTION(WebRtcLiveWdg);
WebRtcLiveWdg::WebRtcLiveWdg(QWidget *parent) :
   CBaseWnd(parent)
   , mRenderManager(nullptr)
   //, mChatWebEngineViewWdg(nullptr)
   //, mDocWebEngineViewWdg(nullptr)
   //, mWhiteBoardWebEngineViewWdg(nullptr)
   , mbIsMaxSize(false)
   , mLeftToolBarWdg(nullptr)
   , mbFirstConnectWebRtcServer(true)
{
   ui.setupUi(this);
   connect(&mNoticeShowMsgTimer, SIGNAL(timeout()), this, SLOT(slot_NoticeShowMsgTimeout()));
   // connect(ui.pushButton_TurnToLogin, SIGNAL(clicked()), this, SLOT(slot_TurnToLogin()));
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
   globalToolManager->GetDataManager()->WriteLog("WebRtcLiveWdg::WebRtcLiveWdg");
   ui.widget_main_title->hide();

   globalToolManager->GetDataManager()->SetMyCameraOpen(true);
   globalToolManager->GetDataManager()->SetMultiClassOtherUserCameraOpen(true);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   mMemberListWdg = new CMemberListWdg(ui.tabWidget); 
   if (mMemberListWdg)
   {
      mMemberListWdg->hide();
   }

   connect(ui.pushButton_change_main, SIGNAL(clicked()), this, SLOT(slot_changeMixStreamViewLayout()));
   connect(ui.classRoomTitleWd, &VhallLiveTitle::sig_close, this, &WebRtcLiveWdg::slot_CloseClicked);
   connect(ui.classRoomTitleWd, &VhallLiveTitle::sig_normal, this, &WebRtcLiveWdg::slot_MaxClicked);
   connect(ui.classRoomTitleWd, &VhallLiveTitle::sig_min, this, &WebRtcLiveWdg::slot_MinClicked);

   connect(ui.invStateWdg, &InvitationStateWdg::sig_inviteDoubleVrtc,  this, &WebRtcLiveWdg::slot_StartStuDoublePush);
   connect(ui.invStateWdg, &InvitationStateWdg::sig_Close, this, &WebRtcLiveWdg::slot_showInitPage);
   mbIsMaxSize = false;
   mNormalRect = this->geometry();
   if (QApplication::desktop()->availableGeometry().width() < mNormalRect.width() ||
      QApplication::desktop()->availableGeometry().height() < mNormalRect.height()) {
      this->setGeometry(QApplication::desktop()->availableGeometry());
   }
   mNormalRect = this->geometry();

   //setFixedSize(QSize(1140, 680));
   ui.widget_right_tool->setFixedWidth(320);
   mpFullWebView = new FullWebView();
   if (mpFullWebView) {
      mpFullWebView->hide();
      connect(mpFullWebView, SIGNAL(sig_exitFull()), this, SLOT(slot_LiveMixStreamWebKeyEsc()));//mMemberListWdg
   }

   ui.classRoomTitleWd->setFixedHeight(50);
   installEventFilter(this);

   //mChangeBroadCastLayout = new ChangeView(this);
   //if (mChangeBroadCastLayout) {
   //   mChangeBroadCastLayout->hide();
   //   connect(mChangeBroadCastLayout,SIGNAL(sig_changeLayout()),this,SLOT(slot_changeMixStreamViewLayout()));
   //}
   //if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) 
   {
      mMultiClassFloatWnd = new MultiClassFloatWnd();
      connect(mMultiClassFloatWnd, &MultiClassFloatWnd::sig_close,this, &WebRtcLiveWdg::slot_closeMultiFloat);
      connect(mMultiClassFloatWnd, &MultiClassFloatWnd::sig_close, this, &WebRtcLiveWdg::slot_closeFloat);
   }
   ui.otherMediaWdg->SetWidthHeightRatio(16, 9);
   mMixStreamLayout.SetRetLiveWdg(this);

   for (int i = 0; i < 3; i++) {
      mpFadeOutTipWidget[i] = nullptr;
   }
}

WebRtcLiveWdg::~WebRtcLiveWdg()
{
   if (mpApplyPublishTime) {
      if (mpApplyPublishTime->isActive()) {
         mpApplyPublishTime->stop();
      }
      mpApplyPublishTime->deleteLater();
      mpApplyPublishTime = nullptr;
   }
   mstrApplyPublishUsers.clear();
   if (mRenderManager) {
      delete mRenderManager;
      mRenderManager = nullptr;
   }
   if (mDocCefViewWdg) {
      delete mDocCefViewWdg;
      mDocCefViewWdg = nullptr;
   }

   if (mpReSetMainStreamTimer)
   {
      if (mpReSetMainStreamTimer->isActive()) {
         mpReSetMainStreamTimer->stop();
      }
      mpReSetMainStreamTimer->deleteLater();
      mpReSetMainStreamTimer = nullptr;
   }
   if (mChatCefViewWdg) {
      delete mChatCefViewWdg;
      mChatCefViewWdg = nullptr;
   }

   if (mWhiteBoardCefViewWdg) {
      delete mWhiteBoardCefViewWdg;
      mWhiteBoardCefViewWdg = nullptr;
   }

   if (mRecordAndLiveStreamCefViewWdg) {
      delete mRecordAndLiveStreamCefViewWdg;
      mRecordAndLiveStreamCefViewWdg = nullptr;
   }

   if (mAnswerCefViewWdg) {
      delete mAnswerCefViewWdg;
      mAnswerCefViewWdg = nullptr;
   }

   if (mMultiClassFloatWnd) {
      delete mMultiClassFloatWnd;
      mMultiClassFloatWnd = nullptr;
   }

   if (mFloatWnd) {
      delete mFloatWnd;
      mFloatWnd = nullptr;
   }


   if (NULL != m_pScreenShareToolWgd)
   {
      delete m_pScreenShareToolWgd;
      m_pScreenShareToolWgd = NULL;
   }

   /*QMap<QString, FloatRender*>::iterator iter = m_pMapFloatRender.begin();
   while (iter != m_pMapFloatRender.end())
   {
      FloatRender* pRender = iter.value();
      delete pRender;
      pRender = NULL;
      iter++;
   }*/
}


//FloatRender* WebRtcLiveWdg::GetFloatRender(QString join_id)
//{
//   QMap<QString, FloatRender*>::iterator iter = m_pMapFloatRender.find(join_id);
//   FloatRender* pRender = NULL;
//   if (iter == m_pMapFloatRender.end()){
//      pRender = new FloatRender(this, 1);
//	  pRender->hide();
//      m_pMapFloatRender.insert(join_id, pRender);
//   }
//   else{
//      pRender = iter.value();
//   }
//   return pRender;
//}

VhallIALiveSettingDlg* WebRtcLiveWdg::GetPtrVhallIALiveSettingDlg()
{
   if (nullptr == mVhallIALiveSettingDlg)
   {
      mVhallIALiveSettingDlg = new VhallIALiveSettingDlg(this);
      //if(nullptr!=mRenderManager)
      connect(mVhallIALiveSettingDlg, &VhallIALiveSettingDlg::sigNoMicAndPlayer, this ,&WebRtcLiveWdg::slot_NoMicAndPlayer);
   }
   return mVhallIALiveSettingDlg;
}

void WebRtcLiveWdg::slot_NoMicAndPlayer(const QString& msg) {
	if (msg == "1") {
		if (mLeftToolBarWdg) {
			mLeftToolBarWdg->SetscreenChoiceDlgState(true);
		}
	}
	else{
	   TipsWdg* tips = new TipsWdg(this);
	   if (tips) {
	   	CSTDMapParam param;
	   	param[TipsMsg] = msg;
	   	param[ShowCancelButton] = PARAM_TRUE;
	   	tips->Notice(param);
	   	int accept = tips->exec();
	   	if (accept == QDialog::Accepted) {
	   		if (mLeftToolBarWdg) {
	   			mLeftToolBarWdg->SetscreenChoiceDlgState(true);
	   		}
	   	}
	   	else {
	   		tips->hide();
	   		delete tips;
	   		tips = NULL;
	   	}
	   }
	}
}

void WebRtcLiveWdg::showNormal()
{
   CBaseWnd::showNormal();
}

bool WebRtcLiveWdg::IsExitFloatRender(const QString& join_id)
{
   //QMap<QString, FloatRender*>::iterator iter = m_pMapFloatRender.find(join_id);
   //return iter != m_pMapFloatRender.end();
   bool IsExit = false;

   if (mMultiClassFloatWnd ) {
      QString strUid = mMultiClassFloatWnd->strUserId().trimmed();
      if (!strUid.isEmpty()) {
         IsExit |= (0==join_id.compare(strUid));
      }
   }

   if (mFloatWnd) {
      QString strUid = mFloatWnd->strUserId().trimmed();
      if (!strUid.isEmpty()) {
         IsExit |= (0 == join_id.compare(strUid));
      }
   }

   return IsExit;
}

bool WebRtcLiveWdg::IsExitRender()
{
   int iFloatCout = 0/*m_pMapFloatRender.count()*/;
   if (mMultiClassFloatWnd && mMultiClassFloatWnd->isVisible()) {
      if (mMultiClassFloatWnd->RenderWidget() != mTeacherRender) {
         iFloatCout++;
      }
   }

   if (mFloatWnd && mFloatWnd->isVisible()) {
      if (mFloatWnd->RenderWidget() != mTeacherRender) {
         iFloatCout++;
      }
   }

   int iRenderCout = 0;
   if (nullptr != mRenderManager){
      iRenderCout = mRenderManager->iRenderSize();
   }
   return (iFloatCout + iRenderCout) >0;
}

bool WebRtcLiveWdg::IsUserSpeaking(QString strId)
{
   bool bRef = false;
   if (nullptr != mMemberListWdg) {
      bRef = mMemberListWdg->IsUserSpeaking(strId);
   }
   

   return bRef;
}

void WebRtcLiveWdg::Notice(const CSTDMapParam &mapParam) {

   std::map<QString, QString>::const_iterator iter = mapParam.find(DEV_TEST);
   if (iter != mapParam.end()) {

      //�豸���
      if (eClassType_Classing != globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type) {
         bool mbDevTesting = true;
         /*CBaseWnd* deciceWnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_DECICE_TESTING);
         if (deciceWnd) {
            deciceWnd->exec();
         }*/
         if (nullptr == mpDeviceTestWdg) {
            mpDeviceTestWdg = new DeviceTestingWdg(this);
         }

         if (mpDeviceTestWdg) {
            //mpDeviceTestWdg->raise();
            mpDeviceTestWdg->show();
            mpDeviceTestWdg->activateWindow();
            //mpDeviceTestWdg->raise();
         }

      }
      return;
   }

   iter = mapParam.find(START_WEB_RTC_LIVE);
   if (iter != mapParam.end()) {
	   InitMicAndPlayDev();
	   InitLayOut();

      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if ( QString::fromStdString( baseInfo.mExamInitInfo.strExaming).toLower().contains( "yes")
        ) {//�жϵ�ǰ�Ƿ���ing
         int inaireStatus = baseInfo.mExamInitInfo.naire_status;
         int iUserStatus = baseInfo.mExamInitInfo.user_status;
         if (1==inaireStatus 
            || (2 == inaireStatus &&  (1==iUserStatus || 2==iUserStatus))
            || (3 == inaireStatus && (1 == iUserStatus || 2 == iUserStatus))) {
            mLeftToolBarWdg->StuStartExamination();
         } 
      }

      if (baseInfo.mWebinarInfo.answing_question_id > 0) {//�жϵ�ǰ�Ƿ����ing
         if (mLeftToolBarWdg)
         {
            AnswerWebView* wnd = mLeftToolBarWdg->AnswerWebViewDlg();
            //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
            if (wnd) {
               CSTDMapParam param;
               //param[MEDIA_FILE_CTRL_MSG] = method;
               //globalToolManager->GetDataManager()->WriteLog("%s msg %s", __FUNCTION__, method.toLatin1().data());
               wnd->Notice(param);
               //if (type == "*push_question" || type == "*publish_answer" || type == "*new_push_question" || type == "*new_publish_answer") 
               {
                  wnd->show();
               }
            }
         }
      }

      if (baseInfo.mWebinarInfo.mInterstitialInfo.interstitial > 0) {//�жϵ�ǰ�Ƿ�岥ing
         if (mLeftToolBarWdg)
         {
            MediaPlayWebView* wnd = mLeftToolBarWdg->MediaPlayWebViewDlg();

            //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
            if (wnd) {
               CSTDMapParam param;
               //param[MEDIA_FILE_CTRL_MSG] = method;
               wnd->Notice(param);
               wnd->show();
            }
         }
      }

      if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
         if (0==baseInfo.mDocInfo.doc_mode) {//�жϵ�ǰ�����ڲ岥�ĵ�
            slot_btnDoc();
         }
         else if (1 == baseInfo.mDocInfo.doc_mode){//��ǰ���ڲ岥�װ�
            slot_Whiteboard(true);
         }
      }

   }

   iter = mapParam.find(TipsMsg);
   if (iter != mapParam.end()) {
      QString value = iter->second;
      FadeOutTip(value, Tost_Message, false);
      return;
   }

   iter = mapParam.find(STOPSHOWMEDIAVIDEO);
   if (iter != mapParam.end()) {
      StopShowMediaVideo();
      return;
   }

   std::map<QString, QString>::const_iterator iterJoinId = mapParam.find(USER_ID);
   iter = mapParam.find(TEACHER_VIEW);
   if (iter != mapParam.end() && iterJoinId != mapParam.end()) {
      if (iterJoinId->second == globalToolManager->GetDataManager()->GetTeacherJoinId()) {
         ui.widget_leftTopRender->hide();
      }
   }
}

void WebRtcLiveWdg::InitMicAndPlayDev() {
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
   QString msConfPath = globalToolManager->GetConfigPath();
   QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
   QString configPlayId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, "");

   bool findConfigMic = false;
   int micIndex = 0;
   std::string fristMicId;
   std::list<vhall::AudioDevProperty> micList;
   globalToolManager->GetClassSDK()->GetMicDevices(micList);
   std::list<vhall::AudioDevProperty>::iterator iterMic = micList.begin();
   while (iterMic != micList.end()) {
      vhall::AudioDevProperty audioInfo = (*iterMic);
      if (iterMic == micList.begin()) {
         fristMicId = QString::fromStdWString(audioInfo.mDevGuid).toStdString();
         micIndex = audioInfo.mIndex;
         //ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdString( fristMicId ));
      }
      if (configMicId == QString::fromStdWString(audioInfo.mDevGuid)) {
         findConfigMic = true;
         micIndex = audioInfo.mIndex;
         break;
      }
      iterMic++;
   }

   if (findConfigMic) {
      globalToolManager->GetClassSDK()->SetUsedMic(micIndex, configMicId.toStdString(), L"");
      globalToolManager->GetClassSDK()->SetCurrentMicVol(100);
   }
   else if (micList.size() > 0) {
      globalToolManager->GetClassSDK()->SetUsedMic(micIndex, fristMicId, L"");
      globalToolManager->GetClassSDK()->SetCurrentMicVol(100);
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, QString::fromStdString(fristMicId));
   }

   ///////////////////////////////////////
   bool findConfigPlayer = false;
   int playerIndex = 0;
   std::string fristPlayId;
   std::list<vhall::AudioDevProperty> playList;
   globalToolManager->GetClassSDK()->GetPlayerDevices(playList);
   std::list<vhall::AudioDevProperty>::iterator iterPlay = playList.begin();
   while (iterPlay != playList.end()) {
      vhall::AudioDevProperty audioInfo = (*iterPlay);
      if (iterPlay == playList.begin()) {
         fristPlayId = QString::fromStdWString(audioInfo.mDevGuid).toStdString();
         playerIndex = audioInfo.mIndex;
      }
      if (configPlayId == QString::fromStdWString(audioInfo.mDevGuid)) {
         findConfigPlayer = true;
         playerIndex = audioInfo.mIndex;
         break;
      }
      iterPlay++;
   }

   if (findConfigPlayer) {
      globalToolManager->GetClassSDK()->SetUsePlayer(playerIndex, configPlayId.toStdString());
      globalToolManager->GetClassSDK()->SetCurrentPlayVol(100);
   }
   else if (playList.size() > 0) {
      globalToolManager->GetClassSDK()->SetUsePlayer(playerIndex, fristPlayId);
      globalToolManager->GetClassSDK()->SetCurrentPlayVol(100);
      ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PLAY_OUT_DEV_ID, QString::fromStdString(fristPlayId));
   }
}

void WebRtcLiveWdg::GetParam(CSTDMapParam &mapParam) {

   std::map<QString, QString>::iterator iterFunction = mapParam.find(Str_Function);
   if (iterFunction != mapParam.end()) {
      if (iterFunction->second == Str_Function_GetStartTime) {
         mapParam[Str_Start_Time] = QString::number(ui.classRoomTitleWd->GetStartTime());
      }
      else if (iterFunction->second == Str_Function_UserLine) {
         mapParam[Str_OnLineNum] = QString::number(mMemberListWdg->GetOnLineMemberSize());
      }
   }
   else {
      std::map<QString, QString>::iterator iterWndMangerWidth = mapParam.find(WND_MANAGER_WIDTH);
      if (iterWndMangerWidth != mapParam.end()) {
         int width = 0;
         if (mRenderManager) {
            width = mRenderManager->GetListWidgetWidth();
         }
         mapParam[WND_MANAGER_WIDTH] = QString::number(width);
         return;
      }
      std::map<QString, QString>::iterator iterCenterWidth = mapParam.find(WND_CENTER_WIDTH);
      if (iterCenterWidth != mapParam.end()) {
         int width = ui.widget_centerWidget->width();
         int height = ui.widget_centerWidget->width();
         mapParam[WND_CENTER_WIDTH] = QString::number(width);
         mapParam[WND_CENTER_HEIGHT] = QString::number(height);
         return;
      }

      std::map<QString, QString>::iterator iterUserId = mapParam.find(USER_ID);
      std::map<QString, QString>::iterator iterUserLineState = mapParam.find(USER_LINE_STATE);
      if (iterUserId != mapParam.end() && iterUserLineState != mapParam.end()) {
         int role = mMemberListWdg->getRolType(iterUserId->second);
         if (role == JOIN_ROLE_None) {
            iterUserLineState->second = QString::number(0);
         }
         else {
            iterUserLineState->second = QString::number(1);
         }
      }
   }

   std::map<QString, QString>::iterator iterTrophy = mapParam.find(TROPHYREWARDNUM);
   if (iterTrophy != mapParam.end()) {
      iterTrophy->second = QString::number(ui.classRoomTitleWd->iTrophyCount());
   }
}

void WebRtcLiveWdg::OnRecvMsg(std::string msg_name, std::string msg)
{
   CefMsgEvent *cefMsg = new CefMsgEvent(CustomEvent_LibCefMsg, QString::fromStdString(msg_name), QString::fromStdString(msg));
   QApplication::postEvent(this, cefMsg);
}

void WebRtcLiveWdg::OnWebViewLoadEnd()
{

}

void WebRtcLiveWdg::OnWebViewDestoryed()
{

}

void WebRtcLiveWdg::StopShowMediaVideo() {
   globalToolManager->GetDataManager()->WriteLog("%s enter\n", __FUNCTION__);
   showMediaFilePlayWdg(false);
   //SetPlayDeskTopAndMediaFileState(false);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mMixStreamLayout.SetMixStreamLayout();
   }
   globalToolManager->GetDataManager()->WriteLog("%s leaveHandleConfigMediaFileMainView\n", __FUNCTION__);

}
void WebRtcLiveWdg::Notice(const CSTDMapParamPtr &mapParam) {

}

DoubleRenderWdg*  WebRtcLiveWdg::GetRenderWdg() {
   return mDoubleRenderWdg;
}

AnswerWebView*  WebRtcLiveWdg::AnswerWebViewDlg()
{
   return  mLeftToolBarWdg->AnswerWebViewDlg();
}

bool WebRtcLiveWdg::eventFilter(QObject *o, QEvent *e)
{
   if (o == this)
   {
      //����״̬���ı���¼�.
      if (e->type() == QEvent::WindowStateChange)
      {
         if (this->windowState() == Qt::WindowMinimized)
         {

         }
         else//��ԭ
            qDebug() << "others";
         return true;
      }
   }
   return CBaseWnd::eventFilter(o, e);
}

bool  WebRtcLiveWdg::IsExitAnserWebView()
{
   bool bRef = false;
   if (mLeftToolBarWdg)
   {
      bRef = mLeftToolBarWdg->IsExitAnserWebView();
   }
   return bRef;
}

void WebRtcLiveWdg::HideVhallRenderMaskWdg()
{
   //if (nullptr != ui.mpVhallRenderMaskWdg) {
   //   ui.mpVhallRenderMaskWdg->hide();
   //}
}

void WebRtcLiveWdg::HideNickName()
{
   //if (nullptr!=mUserNickNameShowTimer && mUserNickNameShowTimer->isActive()){
   //   mUserNickNameShowTimer->stop();
   //}

   //if (nullptr != ui.mpVhallRenderMaskWdg && ui.mpVhallRenderMaskWdg->isVisible()) {
   //   ui.mpVhallRenderMaskWdg->hide();
   //}
}

void WebRtcLiveWdg::setCurrentIndex(PAGE_INDEX index)
{
   globalToolManager->GetDataManager()->WriteLog("%s page index %d", __FUNCTION__, index);
   if (page_InvStateWdg == index && mLeftToolBarWdg && mLeftToolBarWdg->IsLocalMediaPlaying()) {
      miOldStackedTab = page_InvStateWdg;
   }
   else {

      ui.CtrlDoubPubWdg->hide();
      ui.stackedWidget_Center->setCurrentIndex(index);
      InitToolWdg();
      switch (index)
      {
      case page_Doc:
         HideNickName();
         HideVhallRenderMaskWdg();
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_Doc);
         break;
      case page_Whiteboard:
         HideNickName();
         HideVhallRenderMaskWdg();
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_Whiteboard);
         break;
      case page_otherMediaWdg:
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_MediaMenu);
         break;
      case page_DesktopScreen: {
         HideNickName();
         HideVhallRenderMaskWdg();
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_DesktopShar);
      }
                               break;
      case page_SoftwareSharing: {
         ui.stackedWidget_Center->setCurrentIndex(page_otherMediaWdg);
         HideVhallRenderMaskWdg();
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_SoftWareShar);
      }
                                 break;
      case page_InvStateWdg:

         HideNickName();
         HideVhallRenderMaskWdg();
         if (mLeftToolBarWdg && DoublePubStuChoiceUI::eChoiceType_Deskop == mLeftToolBarWdg->GetDoublePushType()) {
            mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_StuDesktopShar);
         }
         else {
            mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_StuDoublCamera);
         }

         break;
      case page_kickOut:
      {
         //if (nullptr != mRecordAndLiveStreamWebView) {
         //   delete mRecordAndLiveStreamWebView;
         //   mRecordAndLiveStreamWebView = nullptr;
         //}

         if (nullptr != mRecordAndLiveStreamCefViewWdg) {
            delete mRecordAndLiveStreamCefViewWdg;
            mRecordAndLiveStreamCefViewWdg = nullptr;
         }
      }
      break;
      default:
         HideNickName();
         HideVhallRenderMaskWdg();
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
         break;
      }
   }
}

void WebRtcLiveWdg::slot_showInitPage() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //���칫���ؼ���
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
       if (baseInfo.mDocInfo.doc_mode == 0) {
          setCurrentIndex(page_Doc);
          globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTIONW__);
       }
       else {
           setCurrentIndex(page_Whiteboard);
        }   
   }
   else if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
	   recoveryOnlyVideoLayOut();
	   setCurrentIndex(page_CenterRender);
   }
   else {
      if (baseInfo.mWebinarInfo.type == CLASS_STATUS_PLAY_BACK) {
         setCurrentIndex(page_CenterRender);
      }
      else {
         //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            setCurrentIndex(page_Doc);
         //}
         //else {
            //setCurrentIndex(page_ClassStatus);
         //}
      }

      if (baseInfo.mWebinarInfo.type == CLASS_STATUS_OVER) {
         ui.label_class_status->setText(Str_ClassIsOver);
      }
      else if (baseInfo.mWebinarInfo.type == CLASS_STATUS_PRE) {
         ui.label_class_status->setText(Str_ClassNotStart);
      }

      ui.label->setStyleSheet(CLASS_OVER_NOTICE_SHEET);
   }
   //recoveryOnlyVideoLayOut();
}

void WebRtcLiveWdg::recoveryOnlyVideoLayOut()
{
   if (nullptr != mTeacherRender) {
      ui.verticalLayout_teacher_render->removeWidget(mTeacherRender);
      globalToolManager->GetDataManager()->WriteLog(" %s 691 ui.verticalLayout_teacher_render->removeWidget(mTeacherRender) ",
         __FUNCTION__);

      mTeacherRender->setFixedSize(ui.stackedWidget_Center->width(), ui.stackedWidget_Center->height());
      mTeacherRender->setMinimumSize(0, 0);
      mTeacherRender->setMaximumSize(16777215, 16777215);
		if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
			mTeacherRender->ShowBigRenderCtrl(true);
		}
		else {
			mTeacherRender->ShowBigRenderCtrl(false);
		}
      ui.verticalLayout_CenterRender->addWidget(mTeacherRender);
      ui.widget_leftTopRender->hide();
   }
}

void WebRtcLiveWdg::InitLayOut() {
   InitClassState();
   //InitTabTool();   
   InitToolWdg();
   slot_showInitPage();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   bool bShowMemeber = true;
   if (1 == baseInfo.mToolConfig.memberList || baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher)
   {
      bShowMemeber = false;
   }
   bool showNotic = baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher;
   creatTab(bShowMemeber, showNotic);
   //������
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
         ////����,����Ƶ ��ʦ
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            mMemberListWdg->showAllowRaiseHands();
         }
         ////����,����Ƶ ѧԱ/�α�
         else {
            //ui.widget_left_tool->hide();
         }
         mLeftToolBarWdg->setDocView(false);
      }
      else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
         AddDocWebView();
         AddWhiteBoradWebView();
         ////����,�ĵ�����Ƶ ��ʦ
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            mMemberListWdg->showAllowRaiseHands();
         }
         else {////����,�ĵ�����Ƶ ѧԱ/�α�
         //ui.widget_left_tool->hide();
         }
      }
   }
   //С���
   else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
      mLeftToolBarWdg->setEnableDoublePush(true);
      ui.classRoomTitleWd->SetTitleType(baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher ? eTitleType_SmallClassLecturer : eTitleType_Student);
      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
         ////С���,����Ƶ ��ʦ
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            //mLeftToolBarWdg->setDocView();
         }
         ////С���,����Ƶ ѧԱ/�α�
         else {
            //ui.widget_left_tool->hide();
         }
         mLeftToolBarWdg->setDocView(false);
      }
      else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
         AddDocWebView();
         AddWhiteBoradWebView();
         ////С���,�ĵ�����Ƶ ��ʦ
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {

            //mLeftToolBarWdg->setDocView();
         }
         ////С���,�ĵ�����Ƶ ѧԱ/�α�
         else {
            //ui.widget_left_tool->hide();
         }
      }
   }


   AddChatWebView();
   //int banchatall;         //�Ƿ�ȫ����� 1 �� 0 ��
   //int mutingAll;          //�Ƿ�ȫ�徲�� 1�� 0��
   //int openhand;           // �Ƿ������� 1�� 0��
   mMemberListWdg->setHansUpStates(1 == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().openhand);
   if (eClassType_Classing == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type)
   {
      ui.classRoomTitleWd->startClassTime();
      if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.mInterstitialInfo.interstitial == 1) {
         if (nullptr != mLeftToolBarWdg)
         {
            MediaPlayWebView* wnd = mLeftToolBarWdg->MediaPlayWebViewDlg();
            //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
            if (wnd) {
               CSTDMapParam param;
               wnd->Notice(param);
               wnd->show();
            }
            //mLeftToolBarWdg->GetVideoChoiceUi()->show();
         }

      }
   }
   bool bSpeedmode = true;
   //ֻ�н�ʦ���������ӷ��䡣ѧԱ�����ſ������ӷ��䡣
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      globalToolManager->GetClassSDK()->ConnectVHMediaRoom(/*baseInfo.play_type*/);
   }
   else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mWebinarInfo.speedmode)
   {
      globalToolManager->GetClassSDK()->ConnectVHMediaRoom(/*baseInfo.play_type*/);
   }
}

void WebRtcLiveWdg::InitToolWdg() {
   if (nullptr == mLeftToolBarWdg)
   {
      mLeftToolBarWdg = new LeftToolBarWdg(this);
      if (mLeftToolBarWdg) {
         //mLeftToolBarWdg->setEnabled(false);
         mLeftToolBarWdg->setMaximumWidth(68);
         mLeftToolBarWdg->setWebRtcLiveWdg(this);
         connect(mLeftToolBarWdg, SIGNAL(sig_btnDoc()), this, SLOT(slot_btnDoc()));
         connect(mLeftToolBarWdg, SIGNAL(sig_Whiteboard(bool)), this, SLOT(slot_Whiteboard(bool)));
         connect(mLeftToolBarWdg, SIGNAL(sig_Answer()), this, SLOT(slot_Answer()));
         connect(mLeftToolBarWdg, SIGNAL(sig_DesktopSharing()), this, SLOT(slot_DesktopSharing()));
         connect(mLeftToolBarWdg, SIGNAL(sig_Media()), this, SLOT(slot_Media()));
         connect(mLeftToolBarWdg, SIGNAL(sig_CameraDoublePush(bool)), this, SLOT(slot_CameraDoublePush(bool)));
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_StudentDoublePush, this, &WebRtcLiveWdg::slot_StudentDoublePush);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_stopStuDoublePush, ui.CtrlDoubPubWdg, &CtrlDoublePubStuWdg::slot_StopPush);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_stopStuDestopSharingPush, ui.CtrlDoubPubWdg, &CtrlDoublePubStuWdg::slot_StopDesktopSharing);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sigStopReDoublePush, this, &WebRtcLiveWdg::slotStopReDoublePush);

         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_InteractionTool, this, &WebRtcLiveWdg::slot_interactionTool);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sigInteractionItemClicked, this, &WebRtcLiveWdg::slot_interactionItemClicked);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_DoublePush, this, &WebRtcLiveWdg::slot_DoublePush);

         connect(mLeftToolBarWdg, &LeftToolBarWdg::sigStopScreenSharing, this, &WebRtcLiveWdg::slot_BtnStopScreenClicked);
         connect(ui.classRoomTitleWd, &VhallLiveTitle::sig_OverClass, mLeftToolBarWdg, &LeftToolBarWdg::slot_OverClass);
			connect(ui.classRoomTitleWd, &VhallLiveTitle::sig_StopClass, mLeftToolBarWdg, &LeftToolBarWdg::slot_StopClass);
			connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_StopClass, ui.classRoomTitleWd, &VhallLiveTitle::slot_StopClass);
         connect(mLeftToolBarWdg, &LeftToolBarWdg::sig_inviteDoubleVrtc, this, &WebRtcLiveWdg::slot_StartStuDoublePush);
         connect(ui.CtrlDoubPubWdg, &CtrlDoublePubStuWdg::sig_CameraDoublePush, this, &WebRtcLiveWdg::slot_CameraDoublePush);
			connect(ui.CtrlDoubPubWdg, &CtrlDoublePubStuWdg::sig_StopDoublePush, this, &WebRtcLiveWdg::slot_StopDoublePush);
         connect(ui.invStateWdg, &InvitationStateWdg::sig_ReSelection, mLeftToolBarWdg, &LeftToolBarWdg::slot_ReSelection);
         connect(ui.invStateWdg, &InvitationStateWdg::sig_StateChanged, mLeftToolBarWdg, &LeftToolBarWdg::slot_InvitaStateChanged);
         connect(ui.CtrlDoubPubWdg, &CtrlDoublePubStuWdg::sig_inviteDoubleVrtc, this, &WebRtcLiveWdg::slot_StartStuDoublePush);

         if (nullptr != mTeacherRender)
            connect(mLeftToolBarWdg, &LeftToolBarWdg::sigMuteSelf, mTeacherRender, &TeacherRenderWdg::slotMuteSelft);
         ui.verticalLayout_tool->addWidget(mLeftToolBarWdg);
         //mLeftToolBarWdg->show();
      }
   }

   if (nullptr != mLeftToolBarWdg){
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
         mLeftToolBarWdg->hide();
      }
      else {
         mLeftToolBarWdg->initView();
      }
   }

}

void WebRtcLiveWdg::InitClassState() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   InitDocAndVideoRenderManager();
   AddRecordAndLiveStreamWebView();
   //�Ͽ��У����Ҳ�����ʦ����û��������ô���ľ���ֱ����
   if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      if (mTeacherRender) {
         mTeacherRender->hide();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: hide", __FUNCTION__);
      }
      //�ط�
      if (baseInfo.mWebinarInfo.type == CLASS_STATUS_PLAY_BACK) {

         ui.verticalLayout_CenterRender->addWidget(mRecordAndLiveStreamCefViewWdg);
         globalToolManager->GetDataManager()->WriteLog(" %s CLASS_STATUS_PLAY_BACK addWidget(mRecordAndLiveStreamCefViewWdg) ", __FUNCTION__);
         mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_CENTER);

         QString replayUrl = QString::fromStdString(baseInfo.urls.record) + QString("&role_type=%1").arg(baseInfo.mWebinarInfo.type);
         mRecordAndLiveStreamCefViewWdg->LoadUrl(replayUrl);

         ui.widget_right_tool->hide();
         setCurrentIndex(page_CenterRender);
         mRecordAndLiveStreamCefViewWdg->show();
         globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: show", __FUNCTION__);
         globalToolManager->GetDataManager()->WriteLog(" %s show (mRecordAndLiveStreamCefViewWdg) ", __FUNCTION__);
         ui.widget_main_title->hide();
      }
      //ֱ����
      else if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
         if (baseInfo.mWebinarInfo.speedmode && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL){//����ģʽ  С���
            
            if (mTeacherRender) {
               mTeacherRender->show();
               globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
            }
            ui.widget_main_title->hide();
         }
         else {//�Ǽ���ģʽС���/������
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
               ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
               if (mRecordAndLiveStreamCefViewWdg) {
                  mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_RIGHT_TOP);
               }
               
               globalToolManager->GetDataManager()->WriteLog(" %s 905 mWebinarInfo.type %d speedmode %d��course_type %d  ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ", 
                  __FUNCTION__, (int)baseInfo.mWebinarInfo.type, baseInfo.mWebinarInfo.speedmode , baseInfo.mWebinarInfo.course_type);

               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                  ui.widget_main_title->show();
               }
            }
            else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
               ui.verticalLayout_CenterRender->addWidget(mRecordAndLiveStreamCefViewWdg);
               if(mRecordAndLiveStreamCefViewWdg)
                  mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_CENTER);

               setCurrentIndex(page_CenterRender);
               ui.widget_main_title->hide();
            }
            QString url = QString::fromStdString(baseInfo.urls.mixvideo);
            if (mRecordAndLiveStreamCefViewWdg) {
               mRecordAndLiveStreamCefViewWdg->LoadUrl(url/*, URLType_LiveMixedStream*/);
               mRecordAndLiveStreamCefViewWdg->show();
               globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: show", __FUNCTION__);
            }
         }
      }
      else {
         if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
            ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
            globalToolManager->GetDataManager()->WriteLog(" %s 930 mWebinarInfo.type %d speedmode %d mWebinarInfo.layout %d course_type %d  ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
               __FUNCTION__, (int)baseInfo.mWebinarInfo.type, baseInfo.mWebinarInfo.layout,
               baseInfo.mWebinarInfo.speedmode, baseInfo.mWebinarInfo.course_type);

            mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_RIGHT_TOP);
         }
         else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
            ui.verticalLayout_CenterRender->addWidget(mRecordAndLiveStreamCefViewWdg);
            mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_CENTER);
         }
         mRecordAndLiveStreamCefViewWdg->hide();
         globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: hide", __FUNCTION__);
         ui.widget_main_title->hide();
         if (mTeacherRender) {
            mTeacherRender->show();
            globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
         }
      }
      // globalToolManager->GetClassSDK()->DisableSubScribeStream();
      if (baseInfo.mWebinarInfo.mInterstitialInfo.interstitial == 1) {

         if (nullptr != mLeftToolBarWdg)
         {
            MediaPlayWebView* wnd = mLeftToolBarWdg->MediaPlayWebViewDlg();
            //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
            if (wnd) {
               CSTDMapParam param;
               wnd->Notice(param);
               wnd->show();
            }
            //mLeftToolBarWdg->GetVideoChoiceUi()->show();
         }
      }
      if (baseInfo.mWebinarInfo.answing_question_id > 0) {
         if (mLeftToolBarWdg)
         {
            AnswerWebView* wnd = mLeftToolBarWdg->AnswerWebViewDlg();
            //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
            if (wnd) {
               CSTDMapParam param;
               wnd->Notice(param);
               wnd->show();
            }
         }

      }
   }
   else {
      if (mTeacherRender) {
         mTeacherRender->show();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
      }
   }
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      ui.classRoomTitleWd->UpdateStartClassState(true);
      //���� ����������   true
      //CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
      CBaseWnd* wnd = nullptr;
      CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      if (pWdg)
      {
         WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
         wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
      }

      if (wnd) {
         CSTDMapParam mapParam;
         mapParam[Str_Quality_Enabeld] = Str_Quality_UnEnabeld;
         wnd->Notice(mapParam);
         //wnd->exec();
      }
   }
}

void WebRtcLiveWdg::InitDocAndVideoRenderManager() {
   //С�����Ҫ����������Ⱦ����
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   mTeacherRender = new TeacherRenderWdg(this, true);
   if (nullptr != mLeftToolBarWdg)
   {
      connect(mLeftToolBarWdg, &LeftToolBarWdg::sigMuteSelf, mTeacherRender, &TeacherRenderWdg::slotMuteSelft);
      connect(mLeftToolBarWdg, &LeftToolBarWdg::sigMuteSelf, mTeacherRender, &TeacherRenderWdg::slotTrophyAdd);
   }
   GetPtrVhallIALiveSettingDlg();
   connect(GetPtrVhallIALiveSettingDlg(), &VhallIALiveSettingDlg::sigPicPush, mTeacherRender, &TeacherRenderWdg::sig_PicPush);
   GetPtrVhallIALiveSettingDlg()->SetStreamType();


    if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
      mTeacherRender->InitRenderInsertPos(InsertPos_Center);
      ui.verticalLayout_CenterRender->addWidget(mTeacherRender);
      ui.widget_leftTopRender->hide();
   }
   else    if (CLASS_LAYOUT_AUDIO_AND_DOC!= baseInfo.mWebinarInfo.layout/*baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC*/) {
      ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
      globalToolManager->GetDataManager()->WriteLog(" %s 1022 mWebinarInfo.layout %d ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
         __FUNCTION__, baseInfo.mWebinarInfo.layout);
      mTeacherRender->InitRenderInsertPos(InsertPos_Speaker);
      mTeacherRender->setFixedWidth(320);
      mTeacherRender->setFixedHeight(240);
   }
     

   globalToolManager->GetDataManager()->WriteLog("%s baseInfo.mWebinarInfo.layout:%d", __FUNCTION__, baseInfo.mWebinarInfo.layout);
   mTeacherRender->hide();
   globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: hide", __FUNCTION__);
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mTeacherRender->InitRenderCtrl(QString::fromStdString(baseInfo.mUserInfo.join_id));
      mTeacherRender->SetUserJoinID(QString::fromStdString(baseInfo.mUserInfo.join_id));
      mTeacherRender->SetNickName(QString::fromStdString(baseInfo.mUserInfo.nick_name));
      mTeacherRender->SetRoleType(JOIN_ROLE_Teacher);
      globalToolManager->GetDataManager()->SetTeacherJoinId(QString::fromStdString(baseInfo.mUserInfo.join_id));
   }
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
      mRenderManager = new RenderManager(this, this);
      //connect(mLeftToolBarWdg, &LeftToolBarWdg::sigBtnAdd, mRenderManager, &RenderManager::slotAddTrophyWdg);
      ui.horizontalLayout_render->addWidget(mRenderManager);
      //if (nullptr != mVhallIALiveSettingDlg)
      //	connect(mVhallIALiveSettingDlg, &VhallIALiveSettingDlg::sig_MuteAllSubScribeAudio,
      //		mRenderManager, &RenderManager::slot_MuteAllSubScribeAudio);
      mRenderManager->hide();
   }

   if (baseInfo.brush > 0) {
      mstrIDPenAuth = QString::number(baseInfo.brush);
   }
   else {
      mstrIDPenAuth = "";
   }

   std::list<PublishUser> publishUser = baseInfo.ready_list;
   std::list<PublishUser>::iterator iter = publishUser.begin();
   while (iter != publishUser.end()) {
      PublishUser user = (*iter);
      if (user.role == JOIN_ROLE_Teacher) {
         globalToolManager->GetDataManager()->SetTeacherJoinId(QString::fromStdString(user.join_id));
         if (mTeacherRender) {
            mTeacherRender->InitRenderCtrl(QString::fromStdString(user.join_id));
            mTeacherRender->SetUserJoinID(QString::fromStdString(user.join_id));
            mTeacherRender->SetNickName(QString::fromStdString(user.nick_name));
            if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
               mTeacherRender->SetCameraState(!user.is_frame);
            }
            else {
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  mTeacherRender->SetCameraState(!user.is_frame);
               }
               else {
                  user.is_frame == 1 ? mTeacherRender->SetTeacherCameraCloseSheet() : mTeacherRender->SetTeacherCameraOpenSheet();
                  globalToolManager->GetDataManager()->SetMultiClassOtherUserCameraOpen(user.is_frame == 1 ? false : true);//is_frame 1 ����ͷ�رգ� 0 �򿪡������Σ�ѧԱ��⵽����������ͷ���ر��ˡ�
               }
            }
            mTeacherRender->SetAllMuteAudioState(baseInfo.mutingAll);
            mTeacherRender->SetMicState(!user.is_quiet);
            mTeacherRender->SetRoleType(user.role);
         }
      }
      else {
         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && mRenderManager) {
            if (globalToolManager->GetClassSDK()->isConnetWebRtcRoom()) {
               mRenderManager->AddInitReadyPublishUser(QString::fromStdString(user.join_id), QString::fromStdString(user.nick_name), user.role, !user.is_frame, !user.is_quiet);
            }
               
            InitToolWdg();

            if (nullptr != mLeftToolBarWdg) {
               mLeftToolBarWdg->EnableStuDoubleVrtc(true);
            }
               
            if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               mTeacherRender->ShowAllMuteCtrl(true);
            }
            mRenderManager->initTrophyRewardNum(QString::fromStdString(user.join_id), user.iRewardNum);
         }
         else {
            if (mTeacherRender) {
               mTeacherRender->SetSmallWndUserJoinID(QString::fromStdString(user.join_id));
               mTeacherRender->SetSmallRenderNickName(QString::fromStdString(user.nick_name));
               mTeacherRender->SetSmallRenderCameraState(!user.is_frame);
               mTeacherRender->SetSmallRenderMicState(!user.is_quiet);
               mTeacherRender->SetSmallRenderRole(user.role);
            }
         }
      }
      iter++;
   }
}

//void WebRtcLiveWdg::InitTabTool() {
//    //if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type != JOIN_ROLE_Teacher) {
//    //    ui.pushButton_noticePage->hide();
//    //    ui.pushButton_memberListPage->hide();
//    //    ui.stackedWidget->setCurrentIndex(1);
//    //}
//
// //   connect(ui.pushButton_chatPage, SIGNAL(clicked()), this, SLOT(slot_selectChatPage()));
// //   connect(ui.pushButton_memberListPage, SIGNAL(clicked()), this, SLOT(slot_selectMemberPage()));
// //   connect(ui.pushButton_noticePage, SIGNAL(clicked()), this, SLOT(slot_NoticePage()));
//	//slot_selectChatPage();
//}

void WebRtcLiveWdg::AddDocWebView() {
   //mDocWebEngineViewWdg = new WebEngineViewWdg(this);
   //if (mDocWebEngineViewWdg) {
   //   mDocWebEngineViewWdg->CreateWebView();
   //   connect(mDocWebEngineViewWdg, SIGNAL(sig_loadFinished(bool)), this, SLOT(slot_DocLoadFinished(bool)));
   //   std::string doc = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.doc;mDocCefViewWdg->InitLibCef
   //   int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
   //   QString loadUrl = QString::fromStdString(doc) + QString("&role_type=%1").arg(roleType);
   //   //loadUrl = loadUrl.replace("http://t-class.e.vhall.com","http://192.168.31.227:8088");
   //   mDocWebEngineViewWdg->LoadUrl(loadUrl);
   //   ui.horizontalLayout_Doc->addWidget(mDocWebEngineViewWdg);
   //}

   mDocCefViewWdg = new LibCefViewWdg(this);
   if (mDocCefViewWdg) {
      //connect(mDocCefViewWdg, &LibCefViewWdg::sig_TitleChanged, this, &WebRtcLiveWdg::slot_DocLoadFinished);

      std::string doc = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.doc;
      int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
      QString loadUrl = QString::fromStdString(doc) + QString("&role_type=%1").arg(roleType);
   //http://t-class.e.vhall.com/doc.html?webinar_id=edu_240b73ab&join_id=283823&token=34752EEt26F7B&type=doc&play_type=1&role_type=2
   //localhost:8088 / doc.html ? webinar_id = edu_c789a920 & join_id = 283331 & token = 2F3CE42Bt26F68&type = doc & role_type = 1 & view_type = cef & play_type = 1# /

      //loadUrl = loadUrl.replace("http://t-class.e.vhall.com/doc.html?","localhost:8088/doc.html?");
      //loadUrl.append("&view_type=cef");

      mDocCefViewWdg->InitLibCef();
      mDocCefViewWdg->Create(loadUrl);
      ui.horizontalLayout_Doc->addWidget(mDocCefViewWdg);
   }
}

void WebRtcLiveWdg::AddWhiteBoradWebView() {
   //mWhiteBoardWebEngineViewWdg = new WebEngineViewWdg(this);
   //if (mWhiteBoardWebEngineViewWdg) {
   //   mWhiteBoardWebEngineViewWdg->CreateWebView();
   //   std::string board = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.board;
   //   int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
   //   QString loadUrl = QString::fromStdString(board) + QString("&role_type=%1").arg(roleType);
   //   mWhiteBoardWebEngineViewWdg->LoadUrl(loadUrl);
   //   ui.horizontalLayout_WhiteBoard->addWidget(mWhiteBoardWebEngineViewWdg);
   //}

   mWhiteBoardCefViewWdg = new LibCefViewWdg(this);
   if (mWhiteBoardCefViewWdg) {
      std::string board = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.board;
      int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
      QString loadUrl = QString::fromStdString(board) + QString("&role_type=%1").arg(roleType);

	  //loadUrl = loadUrl.replace("http://t-class.e.vhall.com/doc.html?", "localhost:8088/doc.html?");
	  //loadUrl.append("&view_type=cef");

      mWhiteBoardCefViewWdg->InitLibCef();
      mWhiteBoardCefViewWdg->Create(loadUrl);

      ui.horizontalLayout_WhiteBoard->addWidget(mWhiteBoardCefViewWdg);
   }
}

void WebRtcLiveWdg::AddRecordAndLiveStreamWebView() {
   //mRecordAndLiveStreamWebView = new WebEngineViewWdg(this);
   //if (mRecordAndLiveStreamWebView) {
   //   connect(mRecordAndLiveStreamWebView, SIGNAL(sig_fullScreen()), this, SLOT(slot_LiveMixStreamWebFull()));
   //   connect(mRecordAndLiveStreamWebView, SIGNAL(sig_Enter()), this, SLOT(slot_LiveMixStreamWebEnter()));
   //   connect(mRecordAndLiveStreamWebView, SIGNAL(sig_KeyEsc()), this, SLOT(slot_LiveMixStreamWebKeyEsc()));
   //   connect(mRecordAndLiveStreamWebView, SIGNAL(sig_Leave()), this, SLOT(slot_LiveMixStreamWebLeave()));
   //   mRecordAndLiveStreamWebView->CreateWebView(WebViewType_LiveStream);
   //   mRecordAndLiveStreamWebView->hide();
   //}

   mRecordAndLiveStreamCefViewWdg = new LibCefViewWdg(this);
   if (mRecordAndLiveStreamCefViewWdg) {

      mRecordAndLiveStreamCefViewWdg->InitLibCef();
      //mWhiteBoardCefViewWdg->Create(loadUrl);

      mRecordAndLiveStreamCefViewWdg->hide();
      globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: hide", __FUNCTION__);
   }
}

void WebRtcLiveWdg::AddChatWebView() {
 
   if (nullptr == mChatCefViewWdg)
      mChatCefViewWdg = new LibCefViewWdg(this);
   if (mChatCefViewWdg) {
      //mChatCefViewWdg->hide();
      //connect(mChatCefViewWdg, &WebEngineViewWdg::sig_JsCallQt, mMemberListWdg, &CMemberListWdg::slot_JsCallQtMsg);
      connect(mMemberListWdg, &CMemberListWdg::sig_UpdateMemberCount, this, &WebRtcLiveWdg::slot_JsCallQtMsg);
      //mChatCefViewWdg->CreateWebView();
      std::string chat = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.chat;
      int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
      QString loadUrl = QString::fromStdString(chat) + QString("&role_type=%1").arg(roleType);
      //loadUrl =  "http://e.vhall.com/";
      mChatCefViewWdg->InitLibCef();
      mChatCefViewWdg->Create(loadUrl);
      mChatCefViewWdg->show();

      //ui.verticalLayout_chat->addWidget(mChatWebEngineViewWdg);
   }
}

void WebRtcLiveWdg::slot_JsCallQtMsg(const int& membercount)
{
   mTotal = membercount - 1;
   if (mTotal > 0 && nullptr != mLeftToolBarWdg && mLeftToolBarWdg->IsExitSignInDlg())
   {
      mLeftToolBarWdg->AddOnline(mTotal);
   }
}

void WebRtcLiveWdg::slot_StartStuDoublePush(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType)
{
   ui.CtrlDoubPubWdg->slot_StartStuDoublePush(strId, eType);
   setCurrentIndex(page_InvStateWdg);
   ui.invStateWdg->SetStateType(InvitationStateWdg::eStateType_FrameGeting);
  
   if (DoublePubStuChoiceUI::eChoiceType_Deskop == eType){//˫�����湲��
      mLeftToolBarWdg->SetBtnDesktopSharingEnabled(false);
   }
   else if(DoublePubStuChoiceUI::eChoiceType_Camera == eType) {//˫��
      mLeftToolBarWdg->SetDoublePushCameraEnabled(false);
   }

   ui.invStateWdg->slotChangeTargetId(strId, eType);
}

void WebRtcLiveWdg::slot_StopDoublePush() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mMixStreamLayout.SetMixStreamLayout();
   }
}

int WebRtcLiveWdg::GetOnlineTotal() {
	return mMemberListWdg->GetMemberCount();
}

void WebRtcLiveWdg::NoticeShowNickName()
{
   //if (ui.mpVhallRenderMaskWdg && this->isVisible() && !isMinimized() && ui.otherMediaWdg->isVisible()) {

   //   if (ui.mpVhallRenderMaskWdg->geometry().width() != ui.otherMediaWdg->width()
   //      || ui.mpVhallRenderMaskWdg->geometry().height() != ui.otherMediaWdg->height()) {
   //      int iw = ui.otherMediaWdg->width();

   //      ui.mpVhallRenderMaskWdg->setFixedWidth(iw);
		 //int iH = ui.otherMediaWdg->height();
   //      ui.mpVhallRenderMaskWdg->setFixedHeight(iH);
   //   }

   //   //QPoint pos = ui.otherMediaWdg->mapToGlobal(QPoint(0, 0));
   //   //ui.mpVhallRenderMaskWdg->move(pos.rx(), pos.ry() + ui.otherMediaWdg->height() - ui.mpVhallRenderMaskWdg->height());
   //   ui.mpVhallRenderMaskWdg->show();

   //}
}

bool WebRtcLiveWdg::IsDesktopShare()
{
   bool bRef = false;
   if (m_pScreenShareToolWgd && !m_pScreenShareToolWgd->isHidden()) {
      bRef = true;
   }
   return bRef;
}

bool WebRtcLiveWdg::IsTimerWdgVisable() {

	if (mpTimerWdg && mpTimerWdg->isVisible()) {
		return true;
	}
	else {
		return false;
	}
}

bool WebRtcLiveWdg::isEixMediaScreen()
{
   int indx = ui.stackedWidget_Center->currentIndex();
   return (indx== page_MediaFilePlay || indx == page_otherMediaWdg);
}

bool WebRtcLiveWdg::isStudentSpeaking()
{
   bool bRef = false;
   if (mTeacherRender) {
      bRef = mTeacherRender->isSmallRender();
   }

   return bRef;
}

int WebRtcLiveWdg::iRenderCount()
{
   //RenderManager* mRenderManager = nullptr;
   int iCount = 0;
   if (mRenderManager) {
      iCount = mRenderManager->iRenderSize();
   }
   else
   {
      globalToolManager->GetDataManager()->WriteLog("%s mRenderManager  is nullptr ", __FUNCTION__, iCount);
   }
   globalToolManager->GetDataManager()->WriteLog("%s iRenderCount %d ", __FUNCTION__, iCount);
   return iCount;
}

bool WebRtcLiveWdg::StartInvitationSpeech()
{
   bool bRef = false;

   if (mMemberListWdg) {
      bRef = mMemberListWdg->StartInvitationSpeech();
   }
   return bRef;
}

bool WebRtcLiveWdg::HasPenAuth()
{
   //bool bb = mstrIDPenAuth.isEmpty();
   bool bAuth = mstrIDPenAuth.trimmed().isEmpty();
   return bAuth;
}

QString WebRtcLiveWdg::LocalStream()
{
   return mLocalStream;
}

bool WebRtcLiveWdg::CheckAndStopScreenShare(const DoublePubStuChoiceUI::eChoiceType& type)
{
   bool bRef = true;
   if (m_pScreenShareToolWgd && !m_pScreenShareToolWgd->isHidden()) {
      //FadeOutTip(Str_PleaseStopDesktop,warning);
      TipsWdg* tips = new TipsWdg(this);
      if (tips) {
         CSTDMapParam param;
         if (DoublePubStuChoiceUI::eChoiceType_Camera == type) {
            param[TipsMsg] = QStringLiteral("����˫�ƹ��ܻ������ǰ��Ļ�����Ƿ�ȷ�Ͽ���?");
         }
         else {
            param[TipsMsg] = QStringLiteral("����ѧԱ���湲�����Ƚ�����ǰ��Ļ�����Ƿ�ȷ�Ͻ�����Ļ����?");

         }
         
         param[ShowCancelButton] = PARAM_TRUE;
         param[ShowTipsSize] = QString::number(50);
         tips->Notice(param);
         int accept = tips->exec();
         if (accept != QDialog::Accepted) {
            bRef = false;
         }
         else
         {
            slot_BtnStopScreenClicked();
         }
      }
   }

   if (bRef && nullptr != mLeftToolBarWdg && mLeftToolBarWdg->isPreViewAuxiliaryVisible()/*globalToolManager->GetWndManager()->IsWndExist(WND_ID_CAMERA_DOUBLE_PUSH_UI)*/) {
      bRef = false;
      //return bRef;
   }


   if (bRef && mLeftToolBarWdg && (mLeftToolBarWdg->isMediaPlayExist()/*||globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/)) {
      mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
      FadeOutTip(Str_PleaseStopMedia, Tost_Warning);
      bRef = false;
      //return;
   }

	if (bRef && mLeftToolBarWdg->isVideoChoiceUiExist()/*||globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/) {
		//mLeftToolBarWdg->AnserDlgHide();
		FadeOutTip(Str_PleaseStopMedia, Tost_Warning);
		bRef = false;
	}

   return bRef;
}

void WebRtcLiveWdg::slotChangeHandsUpState(const int& iState)
{
   if (!mMemberListWdg->isVisible() && eHandsUpState_existence == iState && nullptr != mpPaintLable)
   {
      //ui.pushButton_memberListPage->SetHandsUpState(eHandsUpState_existence);
      //mpPaintLable->show();
      mpPaintLable->SetHandsUpState(eHandsUpState_existence);
   }
}

void WebRtcLiveWdg::slot_LiveMixStreamWebFull() {
   if (mpFullWebView) {
      mpFullWebView->AddWebView(mRecordAndLiveStreamCefViewWdg);
      mpFullWebView->show();
      this->hide();
   }
}

void WebRtcLiveWdg::slot_LiveMixStreamWebLeave() {

}

void WebRtcLiveWdg::slot_LiveMixStreamWebEnter() {
   //if (mpFullWebView) {
   //   mpFullWebView->slot_ShowExitFullScreen();
   ////}
   //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //if (mChangeBroadCastLayout && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
   //   mChangeBroadCastLayout->show();
   //   mChangeBroadCastLayout->move(0,ui.classRoomTitleWd->height());
   //}
}

void WebRtcLiveWdg::slot_changeMixStreamViewLayout() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (mRecordAndLiveStreamCefViewWdg && mDocCefViewWdg && mWhiteBoardCefViewWdg && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
      bool isNormalLayout = false;
      int iCount = ui.verticalLayout_8->count();
      for(int i = 0; i < iCount; i++){
         if(ui.verticalLayout_8->itemAt(i)->widget() == ui.stackedWidget_Center){
            isNormalLayout = true;
            break;
         }
      }

      if(isNormalLayout){
         ui.verticalLayout_teacher_render->addWidget(ui.stackedWidget_Center);
         globalToolManager->GetDataManager()->WriteLog(" %s 1414  isNormalLayout %d ui.verticalLayout_teacher_render->addWidget(ui.stackedWidget_Center) ",
            __FUNCTION__, isNormalLayout);
         ui.verticalLayout_8->insertWidget(1, mRecordAndLiveStreamCefViewWdg);
      }
      else{
         ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
         globalToolManager->GetDataManager()->WriteLog(" %s 1420 isNormalLayout %d ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
            __FUNCTION__, isNormalLayout);
         ui.verticalLayout_8->insertWidget(1, ui.stackedWidget_Center);
      }
   }


}

void WebRtcLiveWdg::slot_LiveMixStreamWebKeyEsc() {
   if (mpFullWebView) {
      mpFullWebView->hide();
      mpFullWebView->slot_HideExitFullScreen();
      //mRecordAndLiveStreamWebView->SetFullState(false);

      this->show();
      int pos = mRecordAndLiveStreamCefViewWdg->GetLayOutPos();
      if (pos == LAYOUT_POS_RIGHT_TOP) {
         ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
         globalToolManager->GetDataManager()->WriteLog(" %s 1439 pos %d ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
            __FUNCTION__, pos);
      }
      else {
         ui.verticalLayout_CenterRender->addWidget(mRecordAndLiveStreamCefViewWdg);
      }
   }
}

void WebRtcLiveWdg::JsCallQtMsg(QString name, QString param)
{
   if (name.compare("JsCallQtMsg") == 0) {
      mMemberListWdg->slot_JsCallQtMsg(param);
   }
   else if (name.compare("JsCallFadeOutTip") == 0) {
      FadeOutTip(param, Tost_Message, false);
   }
}

void WebRtcLiveWdg::customEvent(QEvent* event) {

   if (event) {
      globalToolManager->GetDataManager()->WriteLog("Enter customEvent:%d", event->type());
      switch (event->type())
      {
      case CustomEvent_LibCefMsg: {
         CefMsgEvent* cus_event = dynamic_cast<CefMsgEvent*>(event);
         if (cus_event) {
            JsCallQtMsg(cus_event->mFunName, cus_event->mCefMsgData);
         }
      }
                                  break;

      case CustomEvent_KickOut_Room:
      {
         HandleKickOut(event);
         break;
      }
      case CustomEvent_FloatType: {
         if (mMultiClassFloatWnd && mTeacherRender) {
            mMultiClassFloatWnd->setFixedSize(QSize(720,570));
            mMultiClassFloatWnd->AddRenderWidget(mTeacherRender,false);
            mTeacherRender->setFixedSize(QSize(720, 540));
            mTeacherRender->SetFloatTypeValue(WndFloatType_Float);
            mMultiClassFloatWnd->SetUserid(mTeacherRender->GetUserJoinID());
            mMultiClassFloatWnd->show();
            QRect desktopWidget = QApplication::desktop()->screenGeometry(this);
            mMultiClassFloatWnd->move((desktopWidget.x() + desktopWidget.width() - mMultiClassFloatWnd->width()) / 2, (desktopWidget.y() + +desktopWidget.height() - mMultiClassFloatWnd->height()) / 2);
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC  
               ||
               (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO
                  && (globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
                     || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
                     || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture))
                  )
               ) {
               ui.widget_leftTopRender->hide();
            }
            //ui.widget_leftTopRender->hide();
         }
         break;
      }
      case CustomEvent_FloatType_Renders: {
         CustomEventType* msgEvent = (CustomEventType*)event;
         if (msgEvent)
         {
            QString strId = msgEvent->msg.trimmed();
            //QMap<QString, MultiClassFloatWnd*>::iterator ite =mapMultiClassFloatWnd.find(strId);
            //MultiClassFloatWnd* tempFloateWnd = nullptr;
            if (nullptr != mFloatWnd && mFloatWnd->isVisible())
            {
               slot_closeFloat(mFloatWnd->strUserId());
            }
            else {
               mFloatWnd = new MultiClassFloatWnd();
               if (mFloatWnd) {
                  //mapMultiClassFloatWnd.insert(strId, tempFloateWnd);
                  connect(mFloatWnd, &MultiClassFloatWnd::sig_close, this, &WebRtcLiveWdg::slot_closeFloat);
               } 
            }

            if (mFloatWnd) {
               mFloatWnd->SetUserid(strId);
               TeacherRenderWdg* Render = nullptr;

               if (0 == mTeacherRender->GetUserJoinID().compare(strId)) {
                  Render = mTeacherRender;
               }
               else {
                  Render = mRenderManager->GetFloateRender(strId);
               }
               if (Render) {
                  mFloatWnd->setFixedSize(QSize(720, 570));
                  mFloatWnd->AddRenderWidget(Render, false);
                  Render->setFixedSize(QSize(720, 540));
                  Render->SetFloatTypeValue(WndFloatType_Float);
                  Render->show();
                  mFloatWnd->show();


                  QRect desktopWidget = QApplication::desktop()->screenGeometry(this);
                  mFloatWnd->move((desktopWidget.x() + desktopWidget.width() - mMultiClassFloatWnd->width()) / 2, (desktopWidget.y() + +desktopWidget.height() - mMultiClassFloatWnd->height()) / 2);
                  ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
                  //ui.widget_leftTopRender->hide();
                  if ((mTeacherRender == Render && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC)
                     ||
                     (mTeacherRender == Render && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO
                        && ( globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) 
                           || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
                           || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture))
                        )
                     ) {
                     ui.widget_leftTopRender->hide();
                  }
                  else if (mTeacherRender == Render) {
                  }
               }
               else {
                  globalToolManager->GetDataManager()->WriteLog("%s not Find Render", __FUNCTION__);
               }
            }
         }
         break;
      }
      case CustomEvent_FullType: {
         if (mMultiClassFloatWnd && mTeacherRender) {
            QRect desktopWidget = QApplication::desktop()->screenGeometry(this);
            mMultiClassFloatWnd->setFixedSize(QSize(desktopWidget.width(), desktopWidget.height()));
            mMultiClassFloatWnd->AddRenderWidget(mTeacherRender, true);
            mTeacherRender->setFixedSize(QSize(desktopWidget.width(), desktopWidget.height()));
            mTeacherRender->SetFloatType(WndFloatType_Full);
            mMultiClassFloatWnd->show();
            mMultiClassFloatWnd->move(desktopWidget.x(),desktopWidget.y());
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC
               ||
               ( baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO
                  && (globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
                     || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
                     || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture))
                  )
               ) {
               ui.widget_leftTopRender->hide();
            }
            this->hide();
         }
         break;
      }
      case CustomEvent_OnNetWork_Close: {
         globalToolManager->GetDataManager()->WriteLog("%s CustomEvent_OnNetWork_Close", __FUNCTION__);
         FadeOutTip(Str_WebRtcRoomDisConnect, Tost_Warning);
         globalToolManager->GetClassSDK()->ReconnectNetWork();
         break;
      }
      case CustomEvent_ChangeToToolPage://�յ���ʦ�� �л��ĵ����װ塢����Ȳ���
      {
         if (JOIN_ROLE_Teacher != globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)
            HandleChangeToolPage(event);
         break;
      }
      case CustomEvent_RecvPublishFailed:  //���յ��û����͵�����ʧ�ܡ�����ʧ����Ϣ
      {
         HandlePublishFailed(event);
         break;
      }
      case CustomEvent_RecvHandDelay:  //������ֳ�ʱ��Ϣ
      {
         HandleHandDelay(event);
         break;
      }
      case CustomEvent_StartSign:  //������ֳ�ʱ��Ϣ
      {
         HandleStartSign(event);
         break;
      }
      case CustomEvent_SignIn:
         HandleSignIn(event);
         break;
      case CustomEvent_SignStop:
         HandleSignStop(event);
         break;
      case CustomEvent_StartCountdown:  //
      {
         HandleStartCountdown(event);
         break;
      }
      case CustomEvent_StartAnswerTool:  //�����������ʼ��Ϣ 
      {
         HandleStartAnswerTool(event);
         break;
      }
      case CustomEvent_AnswerDetail:  //�����������ʼ��Ϣ 
      {
         HandleAnswerDetail(event);
         break;
      }
      case CustomEvent_StartAnswer:
      {
         HandleStartAnswer(event);
         break;
      }
      case CustomEvent_OnRecvInavCloseMsg:
      {
         globalToolManager->GetClassSDK()->LeaveClassRoom();
         break;
      }
      case CustomEvent_OnRecvKickInavStreamMsg:
      {
         HandleKickInavStream(event);
         break;
      }
      case CustomEvent_OnRecvAuditInavPublishMsg:
      {
         HandleAuditInavPublish(event);
         break;
      }
      case CustomEvent_User_ForbiddenList:
      {
         HandleForbiddenList(event);
         break;
      }
      case CustomEvent_User_HandUpList:
      {
         HandleHandUpList(event);
         break;
      }
      case CustomEvent_User_KickOutList:
      {
         HandleKickOutList(event);
         break;
      }
      case CustomEvent_User_OnLineList:
      {
         HandleOnLineList(event);
         break;
      }
      case CustomEvent_RecvCancelHand:
      {
         HandleCancelHand(event);
         break;
      }
      case CustomEvent_RewardStu:
      {
         HandleRewardStu(event);
         break;
      }
      case CustomEvent_ChangeBigScreen: {
         HandleChangeBigScreen(event);
         break;
      }
                                        //case CustomEvent_User_OnLine://�յ��û�������
                                        //{
                                        //	break;
                                        //}
      case CustomEvent_SetLayoutCameraAuxiliary: {
         HandleSetLayoutCameraAuxiliary(event);
         break;
      }
      case CustomEvent_CloseFloatRender: { //�رո�������
         HandleCloseFloatRender(event);
         break;
      }
      case CustomEvent_WebRtcRoomMixedReady: {  //����˻���׼�����
         HandleWebRtcRoomMixedReady(event);
         break;
      }
      case CustomEvent_ShowNoticeMsg: {
         HandleShowNoticeMsg(event);
         break;
      }
      case CustomEvent_ShowFadOutTipMsg: {
         HandleShowFadOutTipsMsg(event);
         break;
      }
      case CustomEvent_StartMixStream: {
         HandleStartMixStream(event);
         break;
      }
      case CustomEvent_ChangeRrlayType: {
         //��ʾ�������� �����б�
         ui.classRoomTitleWd->ShowHandsUpBtn();
         break;
      }
      case CustomEvent_RecvFlashMsg:
      case CustomEvent_RecvUploadDoc: {
         HandleRecvFlashMsg(event);
         break;
      }
      case CustomEvent_RemoteStreamAdd: {
         HandleStreamAdd(event);
         break;
      }
      case CustomEvent_RecvSetBrush: {
         HandleRecvSetBrush(event);
         break;
      }
      case CustomEvent_RecvMediaMsg: {
         HandleRecvMediaMsg(event);
         break;
      }
      case CustomEvent_RecvAnswerMsg: {
         HandleRecvAnswerMsg(event);
         break;
      }
      case CustomEvent_StartSetMainStream: {
         HandleStartSetMainStream(event);
         break;
      }
      case CustomEvent_WebRtcRoomConnetSuc: {    //�����������ӳɹ�
         HandleWebRtcRoomConnetSuc(event);
         break;
      }
      case CustomEvent_WebRtcRoomConnetErr: {
         ui.classRoomTitleWd->ShowBeginClass(false);
         HandleWebRtcRoomConnectErr();
         break;
      }
      case CustomEvent_SubScribSuc: {           //���ձض��ĳɹ�Զ����
         HandleSubScribStream(event);
         break;
      }
      case CustomEvent_STREAM_REMOVED: {//����Զ�����˳�����
         HandleRemoteStream(event);
         break;
      }
      case CustomEvent_OnCaptureCallback: {           //�豸�����湲���ļ��岥 ��ʼ�ɼ��ص�
         HandleOpenCaptureCallback(event);
         break;
      }
                                          //case CustomEvent_Preview_Auxiliary_Camera: {           //�豸�����湲���ļ��岥 ��ʼ�ɼ��ص�
                                          //   //HandleOpenCaptureCallback(event);
                                          //   if (mLeftToolBarWdg) {
                                          //      mLeftToolBarWdg->PreviewAuxiliaryCamera(event);
                                          //   }
                                          //       
                                          //   break;
                                          //}
      case CustomEvent_PreOnCaptureCallback:
      {
         HandlePreOpenCaptureCallback(event);
      }
      break;
      case CustomEvent_HandleStartLocalCapture: {     //��ʼ���زɼ�
       //mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
         HandleStartLocalCapture();
         break;
      }
      case CustomEvent_StartAndStopClass: {           //�ϿΡ��¿�ִ�лص���Ϣ
         HandleStartAndStopClass(event);
		 mbQuitAfterUpPublish = false;
         break;
      }
      case CustomEvent_OnRecvAskforInavPublishMsg: {  //�յ�����������������������Ϣ
         HandleRecvAskForPublish(event);
         break;
      }
      case CustomEvent_PushLocalStreamSuc: {         //���������ɹ�
         HandlePushLocalStreamSuc(event);
         break;
      }
      case CustomEvent_LocalStreamIdChanged: {       //����������ID�ı�
         HandleLocalStreamIdChanged(event);
         break;
      }
      case CustomEvent_PushLocalStreamError: {
         HandlePushLocalStreamError(event);
         break;
      }
      case CustomEvent_UserPublishCallback: {       //�������ִ
         HandleUserPublishCallback(event);
		 mbQuitAfterUpPublish = false;
         break;
      }
      case CustomEvent_OnRecvApplyInavPublishMsg: {   //�յ�����������Ϣ
         HandleRecvApplyInavPublishMsg(event);
         break;
      };
      case CustomEvent_RecvSignUsers:    //ǩ��ѧԱ�б�
         HandleRecvSignUsers(event);
         break;
      case CustomEvent_SendApplyPublish: {       //����http����������Ϣ��Ӧ
         HandleSendApplyPublishMsg(event);
         break;
      }
      case CustomEvent_RewardError: {       //����http����������Ϣ��Ӧ
         QEventRoomEvent* resEvent = dynamic_cast<QEventRoomEvent*>(event);
         if (NULL != resEvent)
         {
            FadeOutTip(QString::fromStdString(resEvent->mRespMsg), Tost_Message);
         }
         break;
      }
      case CustomEvent_OnRecvDownAllUserPublishMsg: {
         HandleRecvDownAllUserPublishMsg();
         break;
      }
      case CustomEvent_GetVHMemberList: {
         HandleGetOnLineUserList(event);
         break;
      }
      case CustomEvent_OpenHandsUp: {   // ������رվ��ֿ���http��Ӧ
         HandleEnableHandsUp(event);
         break;
      }
      case CustomEvent_DestoryWnd: {
         //ui.widget_left_tool->GetVideoChoiceUi()->
         HandleCloseWnd(event);
         break;
      }
      case CustomEvent_SendAgreePublishMsg: {
         HandleSendAgreePublishMsg(event);
         break;
      }
      case CustomEvent_SendInvitePublishMsg: {
         HandleSendInvitePublishMsg(event);
         break;
      }
      case CustomEvent_CanclePublish: {
         HandleCanclePublish(event);
         break;
      }
      case CustomEvent_UpdatePublishAbility: {
         HandleUpdatePublishAbility(event);
         break;
      }
      case CustomEvent_InviteDoubleVrtcCallback: {
         HandleInviteDoubleVrtcCallback(event);
         break;
      }
      case CustomEvent_UserPublishFailedCallback: {//����������ʧ�ܻ�ִ
         HandleUserPublishFailedCallback(event);
         break;
      }
      case CustomEvent_OnRecvKickOutRoomMsg: { //�߳����÷���
         HandleOnRecvKickOutRoomMsg(event);
         break;
      }
      case CustomEvent_OnUserPublishCallback: {  //�յ�socketio��Ϣ���û�������
         HandleRecvOnUserPublishCallback(event);
         break;
      }
      case CustomEvent_WebRtcRoomRecover: {//��������
         mMemberListWdg->on_btnRefresh_clicked();
         break;
      }
      case CustomEvent_SendKickPublishMsg: {//  ������������ص��¼�
         HandleSendKickPublishMsg(event);
         break;
      }
      case CustomEvent_SendSetBrush: { //  ���ͻ��ʻص��¼�
         HandleSendSetBrush(event);
         break;
      }
      case CustomEvent_SendForbiddenChat: { //���ͽ���ȡ�����Իص�
         HandleSendForbiddenChat(event);
         break;
      }
      case CustomEvent_SendForbiddenAllChat: {
         HandleSendForbiddenAllChat(event);
         break;
      }
      case CustomEvent_OnRecvForbiddenChatMsg: {
         HandleOnRecvForbiddenChatMsg(event);
         break;
      }
      case CustomEvent_OnRecvForbiddenAllChatMsg: {
         HandleOnRecvForbiddenAllChatMsg(event);
         break;
      }
      case CustomEvent_OnRecvUpdatePublishAbility: {
         HandleOnRecvUpdatePublishAbility(event);
         break;
      }
      case CustomEvent_MuteMic: {
         HandleMuteMic(event);
         break;
      }
      case CustomEvent_RecvMuteMic: {
         HandleRecvMuteMic(event);
         break;
      }
      case CustomEvent_MuteMicAll: {
         HandleOperaMuteAllMic(event);
         break;
      }
      case CustomEvent_AllUnPublish: {
         HandleAllUnPublish(event);
         break;
      }
      case CustomEvent_RecvAllMuteMic: {
         HandleRecvAllMuteMic(event);
         break;
      }
      case CustomEvent_MuteVideo:
      case CustomEvent_RecvMuteVideo: {
         HandleMuteVideo(event);
         break;
      }
      case CustomEvent_OnRecvOpenHandUpMsg: {
         HandleOnRecvOpenHandUpMsg(event);
         break;
      }
      case CustomEvent_RecvUserOnLine:
      case CustomEvent_RecvUserOffLIne: {
         HandleUserLineState(event);
         break;
      }
      case CustomEvent_StartRenderCameraAuxiliary: {
         HandlePlayCameraAuxiliary();
         break;
      }
      case CustomEvent_StartDoubleVrtcCallback: {
         HandleStartDoubleVrtc(event);
         break;
      }
      case CustomEvent_inviteDoubleVrtc: {
         HandleinviteDoubleVrtc(event);
         break;
      }
      case CustomEvent_InviteScreenSharing:
      {
         HandleInviteScreenSharing(event);
         break;
      }
      case CustomEvent_EndDoubleVrtcCallback: {
         HandleEndDoubleVrtc(event);
         break;
      }
      case CustomEvent_Examination: {
         mLeftToolBarWdg->StuStartExamination();
         break;
      }
	  case CustomEvent_WhiteListUserJoinRoom: {
		  HandleWhiteListUserJoinRoom(event);
		  break;
	  }
      case CustomEvent_EndDesktopSharingCallback: {
         HandleStopDesktopSharingVrtc(event);
         break;
      }
      case CustomEvent_DoubleVrtUsersCallback:
      {
         HandleDoubleVrtUsers(event);
         break;
      }
      break;
      case CustomEvent_RequestStartDoubleVrtcCallback:
      {
         HandleRequestStartDoubleVrtc(event);
         break;
      }
      break;
      case CustomEvent_DoubleVrtUsersList:
      {
         HandleGetDoubleVrtUsers(event);
         break;
      }
      break;
	  case CustomEvent_CloseTest:
	  {
		  mLeftToolBarWdg->DestoryTestPaperDlg();
	  }
	  break;
      default:
         break;
      }
      globalToolManager->GetDataManager()->WriteLog("Leave customEvent:%d", event->type());
   }
}

bool WebRtcLiveWdg::StartPushStream(int & iState)
{
	bool bRef = false;
	if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_AVCapture)) {
		globalToolManager->GetDataManager()->WriteLog(" %s : StartPushLocalStream\n", __FUNCTION__);
		iState = globalToolManager->GetClassSDK()->StartPushLocalStream();
		bRef = true;
	}

	if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_Desktop)) {
		globalToolManager->GetDataManager()->WriteLog(" %s : StartPushDesktopStream\n", __FUNCTION__);
		globalToolManager->GetClassSDK()->StartPushDesktopStream();
		bRef = true;
	}
	else if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile)) {
		globalToolManager->GetDataManager()->WriteLog(" %s : StartPushMediaFileStream\n", __FUNCTION__);
		globalToolManager->GetClassSDK()->StartPushMediaFileStream();
		bRef = true;
	}
	return bRef;
}

void WebRtcLiveWdg::HandlePublishFailed(QEvent* event)
{

	QEventVHPublishFailed* msgEvent = (QEventVHPublishFailed*)event;
	if (msgEvent)
	{
        ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
        if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            if (1 == msgEvent->miStatus) {
                QString tips = msgEvent->mstrNick + Str_NoDeviceCannotPublish;
                FadeOutTip(tips,Tost_Warning);
            }
        }
	}
}

void WebRtcLiveWdg::HandleHandDelay(QEvent* event)
{

	QEventVHPublishFailed* msgEvent = (QEventVHPublishFailed*)event;
	if (msgEvent)
	{
		mMemberListWdg->addRaiseHandsMember(msgEvent->mstrUid, COnlineItemWdg::eRaiseHandsState_normal);
		std::string strCurId =	globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id;

		if (msgEvent->mstrUid.compare(QString::fromStdString(strCurId) ) == 0)//����ǵ�ǰ�û����ֳ�ʱ����ֹͣ����ʱ
		{
			ui.classRoomTitleWd->CancleRaiseHands();
		}
	}
}

void WebRtcLiveWdg::HandleStartSign(QEvent* event)
{
   QEventStartSign* msgEvent = (QEventStartSign*)event;
   if (msgEvent)
   {

      if (msgEvent->mstrwebinar.compare(QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.id)) == 0)
      {
         std::string strCurId = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id;
         if (/*msgEvent->mstrUid.compare(QString::fromStdString(strCurId)) == 0 && */JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)//��ǰ�û����Ƿ���ǩ����
         {
            mLeftToolBarWdg->StartSignin(msgEvent->miTime, msgEvent->mstrUid);
         }
         else//ѧ����ʾǩ��ҳ��
         {
            if (nullptr == mpStuSignInDlg)
            {
               mpStuSignInDlg = new StuSignInDlg(this);
            }

            if (nullptr != mpStuSignInDlg)
            {
               mpStuSignInDlg->ShowExec(msgEvent->miTime, msgEvent->mstrUid);
            }
         }
      }
   }
}

void WebRtcLiveWdg::HandleSignIn(QEvent* event)
{
   QEventSignIn* msgEvent = (QEventSignIn*)event;
   if (msgEvent)
   {
      if (msgEvent->mstrwebinar.compare(QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.id)) == 0)
      {
         std::string strCurId = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id;
         if (/*msgEvent->mstrAddSignid.compare(QString::fromStdString(strCurId)) == 0 && */JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)//��ǰ�û����Ƿ���ǩ����
         {
            mLeftToolBarWdg->SomeOneSignin(msgEvent->mstrSigner, msgEvent->msignerNickname);
         }
         else//ѧ����ʾǩ��ҳ��
         {
            if (strCurId == msgEvent->mstrSigner.toStdString())
            {
               FadeOutTip(Str_SignSuccess, Tost_Success);
            }
            /*if (nullptr == mpStuSignInDlg)
            {
               mpStuSignInDlg = new StuSignInDlg(this);
            }

            if (nullptr != mpStuSignInDlg)
            {
               mpStuSignInDlg->exec(msgEvent->miTime, msgEvent->mstrUid);
            }*/
         }
      }
   }
}

void WebRtcLiveWdg::HandleSignStop(QEvent* event)
{
   QEventSignStop* msgEvent = (QEventSignStop*)event;
   if (msgEvent)
   {
      if (msgEvent->mstrwebinar.compare(QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.id)) == 0)
      {
         std::string strCurId = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id;
         if (/*msgEvent->mstrAddSignid.compare(QString::fromStdString(strCurId)) == 0 && */JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)//��ǰ�û����Ƿ���ǩ����
         {
            //mLeftToolBarWdg->SomeOneSignin(msgEvent->mstrSigner, msgEvent->msignerNickname);
         }
         else//ѧ���ر�ǩ��ҳ��
         {
            if (strCurId == msgEvent->mstrUid.toStdString())
            {
               FadeOutTip(Str_SignSuccess, Tost_Success);
            }
            if (nullptr != mpStuSignInDlg && mpStuSignInDlg->isVisible())
            {
               mpStuSignInDlg->CloseExec();
            }
         }
      }
   }
}

void WebRtcLiveWdg::createTimerWdg()
{
   if (nullptr == mpTimerWdg)
   {
      mpTimerWdg = new TimerWdg(this);
   }
}

void WebRtcLiveWdg::HandleStartCountdown(QEvent* event)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   createTimerWdg();
   QEventStartCountdown* msgEvent = (QEventStartCountdown*)event;
   if (msgEvent && nullptr != mpTimerWdg)
   {
      mpTimerWdg->StartCountDown(msgEvent->thirdPartyUserId, msgEvent->miplayEndSound, msgEvent->miTime);
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandleStartAnswerTool(QEvent* event)
{
	QEventStartAnswerTool* msgEvent = (QEventStartAnswerTool*)event;
	QJsonDocument doc = QJsonDocument::fromJson(msgEvent->mstrItems.toUtf8());
	//QByteArray dataArray(msgEvent->mstrItems.toStdString().c_str(), msgEvent->mstrItems.toStdString().c_str());
	//QJsonDocument doc = QJsonDocument::fromJson(dataArray);

	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

	if (doc.isArray()) {
		QJsonArray obj = doc.array();
		if (obj.count() == 2) {
			QJsonObject cmd = obj[1].toObject();
			QString items;
			QString ques_id;

			if (cmd.contains("type")) {
				QString byte_array = cmd["type"].toString();
				if (byte_array == "*stop_answer") {
					CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
					if (wnd && wnd->isVisible()) {
						CSTDMapParam mapParam;
						mapParam["close"] = "1";
						wnd->Notice(mapParam);
						return;
					}
				}
				if (cmd.contains("items") && cmd["items"].isArray()) {
					QJsonDocument document;
					document.setArray(cmd["items"].toArray());
					QByteArray byte_array = document.toJson(QJsonDocument::Compact);
					items = (QString)byte_array;
				}
				if (cmd.contains("ques_id")) {
					ques_id = QString::number(cmd["ques_id"].toInt());
				}

				if (byte_array == "*new_push_question") {
					if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
						QString online_num;
						if (cmd.contains("online_num")) {
							QJsonDocument document;
							document.setObject(cmd["online_num"].toObject());

							//QByteArray byte_array = document.toJson(QJsonDocument::Compact);
							online_num = QString::number(cmd["online_num"].toInt());

							CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
							if (wnd && wnd->isVisible()) {
								CSTDMapParam mapParam;
								mapParam[Str_Function] = Str_Function_AnswerToolTeacher;
								mapParam["new_push_question"] = "1";
								mapParam["online_num"] = online_num;
								wnd->Notice(mapParam);
							}
						}
						return;
					}
					CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
					if (wnd) {
						CSTDMapParam mapParam;
						mapParam[Str_Function] = Str_Function_AnswerToolStudent;
						mapParam[Str_Function_AnswerToolItems] = items;
						mapParam["ques_id"] = ques_id;
						wnd->setParent(this);
						wnd->Notice(mapParam);
						wnd->show();
					}
				}
				else if (byte_array == "*update_answer") {
					if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
						return;
					}
					QString online_num;
					if (cmd.contains("online_num")) {
						QJsonDocument document;
						document.setObject(cmd["online_num"].toObject());

						//QByteArray byte_array = document.toJson(QJsonDocument::Compact);
						online_num = QString::number(cmd["online_num"].toInt());
					}
					QString answers;
					if (cmd.contains("answers")) {
						QJsonDocument document;
						document.setObject(cmd["online_num"].toObject());

						//QByteArray byte_array = document.toJson(QJsonDocument::Compact);
						answers = QString::number(cmd["answers"].toInt());
					}

					CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
					if (wnd && wnd->isVisible()) {
						CSTDMapParam mapParam;
						mapParam[Str_Function] = Str_Function_AnswerToolTeacher;
						mapParam[Str_Function_AnswerToolItems] = items;
						mapParam["ques_id"] = ques_id;
						mapParam["online_num"] = online_num;
						mapParam["answers"] = answers;
						wnd->Notice(mapParam);
						wnd->show();
					}
				}
				else if (byte_array == "*new_publish_answer") {
					if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
						return;
					}
					CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
					if (wnd && wnd->isVisible()) {
						CSTDMapParam mapParam;
						mapParam[Str_Function] = Str_Function_AnswerToolStudent;
						mapParam[Str_Function_AnswerToolItems] = items;
						wnd->Notice(mapParam);
						wnd->show();
					}
				}
			}
		}
	}
}

void WebRtcLiveWdg::HandleAnswerDetail(QEvent* event) {
	globalToolManager->GetDataManager()->WriteLog("%s START ", __FUNCTION__);
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
		return;
	}
	QEventStartAnswerDetail* detailEvent = dynamic_cast<QEventStartAnswerDetail*>(event);
	if (detailEvent) {
		if (detailEvent->mcode != 200) {
		}
		else {
			CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
			if (wnd && wnd->isVisible()) {
				CSTDMapParam mapParam;
				mapParam[Str_Function] = Str_Function_AnswerToolTeacher;
				QString items = detailEvent->mstrItems;
				mapParam[Str_Function_AnswerToolItems] = items;
				mapParam["detail"] = "1";
				wnd->Notice(mapParam);
				wnd->show();
			}
		}
	}

}

void WebRtcLiveWdg::HandleStartAnswer(QEvent* event) {
	globalToolManager->GetDataManager()->WriteLog("%s START ", __FUNCTION__);
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
		return;
	}
	QEventStartAnswer* startEvent = dynamic_cast<QEventStartAnswer*>(event);
	if (startEvent) {
		if (startEvent->mcode != 200) {
			return;
		}
		else {
			CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
			if (wnd && wnd->isVisible()) {
				CSTDMapParam mapParam;
				mapParam[Str_Function] = Str_Function_AnswerToolTeacher;
				QString items = startEvent->mstrItems;
				mapParam[Str_Function_AnswerToolItems] = items;
				//mapParam["online_num"] =QString::number(GetOnlineTotal());
				mapParam["create"] = "1";
				//wnd->setParent(this);
				wnd->Notice(mapParam);
				wnd->show();
			}
		}
	}
}

void WebRtcLiveWdg::HandleKickInavStream(QEvent* event)
{
   //ui.widget_ctrl->EnableStartLive(true);
   //if (m_pScreenShareToolWgd)
   //{
   //	m_pScreenShareToolWgd->EnableStartLive(true);
   //}
   //m_SelfStopPush = false;
   QEventRecvMsg* msgEvent = (QEventRecvMsg*)event;

   ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (0 == msgEvent->mUserId.compare( QString::fromStdString(roomInfo.mUserInfo.join_id) ))
   {
      //ui.widget_ctrl->OnStartLive(false);
      //ui.widget_ctrl->intShowStartLive(per.publish_inav_stream);//���� /����
      //ui.widget_ctrl->SetToSpeakBtnState(!per.apply_inav_publish);//��������
      HandleStopLiving();
   }
   else
   {
      mMemberListWdg->addRaiseHandsMember(QString::fromStdString(roomInfo.mUserInfo.join_id), COnlineItemWdg::eRaiseHandsState_normal);
   }
}

void WebRtcLiveWdg::HandleStopLiving()
{
   QMutexLocker l(&mStreamMutexCS);
   globalToolManager->GetClassSDK()->StopPushLocalStream();
   //globalToolManager->GetPaasSDK()->StopDesktopCapture();
   //globalToolManager->GetPaasSDK()->StopMediaFileCapture();
   globalToolManager->GetClassSDK()->StopPushMediaFileStream();
   globalToolManager->GetClassSDK()->StopPushDesktopStream();
}

void WebRtcLiveWdg::HandleAuditInavPublish(QEvent* event)
{
   QEventVHMember* vhmember = (QEventVHMember*)event;
   if (vhmember)
   {
      ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      QString curId = QString::fromStdString(roomInfo.mUserInfo.join_id);
      //��ʼ��������
      if (vhmember->thirdPartyUserId.compare(curId) == 0)
      {
         globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Accept", __FUNCTION__);
         //globalToolManager->GetClassSDK()->StartPushLocalStream();
         int iStat = 0;
         StartPushStream(iStat);

         QString tips = QStringLiteral("����������ͬ��");
         FadeOutTip(tips, Tost_Success);
         ui.classRoomTitleWd->SetStudentHandsUp(true);
         //ui.classRoomTitleWd->SetToSpeakBtnState(true);
      }
      else//�ܾ�����
      {
         globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Refused", __FUNCTION__);
         QString tips = QStringLiteral("�����������뱻�ܾ�");
         FadeOutTip(tips, Tost_Warning);
         ui.classRoomTitleWd->SetStudentHandsUp(false);
         //ui.classRoomTitleWd->SetToSpeakBtnState(false);
      }

   }

}

void WebRtcLiveWdg::HandleKickOutList(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   QEventOnLineList *msgEvent = (QEventOnLineList*)event;

   QList<CMember>::iterator iter = msgEvent->mUsers.begin();
   for (; iter != msgEvent->mUsers.end(); iter++)
   {
      mMemberListWdg->addKickOutMember(*iter);
   }

}

void WebRtcLiveWdg::HandleForbiddenList(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   QEventOnLineList *msgEvent = (QEventOnLineList*)event;

   QList<CMember>::iterator iter = msgEvent->mUsers.begin();
   for (; iter != msgEvent->mUsers.end(); iter++)
   {
      mMemberListWdg->addForbidChatMember(*iter);
   }
}

void WebRtcLiveWdg::HandleHandUpList(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   QEventOnLineList *msgEvent = (QEventOnLineList*)event;

   QList<CMember>::iterator iter = msgEvent->mUsers.begin();
   QStringList strListId;
   for (; iter != msgEvent->mUsers.end(); iter++)
   {
      mMemberListWdg->addRaiseHandsMember(*iter);
      if (1==iter->is_joinmic) {
         strListId.append(iter->user_id);       
      }  
      globalToolManager->GetDataManager()->WriteLog(" %s mRenderManager->ShowRender %s is_joinmic %d", 
         __FUNCTION__, iter->user_id.toStdString().c_str(), iter->is_joinmic);
   }
   
   if (mRenderManager) {
      mRenderManager->ShowRender(strListId);
   }

   mMemberListWdg->ClearListRaiseHans();
}


void WebRtcLiveWdg::HandleGetDoubleVrtUsers(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   DoubleVrtcUsersEvent *msgEvent = (DoubleVrtcUsersEvent*)event;
   bool isvisble = mRenderManager->isVisible();
   if (mRenderManager && !isvisble) {
      if (mLeftToolBarWdg->GetDoublePushType() == DoublePubStuChoiceUI::eChoiceType_Deskop) {
         FadeOutTip(Str_NoScribeStudent_Deskop, Tost_Warning);
      }
      else if (mLeftToolBarWdg->GetDoublePushType() == DoublePubStuChoiceUI::eChoiceType_Camera) {
         FadeOutTip(Str_NoScribeStudent_DoublePush, Tost_Warning);
      }

   }
	else if (msgEvent && msgEvent->mUsers.size()>0 ){
      ui.CtrlDoubPubWdg->setEnabledSig(false);
      QString curId = ui.CtrlDoubPubWdg->CurDoubleStuId();
      ui.CtrlDoubPubWdg->initItem(DoublePubStuChoiceUI::eChoiceType_Invalid);
      mLeftToolBarWdg->initDoubleVrtcUsers(DoublePubStuChoiceUI::eChoiceType_Invalid);

      QList<CDoubleVrtcUsers>::iterator iter = msgEvent->mUsers.begin();
      for (; iter != msgEvent->mUsers.end(); iter++)//�������¸����б�����
      { 
         mLeftToolBarWdg->DoubleVrtcUsers(*iter);
         ui.CtrlDoubPubWdg->AddUser(*iter);
      }
      if (mLeftToolBarWdg) {
         mLeftToolBarWdg->slot_ShowDoublePubStuChoiceUI();
      }
      ui.CtrlDoubPubWdg->setEnabledSig(true);
      QString strUi = ui.CtrlDoubPubWdg->CurDoubleStuId().trimmed();
      ui.CtrlDoubPubWdg->setCurrentDoubleStu(strUi);
   }
   else 
	{
      mLeftToolBarWdg->CloseVrtcUsersDlg();
      slot_showInitPage();

      if (mLeftToolBarWdg->GetDoublePushType() == DoublePubStuChoiceUI::eChoiceType_Deskop) {
         FadeOutTip(Str_NoStudent_DoublePush, Tost_Warning);
      }
      else if (mLeftToolBarWdg->GetDoublePushType() == DoublePubStuChoiceUI::eChoiceType_Camera) {
         FadeOutTip(Str_NoVrtcStudent, Tost_Warning);
      } 
   }

   //�������õ�ǰcurindexʱ�� ���ܻ��������¿������� ��
   //��Ҫ��ӱ�ʶ��ֹ�˴����½����ɵ�˫�ƣ�������ǰindex˫��
   ui.CtrlDoubPubWdg->CurDoubleStuId();
}

void WebRtcLiveWdg::HandleOnLineList(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   QEventOnLineList *msgEvent = (QEventOnLineList*)event;
   mMemberListWdg->setTotalMemberCount(msgEvent->total);

   QList<CMember>::iterator iter = msgEvent->mUsers.begin();
   for (; iter != msgEvent->mUsers.end(); iter++)
   {
      if ((*iter).role_type == JOIN_ROLE_Teacher) {
         globalToolManager->GetDataManager()->SetTeacherJoinId((*iter).user_id);
      }
      mMemberListWdg->addOnlineMember(*iter);
   }

   if (msgEvent->curr_page < msgEvent->total_page)
   {
      //������һҳ����
      globalToolManager->GetClassSDK()->AsynGetClassRoomMembers(MEMBERLISTCOUNT, msgEvent->curr_page);
   }
   else//�ɹ���ȡ�������б�� �ٻ�ȡ�߳��б�
   {
      globalToolManager->GetClassSDK()->GetSilencedUserList();//��ȡ�����б�
      globalToolManager->GetClassSDK()->GetKickInavList();//��ȡ�߳��б�
      globalToolManager->GetClassSDK()->GetAlreadyHandsUpUserList();//��ȡ�����б�
   }
}

//VideoProfileIndex WebRtcLiveWdg::GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType /*= -1*/) {
//   VideoProfileIndex profileIndex = RTC_VIDEO_PROFILE_360P_4x3_M/*VIDEO_PROFILE_360P_0_15F*/;
//   if (iVideoProFileType<=0) {
//      switch (index)
//      {
//      case 240:
//         profileIndex = RTC_VIDEO_PROFILE_240P_4x3_M/*VIDEO_PROFILE_240P_0_15F*/;
//         break;
//      case 360:
//         profileIndex = RTC_VIDEO_PROFILE_360P_4x3_M/*VIDEO_PROFILE_360P_0_15F*/;
//         break;
//      case 480:
//         profileIndex = RTC_VIDEO_PROFILE_480P_4x3_M/*VIDEO_PROFILE_480P_0_15F*/;
//         break;
//      case 720:
//         profileIndex = RTC_VIDEO_PROFILE_720P_4x3_M/*VIDEO_PROFILE_720P_0_15F*/;
//         break;
//      case 1080:
//         profileIndex = RTC_VIDEO_PROFILE_1080P_4x3_M/*VIDEO_PROFILE_1080P_0_15F*/;
//         break;
//      default:
//         break;
//      }
//   }
//   else {
//      profileIndex = RTC_VIDEO_PROFILE_360P_16x9_M;
//      switch (index)
//      {
//      case 240:
//         profileIndex = RTC_VIDEO_PROFILE_240P_16x9_M/*VIDEO_PROFILE_240P_0_15F*/;
//         break;
//      case 360:
//         profileIndex = RTC_VIDEO_PROFILE_360P_16x9_M/*VIDEO_PROFILE_360P_0_15F*/;
//         break;
//      case 480:
//         profileIndex = RTC_VIDEO_PROFILE_480P_16x9_M/*VIDEO_PROFILE_480P_0_15F*/;
//         break;
//      case 720:
//         profileIndex = RTC_VIDEO_PROFILE_720P_16x9_M/*VIDEO_PROFILE_720P_0_15F*/;
//         break;
//      case 1080:
//         profileIndex = RTC_VIDEO_PROFILE_1080P_16x9_M/*VIDEO_PROFILE_1080P_0_15F*/;
//         break;
//      default:
//         break;
//      }
//   }
//   return profileIndex;
//}

void WebRtcLiveWdg::RenderSmallClass(QEventStream *msgEvent)
{
   QString join_id = msgEvent->strUser;
   int iRol = mMemberListWdg->getRolType(join_id);//JOIN_ROLE
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {//3Ϊ�ĵ� + ��Ƶ
      if (/*join_id == current_speaker*/ iRol == JOIN_ROLE_Teacher && mTeacherRender) {
         mTeacherRender->InitRenderCtrl(msgEvent->strUser);
         mTeacherRender->SetUserJoinID(msgEvent->strUser);
         mTeacherRender->SetStreamID(QString::fromStdString(msgEvent->strStream));
         mTeacherRender->SetRoleType(JOIN_ROLE_Teacher);
         mTeacherRender->SetStreamHasAudioFormat(msgEvent->mbHasAudio);
         mTeacherRender->SetStreamHasVideoFormat(msgEvent->mbHasVideo);
         if (!msgEvent->mbHasVideo) {
            mTeacherRender->SetCameraState(false);
         }

         mTeacherRender->SetStreamID(QString::fromStdString(msgEvent->strStream));
         mTeacherRender->SetPageIndex(msgEvent->mbHasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

         //HWND wnd = mTeacherRender->GetRenderWndID();
         VHStreamType  iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);
         globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
		 //globalToolManager->GetClassSDK()->StartRenderRemoteStream(msgEvent->strUser.toStdWString(), iStreamType, (void*)(wnd));
		 globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(msgEvent->strUser.toStdWString(), iStreamType, mTeacherRender->GetRenderWnd());
      }
      else {
         if (nullptr != mRenderManager)
            mRenderManager->HandleSubScribStream(msgEvent);
      }
   }
   else {//��Ƶ����
      if (/*join_id == current_speaker*/ iRol == JOIN_ROLE_Teacher && mDoubleRenderWdg) {
         mDoubleRenderWdg->SetTeacherJoinId(join_id);
         mDoubleRenderWdg->SetTeacherCameraStreamId(QString::fromStdString(msgEvent->strStream));
         mDoubleRenderWdg->StartRenderTeacherView(join_id);
      }
      else
      {
         if (nullptr != mRenderManager)
            mRenderManager->HandleSubScribStream(msgEvent);
      }
   }
}

void WebRtcLiveWdg::handleOtherMedia(QEventStream* msgEvent)
{
   mLeftToolBarWdg->SetBtnDocEnabled(false);
   mLeftToolBarWdg->SetBtnWhiteboardEnabled(false);
   HWND wnd = (HWND)(ui.otherMediaWdg->winId());
   setCurrentIndex(page_otherMediaWdg);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (CLASS_TYPE_MULTI == baseInfo.mWebinarInfo.course_type  
      && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO
      && JOIN_ROLE_Student == baseInfo.mUserInfo.role_type 
      && nullptr != mTeacherRender) {//������ �� ��Ƶģʽ  ����ѧԱ/����ģʽ
      AuxiliaryShowTeacherWdg();
   }

   globalToolManager->GetClassSDK()->StartRenderRemoteStream(msgEvent->strUser.toStdWString(), (vlive::VHStreamType)msgEvent->mStreamType, (void*)(wnd));
   //globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(msgEvent->strUser.toStdWString(), (vlive::VHStreamType)msgEvent->mStreamType, ui.otherMediaWdg);
}

void WebRtcLiveWdg::slot_NoticeShowNickName()
{
   if (isVisible() && !isMinimized()&& page_otherMediaWdg == ui.stackedWidget_Center->currentIndex()) {
      //if (ui.mpVhallRenderMaskWdg) {
      //   //int iW = ui.stackedWidget_Center->width();
      //   //int iH = ui.stackedWidget_Center->height();
      //   //if (ui.mpVhallRenderMaskWdg->geometry().width() != iW
      //   //   || ui.mpVhallRenderMaskWdg->geometry().height() != iH){
      //   //   ui.mpVhallRenderMaskWdg->setFixedWidth(iW);
      //   //   ui.mpVhallRenderMaskWdg->setFixedHeight(iH);
      //   //}

      //   //QPoint pos = ui.stackedWidget_Center->mapToGlobal(QPoint(0, 0));
      //   //ui.mpVhallRenderMaskWdg->move(pos.rx(), pos.ry() + ui.stackedWidget_Center->height() - ui.mpVhallRenderMaskWdg->height());
      //   ui.mpVhallRenderMaskWdg->show();
      //}
   }

   //if (mTeacherRender && !mTeacherRender->isHidden()) {
   //   mTeacherRender->slot_NoticeShowNickName();
   //}
}

//void WebRtcLiveWdg::handleRemoveRemoteStream(QEventStream* msgEvent)
//{
//   //mPreRemoveListRemoteStream.push_back(msgEvent->strStream);
//}

void WebRtcLiveWdg::handleOtherMediaRemote(QEventStream* msgEvent)
{
   if ( (page_InvStateWdg ==ui.stackedWidget_Center->currentIndex() && ui.invStateWdg->StateType() == InvitationStateWdg::eStateType_FrameGeting) 
      || (ui.CtrlDoubPubWdg->isChangeUser()) ) {

   }
   else {
      //ui.CtrlDoubPubWdg->SetTypehttp://chandao.vhallops.com/zentao/file-read-24657.png(DoublePubStuChoiceUI::eChoiceType::eChoiceType_Invalid);
      globalToolManager->GetDataManager()->WriteLog("%s slot_showInitPage", __FUNCTION__);
      mLeftToolBarWdg->SetBtnDocEnabled(true);
      mLeftToolBarWdg->SetBtnWhiteboardEnabled(true);
      if (page_InvStateWdg != ui.stackedWidget_Center->currentIndex()) {//����˫�Ƴ�Ա������ʾҳ��
         slot_showInitPage();
      }
      
   }

   ui.CtrlDoubPubWdg->CurDoubleStuId();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (CLASS_TYPE_MULTI == baseInfo.mWebinarInfo.course_type
      && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO
      && JOIN_ROLE_Student == baseInfo.mUserInfo.role_type) {//������ �� ��Ƶģʽ  ����ѧԱ/����ģʽ
      recoveryOnlyVideoLayOut();
   }
}

void WebRtcLiveWdg::HandleRemoteStream(QEvent* event)
{
   QEventStream *msgEvent = (QEventStream*)event;
   if (msgEvent) {
      std::string strStreamId = msgEvent->strStream;
      switch (msgEvent->mStreamType)
      {
      case VHStreamType_AUDIO:
      case VHStreamType_VIDEO:
      case VHStreamType_AVCapture://ͨ��������ˣ��˴��ݲ�����
      {
         if (mRenderManager) {
            mRenderManager->RemovePublishUser(msgEvent->strUser );
         }   
      }
      break;
      case VHStreamType_Desktop:    //���湲������
      case	VHStreamType_MediaFile:   //�岥��Ƶ����
      case	VHStreamType_SoftWare:   //�����������
		{
			handleOtherMediaRemote(msgEvent);
		}
		break;
      case VHStreamType_Auxiliary_CamerCapture://˫������ͷ
      {
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher && globalToolManager->GetClassSDK()->IsExitSubScribeStream(msgEvent->strStream, VHStreamType_Auxiliary_CamerCapture)
            || globalToolManager->GetClassSDK()->IsExitSubScribeStream(msgEvent->strStream, VHStreamType_Stu_Desktop)) {
            handleOtherMediaRemote(msgEvent);
         }
         else {
            handleOtherMediaRemote(msgEvent);
         }
      }
      break;
      case VHStreamType_Stu_Desktop://ѧԱ���湲��
      {
			if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher && globalToolManager->GetClassSDK()->IsExitSubScribeStream(msgEvent->strStream,VHStreamType_Auxiliary_CamerCapture)
				|| globalToolManager->GetClassSDK()->IsExitSubScribeStream(msgEvent->strStream, VHStreamType_Stu_Desktop)) {
				handleOtherMediaRemote(msgEvent);
			}    
			else {
				handleOtherMediaRemote(msgEvent);
			}

         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher ) {
            mMixStreamLayout.SetMixStreamLayout(true);
         }
      }
      break;
      default:
         break;
      }
   }
}

void WebRtcLiveWdg::HandleSubScribStream(QEvent* event) {
   QEventStream *msgEvent = (QEventStream*)event;

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   globalToolManager->GetDataManager()->WriteLog("%s class state %d", __FUNCTION__, (int)baseInfo.mWebinarInfo.type);
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      if (msgEvent) {
         switch (msgEvent->mStreamType)
         {
         case VHStreamType_AUDIO:
         case VHStreamType_VIDEO:
         case VHStreamType_AVCapture: {
            handleAVCapture(msgEvent);
            break;
         }                     
         case VHStreamType_Desktop: {    //���湲������
            QString strJoineId = QString::fromStdString(baseInfo.mUserInfo.join_id).trimmed();
            if (JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type && 0 != msgEvent->strUser.compare(strJoineId)) {//���ǽ�ʦ�����湲��   ѧԱ˫��˫��
               //msgEvent->mStreamType = VHStreamType_Stu_Desktop;
               showAuxiliaryWdg(msgEvent, DoublePubStuChoiceUI::eChoiceType_Deskop);
              // mMixStreamLayout.SetMixStreamLayout(true, layout, msg->mStreamType == VHStreamType_Desktop ? true : false);
               SetDesktopMeidaMixStreamLayout(VHStreamType_Desktop);
            }
            else{
               HideNickName();
               ui.CtrlDoubPubWdg->hide();
               handleOtherMedia(msgEvent);
            }
         }
         break;
         case VHStreamType_MediaFile:  //�岥��Ƶ����
         case VHStreamType_SoftWare:   //�����������
         {
            HideNickName();
            ui.CtrlDoubPubWdg->hide();
            handleOtherMedia(msgEvent);
         }
         break;
         case VHStreamType_Auxiliary_CamerCapture://˫������ͷ
         {
            showAuxiliaryWdg(msgEvent, DoublePubStuChoiceUI::eChoiceType_Camera);
            break;
         }
           
         case VHStreamType_Stu_Desktop://˫��˫��
         {

            showAuxiliaryWdg(msgEvent, DoublePubStuChoiceUI::eChoiceType_Deskop);
         }
         break;
         default:
            break;
         }

         
         //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         //   mRecivePubIdList.append(msgEvent->strUser);
         //   QTimer::singleShot(5000, this, SLOT(slotCheckUserIsPublish()));
         //}
      }
   }

}

void WebRtcLiveWdg::slotCheckUserIsPublish()
{
   while (!mRecivePubIdList.isEmpty())
   {
      QString strId = mRecivePubIdList.takeFirst();
      //�����˼�������ɹ��û��Ƿ������ϣ���ֹ���û��쳣���ߺ��߳�����û���յ��߳���Ϣ������ָ����Լ�����������
      globalToolManager->GetClassSDK()->CheckUserIsPublish(strId.toStdString());

   }

}

void WebRtcLiveWdg::slot_KickUserPublish(QString strId)
{
   if (!mstrIDPenAuth.isEmpty()) {
      globalToolManager->GetClassSDK()->ChangeUserPenAuth(mstrIDPenAuth.toStdString(), false);
   }

   globalToolManager->GetClassSDK()->KickUserPublish(strId.toStdString());
}

void WebRtcLiveWdg::slot_AuditPublish(QString strId)
{
   if (addApplyPublishUser(strId)) {
      globalToolManager->GetClassSDK()->AuditPublish(strId.toStdString());
   }
}

void WebRtcLiveWdg::slot_AskforPublish(QString strId)
{
   if (addApplyPublishUser(strId)) {
      globalToolManager->GetClassSDK()->AskforPublish(strId.toStdString());
   }  
}

bool WebRtcLiveWdg::addApplyPublishUser(QString strUser)
{
   bool bRef = false;
   int JoinMaxNum = 1;
   ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (CLASS_TYPE_MULTI == roomInfo.mWebinarInfo.course_type ) {

   }
   else if (CLASS_TYPE_SMALL == roomInfo.mWebinarInfo.course_type) {
      switch (roomInfo.mWebinarInfo.linkmic_apply_type)
      {
      case CLASS_JOIN_MEMBER_1V4:
      {
         JoinMaxNum = 4;
      }
      break;
      case CLASS_JOIN_MEMBER_1V8:
      {
         JoinMaxNum = 8;
      }
      break;
      case CLASS_JOIN_MEMBER_1V12:
      {
         JoinMaxNum = 12;
      }
      break;
      default: break;
      }
   }
   
   int curJoinNum = mstrApplyPublishUsers.size();
   if (mRenderManager) {
      curJoinNum += mRenderManager->iRenderSize();
   }

   if (curJoinNum >= JoinMaxNum) {
         FadeOutTip(Str_IsInvitedUser, Tost_Message, false);
    }
   else
   {
      mstrApplyPublishUsers.insert(strUser, QDateTime::currentDateTime());
      if (nullptr == mpApplyPublishTime)
      {
         mpApplyPublishTime = new QTimer();
         connect(mpApplyPublishTime, &QTimer::timeout, this, &WebRtcLiveWdg::slot_ApplyPublishTimeOut);
      }

      if (mpApplyPublishTime && !mpApplyPublishTime->isActive()) {
         mpApplyPublishTime->start(10000);
      }
      bRef = true;
   }
   return bRef;
}

void WebRtcLiveWdg::removeApplyPublishUser(QString strUser)
{
   if (mpApplyPublishTime && mpApplyPublishTime->isActive())
   {
      mpApplyPublishTime->stop();
   }

   QDateTime  curTime = QDateTime::currentDateTime();
   QDateTime  offsetTime = curTime.addSecs(-10);

   //QMap<QString, QDateTime>::iterator iter = mstrApplyPublishUsers.begin();
   QList<QString>  strKeys = mstrApplyPublishUsers.keys();
   QList<QString>::iterator ite = strKeys.begin();
   while (ite != strKeys.end())
   {
      QMap<QString, QDateTime>::iterator itMap = mstrApplyPublishUsers.find( *ite );
      if (offsetTime >= itMap.value() || (!strUser.isEmpty() && strUser.compare(itMap.key().trimmed()) == 0))
      {
         mstrApplyPublishUsers.erase(itMap);
      }
      ite++;
   }

   if (mpApplyPublishTime && mstrApplyPublishUsers.size() > 0)
   {
      mpApplyPublishTime->start(10000);
   }
}

void WebRtcLiveWdg::slot_ApplyPublishTimeOut() {
   removeApplyPublishUser("");
}

void WebRtcLiveWdg::LiveTypeChangeToLogout()
{
   TipsWdg* tips = new TipsWdg(QStringLiteral("���Ż��������ı䣬���ý��˳��������½���"), this, QStringLiteral("ȷ��"));
   if (tips) {
      //CSTDMapParam param;
      //param[TipsMsg] = QStringLiteral("�Ƿ�ȷ�Ͻ�ɫ�˳�?");
      //param[ShowCancelButton] = PARAM_TRUE;
      //int createDefaultRecord = 0;
      //tips->Notice(param);
      int accept = tips->exec();
      if (accept == QDialog::Accepted) {
         globalToolManager->LogOut();
      }
   }

   //TipsWdg* tips = new TipsWdg();
   //if (tips) {
   //   CSTDMapParam param;
   //   //param[TipsMsg] = ;
   //   param[ShowCancelButton] = PARAM_TRUE;
   //   if (ui.classRoomTitleWd->GetStartTime() > 30) {
   //      param[ShowRecordSet] = "1";
   //   }
   //   int createDefaultRecord = 0;
   //   //tips->Notice(param);
   //   int accept = tips->exec();
   //   if (accept == QDialog::Accepted) {
   //      globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   //      //tips->GetParam(param);
   //      delete tips;
   //      tips = NULL;
   //      CBaseWnd* AnswerWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
   //      if (AnswerWdg) {
   //         globalToolManager->GetWndManager()->DestoryWnd(WND_ID_ANSWER_TOOL);
   //      }

   //      std::map<QString, QString>::iterator iterShowRecordSet = param.find(QString::fromStdString(ShowRecordSet));
   //      if (iterShowRecordSet != param.end()) {
   //         createDefaultRecord = iterShowRecordSet->second.toInt() == Qt::CheckState::Checked ? 1 : 0;
   //      }

   //      globalToolManager->GetClassSDK()->StopClass(createDefaultRecord);
   //      mbQuitAfterUpPublish = true;

   //      //bRef = true;
   //   }
   //}
}


void WebRtcLiveWdg::AuxiliaryShowTeacherWdg()
{
   if (nullptr != mTeacherRender) {
      ui.verticalLayout_CenterRender->removeWidget(mTeacherRender);
      ui.widget_leftTopRender->show();
      ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
      globalToolManager->GetDataManager()->WriteLog(" %s 2897  ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
         __FUNCTION__);
      mTeacherRender->InitRenderInsertPos(InsertPos_Speaker);
      mTeacherRender->setFixedWidth(320);
      mTeacherRender->setFixedHeight(240);
      mTeacherRender->ShowBigRenderCtrl(false);
   }
}

void WebRtcLiveWdg::showAuxiliaryWdg(QEventStream* msgEvent, DoublePubStuChoiceUI::eChoiceType type){
   handleOtherMedia(msgEvent);
   ui.invStateWdg->stopTimer();

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type) {
		QString stream_id = QString::fromStdString(msgEvent->strStream);
      ToolManager *callbackTool = globalToolManager;
      int layout = baseInfo.mWebinarInfo.videoPushAndMixSetting.share_desktop_mixed_flow_layout == 0 ? CANVAS_LAYOUT_PATTERN_GRID_1 : CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
      //���ݺ�̨�·��Ĳ����ж�ѧԱ�Ƿ���Ҫ����������ʾ�����˻��档
      if (type == DoublePubStuChoiceUI::eChoiceType::eChoiceType_Deskop) {
         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && type == VHStreamType_Desktop) {
            //�����θ������ö�ȡ�������͡�
            //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
			 QString msConfPath = globalToolManager->GetConfigPath();
            int desktop_layout = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_on_camera);
            if (desktop_layout == ShareScreenLayout_camera_top) {
               layout = CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL;
            }
            else {
               layout = CANVAS_LAYOUT_PATTERN_GRID_1;
            }
         }
         BROAD_CAST_DPI profile = BROAD_CAST_DPI_1920_1080;
         int nRet = globalToolManager->GetClassSDK()->StartBroadCastLayout((LayoutMode)layout, profile,
            [&, callbackTool, stream_id](const std::string& result, const std::string& msg) {
            if (result == "success") {
               callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomRecvMsgEvent(CustomEvent_SetLayoutCameraAuxiliary, stream_id));
            }           
         });
      }
      else {
         globalToolManager->GetClassSDK()->SetConfigBroadCastLayOut((LayoutMode)layout, [&, callbackTool, stream_id](const std::string& result, const std::string& msg)->void {
            if (result == "success") {
               callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomRecvMsgEvent(CustomEvent_SetLayoutCameraAuxiliary, stream_id));
            }
         });
      }

      ui.CtrlDoubPubWdg->show();
      ui.CtrlDoubPubWdg->setCurComboBoxById(msgEvent->strUser);
   }

   if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO ) {
      AuxiliaryShowTeacherWdg();
   }

   ui.CtrlDoubPubWdg->SetType(type);
   if (!ui.CtrlDoubPubWdg->isInitDoublePushComb() && JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type) {
      globalToolManager->GetClassSDK()->doubleVrtcUsers();
      CDoubleVrtcUsers vrtc;
      vrtc.mstrId = msgEvent->strUser;
      vrtc.mstrNickName = msgEvent->strUser;
      vrtc.miNoDoubleVrtc = 0;
      
      ui.CtrlDoubPubWdg->AddUser(vrtc);
      
   }

   ui.CtrlDoubPubWdg->setCurrentDoubleStu(msgEvent->strUser);
   if (VHStreamType_Auxiliary_CamerCapture == msgEvent->mStreamType) {
      mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_StuDoublCamera);
      //mLeftToolBarWdg->SetDoubleCamera(true);
   }
   else if(VHStreamType_Stu_Desktop == msgEvent->mStreamType || VHStreamType_Desktop == msgEvent->mStreamType) {
      mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_StuDesktopShar);
   }
   
   if (nullptr != mLeftToolBarWdg) {
      mLeftToolBarWdg->slot_InvitaStateChanged(InvitationStateWdg::eStateType_DesktopSharing);
   }

   //if (nullptr == mpVhallRenderMaskWdg)
   //{
   //   mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
   //}

  // if (nullptr != ui.mpVhallRenderMaskWdg)
  // {
		//if (mDoublePushNickName.isEmpty()) {
  //       ui.mpVhallRenderMaskWdg->SetUserName(GetMemberPersonalInfo(msgEvent->strUser).nick_name);
		//}
		//else {
  //       ui.mpVhallRenderMaskWdg->SetUserName(mDoublePushNickName);
		//}
  //    ui.mpVhallRenderMaskWdg->show();
  // }

   //if (nullptr == mUserNickNameShowTimer) {
   //   mUserNickNameShowTimer = new QTimer(this);
   //   connect(mUserNickNameShowTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_NoticeShowNickName);
   //}

   //if (nullptr != mUserNickNameShowTimer) {
   //   mUserNickNameShowTimer->start(10);
   //}
}


void WebRtcLiveWdg::handleAVCapture(QEventStream* msgEvent)
{
   globalToolManager->GetDataManager()->WriteLog("%s ", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   QString join_id = msgEvent->strUser;
   QString techerid = globalToolManager->GetDataManager()->GetTeacherJoinId();
   globalToolManager->GetDataManager()->WriteLog("%s join_id %s  techerid %s", __FUNCTION__, join_id.toStdString().c_str(), techerid.toStdString().c_str());
   //���������˵�
   if (join_id == techerid && mTeacherRender) {
	  globalToolManager->GetDataManager()->WriteLog("%s join_id %s", __FUNCTION__, join_id.toStdString().c_str());
      mTeacherRender->InitRenderCtrl(msgEvent->strUser);
      mTeacherRender->SetUserJoinID(msgEvent->strUser);
      mTeacherRender->SetStreamID(QString::fromStdString(msgEvent->strStream));
      mTeacherRender->SetRoleType(JOIN_ROLE_Teacher);
      mTeacherRender->SetStreamHasAudioFormat(msgEvent->mbHasAudio);
      mTeacherRender->SetStreamHasVideoFormat(msgEvent->mbHasVideo);
      if (!msgEvent->mbHasVideo) {
         mTeacherRender->SetCameraState(false);
      }
      mTeacherRender->SetStreamID(QString::fromStdString(msgEvent->strStream));
      mTeacherRender->SetPageIndex(msgEvent->mbHasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);

      //HWND wnd = mTeacherRender->GetRenderWndID();
      std::shared_ptr<vhall::VideoRenderReceiveInterface> pwd = mTeacherRender->GetRenderWnd();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != join_id.toStdString()){
		 //wnd = mTeacherRender->GetSmallRenderRenderWndID();
		 pwd = mTeacherRender->GetSmallRenderRenderWnd();
         globalToolManager->GetDataManager()->WriteLog("StartRenderRemoteStream  small");
      }
      VHStreamType iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);
      globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s  teacher join_id %s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str(), join_id.toStdString().c_str());
	  //globalToolManager->GetClassSDK()->StartRenderRemoteStream(msgEvent->strUser.toStdWString(), iStreamType, (void*)(wnd));
	  globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(msgEvent->strUser.toStdWString(), iStreamType, pwd);

      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         if (!globalToolManager->GetDataManager()->IsMultiClassOtherUserCameraOpen()) {
            globalToolManager->GetClassSDK()->StopRenderRemoteStream(msgEvent->strUser.toStdWString(),"",VHStreamType_AVCapture);
            mTeacherRender->SetTeacherCameraCloseSheet();
         }
      }

      ////�����˻����������仯��������ڴ�����
      //QString floatWndID = WND_ID_FLOAT_RENDER + join_id;
      //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + join_id;
      //CSTDMapParam mapParam;
      //mapParam[Str_Function] = Str_Function_SubScribe;
      //mapParam[STREAM_ID] = QString::fromStdString(msgEvent->strStream);
      //mapParam[USER_ID] = join_id;
      //mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = msgEvent->mbHasAudio == false ? PARAM_TRUE : PARAM_FALSE;
      //mapParam[TEACHER_VIEW] = PARAM_TRUE;
      //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
      //WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;

      //if (pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(join_id)/*globalToolManager->GetWndManager()->IsWndExist(floatWndID)*/) {
      //   FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);
      //   if (wnd) {
      //      wnd->Notice(mapParam);
      //      wnd->show();
      //   }
      //}
      //else if (mTeacherRender->IsFullRenderVisible() /*globalToolManager->GetWndManager()->IsWndExist(fullWndID)*/) {

      //   CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, join_id);
      //   if (wnd) {
      //      wnd->Notice(mapParam);
      //      wnd->show();
      //   }
      //   mTeacherRender->FullRenderNotice(mapParam);
      //   mTeacherRender->showFullRender();
      //}
      mRecordAndLiveStreamCefViewWdg->hide();
      if (mTeacherRender) {
         mTeacherRender->show();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
      }
      ui.widget_main_title->hide();

   }
   else {
     
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) { //����С���
		 globalToolManager->GetDataManager()->WriteLog("%s  CLASS_TYPE_SMALL", __FUNCTION__);
         if (mRenderManager->IsExistRender(join_id)) {
            mRenderManager->HandleSubScribStream(msgEvent);
         }
         else {
            CMember info;
            bool bFind = mMemberListWdg->GetMember(join_id, info);
			globalToolManager->GetDataManager()->WriteLog("%s new render join_id %s", __FUNCTION__, join_id.toStdString().c_str());
            mRenderManager->AddInitReadyPublishUser(join_id, info.nick_name, info.role_type, msgEvent->mbHasVideo, msgEvent->mbHasAudio);
            //mMixStreamLayout.SetMixStreamLayout();
            if (bFind)
            {
            }
            else
            {
				//mMemberListWdg->SetInviteList(join_id.toStdString());
               globalToolManager->GetDataManager()->WriteLog("Can't Find Member(%s) From member list", join_id.toStdString().c_str());
            }
         }
      }
      //��������
      else {
         HandleOpenClassSubScribStream(msgEvent);
         //if (mTeacherRender) {
         //   //QString mainUserId = globalToolManager->GetDataManager()->GetTeacherJoinId();
         //   //�����˻����������仯��������ڴ�����
         //   //QString floatWndID = WND_ID_FLOAT_RENDER + mainUserId;
         //   //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + mainUserId;
         //  /* CSTDMapParam mapParam;
         //   mapParam[Str_Function] = Str_Function_SubScribe;
         //   mapParam[STREAM_ID] = QString::fromStdString(msgEvent->strStream);
         //   mapParam[USER_ID] = join_id;
         //   mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = msgEvent->mbHasAudio == false ? PARAM_TRUE : PARAM_FALSE;
         //   mapParam[TEACHER_VIEW] = PARAM_FALSE;
         //   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
         //   WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;*/
         //   //if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(mainUserId)/*globalToolManager->GetWndManager()->IsWndExist(floatWndID)*/) {
         //   //   //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FLOAT_RENDER, mainUserId);
         //   //   FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(mainUserId);
         //   //   if (wnd) {
         //   //      wnd->Notice(mapParam);
         //   //      wnd->show();
         //   //   }
         //   //}
         //   //else if (mTeacherRender->IsFullRenderVisible() /*globalToolManager->GetWndManager()->IsWndExist(fullWndID)*/ ) {

         //   //   /*CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, mainUserId);
         //   //   if (wnd) {
         //   //      wnd->Notice(mapParam);
         //   //      wnd->show();
         //   //   }*/

         //   //   mTeacherRender->FullRenderNotice(mapParam);
         //   //   mTeacherRender->showFullRender();
         //   //}
         //}
      }
   }
   //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
   //   mMemberListWdg->SetUpperStates(join_id, (int)COnlineItemWdg::eRaiseHandsState_Speaking);
   //}

}

void WebRtcLiveWdg::HandleOpenClassSubScribStream(QEvent* event) {
   QEventStream *msgEvent = (QEventStream*)event;
   if (mTeacherRender) {
      globalToolManager->GetDataManager()->WriteLog("%s join_id:%s stream_id:%s", __FUNCTION__, msgEvent->strUser.toStdString().c_str(), msgEvent->strStream.c_str());
      mTeacherRender->SetSmallWndUserJoinID(msgEvent->strUser);
      mTeacherRender->SetSmallWndStreamID(QString::fromStdString(msgEvent->strStream));
      mTeacherRender->SetSmallWndStreamHasAudioFormat(msgEvent->mbHasAudio);
      mTeacherRender->SetSmallWndStreamHasVideoFormat(msgEvent->mbHasVideo);
      mTeacherRender->SetShowSmallRender(true);
      mTeacherRender->ShowAllMuteCtrl(true);
	  mTeacherRender->SetSmallRenderCameraState(msgEvent->mbHasVideo);
      //HWND wnd_id = nullptr;
     std::shared_ptr<vhall::VideoRenderReceiveInterface> pwd;
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.bigScreen == 0 || (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id)) {
		//wnd_id = (HWND)(mTeacherRender->GetSmallRenderRenderWndID());
		pwd = mTeacherRender->GetSmallRenderRenderWnd();
         globalToolManager->GetDataManager()->WriteLog("StartRenderRemoteStream big");
      } else {
		 //wnd_id = (HWND)(mTeacherRender->GetRenderWndID());
		 pwd = mTeacherRender->GetRenderWnd();
         globalToolManager->GetDataManager()->WriteLog("StartRenderRemoteStream small");
      }
      globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__,QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
      VHStreamType iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);
      globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(msgEvent->strUser.toStdWString(), iStreamType, VHSimulCastType_SmallStream);
      //globalToolManager->GetClassSDK()->StartRenderRemoteStream(msgEvent->strUser.toStdWString(), iStreamType, (void*)(wnd_id));
      globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(msgEvent->strUser.toStdWString(), iStreamType, pwd);
   }
}

void WebRtcLiveWdg::HandleChangeToolPage(QEvent* event)
{
   if (event == NULL) {
      return;
   }
   CustomChangeToolPage *msgEvent = (CustomChangeToolPage*)event;

   switch (msgEvent->miPage)
   {
   case eToolPage_Doc:
   {
      int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
      if (JOIN_ROLE_Student == roleType) {
         globalToolManager->GetClassSDK()->ChangeDocMode(0);
      }

      slot_btnDoc();
   }
   break;
   case eToolPage_Board:
   {
      int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
      if (JOIN_ROLE_Student == roleType){
         globalToolManager->GetClassSDK()->ChangeDocMode(1);
      }

      slot_Whiteboard(true);
   }
   break;
   case eToolPage_Answer_Open:
   {
      slot_Answer();
   }
   break;
   case eToolPage_Answer_Close:
   {

   }
   break;

   default:
      break;
   }
}

CMember WebRtcLiveWdg::GetMemberPersonalInfo(const QString& strUserId)
{
   return mMemberListWdg->GetMemberPersonalInfo(strUserId);
	
}

void WebRtcLiveWdg::HandleKickOut(QEvent* event)
{
   if (NULL == event)
      return;

   QEventRoomEvent* msgEvent = (QEventRoomEvent*)event;
   if (NULL != msgEvent && 200 == msgEvent->mRespCode) {
      CMember info = mMemberListWdg->GetMemberPersonalInfo(msgEvent->mUserId);
      if (msgEvent->mUserData.toInt() == 1) {
         if (!info.nick_name.isEmpty()) {
            QString tips = info.nick_name + Str_KickOutClassNotice;
            FadeOutTip(tips, Tost_Warning);
         }
      }
      else {
         if (!info.nick_name.isEmpty()) {
            QString tips = info.nick_name + Str_CancelKickOutUser;
            FadeOutTip(tips, Tost_Message);
         }
      }
      mMemberListWdg->addKickOutMember(msgEvent->mUserId, msgEvent->mProcessType);
   }
   else if (NULL != msgEvent) {
      FadeOutTip(QString::fromStdString(msgEvent->mRespMsg), Tost_Message);
   }
}

void WebRtcLiveWdg::HandleChangeBigScreen(QEvent* event) {
   CustomEventType* msgEvent = dynamic_cast<CustomEventType*>(event);
   if (msgEvent) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      QString big_screen_user = msgEvent->msg;
      if (mTeacherRender) {
         mTeacherRender->ChangeBigScreen(big_screen_user);
         if(ui.stackedWidget_Center->currentIndex() == page_DesktopScreen || ui.stackedWidget_Center->currentIndex() == page_MediaFilePlay) {
            return;
         }
         QString big_user_id = mTeacherRender->GetUserJoinID();
         QString samll_user_id = mTeacherRender->GetSmallWndUserJoinID();
         QString stream_id;
         if (big_user_id == big_screen_user) {
            stream_id = mTeacherRender->GetStreamID();
         } else {
            stream_id = mTeacherRender->GetSmallWndStreamID();
         }
         globalToolManager->GetClassSDK()->SetMainView(stream_id.toStdString());
         globalToolManager->GetDataManager()->WriteLog("%s %s", big_screen_user.toStdString().c_str(), stream_id.toStdString().c_str());
      }      
   }
}

void WebRtcLiveWdg::HandleRewardStu(QEvent*event)
{
   QEventVHMember* msgEvent = (QEventVHMember*)event;
   if (msgEvent)
   {

      int iAddRewardNum = 0;
      if (msgEvent->memberStatusType >= 0)
         iAddRewardNum = msgEvent->memberStatusType;
      //QString strTip = msgEvent->strNick + QStringLiteral("���") + QString::number(iAddRewardNum) + QStringLiteral("��������");
      //FadeOutTip(strTip);

      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (msgEvent->thirdPartyUserId.trimmed().compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0)//����
         ui.classRoomTitleWd->AddTrophyCount(iAddRewardNum);

      mRenderManager->AddTrophyCount(msgEvent->thirdPartyUserId, iAddRewardNum);
   }
}

void WebRtcLiveWdg::HandleCancelHand(QEvent* event)
{
   QEventVHPublishFailed* msgEvent = (QEventVHPublishFailed*)event;
   if (msgEvent)
   {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student &&//���ʱ��ǰ�ľ����ߣ��յ����Լ���ȡ��������Ϣ����ֹͣ����ʱ
         msgEvent->mstrUid.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0) {
         ui.classRoomTitleWd->CancleRaiseHands();
      }
      else//��������յ�ȡ��������Ϣ�����¾����б�
      {
         QString msg = msgEvent->mstrNick + QStringLiteral("ȡ������");
         FadeOutTip(msg, Tost_Message);
         mMemberListWdg->addRaiseHandsMember(msgEvent->mstrUid, COnlineItemWdg::eRaiseHandsState_normal);
      }
   }
}

void WebRtcLiveWdg::slot_closeMultiFloat(QString strId) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (mMultiClassFloatWnd && /*mMultiClassFloatWnd->isVisible() &&*/ mTeacherRender /*mTeacherRender->UserId().compare(strId)==0 */) {
      if (mTeacherRender->GetFloatType() == WndFloatType_Full) {
         show();
      }
      TeacherRenderWdg* pRender = nullptr;
      QWidget* pWidget = mMultiClassFloatWnd->RenderWidget();
      if (pWidget && pWidget->inherits("TeacherRenderWdg") ) {
         pRender = qobject_cast<TeacherRenderWdg*>(pWidget);
      }

      mMultiClassFloatWnd->hide();
      mMultiClassFloatWnd->RemoveRenderWidget(nullptr);
      
      if (pRender == mTeacherRender) {
         if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC && mTeacherRender->GetFloatType() == WndFloatType_Float) {
            ui.widget_leftTopRender->show();
            ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
            globalToolManager->GetDataManager()->WriteLog(" %s 3322 mWebinarInfo.layout %d mTeacherRender->GetFloatType() %d ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
               __FUNCTION__, baseInfo.mWebinarInfo.layout, mTeacherRender->GetFloatType());
            mTeacherRender->setFixedSize(QSize(320, 240));
         }
         else if ((baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC )
            && mTeacherRender->GetFloatType() == WndFloatType_Full) {
            ui.widget_leftTopRender->show();
            ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
            globalToolManager->GetDataManager()->WriteLog(" %s 3330 mWebinarInfo.layout %d mTeacherRender->GetFloatType() %d ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
               __FUNCTION__, baseInfo.mWebinarInfo.layout, mTeacherRender->GetFloatType());
            mTeacherRender->setFixedSize(QSize(320, 240));
         }
         else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO && mTeacherRender->GetFloatType() == WndFloatType_Full) {
            if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture) 
               || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
               || isEixMediaScreen()
               /*globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_MediaFile)
               || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Desktop)
               || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_SoftWare)*/
               ) {
               ui.widget_leftTopRender->show();
               ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
               globalToolManager->GetDataManager()->WriteLog(" %s 3343  ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
                  __FUNCTION__);
               mTeacherRender->setFixedSize(QSize(320, 240));
            }
            else
            {
               ui.verticalLayout_CenterRender->addWidget(mTeacherRender);
               mTeacherRender->setFixedSize(QSize(ui.page_CenterRender->width(), ui.page_CenterRender->height()));
            }
         }
         mTeacherRender->SetFloatType(WndFloatType_Normal);
      }
      }
      
   this->activateWindow();
}

void WebRtcLiveWdg::slot_closeFloat(QString strId)
{
   if (mTeacherRender->GetUserJoinID().compare(strId) == 0) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      //QMap<QString, MultiClassFloatWnd*>::iterator it = mapMultiClassFloatWnd.find(strId);
      if (mFloatWnd /*&& mFloatWnd->isVisible()*/) {
         //MultiClassFloatWnd* pMultiClassFloatWnd = it.value();

 /*        if (mFloatWnd)
         {*/
         TeacherRenderWdg* pRender = nullptr;
         QWidget* pWidget = mFloatWnd->RenderWidget();
         if (pWidget && pWidget->inherits("TeacherRenderWdg")) {
            pRender = qobject_cast<TeacherRenderWdg*>(pWidget);
         }

            if ( mTeacherRender && pRender == mTeacherRender) {
               if (mTeacherRender->GetFloatType() == WndFloatType_Full) {
                  show();
               }
               mFloatWnd->hide();
               mFloatWnd->RemoveRenderWidget(nullptr);
               if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC && mTeacherRender->GetFloatType() == WndFloatType_Float) {//�ĵ���Ƶģʽ ����
                  ui.widget_leftTopRender->show();
                  ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
                  globalToolManager->GetDataManager()->WriteLog(" %s 3385 ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
                     __FUNCTION__);
                  mTeacherRender->setFixedSize(QSize(320, 240));
               }
               else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC ) {//�ĵ���Ƶģʽ ȫ��Ļ
                  ui.widget_leftTopRender->show();
                  ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
                  globalToolManager->GetDataManager()->WriteLog(" %s 3392 ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
                     __FUNCTION__);
                  mTeacherRender->setFixedSize(QSize(320, 240));
               }
               else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
                  if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
                     || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
                     || isEixMediaScreen() /*globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_MediaFile)
                     || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Desktop)
                     || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_SoftWare)*/
                     ) {
                     ui.widget_leftTopRender->show();
                     ui.verticalLayout_teacher_render->addWidget(mTeacherRender);
                     globalToolManager->GetDataManager()->WriteLog(" %s  3406 ui.verticalLayout_teacher_render->addWidget(mTeacherRender) ",
                        __FUNCTION__);
                     mTeacherRender->setFixedSize(QSize(320, 240));
                  }
                  else
                  {
                     ui.verticalLayout_CenterRender->addWidget(mTeacherRender);
                     
                     mTeacherRender->setFixedSize(QSize(ui.page_CenterRender->width(), ui.page_CenterRender->height()));
                  }
                  
               }
               mTeacherRender->SetFloatType(WndFloatType_Normal);
         }
      //}
      }
      this->activateWindow();
   }
   else  if(!strId.isEmpty()){

      //QMap<QString, MultiClassFloatWnd*>::iterator it = mapMultiClassFloatWnd.find(strId);
      if (mFloatWnd && mFloatWnd->isVisible()) {

         //MultiClassFloatWnd* pMultiClassFloatWnd =  it.value();

     /*    if (pMultiClassFloatWnd)
         {*/

            mFloatWnd->hide();
            mFloatWnd->RemoveRenderWidget(nullptr);
            TeacherRenderWdg*  pRnder = (TeacherRenderWdg*)mFloatWnd->RenderWidget();

            if (mRenderManager && pRnder /*&& pRnder->UserId().compare(strId) == 0*/) {
               pRnder->SetFloatTypeValue(WndFloatType_Normal);
               mRenderManager->addRender(pRnder);
            }
            else {
               globalToolManager->GetDataManager()->WriteLog("%s mRenderManager is  %d , \
                  TeacherRenderWdg�� pRnder��is %d, strId [%s] pRnder->UserId() [%s]", __FUNCTION__, nullptr ==mRenderManager
               , nullptr == pRnder,  strId.toStdString().c_str(), pRnder->GetUserJoinID().toStdString().c_str());
            }
        // }
      }
      this->activateWindow();
   }
}

void WebRtcLiveWdg::HandleOpenCaptureCallback(QEvent* event){
   if (event == NULL) {
      return;
   }
   
   QEventStream *msgEvent = (QEventStream*)event;
   globalToolManager->GetDataManager()->WriteLog("%s Enter mStreamType %d mIVHCapture %d", __FUNCTION__, msgEvent->mStreamType, msgEvent->mIVHCapture);

   if (msgEvent->mStreamType <= VHStreamType_AVCapture) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (msgEvent->mIVHCapture != VHCapture_OK) {
         QString msg = QStringLiteral("��ȡ�豸�쳣����������ʧ�ܣ���鿴�豸�Ƿ�ռ�û��߱�����");
         FadeOutTip(msg, Tost_Error);
         //�ɼ�ʧ�ܡ��ر�����ͷȻ��������ʧ����Ϣ��
         globalToolManager->GetClassSDK()->StopLocalCapture();
         //�ǽ�ʦ����豸�쳣��Ҫ��������ʧ�ܣ�ͬʱ������
         if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
            globalToolManager->GetClassSDK()->UserPublishFailedCallback(PushStreamEvent_Publish);
            ui.classRoomTitleWd->SetStartClassBtnEnable(true);
            if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
               globalToolManager->GetDataManager()->WriteLog("%s  UserPublishCallback(PushStreamEvent_UnPublish)   2669 PushStreamEvent_UnPublish" , __FUNCTION__);
               globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish);
            }
         }
      }
      else {
         //����ͷ�򿪳ɹ�֮��ʼ������
         int nRet = globalToolManager->GetClassSDK()->StartPushLocalStream();
         globalToolManager->GetDataManager()->WriteLog("%s StartPushLocalStream nRet %d", __FUNCTION__, nRet);

         if (nRet != VhallLive_OK) {
            //�ɼ�ʧ�ܡ��ر�����ͷȻ��������ʧ����Ϣ��
            globalToolManager->GetClassSDK()->StopLocalCapture();
            globalToolManager->GetClassSDK()->UserPublishFailedCallback(PushStreamEvent_Publish);
            ui.classRoomTitleWd->SetStartClassBtnEnable(true);
            FadeOutTip(Str_WebRtcRoomConnectErr, Tost_Error);
         }
         else if (VHStreamType_AVCapture >= msgEvent->mStreamType) {
            bool bBigView = true;//С���á������ν�ʦtrue��������ѧԱfalse
            if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               ChangePublishState(true);
               mTeacherRender->SetSmallRenderMicState(!baseInfo.mutingAll);
               mTeacherRender->SetMicState(msgEvent->mbHasAudio);
               mTeacherRender->SetCameraState(msgEvent->mbHasVideo);
               mTeacherRender->SetRoleType(JOIN_ROLE_Teacher);
               mTeacherRender->SetPageIndex(msgEvent->mbHasVideo == true ? TeacherRenderWdg_Page_Index_render : TeacherRenderWdg_Page_Index_NoCamera);

               if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL || 
                  (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && 
                     (baseInfo.bigScreen == 0 || 
                     (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id))
                  )) {
                  if (msgEvent->mbHasVideo) {
                     //HWND id = mTeacherRender->GetRenderWndID();
                     VHStreamType iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);
                     //globalToolManager->GetClassSDK()->StartRenderLocalStream(iStreamType, (void*)id);
                     globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(iStreamType, mTeacherRender->GetRenderWnd());
                  
                     //HWND id = mTeacherRender->GetRenderWndID();
                     //globalToolManager->GetClassSDK()->StartRenderLocalStream(iStreamType, (void*)id);
                  }
               } else {
                  if (msgEvent->mbHasVideo) {
                     //HWND id = mTeacherRender->GetSmallRenderRenderWndID();
                     mTeacherRender->SetShowSmallRender(true);
                     mTeacherRender->SetSmallRenderCameraState(msgEvent->mbHasVideo);
                     VHStreamType iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);
                     //globalToolManager->GetClassSDK()->StartRenderLocalStream(iStreamType, (void*)id);
                     globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(iStreamType, mTeacherRender->GetSmallRenderRenderWnd());
                  }
               }
            }
            else { //ѧԱ
               bBigView = (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL);

               VHStreamType iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msgEvent->mbHasAudio, msgEvent->mbHasVideo);

               if (nullptr != mRenderManager){
                std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = mRenderManager->GetRenderWnd(QString::fromStdString(baseInfo.mUserInfo.join_id));
                  //globalToolManager->GetDataManager()->WriteLog("%s Receiver %x %p", __FUNCTION__, Receiver, Receiver);
                  if (nullptr != Receiver) {
                     globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(iStreamType, Receiver);
                  }
                  else {
                     globalToolManager->GetDataManager()->WriteLog("%s  Receiver NULL", __FUNCTION__);
                  }
               }
			      else {
				      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(iStreamType, mTeacherRender->GetSmallRenderRenderWnd());
			      }
            }

            ////��ʼ�ɼ��У��л�����/ȫ��  �ı����Ŀ�괰��
            ////QString floatWndID = WND_ID_FLOAT_RENDER + QString::fromStdString(baseInfo.mUserInfo.join_id);	
            //CSTDMapParam mapParam;
            //mapParam[Str_Function] = Str_Function_PublishStream;
            //mapParam[STREAM_ID] = mLocalStream;
            //mapParam[USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
            //mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = msgEvent->mbHasVideo == false ? PARAM_TRUE : PARAM_FALSE;
            //mapParam[TEACHER_VIEW] = (bBigView == true) ? PARAM_TRUE : PARAM_FALSE;
            //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
            //WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
            //if (pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(QString::fromStdString(baseInfo.mUserInfo.join_id))) {

            //} else {
            //   globalToolManager->GetDataManager()->WriteLog("%s  : FULL_RENDER  not Show !!! ", __FUNCTION__);
            //}
         }
      }
   }
   else if (VHStreamType_Desktop == msgEvent->mStreamType) {
      if (msgEvent->mIVHCapture != VHCapture_OK) {
         QString msg = QStringLiteral("����ɼ�ʧ��");
         FadeOutTip(msg, Tost_Error);
      }
      else {
         HandleGetDesktopStream(msgEvent);
      }
   }
   else if (VHStreamType_MediaFile == msgEvent->mStreamType) {
      if (nullptr!= mLeftToolBarWdg) {
         mLeftToolBarWdg->StartMadiePlayTimer();
      }

      if (msgEvent->mIVHCapture != VHCapture_OK) {
         QString msg = QStringLiteral("�岥�ļ��ɼ�ʧ��");
         FadeOutTip(msg, Tost_Error);
      }
      else {
         HandleGetMediaFileStream(msgEvent);
      }
   }
   else if (VHStreamType_SoftWare == msgEvent->mStreamType) {
      if (msgEvent->mIVHCapture != VHCapture_OK) {
         QString msg = QStringLiteral("�������ɼ�ʧ��");
         FadeOutTip(msg, Tost_Error);
      }
      else {
         HandleGetSoftwareStream(msgEvent);
      }
   }
   else if (msgEvent->mStreamType == VHStreamType_Auxiliary_CamerCapture) {
	   if (msgEvent->mIVHCapture == VHCapture_STREAM_SOURCE_LOST) {
	   	QString msg = QStringLiteral("˫������ͷ�Ѱγ�");
	   	FadeOutTip(msg, Tost_Error);
	   }
      else if (mLeftToolBarWdg && mLeftToolBarWdg->isPreViewAuxiliaryVisible()) {
			mLeftToolBarWdg->PreviewAuxiliaryCamera(event);		     
      }
      else {
         mLeftToolBarWdg->EnablePreviewAuxiliary();
         if (msgEvent->mIVHCapture == VHCapture_OK) {
            HandlePlayCameraAuxiliary();
         }
         else {
            FadeOutTip(Str_OpenDoublePushCameraErr, Tost_Error);
         }
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandlePreOpenCaptureCallback(QEvent* event)
{
   if (event == NULL) {
      return;
   }

   VhallIALiveSettingDlg*  SetDlg = GetPtrVhallIALiveSettingDlg();
   if (nullptr != SetDlg && SetDlg->isVisible())
   {
		GetPtrVhallIALiveSettingDlg()->HandelCustomEvent(event);
   }
   else  if (mpDeviceTestWdg && mpDeviceTestWdg->isVisible()) {
      //QCoreApplication::postEvent(mpDeviceTestWdg, event);
      mpDeviceTestWdg->HandelCustomEvent(event);
      //QEventStream *customEvent = dynamic_cast<QEventStream*>(event);

      //QCoreApplication::postEvent(mpDeviceTestWdg, new QEventStream(CustomEvent_PreOnCaptureCallback, "", "", 
      //   customEvent->mStreamType,customEvent->mIVHCapture, customEvent->mbHasVideo, customEvent->mbHasAudio));

   }
   
}

void WebRtcLiveWdg::showMediaFilePlayWdg(const bool&  bShow)
{
   if (bShow)
   {
      int iTab = ui.stackedWidget_Center->currentIndex();
      if (iTab != page_MediaFilePlay)
         miOldStackedTab = iTab;

      if (nullptr != mDoubleRenderWdg)
         mDoubleRenderWdg->hide();
      setCurrentIndex(page_MediaFilePlay);
   }
   else
   {
      if (nullptr != mDoubleRenderWdg)
         mDoubleRenderWdg->show();

      setCurrentIndex((PAGE_INDEX)miOldStackedTab);
   }
}

std::wstring WebRtcLiveWdg::Ansi2WChar(LPCSTR pszSrc, int nLen)
{
   int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
   if (nSize <= 0) return NULL;

   WCHAR *pwszDst = new WCHAR[nSize + 1];
   if (NULL == pwszDst) return NULL;

   MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
   pwszDst[nSize] = 0;

   if (pwszDst[0] == 0xFEFF) // skip Oxfeff
      for (int i = 0; i < nSize; i++)
         pwszDst[i] = pwszDst[i + 1];

   std::wstring wcharString(pwszDst);
   delete pwszDst;

   return wcharString;
}

void WebRtcLiveWdg::HandleGetSoftwareStream(QEventStream* msgEvent)
{
   if (VHCapture::VHCapture_OK == msgEvent->mIVHCapture)
   {
      globalToolManager->GetDataManager()->WriteLog("%s\n", __FUNCTION__);
      //SetPlayDeskTopAndMediaFileState(true);

      mLeftToolBarWdg->SetBtnDocEnabled(false);
      mLeftToolBarWdg->SetBtnWhiteboardEnabled(false);
      //mLeftToolBarWdg->SetBtnDesktopSharingEnabled(false);
      //HWND id = (HWND)(ui.otherMediaWdg->winId());

      //setCurrentIndex(page_DesktopScreen);
      setCurrentIndex(page_SoftwareSharing);
      //ui.otherMediaWdg->SetWidthHeightRatio(16, 9);//��SelectSource ����ͬ��
      //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_SoftWare, (void*)id);
      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_SoftWare, ui.otherMediaWdg->GetVideoReciver());
      globalToolManager->GetDataManager()->WriteLog("%s mMediaStream play\n", __FUNCTION__);

      CreateScreenShareWgd();
      QDesktopWidget desktop;
      QWidget * deskWdg = desktop.screen(0);
      QRect rect = deskWdg->rect();

      if (m_pScreenShareToolWgd) {
         m_pScreenShareToolWgd->SetSCreenType(eDestopType_Windows);

         QString strTitel = QStringLiteral("���ڹ���") +
            mLeftToolBarWdg->GetCurShareSoftware()
            + QStringLiteral("��");

         m_pScreenShareToolWgd->SetScreenTitle(strTitel);//��ʾ��Ļ����λ��
         m_pScreenShareToolWgd->move(rect.x() + (rect.width() - m_pScreenShareToolWgd->width()) / 2, 0);
         m_pScreenShareToolWgd->show();

         if (!globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_AVCapture) && NULL != m_pScreenShareToolWgd)
         {
               m_pScreenShareToolWgd->on_btnStop_clicked();
         }
      }

      if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
         globalToolManager->GetClassSDK()->StartPushSoftWareStream();
      }
   }
   else
   {
      //MediaFilePlayFaild();
      FadeOutTip(QStringLiteral("�������ʧ�ܣ�����ϵͳ����������"), Tost_Error, false);
      //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_VIDEO_CHOICE_UI);
      //CSTDMapParam mapParam;
      //mapParam[DEALSTOPPLAY] = "";
      //pWdg->Notice(mapParam);
      mLeftToolBarWdg->DealTopplayChoiceUi();
   }
}

void WebRtcLiveWdg::HandleGetMediaFileStream(QEventStream* msgEvent)
{
   if (VHCapture::VHCapture_OK == msgEvent->mIVHCapture)
   {
      globalToolManager->GetDataManager()->WriteLog("%s\n", __FUNCTION__);
      //SetPlayDeskTopAndMediaFileState(true);
      if (globalToolManager->GetClassSDK()->IsSupportMediaFormat(CaptureStreamAVType_Video)) {
         showMediaFilePlayWdg(true);
         
         ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         QString user_id = QString::fromStdString(roomInfo.mUserInfo.join_id);
		   HWND id = (HWND)(ui.MediaFilePlayWdg->winId());
         globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_MediaFile, id);
         //globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_MediaFile, ui.MediaFilePlayWdg);
		 ui.MediaFilePlayWdg->setEnabled(false);
         globalToolManager->GetDataManager()->WriteLog("%s mMediaStream play\n", __FUNCTION__);
      }
      else {
         showMediaFilePlayWdg(false);
         mTeacherRender->show();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
      }

      //slot_ClickedMic(globalToolManager->GetDataManager()->GetThridPartyUserId(), true);
      if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
         globalToolManager->GetClassSDK()->StartPushMediaFileStream();
      }
   }
   else
   {
      //MediaFilePlayFaild();
      FadeOutTip(QStringLiteral("�岥��Ƶʧ�ܣ�����岥�ļ��Ƿ���ȷ"), Tost_Error, false);
      mLeftToolBarWdg->DealTopplayChoiceUi();
   }
}

void WebRtcLiveWdg::CreateScreenShareWgd()
{
   if (NULL == m_pScreenShareToolWgd)
   {
      m_pScreenShareToolWgd = new CScreenShareToolWgd(/*this*/);
      if (m_pScreenShareToolWgd) {
         m_pScreenShareToolWgd->hide();

         //����
         connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::sigStopScreenSharing, this, &WebRtcLiveWdg::slot_BtnStopScreenClicked);
         connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::sigShowChat, this, &WebRtcLiveWdg::slot_DragChat);
         connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::sigShowMember, this, &WebRtcLiveWdg::slot_DragMember);
         if (nullptr != mLeftToolBarWdg)
            connect(m_pScreenShareToolWgd, &CScreenShareToolWgd::sigChangeScreen, mLeftToolBarWdg, &LeftToolBarWdg::slots_ScreenSharing);
      }
   }

}

void WebRtcLiveWdg::HandleGetDesktopStream(QEventStream* msgEvent)
{
   if (VHCapture::VHCapture_OK == msgEvent->mIVHCapture)
   {
      CreateScreenShareWgd();
      if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_SoftWare))
         globalToolManager->GetClassSDK()->StopSoftwareCapture();

      mLeftToolBarWdg->SetBtnDocEnabled(false);
      mLeftToolBarWdg->SetBtnWhiteboardEnabled(false);
      setCurrentIndex(page_DesktopScreen);
      if (mMemberListWdg && mMemberListWdg->isVisible())
      {
         mMemberListWdg->hide();
         if (mpEjectTabItem&& mpEjectTabItem->isVisible())
         {
            mpEjectTabItem->hide();
            mSunWndList << mpEjectTabItem;
         }
         mSunWndList << mMemberListWdg;
      }
      if (mNoticeShowWdg && mNoticeShowWdg->isVisible())
      {
         mNoticeShowWdg->hide();
         mSunWndList << mNoticeShowWdg;
      }
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type) {//�����ѧԱ ��������ʱ
         this->hide();
      }
      else if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type){
         setCurrentIndex(page_InvStateWdg);
         ui.invStateWdg->SetStateType(InvitationStateWdg::eStateType_DesktopSharing);
      }

      int iDesktopIndex = mLeftToolBarWdg->GetDesktopSharingIndex();
      QRect rect = QApplication::desktop()->screenGeometry(iDesktopIndex);

      if (m_pScreenShareToolWgd) {
         m_pScreenShareToolWgd->SetSCreenType(eDestopType_Desktop);
         QString strTitel = QStringLiteral("���ڹ���") +
            QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.nick_name)
            + QStringLiteral("������Ļ") + QString::number(iDesktopIndex + 1);
         m_pScreenShareToolWgd->SetScreenTitle(strTitel);//��ʾ��Ļ����λ��
         m_pScreenShareToolWgd->move(rect.x() + (rect.width() - m_pScreenShareToolWgd->width()) / 2, 0);
         m_pScreenShareToolWgd->show();

         //bool isPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_AVCapture);
         //if (/*!isPushing &&*/ NULL != m_pScreenShareToolWgd)
         //{
         //   globalToolManager->GetDataManager()->WriteLog("%s  isPushing %d m_pScreenShareToolWgd->on_btnStop_clicked()", __FUNCTION__, isPushing);
         //   m_pScreenShareToolWgd->on_btnStop_clicked();
         //}
         //else {
         //}
      }
      if (0) {
         globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture,nullptr);
      }
   }
   else
   {
      //m_bIsDesktopSharCapturing = false;
   }
}

void WebRtcLiveWdg::slot_StudentDoublePush()
{
   globalToolManager->GetClassSDK()->doubleVrtcUsers();
}

void WebRtcLiveWdg::slot_CameraDoublePush(bool open) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   if (open) {
      std::list<vhall::VideoDevProperty> cameraList;
      globalToolManager->GetClassSDK()->GetCameraDevices(cameraList);
      //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	  QString msConfPath = globalToolManager->GetConfigPath();
      int openPic = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);
		
      if (0==openPic && cameraList.size() <= 1) {
			if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
				FadeOutTip(Str_NotAllowDoublePush, Tost_Warning);
			}
         initLeftToolBarOptype();
         //slot_showInitPage();
         return;
      }
      else if (1 == openPic && cameraList.size() <= 0) {
         FadeOutTip(Str_HaveNoCamereAllowDoublePush, Tost_Warning);
         //slot_showInitPage();
         initLeftToolBarOptype();
         return;
      }

      //if (!CheckAndStopScreenShare(DoublePubStuChoiceUI::eChoiceType_Camera))
      //{
      //   initLeftToolBarOptype();
      //   return;
      //}

		if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Student
			&& (0 == openPic && cameraList.size() == 2) || (1 == openPic && cameraList.size() == 1)) {
			//// ���ʱѧԱ˫�� ����˫������ ���ѡ������ͷ������
			if (nullptr != mLeftToolBarWdg)
			{
				mLeftToolBarWdg->ShowPreViewAuxiliaryWdg();
			}
		}
      else if ((0 == openPic && cameraList.size() == 2)||(1 == openPic && cameraList.size()==1)) {
         //������������ͷ����ֱ�Ӵ�δ��ʹ���е�����ͷ����˫��
         std::string currentUsedCameraId;
         globalToolManager->GetClassSDK()->GetCurrentCamera(currentUsedCameraId);
         std::list<vhall::VideoDevProperty>::iterator iter = cameraList.begin();
         while (iter != cameraList.end()) {
            if (iter->mDevId != currentUsedCameraId) {
               //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
				QString msConfPath = globalToolManager->GetConfigPath();
               //QString configQuality = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
               //ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_LABEL_NUMBER, 1);
               /*int interactiveVideoProfile = 360;
               int mixLayoutProfile = 720;
               globalToolManager->GetVideoProfileByConfig(configQuality, interactiveVideoProfile, mixLayoutProfile);*/
               //VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(interactiveVideoProfile);
               int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
               globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iQualityId);
               int iVioeProFile = 360;
               globalToolManager->GetVideoProfileByConfig(iQualityId, iVioeProFile, ToolManager::eVideoProFileTypeDoublePush);
               VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(iVioeProFile);

               globalToolManager->GetClassSDK()->StartLocalAuxiliaryCapture(iter->mDevId, videoIndex);
               //HandlePlayCameraAuxiliary();//����
               break;
            }
            iter++;
         }
      }
      else {
         //// ���ж�����������ͷ�����ѡ������ͷ������
         if (nullptr != mLeftToolBarWdg)
         {
            mLeftToolBarWdg->ShowPreViewAuxiliaryWdg();
         }
      }
   }
   else {
      slot_showInitPage();
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      ui.CtrlDoubPubWdg->slot_StopPush();
      globalToolManager->GetClassSDK()->StopLocalAuxiliaryCapture();
      //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         mMixStreamLayout.SetMixStreamLayout();
      }

      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {

         if (mTeacherRender) {
            ui.verticalLayout_teacher_render->removeWidget(mTeacherRender);

            globalToolManager->GetDataManager()->WriteLog(" %s 3959 ui.verticalLayout_teacher_render->removeWidget(mTeacherRender) ",
               __FUNCTION__);
            ui.widget_leftTopRender->hide();
            ui.verticalLayout_CenterRender->addWidget(mTeacherRender);
            //setCurrentIndex(page_CenterRender);
            mTeacherRender->ChangeRenderPos(true);
         }
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::slot_DesktopSharing()
{
   //  if (mLeftToolBarWdg->isMediaPlayExist() /*|| globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/) {
       //mLeftToolBarWdg->slots_MenuAboutToHide();
       //FadeOutTip(Str_PleaseStopMedia);
       //return;
   //  }
    //if (globalToolManager->GetWndManager()->IsWndExist(WND_ID_QUESTION_UI)) {
    //	mLeftToolBarWdg->slots_MenuAboutToHide();
    //	FadeOutTip(Str_PleaseStopQUESTION);
    //	return;
    //}
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type != CLASS_STATUS_IN_CLASS
      && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mLeftToolBarWdg->slots_MenuAboutToHide();
      FadeOutTip(Str_PleaseStartClass, Tost_Warning);
      return;
   }
   QPoint pos(0, ui.classRoomTitleWd->height());
   mLeftToolBarWdg->PopDesktopSharingMenu(pos);
}

void WebRtcLiveWdg::slot_interactionTool()
{
   QPoint pos(0, ui.classRoomTitleWd->height());
   mLeftToolBarWdg->PopInteractionToolMenu(pos);
}

void WebRtcLiveWdg::showTimeCountDown()
{
   int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;

   {if (JOIN_ROLE_Teacher == roleType)//��ʦ
      createTimerWdg();
   if (nullptr != mpTimerWdg)
   {
      int moveX = (this->width() - 420) / 2;
      int moveY = (this->height() - 350) / 2;
      mpTimerWdg->move(moveX + this->x(), moveY + this->y());
      mpTimerWdg->showWdg(roleType);

   }
   }
}


void WebRtcLiveWdg::slot_Media()
{
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type != CLASS_STATUS_IN_CLASS
      && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mLeftToolBarWdg->slots_MediaMenuAboutToHide();
      FadeOutTip(Str_PleaseStartClass, Tost_Warning);
      return;
   }
   //if (globalToolManager->GetWndManager()->IsWndExist(WND_ID_QUESTION_UI)) {
   //	mLeftToolBarWdg->slots_MediaMenuAboutToHide();
   //	FadeOutTip(Str_PleaseStopQUESTION);
   //	return;
   //}
   if (m_pScreenShareToolWgd && !m_pScreenShareToolWgd->isHidden()) {
      mLeftToolBarWdg->slots_MediaMenuAboutToHide();
      FadeOutTip(Str_PleaseStopDesktop, Tost_Warning);
      return;
   }

   QPoint pos(0, ui.classRoomTitleWd->height());
   mLeftToolBarWdg->PopMediaMenu(pos);
   miOldStackedTab = ui.stackedWidget_Center->currentIndex();
   globalToolManager->GetDataManager()->WriteLog("%s miOldStackedTab:%d", __FUNCTION__, miOldStackedTab);
   /*CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
   if (wnd) {
       CSTDMapParam param;
       wnd->Notice(param);
       wnd->show();
   }*/
}

void WebRtcLiveWdg::slot_BtnStopScreenClicked()
{
   if (m_pScreenShareToolWgd) {
      m_pScreenShareToolWgd->StopRoll();
      m_pScreenShareToolWgd->hide();
   }

   mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
   mLeftToolBarWdg->SetBtnDocEnabled(true);
   mLeftToolBarWdg->SetBtnWhiteboardEnabled(true);
   slot_showInitPage();
   show();

   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mMixStreamLayout.SetMixStreamLayout(true);
   }

   if (mSunWndList.size() > 0)
   {
      for (int i = 0; i < mSunWndList.size(); i++)
      {
         mSunWndList.at(i)->show();
      }
      mSunWndList.clear();
   }

   if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_Desktop))//���湲����
   {
      globalToolManager->GetClassSDK()->StopDesktopCapture();
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         mMixStreamLayout.SetMixStreamLayout();
      }
      globalToolManager->GetClassSDK()->DesktopShareNotice(false, globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);

		//ӆ����ЌW�T���l
		if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
			GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
			//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
			QString msConfPath = globalToolManager->GetConfigPath();
			ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
			GetPtrVhallIALiveSettingDlg()->RecoverState();
			globalToolManager->GetDataManager()->WriteLog("%s SetAllSubScribeAudio(true)", __FUNCTION__);
		}
		else {
			GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
			globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
			//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
			QString msConfPath = globalToolManager->GetConfigPath();
			ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
			ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
		}

   }
   else if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_SoftWare))//�������
   {
      globalToolManager->GetClassSDK()->StopSoftwareCapture();
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         mMixStreamLayout.SetMixStreamLayout();
      }
      globalToolManager->GetClassSDK()->DesktopShareNotice(false, globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);


      mLeftToolBarWdg->SetBtnDocEnabled(true);
      mLeftToolBarWdg->SetBtnWhiteboardEnabled(true);

      slot_showInitPage();
   }
}

void WebRtcLiveWdg::HandleSetLayoutCameraAuxiliary(QEvent* event) {
   CustomRecvMsgEvent* roomEvent = dynamic_cast<CustomRecvMsgEvent*>(event);
   if (roomEvent) {
      mMixStreamLayout.SetMainViewStream(roomEvent->msg);
   }
}

void WebRtcLiveWdg::slot_DoublePush() {
   QPoint pos(0, ui.classRoomTitleWd->height());
   mLeftToolBarWdg->PopDoublePushMenu(pos);
}

void WebRtcLiveWdg::HandleCloseFloatRender(QEvent* event) {
   QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
   QString userId = roomEvent->mUserId;

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (mTeacherRender && mTeacherRender->GetUserJoinID() == userId) {
      mTeacherRender->show();
      globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
      if (baseInfo.mWebinarInfo.layout != CLASS_LAYOUT_ONLY_VIDEO 
         || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
         || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
         || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
         || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Stu_Desktop)) {
         ui.widget_leftTopRender->show();
      }
      mTeacherRender->RenderBigStreamToWnd(userId);
      //���ιر�ȫ��ʱ��Ҫ������ȾС���塣
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         mTeacherRender->RenderSmallStream();
      }
   }
   else if (mTeacherRender->GetUserJoinID() == userId)
   {
      ui.widget_leftTopRender->show();
   }
   else
   {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
         if (mRenderManager) {
            mRenderManager->CloseFloatRender(userId);
         }
      }
   }

   //QMap<QString, FloatRender*>::iterator iter = m_pMapFloatRender.find(userId);
   //if (iter != m_pMapFloatRender.end())
   //{
   //   FloatRender* pRender = iter.value();
   //   if (NULL != pRender)
   //   {
   //      pRender->hideRender();
   //      delete  pRender;
   //      pRender = NULL;
   //      m_pMapFloatRender.remove(userId);
   //   }
   //}
}

void WebRtcLiveWdg::HandleWebRtcRoomMixedReady(QEvent* event) {
   QEventVHMember* roomEvent = dynamic_cast<QEventVHMember*>(event);

   if (roomEvent) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && !mMixedReady 
         && roomEvent->thirdPartyUserId.compare( QString::fromStdString( baseInfo.mUserInfo.join_id) ) == 0) {
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            mMixStreamLayout.SetMixStreamLayout();
         }
         mMixedReady = true;
      }
   }

}

int WebRtcLiveWdg::iFadeOutTipWidth()
{
   int iRef = ui.widget_centerWidget->width();
   //if (nullptr != mDocWebEngineViewWdg && mDocWebEngineViewWdg->isActiveWindow())
   //{
   //	iRef = mDocWebEngineViewWdg->width();
   //}
   //else if (true) //������Ƶ����
   //{

   //}
   //else if(nullptr != mRenderManager && mRenderManager->isActiveWindow())//��������
   //{
   //	iRef = mRenderManager->width();
   //}
   return iRef;
}

void WebRtcLiveWdg::moveEvent(QMoveEvent *event)
{
   //qDebug() << "Title Height:" << ui.classRoomTitleWd->height() << endl;
   if (nullptr != mpMoveFadeOutTipWdg && !mpMoveFadeOutTipWdg->isHidden()) {
      QPoint pos(ui.widget_centerWidget->x(), ui.classRoomTitleWd->height());
      QPoint posTmpe = mapToGlobal(pos);
      mpMoveFadeOutTipWdg->move(posTmpe);
   }

   for (int i = 0; i < 3; i++) {
      VHFadeOutTipWidget* tempWdg = mpFadeOutTipWidget[i];
      if (nullptr != tempWdg && tempWdg->isVisible()) {
         tempWdg->move(this->x() + ui.widget_centerWidget->x() + (ui.widget_centerWidget->width() - tempWdg->width()) / 2, this->y() + ui.classRoomTitleWd->height() + i*51);
   }
   }


   CBaseWnd::moveEvent(event);
}

void WebRtcLiveWdg::changeEvent(QEvent* event)
{
   if (this->windowState() == Qt::WindowMinimized)
   {
   }
   CBaseWnd::changeEvent(event);
}

void WebRtcLiveWdg::resizeEvent(QResizeEvent *event)
{
	AddChatWebView();
   CBaseWnd::resizeEvent(event);
}

void  WebRtcLiveWdg::FadeOutTip(QString strMsg, int bTostIndex, bool bLongTimeShow)
{
   QString str = strMsg.replace("http request err", Str_ServerTimeOut).trimmed();
   if (str.isEmpty()) return;
   if (bLongTimeShow)//��ʱ����ʾ�ҹ���
   {
      if (nullptr == mpMoveFadeOutTipWdg)
      {
         mpMoveFadeOutTipWdg = new MoveFadeoutTipWidget(this);
      }
      if (mpMoveFadeOutTipWdg)
      {
         int width = iFadeOutTipWidth();
         mpMoveFadeOutTipWdg->setFixedWidth(width);
         mpMoveFadeOutTipWdg->SetTilte(str);
			repaint();
			mpMoveFadeOutTipWdg->show();
         QPoint pos(ui.widget_centerWidget->x(), ui.classRoomTitleWd->height());
         QPoint posTmpe = mapToGlobal(pos);

         mpMoveFadeOutTipWdg->move(posTmpe);
         mpMoveFadeOutTipWdg->raise();
      }
   }
   else if (this->isVisible()/* && this->isActiveWindow()*/)// ��ʾ�������
   {
      int iFindeObj = 0;
      for (int i = 0; i < 3; i++) {
         if (nullptr == mpFadeOutTipWidget[i]){
            mpFadeOutTipWidget[i] = new VHFadeOutTipWidget(this);
            iFindeObj = i;
            break;
         }
         else if (!mpFadeOutTipWidget[i]->isVisible()) {
            iFindeObj = i;
            break;
         }
      }



      if (mpFadeOutTipWidget[iFindeObj]) {
         //int width = iFadeOutTipWidth();
         //mpFadeOutTipWidget->setFixedWidth(width);
         mpFadeOutTipWidget[iFindeObj]->show();
         mpFadeOutTipWidget[iFindeObj]->setDispalyMsg(str);
         
         mpFadeOutTipWidget[iFindeObj]->SetStyleSheet(bTostIndex);
         mpFadeOutTipWidget[iFindeObj]->Tip(this, QPoint(this->x() + ui.widget_centerWidget->x(), this->y() + ui.classRoomTitleWd->height() + iFindeObj * 51 ) , str);
         mpFadeOutTipWidget[iFindeObj]->rePos(ui.widget_centerWidget);
         mpFadeOutTipWidget[iFindeObj]->show();
      }
   }
}

void WebRtcLiveWdg::HandleShowNoticeMsg(QEvent* event) {
   CustomEventType* roomEvent = dynamic_cast<CustomEventType*>(event);
   QString noticeMsg = roomEvent->msg;

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (nullptr==mNoticeShowWdg) {
		mNoticeShowWdg = new NoticeShowWdg(JOIN_ROLE_Student == baseInfo.mUserInfo.role_type, ui.tabWidget);
	}

   mNoticeSendWdg->showNoticShow(noticeMsg);
   mNoticeShowWdg->ShowNoticeMsg(noticeMsg);

   if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type) {
	   mNoticeSendWdg->AppendMsg(noticeMsg);
   }


   if (mbEjectNoticeSend)
   {
      mNoticeShowWdg->hide();
      mNoticeShowMsgTimer.stop();
      mNoticeSendWdg->showNotic(true);
   }
   else if (mNoticeShowWdg) {
      mNoticeShowWdg->setFixedWidth(ui.tabWidget->width());
      mNoticeShowWdg->show();
      //mNoticeShowWdg->move(0, ui.tabWidget->GetTabHeight());
      mNoticeSendWdg->showNotic(false);
      slot_NoticeShowMsgTimeout();
      mNoticeShowMsgTimer.start(10);
   }
}

void WebRtcLiveWdg::HandleShowFadOutTipsMsg(QEvent* event) {
   CustomEventType* roomEvent = dynamic_cast<CustomEventType*>(event);
   if (roomEvent) {
      QString noticeMsg = roomEvent->msg;
      FadeOutTip(noticeMsg, Tost_Message);
   }
}

void WebRtcLiveWdg::HandleStartMixStream(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
   if (roomEvent->mRespCode == 200) {
      mMixStreamLayout.SetAlreadyMixed(true);

      //globalToolManager->GetClassSDK()->SetConfigBroadCastLayOut(CANVAS_LAYOUT_PATTERN_GRID_3_D, [& /*callbackTool,*/](const std::string& result, const std::string& msg)->void {
      //   if (result == "success") {
      //      //callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomRecvMsgEvent(CustomEvent_SetLayoutCameraAuxiliary, stream_id));
      //   }
      //});
      //С�������湲��ʱ�ϴ����������˻����ϴ���
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (roomEvent->mProcessType == CANVAS_LAYOUT_PATTERN_GRID_1 
         || roomEvent->mProcessType == CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL) {
         //globalToolManager->GetClassSDK()->IsPushingStream()

         mMixStreamLayout.SetMainViewStream(mDeskTopStream);
      }
      //���湲��ʱҪЯ�������˻��棬�Ҵ�����ʾ���湲�����½���ʾ�����˻��档
      //���ֲ���ĿǰSDK��֧�֣���Ҫ�Ƚ�����������Ϊ������Ȼ���ٰ����湲������Ϊ������
      else if (roomEvent->mProcessType == CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR) {
         mMixStreamLayout.SetMainViewStream(mLocalStream);
      }
      else {
         mMixStreamLayout.SetMainViewStream(mLocalStream);
      }
   }
   else {

      mRestartSetLayoutType = roomEvent->mProcessType  >= 0 ? roomEvent->mProcessType : globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.mixedLayout;
      mbRestartSetLayout = roomEvent->mUserData == "has_desktop_layout" ? true : false;
      QTimer::singleShot(2000, this, &WebRtcLiveWdg::slot_RestartSetLayout);

   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::slot_RestartSetLayout()
{
   globalToolManager->GetDataManager()->WriteLog("%s type %d desktop %d", __FUNCTION__, mRestartSetLayoutType, mbRestartSetLayout);
   mMixStreamLayout.RestartSetLayout(mRestartSetLayoutType, mbRestartSetLayout);
}

void WebRtcLiveWdg::HandleStartSetMainStream(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);

   globalToolManager->GetDataManager()->WriteLog("%s  roomEvent->mRespCode %d", __FUNCTION__, roomEvent->mRespCode);
   if (roomEvent->mRespCode == 200) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      QString streamId = roomEvent->mUserData;
      globalToolManager->GetDataManager()->WriteLog("%s streamId:%s mLocalStream:%s", __FUNCTION__, streamId.toStdString().c_str(), mLocalStream.toStdString().c_str());
      //�����ô���ɹ���������������������ͷ���档�ж��Ƿ����ڴ������湲��Ĳ��֡�
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && streamId == mLocalStream && mMixStreamLayout.GetCurrentLayOut() == CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR) {
         mMixStreamLayout.SetMainViewStream(mDeskTopStream);
         globalToolManager->GetDataManager()->WriteLog("%s CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR", __FUNCTION__);
      }
   }
   else {
      mStrStreamId = roomEvent->mUserData;
      if (nullptr == mpReSetMainStreamTimer) {
         //QTimer::singleShot(2000, this, 
         mpReSetMainStreamTimer = new QTimer;
         connect(mpReSetMainStreamTimer, &QTimer::timeout, this, &WebRtcLiveWdg::slot_ReSetMainStream);
      }
      
      if (mpReSetMainStreamTimer ) {
         if (mpReSetMainStreamTimer->isActive()) {
            mpReSetMainStreamTimer->stop();
         }
         
         mpReSetMainStreamTimer->start(2000);
         globalToolManager->GetDataManager()->WriteLog("%s QTimer::singleShot(2000) ", __FUNCTION__); 
         
      }
      else {
         globalToolManager->GetDataManager()->WriteLog("%s mpReSetMainStreamTimer is NULL ", __FUNCTION__);
      }
      //mMixStreamLayout.SetMainViewStream(roomEvent->mUserData);
      //globalToolManager->GetDataManager()->WriteLog("%s roomEvent->mUserData: %s", __FUNCTION__, roomEvent->mUserData);
   }
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
}

void WebRtcLiveWdg::slot_ReSetMainStream() {

   if (mpReSetMainStreamTimer) {
      if (mpReSetMainStreamTimer->isActive()) {
         mpReSetMainStreamTimer->stop();
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s ", __FUNCTION__);
   mMixStreamLayout.SetMainViewStream(mStrStreamId);
}

void WebRtcLiveWdg::HandleStreamAdd(QEvent* event) {
   QEventRecvMsg* roomEvent = dynamic_cast<QEventRecvMsg*>(event);
   QString stream_id = roomEvent->mUserData;
   globalToolManager->GetDataManager()->WriteLog("%s stream_id %s", __FUNCTION__, stream_id.toStdString().c_str());
   //if (mListRemoteStream.size()<=0) {
      QString user_id = roomEvent->mUserId;
      int streamType = roomEvent->mStatus;
      globalToolManager->GetClassSDK()->SubScribeRemoteStream(stream_id.toStdString());

      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
         || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) //ѧԱ��˫��
         || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop);

      if (baseInfo.mWebinarInfo.mixedLayout == eMixedLayoutMode_Grid && !bIsPushing)
      {
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            mMixStreamLayout.SetMixStreamLayout();
         }
         //ToolManager *callbackTool = globalToolManager;
         /*globalToolManager->GetClassSDK()->SetConfigBroadCastLayOut((LayoutMode)layout, [&, callbackTool, stream_id](const std::string& result, const std::string& msg)->void {
            if (result == "success") {
               callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomRecvMsgEvent(CustomEvent_SetLayoutCameraAuxiliary, stream_id));
            }
         });*/
      }
   //}
   //else
   //{
   //   addRemoteStream(stream_id.toStdString());
   //}

}

//void WebRtcLiveWdg::addRemoteStream(std::string strStreamId)
//{
//   /*std::list<std::string>::iterator itor = mListRemoteStream.begin();
//   bool bFind = false;
//   while (itor != mListRemoteStream.end())
//   {
//      if (strStreamId.compare( *itor ) == 0)
//      {
//         bFind = true;
//         break;
//      }
//      itor++;
//   }
//
//   if (bFind); 
//   else {
//      mListRemoteStream.push_back(strStreamId);
//
//      std::list<std::string>::iterator itorRemo = mPreRemoveListRemoteStream.begin();
//      std::string strId;
//      while (itorRemo != mPreRemoveListRemoteStream.end()) {
//         strId = *itorRemo++;
//         mPreRemoveListRemoteStream.remove(strId);
//         mListRemoteStream.remove(strId);
//      }
//   }*/
//}

void WebRtcLiveWdg::HandleRecvFlashMsg(QEvent* event) {
   QEventRecvMsg* roomEvent = dynamic_cast<QEventRecvMsg*>(event);

   QByteArray dataArray(roomEvent->mUserId.toStdString().c_str(), roomEvent->mUserId.toStdString().length());
   QString base64EncodeData = dataArray.toBase64();
   QString method = QString("msgByClient('%1')").arg(base64EncodeData);

   if (mDocCefViewWdg) {
      //mDocWebEngineViewWdg->runJavaScript(method);
      if(eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
         mDocCefViewWdg->AppCallJsFunction(method);
   }
   if (mWhiteBoardCefViewWdg) {
      //mWhiteBoardWebEngineViewWdg->runJavaScript(method);
      if (eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
         mWhiteBoardCefViewWdg->AppCallJsFunction(method);
   }
}

void WebRtcLiveWdg::HandleRecvSetBrush(QEvent* event) {
   QEventRecvMsg* roomEvent = dynamic_cast<QEventRecvMsg*>(event);
   QByteArray dataArray(roomEvent->mUserId.toStdString().c_str(), roomEvent->mUserId.toStdString().length());

   // {"join_id":"69640", "status" : 1, "target_id" : "76717", "type" : "*setBrush"}
   // {"join_id":"69640", "status" : 0, "target_id" : "76717", "type" : "*setBrush"}
   QByteArray msg(roomEvent->mUserData.toStdString().c_str(), roomEvent->mUserData.toStdString().length());

   QJsonDocument doc = QJsonDocument::fromJson(msg);
   if (doc.isObject()) {
      QJsonObject obj = doc.object();
      QString join_id = obj["join_id"].toString();
      QString target_id = obj["target_id"].toString();
      int status = obj["status"].toInt();
      if (mRenderManager) {
         mRenderManager->SetUserPenAuth(target_id, status);
      }

      if (status != 1) {
         mstrIDPenAuth = "";
      }
   }
   QString base64EncodeData = dataArray.toBase64();
   QString method = QString("msgByClient('%1')").arg(base64EncodeData);


   if (mDocCefViewWdg) {
      //mDocWebEngineViewWdg->runJavaScript(method);
      if (eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
         mDocCefViewWdg->AppCallJsFunction(method);
   }
   if (mWhiteBoardCefViewWdg) {
      //mWhiteBoardWebEngineViewWdg->runJavaScript(method);
      if (eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
         mWhiteBoardCefViewWdg->AppCallJsFunction(method);
   }

}

void WebRtcLiveWdg::HandleRecvMediaMsg(QEvent* event) {
   QEventRecvMsg* roomEvent = dynamic_cast<QEventRecvMsg*>(event);

   QByteArray dataArray(roomEvent->mUserId.toStdString().c_str(), roomEvent->mUserId.toStdString().length());
   QString base64EncodeData = dataArray.toBase64();
   QString method = QString("msgByClient('%1')").arg(base64EncodeData);

   //["cmd",{"join_id":"69640","nick_name":"754140626792","progress_rate":"0","record_id":"e516845c","type":"*syncInterstitialProgressRate"}]
   QJsonDocument doc = QJsonDocument::fromJson(dataArray);
   if (doc.isArray()) {
      QJsonArray obj = doc.array();
      if (obj.count() == 2) {
         QJsonObject cmd = obj[1].toObject();
         QString type = cmd["type"].toString();
         //1.�λ�ͬ���岥����(��ʼ�岥/���/���� �Ȳ����������¹㲥)
         //2.�λ���ͣ�岥����(��ʼ�岥/���/���� �Ȳ��������ô˽ӿ�)
         if (type == "*syncInterstitialProgressRate" || type == "*pauseInterstitial") {

            if (nullptr != mLeftToolBarWdg)
            {
               MediaPlayWebView* wnd = mLeftToolBarWdg->MediaPlayWebViewDlg();

               //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_MEDIA_PLAY_WEBVIEW);
               if (wnd) {
                  CSTDMapParam param;
                  param[MEDIA_FILE_CTRL_MSG] = method;
                  wnd->Notice(param);
                  wnd->show();
               }
            }
         }
         //��ʦ�ر���ҳ����������뿪ֱ���䣩 �������Ͳ岥��ͣ��Ϣ
         else if (type == "*stopInterstitial") {
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
            //mLeftToolBarWdg->DealTopplayChoiceUi();
         }
      }
   }
}

void WebRtcLiveWdg::HandleRecvAnswerMsg(QEvent* event) {
   QEventRecvMsg* roomEvent = dynamic_cast<QEventRecvMsg*>(event);

   QByteArray dataArray(roomEvent->mUserId.toStdString().c_str(), roomEvent->mUserId.toStdString().length());
   QString base64EncodeData = dataArray.toBase64();
   QString method = QString("msgByClient('%1')").arg(base64EncodeData);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

   QJsonDocument doc = QJsonDocument::fromJson(dataArray);
   if (doc.isArray()) {
      QJsonArray obj = doc.array();
      if (obj.count() == 2) {
         QJsonObject cmd = obj[1].toObject();
         QString type = cmd["type"].toString();
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && (type == "*stop_answer" || type == "*clear_question")) {
            if (mLeftToolBarWdg)
            {
               mLeftToolBarWdg->DestoryAnswerWebViewDlg();
            }
         }
         else {
            if (type == "online" || type == "offline") {
               //if (!globalToolManager->GetWndManager()->IsWndExist(WND_ID_QUESTION_UI)) {
               //    return;
               //}
               if (mLeftToolBarWdg && !mLeftToolBarWdg->IsExitAnserWebView()) {
                  return;
               }
            }

            if (mLeftToolBarWdg)
            {
               AnswerWebView* wnd = mLeftToolBarWdg->AnswerWebViewDlg();
               //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
               if (wnd) {
                  CSTDMapParam param;
                  param[MEDIA_FILE_CTRL_MSG] = method;
                  globalToolManager->GetDataManager()->WriteLog("%s msg %s", __FUNCTION__, method.toLatin1().data());
                  wnd->Notice(param);
                  if (type == "*push_question" || type == "*publish_answer" || type == "*new_push_question" || type == "*new_publish_answer") {
                     wnd->show();
                  }
               }
            }
         }
      }
   }
}

void WebRtcLiveWdg::HandleWebRtcRoomConnetSuc(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   ui.classRoomTitleWd->ShowBeginClass();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   globalToolManager->GetDataManager()->WriteLog("%s mbFirstConnectWebRtcServer:%d  webinar_status:%d role_type:%d", __FUNCTION__,
      mbFirstConnectWebRtcServer, globalToolManager->GetClassSDK()->GetCourseInfo().webinar_status, baseInfo.mUserInfo.role_type);
   if (mbFirstConnectWebRtcServer && globalToolManager->GetClassSDK()->GetCourseInfo().webinar_status == CLASS_STATUS_IN_CLASS && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      if (globalToolManager->GetClassSDK()->HasVideoOrAudioDev()) {
         //������Ƶ�豸���������
         globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_HandleStartLocalCapture, QString::number(1)));
      }
   }
   else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL 
      && baseInfo.mWebinarInfo.speedmode && mbFirstConnectWebRtcServer && !globalToolManager->GetDataManager()->IsAlreadyPublished())
   {
      bool bEnableDoublePub = VhallIALiveSettingDlg::isEnableDoublePublish();
      globalToolManager->GetClassSDK()->UpdatePublishAbility(globalToolManager->GetClassSDK()->HasVideoOrAudioDev(), bEnableDoublePub);
   }
   else { //û����
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
         //������Ƶ�豸��ɼ�
         HandleStartLocalCapture();
      }

      bool bEnableDoublePub = VhallIALiveSettingDlg::isEnableDoublePublish();
      globalToolManager->GetClassSDK()->UpdatePublishAbility(globalToolManager->GetClassSDK()->HasVideoOrAudioDev(), bEnableDoublePub);

		//��Ƶģʽ��ѧԱ�������ʾȫ�ְ�ť
		if(baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO && mTeacherRender != NULL) {
			if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
				mTeacherRender->ShowBigRenderCtrl(true);
			}
			else {
				mTeacherRender->ShowBigRenderCtrl(false);
			}
		}
   }

   //���ӷ���ɹ�Ҳ����������Ͽ�֮����������
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL
	   && baseInfo.mWebinarInfo.speedmode && mbFirstConnectWebRtcServer && !globalToolManager->GetDataManager()->IsAlreadyPublished()) {
      globalToolManager->GetDataManager()->WriteLog("%s repush After Net  off", __FUNCTION__);
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s RePushLocalStream", __FUNCTION__);
	   mMixStreamLayout.RePushLocalStream();
   }

   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      globalToolManager->GetDataManager()->WriteLog("%s RePushDesktopSream&", __FUNCTION__);
      mMixStreamLayout.RePushDesktopSream(VHStreamType_Desktop);
      mMixStreamLayout.RePushDesktopSream(VHStreamType_SoftWare);
   }

   if (CLASS_STATUS_IN_CLASS == baseInfo.mWebinarInfo.type ) {
         mbFirstConnectWebRtcServer = false;
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandleWebRtcRoomConnectErr() {
   globalToolManager->GetDataManager()->WriteLog("%s ", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
      if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {
         FadeOutTip(Str_WebRtcRoomDisConnect, Tost_Warning);
      }
      else {
         FadeOutTip(Str_ConnectRoomErr, Tost_Error);
      }
   }
   else {
      FadeOutTip(Str_WebRtcRoomDisConnect, Tost_Error);
   }
}

void WebRtcLiveWdg::HandleStartAndStopClass(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s start", __FUNCTION__);
   ui.classRoomTitleWd->SetStartClassBtnEnable(true);
   //���� ����������   true

   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   if (pWdg)
   {
      WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
   }

   QEventRoomEvent* roomEvent = dynamic_cast<QEventRoomEvent*>(event);
   if (roomEvent->mRespCode == 300022) {
      LiveTypeChangeToLogout();
   }
   else
   {
      globalToolManager->GetDataManager()->WriteLog("%s start  mCallBackCodeType %1", __FUNCTION__, roomEvent->mCallBackCodeType);
      if (roomEvent->mCallBackCodeType == 1) {  // �Ͽ� �ص�
         globalToolManager->GetDataManager()->WriteLog("%s roomEvent->mCallBackCodeType 1 mRespCode:%d  mTeacherJoinNickName��%s", __FUNCTION__,
            roomEvent->mRespCode, roomEvent->mRespMsg.c_str());
         if (roomEvent->mRespCode == 200) {
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (globalToolManager->GetClassSDK()->LoginPlayType() != roomEvent->mProcessType && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
               //ѧԱ�յ��Ͽι㲥�����ֻ�õĿ������Ͳ�ͬ�ڵ�ǰ�������Ի��������ʾ�����µ�¼
               LiveTypeChangeToLogout();
            }
            else
            {
               ui.widget_right_tool->show();
               FadeOutTip(QStringLiteral("�Ͽ���"), Tost_Message);
               mLeftToolBarWdg->setEnabled(true);

               ui.classRoomTitleWd->UpdateStartClassState(true);

               if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
                  if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mWebinarInfo.speedmode && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO)
                  {

                     ChangePublishState(true);
                     mTeacherRender->SetPageIndex(TeacherRenderWdg_Page_Index_render);
                  }
                  else {
                     ChangePublishState(false);
                  }
                  globalToolManager->GetDataManager()->SetTeacherJoinId(roomEvent->mUserId);
                  mTeacherJoinNickName = QString::fromStdString(roomEvent->mRespMsg);
                  mTeacherRender->InitRenderCtrl(globalToolManager->GetDataManager()->GetTeacherJoinId());
                  mTeacherRender->SetUserJoinID(globalToolManager->GetDataManager()->GetTeacherJoinId());
                  mTeacherRender->SetNickName(mTeacherJoinNickName);

                  if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mWebinarInfo.speedmode)
                  {
                     globalToolManager->GetClassSDK()->ConnectVHMediaRoom(/*baseInfo.play_type*/);
                  }
               }
               else
               {
                  //��ʦ��ʼ�ϿΣ���������·���֡�
                  ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
                  if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                     mMixStreamLayout.SetMixStreamLayout(true);
                  }
                  mMixedReady = true;
                  //QString confPath = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
                  QString confPath = globalToolManager->GetToolConfigPath();
                  QString strLogReportUrl = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_LOGREPORT_URL, defaultLogReportURL);
                  QString strVer = globalToolManager->GetDataManager()->GetVersion();

                  UserJoinRoomBaseInfo info = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo;
                  CourseInfo corse_info = globalToolManager->GetClassSDK()->GetCourseInfo();
                  QString strParam = QString("%1?key=class_start&k=%2&id=%3&nick=%4&role=%5&devid=%6&ver=%7&uid=%8")
                     .arg(strLogReportUrl)
                     .arg(eClassLogRePortK_BeginClass)
                     .arg(info.join_id.c_str())
                     .arg(info.nick_name.c_str())
                     .arg(info.role_type)
                     .arg(globalToolManager->GetClassSDK()->GetDevId().c_str())
                     .arg(strVer)
                     .arg(corse_info.uid.c_str());

                  globalToolManager->GetClassSDK()->UpLoadLog(strParam.toStdString());
               }

               if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
                  setCurrentIndex(page_CenterRender);
               }
               else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
                  if (baseInfo.mDocInfo.doc_mode == 0) {
                     setCurrentIndex(page_Doc);
                  }
                  else {
                     setCurrentIndex(page_Whiteboard);
                  }
                  slot_DocLoadFinished(/*true*/);
               }
            }

         }
         else {
            globalToolManager->GetClassSDK()->StopLocalCapture();
            FadeOutTip(QString::fromStdString(roomEvent->mRespMsg), Tost_Message);
            if (mTeacherRender) {
               mTeacherRender->SetPageIndex(TeacherRenderWdg_Page_Index_classNoStart);
            }
         }
      }
      else if (roomEvent->mCallBackCodeType == 0) {//�¿λص�
         globalToolManager->GetDataManager()->SetIsSpeaking(false);
         globalToolManager->GetDataManager()->WriteLog("%s roomEvent->mCallBackCodeType 0", __FUNCTION__);
         if (roomEvent->mRespCode == 200) {
            if (mMultiClassFloatWnd && mMultiClassFloatWnd->isVisible()) {
               // mMultiClassFloatWnd->hide();
               slot_closeMultiFloat(mMultiClassFloatWnd->strUserId());
            }
            if (mFloatWnd && mFloatWnd->isVisible()) {
               //mFloatWnd->hide();
               slot_closeFloat(mFloatWnd->strUserId());
            }

            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
               recoveryOnlyVideoLayOut();
            }

            if (mMultiClassFloatWnd && !mMultiClassFloatWnd->isHidden()) {
               slot_closeMultiFloat("");
            }
            ui.classRoomTitleWd->UpdateStartClassState(false);
            //���� ���������� false
            //CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
            CBaseWnd* wnd = nullptr;
            CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
            if (pWdg) {
               WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
               wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
            }
            if (wnd) {
               CSTDMapParam mapParam;
               mapParam[Str_Quality_Enabeld] = Str_Quality_Enabeld;
               wnd->Notice(mapParam);
               //wnd->exec();
            }

            FadeOutTip(Str_ClassOver, Tost_Message);
            mMixedReady = false;
            globalToolManager->GetDataManager()->SetMuteCameraByTeacher(false);
            globalToolManager->GetDataManager()->SetMuteMicByTeacher(false);
            mLeftToolBarWdg->setEnabled(false);

            //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type) {
               slot_BtnStopScreenClicked();
            }

            globalToolManager->GetClassSDK()->StopLocalCapture();
            globalToolManager->GetClassSDK()->StopDesktopCapture();
            globalToolManager->GetClassSDK()->StopLocalAuxiliaryCapture();
            //ӆ����ЌW�T���l
            if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
               GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
               //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
               QString msConfPath = globalToolManager->GetConfigPath();
               ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
               GetPtrVhallIALiveSettingDlg()->RecoverState();
               globalToolManager->GetDataManager()->WriteLog("%s SetAllSubScribeAudio(true)", __FUNCTION__);
            }
            else {
               GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
               globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
               //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
               QString msConfPath = globalToolManager->GetConfigPath();
               ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
               ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
            }

            if (ui.CtrlDoubPubWdg->isVisible()) {
               ui.CtrlDoubPubWdg->stopPush();
            }

            //δ�����ѧԱ������л��˲���λ�á���ô�¿κ��޸�λ�á�
            bool bChangedLayout = false;
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
               for (int i = 0; i < ui.verticalLayout_teacher_render->count(); i++) {
                  if (ui.verticalLayout_teacher_render->itemAt(i)->widget() == ui.stackedWidget_Center) {
                     bChangedLayout = true;
                     break;
                  }
               }
            }
            if (bChangedLayout) {
               int i = ui.verticalLayout_8->count();
               ui.verticalLayout_8->insertWidget(i + 1, ui.stackedWidget_Center);
               if (mRecordAndLiveStreamCefViewWdg) {
                  mRecordAndLiveStreamCefViewWdg->hide();
                  globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: hide", __FUNCTION__);
                  ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
                  globalToolManager->GetDataManager()->WriteLog(" %s 4818  ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
                     __FUNCTION__);
               }
            }
            if (!ui.widget_main_title->isHidden()) {
               ui.widget_main_title->hide();
            }

            if (mTeacherRender) {
               mTeacherRender->show();
               globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
               if (baseInfo.mWebinarInfo.layout != CLASS_LAYOUT_ONLY_VIDEO) {
                  ui.widget_leftTopRender->show();
               }

               mTeacherRender->SetPageIndex(TeacherRenderWdg_Page_Index_classNoStart);
            }

            if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
               ui.classRoomTitleWd->SetStudentHandsUp(false);
            }
            if (mRenderManager) {
               mRenderManager->ClearAllRender();
            }
            mMemberListWdg->initUpperStates();

            setCurrentIndex(page_Doc);

            mMixStreamLayout.SetAlreadyMixed(false);
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               ui.label->setStyleSheet(CLASS_OVER_NOTICE_SHEET);
            }
            else {
               ui.label->setStyleSheet(CLASS_OVER_NOTICE_SHEET);
            }
            //�¿κ����¼��أ���ֹ���ط��¿κ���������
            if (mRecordAndLiveStreamCefViewWdg) {
               ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
               QString url = QString::fromStdString(baseInfo.urls.mixvideo);
               mRecordAndLiveStreamCefViewWdg->LoadUrl(url/*, URLType_LiveMixedStream*/);
            }

            mbFirstConnectWebRtcServer = true;
            //�¿κ�������ӷ����ˣ��˳���������
            if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type){
               //if (0 == baseInfo.mWebinarInfo.speedmode) {
                  globalToolManager->GetClassSDK()->LeaveVHMediaRoom();
               //}
            }
            else if (JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type) {
               mTeacherRender->SetShowSmallRender(false);

               if (!mstrIDPenAuth.isEmpty()) {
                  globalToolManager->GetClassSDK()->ChangeUserPenAuth(mstrIDPenAuth.toStdString(), false);
               }

            }
         }
         else {
            FadeOutTip(QString::fromStdString(roomEvent->mRespMsg), Tost_Message);
         }

         if (mbQuitAfterUpPublish && globalToolManager) {
            globalToolManager->SafeToExitApp();
         }
      }
   }

}

void WebRtcLiveWdg::HandleRecvAskForPublish(QEvent* event) {
   QEventRecvMsg *recvMsg = dynamic_cast<QEventRecvMsg*>(event);
   if (recvMsg) {
      globalToolManager->GetDataManager()->WriteLog("%s status : %d ", __FUNCTION__, recvMsg->mStatus);
      if (recvMsg->mStatus == 0) {
         if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_AVCapture))
         {
            //���յ�������Ϣ ����������ɹ���Ϣ
            globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish) 3821 PushStreamEvent_UnPublish", __FUNCTION__);
            globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish);
         }

      }
      else {
         //���յ�������Ϣ:
         //�ȼ�Ȿ���豸�Ƿ������������ܹ�������ʼ�ɼ����ɼ��ɹ�֮�������������ɹ�֮���ϱ�globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_Publish);
         //��������������ɹ���Ϣ֮����ʾ��ǰ��Ա������ͼ��
         if (globalToolManager->GetClassSDK()->HasVideoOrAudioDev()) {
            if (globalToolManager->GetClassSDK()->isConnetWebRtcRoom() ) {//����ģʽ���뷿�� ���������˷��䣬���ʱ��ʦ��������ֱ�ӱ�����������
               HandleWebRtcRoomConnetSuc(NULL);
            }
            else {
               ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
               //ѧԱ������Ҫ�����ӻ������䣬���ӳɹ�֮�󷽿�������
               globalToolManager->GetClassSDK()->ConnectVHMediaRoom(/*baseInfo.play_type*/);
            }

         }
         else {
            //û������Ƶ�豸����������ʧ����Ϣ��
            QString tips = Str_NoDeviceCannotPublish;
            FadeOutTip(tips, Tost_Error);
            globalToolManager->GetClassSDK()->UserPublishFailedCallback(PushStreamEvent_Publish);
         }
         bool bEnableDoublePub = VhallIALiveSettingDlg::isEnableDoublePublish();
         globalToolManager->GetClassSDK()->UpdatePublishAbility(globalToolManager->GetClassSDK()->HasVideoOrAudioDev(), bEnableDoublePub);
      }
   }
}

void WebRtcLiveWdg::HandleSendApplyPublishMsg(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Message);
   }
   ui.classRoomTitleWd->SetHandsUpBtnEnable(true);
}

void WebRtcLiveWdg::HandleCanclePublish(QEvent* event)
{
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
   ui.classRoomTitleWd->SetHandsUpBtnEnable(true);
}

void WebRtcLiveWdg::HandleRecvDownAllUserPublishMsg() {

   globalToolManager->GetClassSDK()->StopLocalCapture();
   globalToolManager->GetClassSDK()->StopDesktopCapture();
   globalToolManager->GetClassSDK()->StopMediaFileCapture();
	//ӆ����ЌW�T���l
	if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
		GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
		//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
		QString msConfPath = globalToolManager->GetConfigPath();
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
		GetPtrVhallIALiveSettingDlg()->RecoverState();
		globalToolManager->GetDataManager()->WriteLog("%s SetAllSubScribeAudio(true)", __FUNCTION__);
	}
	else {
		GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
		globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
		//QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
		QString msConfPath = globalToolManager->GetConfigPath();
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
		ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN,0);
	}
}

void WebRtcLiveWdg::HandleRecvApplyInavPublishMsg(QEvent* event) {
   QEventRecvMsg *recvMsg = dynamic_cast<QEventRecvMsg*>(event);
   if (recvMsg)
   {
      mMemberListWdg->addRaiseHandsMember(recvMsg->mUserId, COnlineItemWdg::eRaiseHandsState_RaiseHands);
   }
}

void WebRtcLiveWdg::HandleRecvSignUsers(QEvent* event)
{
   QEventSignUsers* recvMsg = dynamic_cast<QEventSignUsers*>(event);
   if (recvMsg)
   {
      mLeftToolBarWdg->RecvSignUsers(recvMsg->mstlistsignUsers, recvMsg->mstlistNoSignUsers);
   }
}

//void WebRtcLiveWdg::JustFadoutTips(QEvent* event) {
//	QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
//	if (msg && msg->mRespCode != 200) {
//		FadeOutTip(QString::fromStdString(msg->mRespMsg));
//	}
//}

void WebRtcLiveWdg::HandleGetOnLineUserList(QEvent *event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
}

void WebRtcLiveWdg::HandleCloseWnd(QEvent* event) {
   QEventDestoryWnd *msg = dynamic_cast<QEventDestoryWnd*>(event);
   if (msg && mLeftToolBarWdg) {
      QString wndId = msg->mWndId;
      if (wndId == WND_ID_MEDIA_PLAY_WEBVIEW) {
         mLeftToolBarWdg->DestoryMediaPlayWebViewDlg();
         mLeftToolBarWdg->slots_MediaMenuAboutToHide();
         mLeftToolBarWdg->slot_MutexWdgClose();
      }
      else if (wndId == WND_ID_QUESTION_UI) {
         //mLeftToolBarWdg->setAnserChecked(false);
         mLeftToolBarWdg->DestoryAnswerWebViewDlg();
         mLeftToolBarWdg->slot_MutexWdgClose();
      }
      else if (wndId == WND_ID_ANSWER_TOOL) {
         mLeftToolBarWdg->DestoryAnswerToolDlg();
			mLeftToolBarWdg->slot_MutexWdgClose();
      }
      else if (wndId == WND_ID_TIMER_TOOL) {
         if (mpTimerWdg && mpTimerWdg->isVisible())
         {
            mpTimerWdg->hide();
            mLeftToolBarWdg->DestoryTimerToolDlg();
         }
      }
   }
}


void WebRtcLiveWdg::HandleEnableHandsUp(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200) {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      }
      else {//�����ɹ�
         int status = msg->mUserData.toInt();//1��������0���رգ�
      //ui.classRoomTitleWd->setRaiseHandsState(status);
         mMemberListWdg->setHansUpStates(status == 1 ? true : false);
      }
   }
}

void WebRtcLiveWdg::HandleSendAgreePublishMsg(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      mMemberListWdg->SetUpperStates(msg->mUserId, COnlineItemWdg::eRaiseHandsState_RaiseHands);
   }
}

void WebRtcLiveWdg::HandleSendInvitePublishMsg(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {//����ʧ��
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      mMemberListWdg->SetUpperStates(msg->mUserId, COnlineItemWdg::eRaiseHandsState_normal); 
      removeApplyPublishUser(msg->mUserId);
   }

}

void WebRtcLiveWdg::HandleUserPublishCallback(QEvent* event) {
	QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
	globalToolManager->GetDataManager()->WriteLog("%s Publish Code : %d", __FUNCTION__, msg->mRespCode);
	if (msg && msg->mRespCode != 200) {
		FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
	}
	else {
		ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
		globalToolManager->GetDataManager()->WriteLog("%s Publish OR Stop :%d role_type :%d", __FUNCTION__, 
         msg->mUserData.toInt(), //0
         baseInfo.mUserInfo.role_type);//2
		//����������Ϣ�ɹ�
		if (msg->mUserData.toInt() == 1) {
			globalToolManager->GetDataManager()->SetIsSpeaking(true);
			

         if (msg->mUserId.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0) {
            FadeOutTip(Str_AlreadyPublish, Tost_Success);
         }
         else {
            //if (mMemberListWdg) {
            //   CMember info;
            //   bool bFind = mMemberListWdg->GetMember(msg->mUserId, info);
            //   if (bFind) {
            //      FadeOutTip(QStringLiteral("%1������").arg(info.nick_name), Tost_Message); 
            //   }
            //   else {
            //      FadeOutTip(QStringLiteral("%1������").arg(msg->mUserId), Tost_Message);
            //   }
            //}
         }

			ui.classRoomTitleWd->SetStudentHandsUp(true);
			//��������˿�����ȫԱ��������������ɹ�֮��ر���˷硣
			if (baseInfo.mutingAll == 1) {
				if (globalToolManager->GetClassSDK()->CloseMic()) {
					globalToolManager->GetClassSDK()->OperateUserMic(baseInfo.mUserInfo.join_id, true);
				}
			}
			if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
				//globalToolManager->GetClassSDK()->EnableSubScribeStream();

				//ChangePublishState(true);
				//globalToolManager->GetDataManager()->SetPublished(true);
				//if (mRenderManager) {
				//	mRenderManager->show();s
				//	globalToolManager->GetDataManager()->WriteLog("%s mRenderManager->show()", __FUNCTION__);
				//	mRenderManager->ShowAllRender();
				//}

				//��ʾ������ť
				ui.classRoomTitleWd->setRaiseHandsState(true);
			}
		}
		//��������ɹ�
		else {

         if (msg->mUserId.compare(QString::fromStdString( baseInfo.mUserInfo.join_id )) ==0 ) {
            FadeOutTip(Str_AlreadyUnPublish, Tost_Success);
         }
         else {
            /*if (mMemberListWdg) {
               CMember info;
               bool bFind = mMemberListWdg->GetMember(msg->mUserId, info);
               if (bFind) {
                  FadeOutTip(QStringLiteral("%1������").arg(info.nick_name), Tost_Message);
               }
               else {
                  FadeOutTip(QStringLiteral("%1������").arg(msg->mUserId), Tost_Message);
               }
            }*/
         }

         DisConnectRoom(msg->mUserId.toStdString());

         //�ж�Floatwnd�Ƿ񵯳�
         if (mMultiClassFloatWnd && mMultiClassFloatWnd->isVisible()) {
            mMultiClassFloatWnd->hide();
            if (mMultiClassFloatWnd->strUserId() == msg->mUserId) {
               slot_closeMultiFloat(mMultiClassFloatWnd->strUserId());
            }
            else if (!baseInfo.mWebinarInfo.speedmode) {
               slot_closeMultiFloat(mMultiClassFloatWnd->strUserId());
            }
         }

         if (mFloatWnd && mFloatWnd->isVisible()) {
            if (mFloatWnd->strUserId() == msg->mUserId) {
               slot_closeFloat(mFloatWnd->strUserId());
            }
            else if (!baseInfo.mWebinarInfo.speedmode) {
               slot_closeFloat(mFloatWnd->strUserId());
            }
         }

			if (mbQuitAfterUpPublish) { // �˳�����
				QuitLiveWdg();
			}
			}
		}
	}


void WebRtcLiveWdg::DisConnectRoom(std::string userId)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if ( baseInfo.mUserInfo.join_id.compare(userId ) == 0) {
      globalToolManager->GetDataManager()->SetMuteCameraByTeacher(false);
      globalToolManager->GetDataManager()->SetMuteMicByTeacher(false);
      if (mMultiClassFloatWnd && !mMultiClassFloatWnd->isHidden()) {
         slot_closeMultiFloat("");
      }

      globalToolManager->GetDataManager()->SetIsSpeaking(false);
      if (NULL != m_pScreenShareToolWgd && m_pScreenShareToolWgd->isVisible()) {
         m_pScreenShareToolWgd->on_btnStop_clicked();//ֹͣ���湲��
      }

      //ֹͣ�ɼ�
      if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)) {
         globalToolManager->GetClassSDK()->stopDoubleVrtc(baseInfo.mUserInfo.join_id);  //ֹͣ˫��
      }

      globalToolManager->GetClassSDK()->StopLocalCapture();                //ֹͣ��������ͷ�ɼ�
      globalToolManager->GetClassSDK()->StopLocalAuxiliaryCapture();    //ֹͣ˫��
      globalToolManager->GetClassSDK()->StopDesktopCapture();             //ֹͣ׿�湲��ɼ�

      //InitLayOut();
      ui.classRoomTitleWd->SetStudentHandsUp(false);
      //����������״̬��ֹͣ�������ɼ�
      if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
         GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);
         //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
         QString msConfPath = globalToolManager->GetConfigPath();
         ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
         GetPtrVhallIALiveSettingDlg()->RecoverState();
         globalToolManager->GetDataManager()->WriteLog("%s SetAllSubScribeAudio(true)", __FUNCTION__);
      }
      else {
         GetPtrVhallIALiveSettingDlg()->SetAllSubScribeAudio(false);//�رն�����Ƶ
         globalToolManager->GetClassSDK()->StopLocalCapturePlayer();
         //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
         QString msConfPath = globalToolManager->GetConfigPath();
         ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_MUTE_REMOTE, 0);
         ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_CAPTURE_OPEN, 0);
      }

      if (baseInfo.mWebinarInfo.speedmode 
         && (  globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile) ||
                  globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Desktop) ||
                  globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_MediaFile) ||
                  globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_SoftWare) ||
                  globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_RegionShare) ||
                  globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)  
              )
         ) {
      }
      else if(ui.stackedWidget_Center->currentIndex() != page_kickOut){//�ۿ���· ����Pageҳ
         slot_showInitPage();
      }
   }

   //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      RemoveSamllRenderFromTeacherWnd();
   }
   else {
      if (userId.length() == 0 || baseInfo.mUserInfo.join_id.compare(userId) == 0) {
         if (mRenderManager) {
            if (mRenderManager && baseInfo.mWebinarInfo.speedmode) {
               mRenderManager->RemovePublishUser(QString::fromStdString( userId ));
            }
            else if (mRenderManager)
            {
               mRenderManager->ClearAllRender();
            }
            if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               mTeacherRender->ShowAllMuteCtrl(mRenderManager->isVisible() ? true : false);
            }
         }
      }
      else {
         if (mRenderManager) {
            mRenderManager->RemovePublishUser(QString::fromStdString(userId) );
         }
      }
   }
   mMixStreamLayout.StopAllRepublish();
   if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      //globalToolManager->GetClassSDK()->DisableSubScribeStream();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
         if (baseInfo.mWebinarInfo.speedmode) {
         }
         else {  //�Ǽ���ģʽ
            globalToolManager->GetClassSDK()->LeaveVHMediaRoom();
            ChangePublishState(false);
         }
      }
      else {
         if (!baseInfo.mWebinarInfo.speedmode) {
            globalToolManager->GetClassSDK()->LeaveVHMediaRoom();
            ChangePublishState(false);
         }
      }
      globalToolManager->GetDataManager()->SetPublished(false);
      if (mRenderManager && !baseInfo.mWebinarInfo.speedmode) {
         mRenderManager->ClearAllRender();
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::TestWidgetLayout()
{
   /*globalToolManager->GetDataManager()->WriteLog("%s Enter   *********", __FUNCTION__);
   WritWidgetInfo(ui.classRoomTitleWd);
   globalToolManager->GetDataManager()->WriteLog("%n");
   WritWidgetInfo(ui.label_6);
   ���
   WritWidgetInfo(ui.widget_left_tool);
   WritWidgetInfo(mLeftToolBarWdg);
   globalToolManager->GetDataManager()->WriteLog("%n");

   �и���
   WritWidgetInfo(ui.widget_centerWidget);
   WritWidgetInfo(mRenderManager);
   WritWidgetInfo(ui.CtrlDoubPubWdg);
   WritWidgetInfo(ui.stackedWidget_Center);
   
   WritWidgetInfo(ui.page_Doc);
   WritWidgetInfo(mDocCefViewWdg);
   WritWidgetInfo(ui.page_Whiteboard);
   WritWidgetInfo(mWhiteBoardCefViewWdg);
   WritWidgetInfo(ui.page_CenterRender);
   WritWidgetInfo(mTeacherRender);
   
   WritWidgetInfo(ui.otherMediaWdg);
   WritWidgetInfo(ui.page_ClassStatus);
   
   WritWidgetInfo(ui.page_DesktopScreen);
   WritWidgetInfo(ui.page_kickOut);
   WritWidgetInfo(ui.pageMediaFilePlay);
   WritWidgetInfo(ui.MediaFilePlayWdg);
   WritWidgetInfo(ui.pageInvStateWdg);
   WritWidgetInfo(ui.invStateWdg);
   globalToolManager->GetDataManager()->WriteLog("%n");
   �Ҳ�
   WritWidgetInfo(ui.widget_right_tool);
   WritWidgetInfo(ui.widget_main_title);
   WritWidgetInfo(ui.widget_leftTopRender);
   WritWidgetInfo(ui.tabWidget);

   globalToolManager->GetDataManager()->WriteLog("%s Leave   #######", __FUNCTION__);
*/
}

void WebRtcLiveWdg::WritWidgetInfo(QWidget * pWidget)
{
   if (pWidget) {
      QPoint pos = pWidget->mapToGlobal(pWidget->pos());
      globalToolManager->GetDataManager()->WriteLog("objName: %s isVisble %d  x: %d y:%d widht:%d height: %d", 
         pWidget->objectName().toLatin1().data(), 
		  pWidget->isVisible(), 
		  pos.x(), pos.y(), pWidget->width(), pWidget->height()); 
   }
}

void WebRtcLiveWdg::QuitLiveWdg() {
	globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
	CBaseWnd* AnswerWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
	if (AnswerWdg) {
		globalToolManager->GetWndManager()->DestoryWnd(WND_ID_ANSWER_TOOL);
	}
	if (globalToolManager) {
		globalToolManager->SafeToExitApp();
	}
}

void WebRtcLiveWdg::RemoveSamllRenderFromTeacherWnd() {
   if (mTeacherRender) {
      mTeacherRender->SetShowSmallRender(false);
      mTeacherRender->ShowAllMuteCtrl(false);
      mTeacherRender->SetSmallRenderUserPublish(false);
      mTeacherRender->SetSmallRenderMicState(true);
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         if (globalToolManager) {
            VHClassRoomInterface* RoomInterface = globalToolManager->GetClassSDK();
            if (RoomInterface)
            {
               RoomInterface->SetBigScreen(baseInfo.mUserInfo.join_id, [&](const std::string& user, int code, const std::string& msg) {
                  DataManagerInterface* datamanageInterFace = globalToolManager->GetDataManager();
                  if (datamanageInterFace) {
                     datamanageInterFace->WriteLog(" code %d msg %s", code, msg.c_str());
                  }
                  
                  if (code != 0 && code != 200) {
                     globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromStdString(msg)));
                     return;
                  }
                  globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ChangeBigScreen, QString::fromStdString(user)));
               
               });
            
            }

         }
      }
      //QString wndId = WND_ID_FLOAT_RENDER + mTeacherRender->GetUserJoinID();
      //if (globalToolManager->GetWndManager()->IsWndExist(wndId)) {
      //}
      //QString FullWndId = WND_ID_FULL_SCREEN_RENDER + mTeacherRender->GetUserJoinID();
      //if (globalToolManager->GetWndManager()->IsWndExist(FullWndId)) {
      //}
   }
}

void WebRtcLiveWdg::ChangePublishState(bool publish) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   globalToolManager->GetDataManager()->WriteLog("%s publish: %d", __FUNCTION__, publish);
   QString url = QString::fromStdString(baseInfo.urls.mixvideo);
   if (publish) {
      if (mTeacherRender) {
         mTeacherRender->show();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: show", __FUNCTION__);
      }
      if (mRecordAndLiveStreamCefViewWdg) {
         mRecordAndLiveStreamCefViewWdg->hide();
         globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: hide", __FUNCTION__);
         //���¼����²������ӣ������������и����Ű������������������Ż�����Ƶ���������֮��û�йرջ������棬���ֲ��Ż����������⡣
         mRecordAndLiveStreamCefViewWdg->LoadUrl(url/*, URLType_LiveMixedStream*/);
         if (mpFullWebView && !mpFullWebView->isHidden()) {
            slot_LiveMixStreamWebKeyEsc();
         }      
         ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);
         globalToolManager->GetDataManager()->WriteLog(" %s 5323 publish %d ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
            __FUNCTION__, publish);
         ui.verticalLayout_8->insertWidget(1, ui.stackedWidget_Center);
         ui.widget_main_title->hide();
      }
   }
   else {
      if (mTeacherRender) {
         mTeacherRender->hide();
         globalToolManager->GetDataManager()->WriteLog("%s mTeacherRender: hide", __FUNCTION__);
      }
      if (mRecordAndLiveStreamCefViewWdg) {
         if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
            mRecordAndLiveStreamCefViewWdg->LoadUrl(url/*, URLType_LiveMixedStream*/);
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_VIDEO_AND_DOC) {
               ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg);

               globalToolManager->GetDataManager()->WriteLog(" %s 5339 publish %d ui.verticalLayout_teacher_render->addWidget(mRecordAndLiveStreamCefViewWdg) ",
                  __FUNCTION__, publish);

               mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_RIGHT_TOP);
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                  ui.widget_main_title->show();
               }
            }
            else if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
               ui.verticalLayout_CenterRender->addWidget(mRecordAndLiveStreamCefViewWdg);
               mRecordAndLiveStreamCefViewWdg->SetLayOutPos(LAYOUT_POS_CENTER);
            }
         }
         mRecordAndLiveStreamCefViewWdg->show();
         globalToolManager->GetDataManager()->WriteLog("%s mRecordAndLiveStreamCefViewWdg: show", __FUNCTION__);
      }
   }
}

void WebRtcLiveWdg::HandleUpdatePublishAbility(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
}

void WebRtcLiveWdg::HandleInviteDoubleVrtcCallback(QEvent* event) {
	QEventChangeUser *msg = dynamic_cast<QEventChangeUser*>(event);
	if (msg && msg->iCodeErr == 200) {
		ui.CtrlDoubPubWdg->ChangeUser();
	}
}

void WebRtcLiveWdg::HandleRequestStartDoubleVrtc(QEvent*event)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   QEventChangeUser *msg = dynamic_cast<QEventChangeUser*>(event);
   if (msg && msg->iCodeErr == 200) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.layout != CLASS_LAYOUT_ONLY_VIDEO) {//�ĵ�ģʽ
         HWND wnd = (HWND)(ui.otherMediaWdg->winId());
         setCurrentIndex(page_otherMediaWdg);
         //globalToolManager->GetDataManager()->WriteLog("%s ", __FUNCTION__);
         globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_Auxiliary_CamerCapture, (void*)(wnd));
         //globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_Auxiliary_CamerCapture, ui.otherMediaWdg);
      }
      else {//��Ƶģʽ
         AuxiliaryShowTeacherWdg();
         HWND wnd = (HWND)(ui.otherMediaWdg->winId());
         setCurrentIndex(page_otherMediaWdg);
         globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_Auxiliary_CamerCapture, (void*)(wnd));
         //globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_Auxiliary_CamerCapture, ui.otherMediaWdg);
      }

      mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_TeacherDoublCamera);
      //double_push = 1;  //1����ͷ  2���湲��
      QJsonObject json;
      json.insert("double_push", VHDoubleStreamType_Camera);

      QJsonDocument document;
      document.setObject(json);
      QByteArray byteArray = document.toJson(QJsonDocument::Compact);
      QString strJson(byteArray);
      globalToolManager->GetClassSDK()->StartPushLocalAuxiliaryStream(strJson.toStdString());

      //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	  QString msConfPath = globalToolManager->GetConfigPath();
      QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
      QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");
      QString doubleCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, DOUBLE_CAMERA_ID, "");

      globalToolManager->GetClassSDK()->SendMicsDevice(configMicId.toLatin1().data(), configCameraId.toLatin1().data(),
         doubleCameraId.toLatin1().data(), baseInfo.mUserInfo.role_type);

   }
   else if(msg){
      FadeOutTip(msg->strMsg, Tost_Message);
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave ", __FUNCTION__);
}

void WebRtcLiveWdg::HandleUserPublishFailedCallback(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
   else {
      globalToolManager->GetClassSDK()->LeaveVHMediaRoom();
      mMixStreamLayout.StopAllRepublish();
      if (mRenderManager) {
         mRenderManager->ClearAllRender();
      }
   }
}

void WebRtcLiveWdg::HandleOnRecvKickOutRoomMsg(QEvent* event) {
   QEventRecvMsg *msg = dynamic_cast<QEventRecvMsg*>(event);
   if (msg) {
      QString user_id = msg->mUserId;
      int status = msg->mUserData.toInt();
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

      mMemberListWdg->addKickOutMember(user_id, status);
      if (status == 1) {
         if (user_id.toStdString() == baseInfo.mUserInfo.join_id)
         {
            globalToolManager->GetClassSDK()->LeaveClassRoom();
            ui.classRoomTitleWd->hideMediaSet();
			ui.label_kickout->setText(QString::fromLocal8Bit("���ѱ���ʦ�߳�����~"));
            setCurrentIndex(page_kickOut);
            ui.widget_right_tool->hide();
            if (mRenderManager) {
               mRenderManager->hide();
            }
            globalToolManager->GetWndManager()->DestoryExistWnd(WND_ID_WEBRTC_MAIN);
            ui.classRoomTitleWd->UpdateStartClassState(false);
            //���� ���������� false
            //CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
            CBaseWnd* wnd = nullptr;
            CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
            if (pWdg)
            {
               WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
               wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
            }

            if (wnd) {
               CSTDMapParam mapParam;
               mapParam[Str_Quality_Enabeld] = Str_Quality_Enabeld;
               wnd->Notice(mapParam);
               //wnd->exec();
            }

         }
         else//�߳��б����
         {

         }

      }
      else {//ȡ���߳�����

      }
   }
}

void WebRtcLiveWdg::HandleRecvOnUserPublishCallback(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   QEventUserPublish *msg = dynamic_cast<QEventUserPublish*>(event);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (msg) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      int status = msg->memberStatusType;
      QString user_id = msg->thirdPartyUserId;

      QString nickName = QString::fromStdString(msg->strStreamId);
      bool hasVideo = true;
      bool hasAudio = true;

      VHStreamType  iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(hasAudio, hasVideo);
      globalToolManager->GetClassSDK()->GetSubScribeStreamFormat(user_id.toStdWString(), iStreamType, hasVideo, hasAudio);

		//���������б�
		//mMemberListWdg->SetInviteList(user_id.toStdString());

		if (status == 1) {//����
         globalToolManager->GetDataManager()->WriteLog("3097 %s ", __FUNCTION__);
         //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         mMemberListWdg->SetUpperStates(user_id, (int)COnlineItemWdg::eRaiseHandsState_Speaking);
         //}
         globalToolManager->GetDataManager()->WriteLog("3101 %s ", __FUNCTION__);

         if (ui.CtrlDoubPubWdg->isVisible()) {
            CDoubleVrtcUsers vrtcUser;
            vrtcUser.mstrId = msg->thirdPartyUserId;
            vrtcUser.mstrNickName = QString::fromStdString(msg->strStreamId);
            vrtcUser.miNoDoubleVrtc = msg->miNoDoubleVrtc;

            ui.CtrlDoubPubWdg->AddUser(vrtcUser);
         }

         WebinarInfo infot= globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo;
            
            if (infot.type != CLASS_STATUS_IN_CLASS) {
               globalToolManager->GetDataManager()->WriteLog("%s infot.type��%d  not CLASS_STATUS_IN_CLASS so KickUserPublish %s", __FUNCTION__, (int)infot.type, user_id.toLocal8Bit().data());
               globalToolManager->GetClassSDK()->KickUserPublish(user_id.toStdString());
               return;
            }

            FadeOutTip(QStringLiteral("%1������").arg(nickName),Tost_Message);
           // int dev_type = msg->device_type;
            if (msg->role_type == JOIN_ROLE_Teacher) {
                globalToolManager->GetDataManager()->SetTeacherJoinId(user_id);
                if (mTeacherRender) {
                    mTeacherRender->InitRenderCtrl(user_id);
                    mTeacherRender->SetUserJoinID(user_id);
                    mTeacherRender->SetNickName(nickName);
                    mTeacherRender->SetCameraState(hasVideo);
                    mTeacherRender->SetMicState(hasAudio);
                    mTeacherRender->SetRoleType(msg->role_type);
                }
            }
            else /*if(user_id.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0 && globalToolManager->GetDataManager()->IsAlreadyPublished())*/{
                if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && mRenderManager) {
                    mRenderManager->AddInitReadyPublishUser(user_id, nickName, msg->role_type, hasVideo, hasAudio);
                    
                    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {

                       //bool hasVrtc = mRenderManager->IUserCount() > 0; //ѧԱ����
                       bool bPush = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop) ||
                          globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile) ||
                          globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_SoftWare) ||
                          globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);

                       if (/*!hasVrtc &&*/ !bPush && page_otherMediaWdg != ui.stackedWidget_Center->currentIndex()) {
                          mMixStreamLayout.SetMixStreamLayout();
                       }
                    }
                    if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                        mTeacherRender->ShowAllMuteCtrl(true);
                    }
                }
                else {
                    if (mTeacherRender) {
                        mTeacherRender->SetSmallWndUserJoinID(user_id);
                        mTeacherRender->SetSmallRenderNickName(nickName);
                        mTeacherRender->SetSmallRenderCameraState(hasVideo);
                        mTeacherRender->SetSmallRenderMicState(hasAudio);
                        mTeacherRender->SetSmallRenderRole(msg->role_type);
                    }
                }
            }

			if (user_id.compare( QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0){//��ʾ����
				ui.classRoomTitleWd->SetStudentHandsUp(true);
			}
			globalToolManager->GetDataManager()->WriteLog("3105 %s ", __FUNCTION__);
        }
        else { //����
         FadeOutTip(QStringLiteral("%1������").arg(nickName), Tost_Message);

         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            //bool bCamera = globalToolManager->GetClassSDK()->IsExitSubScribeStream(user_id.toStdString(), VHStreamType_Auxiliary_CamerCapture);
            //bool bStuDesktop = globalToolManager->GetClassSDK()->IsExitSubScribeStream(user_id.toStdString(), VHStreamType_Stu_Desktop);

            if (user_id.compare(mstrIDPenAuth)==0) {
               globalToolManager->GetClassSDK()->ChangeUserPenAuth(mstrIDPenAuth.toStdString(), false);
            }

            bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);

            if (!ui.CtrlDoubPubWdg->CurDoubleStuId().isEmpty() && ui.CtrlDoubPubWdg->CurDoubleStuId() == user_id) {
               if (ui.CtrlDoubPubWdg->GetCurrentType() == DoublePubStuChoiceUI::eChoiceType::eChoiceType_Camera) {
                  setCurrentIndex(page_InvStateWdg);
                  ui.invStateWdg->SetStateType(InvitationStateWdg::eStateType_ReleasWheat);
                  ui.CtrlDoubPubWdg->SetType(DoublePubStuChoiceUI::eChoiceType::eChoiceType_Invalid);
               }
               if (ui.CtrlDoubPubWdg->GetCurrentType() == DoublePubStuChoiceUI::eChoiceType::eChoiceType_Deskop) {
                  setCurrentIndex(page_InvStateWdg);
                  ui.invStateWdg->SetStateType(InvitationStateWdg::eStateType_ReleasVrtc);
                  ui.CtrlDoubPubWdg->SetType(DoublePubStuChoiceUI::eChoiceType::eChoiceType_Invalid);
               }
            }
         }

         if (ui.CtrlDoubPubWdg->isVisible()) {
            ui.CtrlDoubPubWdg->slot_DeleteItem(user_id);
         }

         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
            RemoveSamllRenderFromTeacherWnd();
         }
         else {//С����

            bool hasVrtc = false;
            
            if(mFloatWnd && mFloatWnd->isVisible() && mFloatWnd->strUserId().compare(user_id) == 0){
               slot_closeFloat(user_id);
            }

            if (mMultiClassFloatWnd && mMultiClassFloatWnd->isVisible() && mMultiClassFloatWnd->strUserId().compare(user_id) == 0) {
               slot_closeMultiFloat(user_id);
            }

            if (mRenderManager) {
               mRenderManager->RemovePublishUser(user_id);
               if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  mTeacherRender->ShowAllMuteCtrl(mRenderManager->isVisible() ? true : false);
               }
               hasVrtc = mRenderManager->IUserCount() > 0;
               bool bPush = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop) ||
                  globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile) ||
                  globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_SoftWare) ||
                  globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
               if (!hasVrtc && !bPush && page_otherMediaWdg == ui.stackedWidget_Center->currentIndex()) {
                  slot_showInitPage();
               }
            }

            ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

            if (roomInfo.mWebinarInfo.mixedLayout == eMixedLayoutMode_Grid 
               && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               mMixStreamLayout.SetMixStreamLayout();
            }
         }
         //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            mMemberListWdg->SetUpperStates(user_id, (int)COnlineItemWdg::eRaiseHandsState_normal);
         //}
         if (user_id.compare(QString::fromStdString(baseInfo.mUserInfo.join_id)) == 0)
         {//��ʾ����
            ui.classRoomTitleWd->SetStudentHandsUp(false);
         }

         TestWidgetLayout();
      }

      if (nullptr != mRenderManager && msg->miRewardNum >= 0)
      {
         mRenderManager->initTrophyRewardNum(user_id, msg->miRewardNum);
      }
   }
   //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
   if (mMemberListWdg && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
      mMemberListWdg->on_btnRefresh_clicked();
   }
   globalToolManager->GetDataManager()->WriteLog("Leave %s ", __FUNCTION__);
}

void WebRtcLiveWdg::HandleSendKickPublishMsg(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      //�߳�ʧ�ܡ�����ʾ����״̬
      mMemberListWdg->SetUpperStates(msg->mUserId, (int)COnlineItemWdg::eRaiseHandsState_Speaking);
   }
   else {
      //�߳ɹ���ʾ�ȴ�Զ���û�����ɹ�֮����״̬����HandleRecvOnUserPublishCallback�д���
   }
   removeApplyPublishUser(msg->mUserId);
}

void WebRtcLiveWdg::HandleSendSetBrush(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
   else {
      if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {

      }
      else {
         if (mRenderManager) {

            if ( msg->mUserData.toInt() > 0 ) {//��Ȩ���� �� ��ǰid
               mstrIDPenAuth = msg->mUserId;
            }
            else {
               mstrIDPenAuth = "";
            }

            mRenderManager->SetUserPenAuth(msg->mUserId, msg->mUserData.toInt());
            QString penUser = globalToolManager->GetDataManager()->GetCurrentPenUser();
            if (penUser != msg->mUserId) {
               mRenderManager->SetUserPenAuth(penUser, 0);
               globalToolManager->GetDataManager()->SetCurrentPenUser(msg->mUserId);
            }
         }
      }
   }
}

void WebRtcLiveWdg::HandleSendForbiddenChat(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg && msg->mRespCode != 200) {
      FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
   }
   else if (msg) {
      int state = msg->mUserData.toInt();
      //int limit =  ? COnlineItemWdg::eLimitedState_Forbidden : COnlineItemWdg::eLimitedState_normal;
      mMemberListWdg->SetForbiddenState(msg->mUserId, state == 1);
      CMember member = mMemberListWdg->GetMemberPersonalInfo(msg->mUserId);
      if (state == 1) {
         FadeOutTip(member.nick_name + Str_ChatForbided, Tost_Warning);
      }
   }
}

void WebRtcLiveWdg::HandleSendForbiddenAllChat(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200 && msg->mUserData == "true") {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Message);
      }
      else if (msg->mRespCode != 200 && msg->mUserData == "false") {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Message);
      }
   }
}

void WebRtcLiveWdg::HandleOnRecvForbiddenChatMsg(QEvent* event) {
   QEventRecvMsg *msg = dynamic_cast<QEventRecvMsg*>(event);
   if (msg) {
      QString user_id = msg->mUserId;


      QString strNick;
      //��Ϣ����'
      UserJoinRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo;
      if (user_id.compare(QString::fromStdString(baseInfo.join_id)) == 0)//��ǰѧԱ
      {
         strNick = QStringLiteral("��");
      }
      else if (JOIN_ROLE_Teacher == baseInfo.role_type)
      {
         CMember info;
         mMemberListWdg->GetMember(user_id, info);
         strNick = info.nick_name;
      }
      else
      {
         return;
      }

      int forbid = msg->mStatus;//1 �ǽ��ԣ�0 ȡ������
      mMemberListWdg->addForbidChatMember(user_id, forbid);

      if (CMemberListWdg::eForbidden_Forbidden == forbid)//����
      {
         FadeOutTip(strNick + QStringLiteral("�ѱ�����"), Tost_Warning);
      }
      else//ȡ������
      {
         FadeOutTip(strNick + QStringLiteral("�ѱ��������"), Tost_Message);
      }

      //����js
      QString strTemp = QString("[\"cmd\", %1]").arg(msg->mUserData);
      QByteArray dataArray(strTemp.toStdString().c_str(), strTemp.toStdString().length());
      QString base64EncodeData = dataArray.toBase64();
      QString method = QString("msgByClient('%1')").arg(base64EncodeData);

      //mChatWebEngineViewWdg->runJavaScript(method);
      mChatCefViewWdg->AppCallJsFunction(method);
   }
}

void WebRtcLiveWdg::HandleOnRecvForbiddenAllChatMsg(QEvent* event) {
   QEventRecvMsg *msg = dynamic_cast<QEventRecvMsg*>(event);
   if (msg) {
      int forbid = msg->mStatus;//
      //mMemberListWdg->addForbidChatMember("", CMemberListWdg::eForbidden_Forbidden ==forbid);

      QString strTemp = QString("[\"cmd\", %1]").arg(msg->mUserData);
      QByteArray dataArray(strTemp.toStdString().c_str(), strTemp.toStdString().length());
      QString base64EncodeData = dataArray.toBase64();
      QString method = QString("msgByClient('%1')").arg(base64EncodeData);

      //mChatWebEngineViewWdg->runJavaScript(method);
      mChatCefViewWdg->AppCallJsFunction(method);
      if (CMemberListWdg::eForbidden_Forbidden == forbid)//����
      {
         FadeOutTip(QStringLiteral("����ȫ�����"), Tost_Message);
      }
      else {
         FadeOutTip(QStringLiteral("���ȫ�����"), Tost_Message);
      }
   }
}

void WebRtcLiveWdg::HandleOnRecvUpdatePublishAbility(QEvent* event) {
   QEventRecvMsg *msg = dynamic_cast<QEventRecvMsg*>(event);
   if (msg) {
      int status = msg->mStatus;  //0 ������������1 ��
      QString user_id = msg->mUserId;
      mMemberListWdg->updatePublishAbility(user_id, status);
   }
}

void WebRtcLiveWdg::HandleAllUnPublish(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200) {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      }
      else {
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
            if (mRenderManager) {
               mRenderManager->UnPublishAllUsers();
            }
         }
      }
   }
}

void WebRtcLiveWdg::HandleOperaMuteAllMic(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200) {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      }
      else {
         if (msg->mUserData.toInt() == 1) {
            FadeOutTip(QStringLiteral("����ȫ�徲��"), Tost_Message);
            if (mRenderManager) {
               mRenderManager->SetAllMuteState(false);
            }
            mLeftToolBarWdg->SetMuteStudent(true);
         }
         else {
            FadeOutTip(QStringLiteral("���ȫ�徲��"), Tost_Message);
            if (mRenderManager) {
               mRenderManager->SetAllMuteState(true);
            }
            mLeftToolBarWdg->SetMuteStudent(false);
         }
         if (mTeacherRender) {
            mTeacherRender->SetAllMuteAudioState(msg->mUserData.toInt());
         }
      }
   }
}

void WebRtcLiveWdg::HandleRecvMuteMic(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s enter", __FUNCTION__);
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //������յ��ľ�����Ϣ������Ƕ��ҵĲ�����ֱ�ӹر���˷��򿪡�
   if (msg->mUserId.toStdString() == baseInfo.mUserInfo.join_id) {
      if (msg->mUserData.toInt() == 1) {
         globalToolManager->GetDataManager()->WriteLog("%s SetMuteMicByTeacher", __FUNCTION__);
         globalToolManager->GetDataManager()->SetMuteMicByTeacher(true);
         globalToolManager->GetDataManager()->WriteLog("%s FadeOutTip", __FUNCTION__);
         FadeOutTip(QStringLiteral("���ѱ�����"), Tost_Warning);
         globalToolManager->GetDataManager()->WriteLog("%s CloseMic", __FUNCTION__);
         globalToolManager->GetClassSDK()->CloseMic();
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender &&
            globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Student) {
            mTeacherRender->SetStuSmallRenderMicState(false);
         }
      }
      else {
         FadeOutTip(QStringLiteral("���ѱ��������"), Tost_Message);
         globalToolManager->GetClassSDK()->OpenMic();
         globalToolManager->GetDataManager()->SetMuteMicByTeacher(false);
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender &&
            globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Student) {
            mTeacherRender->SetStuSmallRenderMicState(true);
         }
      }

      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         mTeacherRender->SetSmallRenderMicState(msg->mUserData.toInt() == 1 ? false : true, true);
      }
      else {
         if (mRenderManager) {
            mRenderManager->SetUserMicState(msg->mUserId, msg->mUserData.toInt() == 1 ? false : true);
         }
      }
   }
   else {
	   CMember info = mMemberListWdg->GetMemberPersonalInfo(msg->mUserId);
	   if (msg->mUserData.toInt() == 1) {
		   FadeOutTip(info.nick_name + Str_MuteAudio, Tost_Message);
	   }
	   else {
		   FadeOutTip(info.nick_name + Str_CancelMuteAudio, Tost_Message);
	   }

      //����ǽ�ʦ�Լ��ر��˾�������
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && msg->mUserId == globalToolManager->GetDataManager()->GetTeacherJoinId()) {//�յ��Լ��ľ���/ȡ������
         
		  //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {//ѧ��
			 // if (mTeacherRender)
				//  mTeacherRender->SetStuSmallRenderMicState(msg->mUserData.toInt() == 1 ? false : true);
		  //}
		  //else {//��ʦ
			  if (mTeacherRender)
				  mTeacherRender->SetMicState(msg->mUserData.toInt() == 1 ? false : true);
		  //}
			  
      }
	   else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender &&
		      baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) { //������  ѧԱ  �յ���ʦ��״̬
		  mTeacherRender->SetMultiMicState(msg->mUserData.toInt() == 1 ? false : true, baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO);
	   }
      else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender) {//������ ��ʦ �յ�ѧԱ����˷�״̬
		  mTeacherRender->SetSmallRenderMicState(msg->mUserData.toInt() == 1 ? false : true);
	   }
	   else {
		  if (mRenderManager) {
			  mRenderManager->SetUserMicState(msg->mUserId, msg->mUserData.toInt() == 1 ? false : true);
		  }
	   }
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandleMuteMic(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200) {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      }
      else {
         if (msg->mUserId.toStdString() == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id) {
            if (msg->mUserData.toInt() == 1) {
               globalToolManager->GetClassSDK()->CloseMic();
               globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_YouCMuteMic));
               mLeftToolBarWdg->SetMuteSelf(true);

            }
            else {
               globalToolManager->GetClassSDK()->OpenMic();
               globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_YouCancelMuteMic));
               mLeftToolBarWdg->SetMuteSelf(false);
            }

			if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender &&
				globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Student) {
				if (msg->mUserId == globalToolManager->GetDataManager()->GetTeacherJoinId()) {
					mTeacherRender->SetMicState(msg->mUserData.toInt() != 1);
				}
				else {
					mTeacherRender->SetSmallRenderMicState(msg->mUserData.toInt() != 1, true);
				}
			}
			else if (msg->mUserId == globalToolManager->GetDataManager()->GetTeacherJoinId()) {
               if (mTeacherRender) {
                  mTeacherRender->SetMicState(msg->mUserData.toInt() == 1 ? false : true);
               }  
            }
            else {
               if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
                  mTeacherRender->SetSmallRenderMicState(msg->mUserData.toInt() == 1 ? false : true);
               }
               else {
                  if (mRenderManager) {
                     mRenderManager->SetUserMicState(msg->mUserId, msg->mUserData.toInt() == 1 ? false : true);
                  }
               }
            }
         }
         else {
				QString nick_name = GetMemberPersonalInfo(msg->mUserId).nick_name;
            if (msg->mUserData.toInt() == 1) {				
               FadeOutTip(QStringLiteral("%1�ѱ�����").arg(nick_name), Tost_Message);
            }
            else {
					FadeOutTip(QStringLiteral("%1�ѱ��������").arg(nick_name), Tost_Message);
				}
            if (msg->mUserId == globalToolManager->GetDataManager()->GetTeacherJoinId()) {
               if (mTeacherRender)
                  mTeacherRender->SetMicState(msg->mUserData.toInt() == 1 ? false : true);
            }
            else {
               if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
                  mTeacherRender->SetSmallRenderMicState(msg->mUserData.toInt() == 1 ? false : true);
               }
               else {
                  if (mRenderManager) {
                     mRenderManager->SetUserMicState(msg->mUserId, msg->mUserData.toInt() == 1 ? false : true);
                  }
               }
            }
         }
      }
   }
}

//void WebRtcLiveWdg::HandleMuteMicAll(QEvent* event) {
//    QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
//    if (msg) {
//
//    }
//}

void WebRtcLiveWdg::HandleRecvAllMuteMic(QEvent* event) {
   //����������ȫԱ�����¼���ȡ��ȫԱ������
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mUserData.toInt() == 1) {
         globalToolManager->GetDataManager()->SetMuteMicByTeacher(true);
         FadeOutTip(Str_StartAllMuteAudio, Tost_Message);
         if (globalToolManager->GetClassSDK()->CloseMic()) {
            //globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
         }

         if (mRenderManager) {
            mRenderManager->SetAllMicState(false);
         }

         if(mLeftToolBarWdg){
            mLeftToolBarWdg->SetMuteStudent(true);
         }

      }
      else {
         globalToolManager->GetDataManager()->SetMuteMicByTeacher(false);
         FadeOutTip(Str_CancelAllMuteAudio, Tost_Message);
         if (globalToolManager->GetClassSDK()->OpenMic()) {
            //globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
         }

         if (mRenderManager) {
            mRenderManager->SetAllMicState(true);
         }

         if (mLeftToolBarWdg) {
            mLeftToolBarWdg->SetMuteStudent(false);
         }

      }
   }
}

void WebRtcLiveWdg::HandleMuteVideo(QEvent* event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mRespCode != 200) {
         FadeOutTip(QString::fromStdString(msg->mRespMsg), Tost_Error);
      }
      else {
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (msg->mUserId.toStdString() == baseInfo.mUserInfo.join_id && event->type() == CustomEvent_RecvMuteVideo) {
            //���յ��ر�����ͷ�ʹ�����ͷ����
            if (msg->mUserData.toInt() == 1) {
               FadeOutTip(Str_MuteCamera, Tost_Warning);
               //if (globalToolManager->GetClassSDK()->CloseCamera())
               {
                  globalToolManager->GetDataManager()->SetMyCameraOpen(false);
                  //�ñ�����ͷ��ʽclose
                  if (nullptr != mRenderManager){
                     mRenderManager->SetCameraQss(QString::fromStdString(baseInfo.mUserInfo.join_id), false);
                  }
                  if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                     mTeacherRender->SetCameraState(false);
                  }
               }
               globalToolManager->GetDataManager()->SetMuteCameraByTeacher(true);
            }
            else {
               FadeOutTip(Str_CancelMuteCamera, Tost_Message);
               if (globalToolManager->GetClassSDK()->OpenCamera()){
                  globalToolManager->GetDataManager()->SetMyCameraOpen(true);
                  //�ı�����ͷ��ʽ  open
                  if (nullptr != mRenderManager) {
                     mRenderManager->SetCameraQss(QString::fromStdString(baseInfo.mUserInfo.join_id), true);
                  }
                  if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                     mTeacherRender->SetCameraState(true);
                  }
               }
               globalToolManager->GetDataManager()->SetMuteCameraByTeacher(false);
            }
         }
         if (msg->mUserId.toStdString() == baseInfo.mUserInfo.join_id && event->type() == CustomEvent_MuteVideo) {
            if (msg->mUserData.toInt() == 1) {
               FadeOutTip(Str_MuteCamera, Tost_Warning);
               if (nullptr != mRenderManager) {
                  mRenderManager->SetCameraQss(QString::fromStdString(baseInfo.mUserInfo.join_id), false);
               }
               if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                  mTeacherRender->SetCameraState(false);
               }
            }
            else {
               FadeOutTip(Str_CancelMuteCamera, Tost_Message);
               if (nullptr != mRenderManager) {
                  mRenderManager->SetCameraQss(QString::fromStdString(baseInfo.mUserInfo.join_id), true);
               }
               if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
                  mTeacherRender->SetCameraState(true);
               }
            }
         }
         else {
            CMember info = mMemberListWdg->GetMemberPersonalInfo(msg->mUserId);
			QString strName = info.nick_name;

            if (msg->mUserData.toInt() == 1) {
               FadeOutTip(strName + Str_MuteCameraByOther, Tost_Message);
            }
            else {
               FadeOutTip(strName + Str_CancelMuteCameraByOther, Tost_Message);
            }
			   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && msg->mUserId.toStdString() != baseInfo.mUserInfo.join_id) {
               mTeacherRender->SetMultiCameraState(msg->mUserId,msg->mUserData.toInt() != 1);
               globalToolManager->GetDataManager()->SetMultiClassOtherUserCameraOpen(msg->mUserData.toInt() != 1 ? true : false);
            }
            else if (mTeacherRender && mTeacherRender->GetUserJoinID() == msg->mUserId) {
               mTeacherRender->SetCameraState(msg->mUserData.toInt() == 1 ? false : true);
            }
            else {
                  if (mRenderManager) {
                     mRenderManager->SetUserCameraState(msg->mUserId, msg->mUserData.toInt() == 1 ? false : true);
                  }
                  if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
                     if (mTeacherRender)
                        mTeacherRender->SetSmallRenderCameraState(msg->mUserData.toInt() == 1 ? false : true, msg->mUserId.toStdString() == baseInfo.mUserInfo.join_id);
                  }
               }
            }
      }
   }
}

void WebRtcLiveWdg::HandleOnRecvOpenHandUpMsg(QEvent* event) {
   CustomEventType *msg = dynamic_cast<CustomEventType*>(event);
   if (msg) {
      int status = msg->msg.toInt();//״̬��1����0�أ�
      ui.classRoomTitleWd->setRaiseHandsState(status);
   }
}

void WebRtcLiveWdg::HandlePlayCameraAuxiliary() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   globalToolManager->GetClassSDK()->startDoubleVrtc();
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandleUserLineState(QEvent* event) {
   QEventRecvMsg *lineMsg = dynamic_cast<QEventRecvMsg*>(event);
   if (lineMsg) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      /*if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher)*/
    //{
      QString user_id = lineMsg->mUserId;
      QString user_name = lineMsg->mUserData;
      globalToolManager->GetDataManager()->WriteLog("%s user_id:%s state:%d", __FUNCTION__, user_id.toStdString().c_str(), event->type());
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
         if (mRenderManager) {
            mRenderManager->UpdateInfo(user_id, user_name);
            mRenderManager->HandleUserLineState(user_id, event->type() == CustomEvent_RecvUserOnLine ? true : false);
         }
      }
      else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         if (mTeacherRender) {
            mTeacherRender->HandleOpenClassUserLineState(user_id, event->type() == CustomEvent_RecvUserOnLine ? true : false);
         }
      }
      //}
   }
}

void WebRtcLiveWdg::HandleStartDoubleVrtc(QEvent* event)
{
   DoubleVrtcUserEvent *VrtcUserEvent = dynamic_cast<DoubleVrtcUserEvent*>(event);
   if (VrtcUserEvent) {
      QString joinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
      if (VrtcUserEvent->userId.compare(joinId) == 0)//�Լ�
      {

         FadeOutTip(Str_SelfAlreadyDoubleVrtc, Tost_Message);
      }
      else//�����Լ�
      {
         if (VrtcUserEvent->nickName.trimmed().size() > 0) {
				mDoublePushNickName = VrtcUserEvent->nickName;
         }
         else {
				mDoublePushNickName = VrtcUserEvent->userId;
			}
	/*		if (nullptr != ui.mpVhallRenderMaskWdg)
			{
            ui.mpVhallRenderMaskWdg->SetUserName(mDoublePushNickName);
			}
*/
			FadeOutTip(mDoublePushNickName + Str_SomeOneAlreadyDoubleVrtc, Tost_Message);
      }
   }
}

void WebRtcLiveWdg::HandleinviteDoubleVrtc(QEvent*event)
{
   DoubleVrtcUserEvent *VrtcUserEvent = dynamic_cast<DoubleVrtcUserEvent*>(event);
   if (VrtcUserEvent) {
      QString joinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
      if (VrtcUserEvent->userId.compare(joinId) == 0)//�Լ�
      {
         slot_CameraDoublePush(true);
      }
      else//�����Լ�
      {
         //FadeOutTip(VrtcUserEvent->nickName + Str_SelfAlreadyDoubleVrtc, message);
      }
   }
}

void WebRtcLiveWdg::HandleInviteScreenSharing(QEvent* event)
{
   DoubleVrtcUserEvent *VrtcUserEvent = dynamic_cast<DoubleVrtcUserEvent*>(event);
   if (VrtcUserEvent) {
      if (nullptr == mLeftToolBarWdg) {
         InitToolWdg();
      }

      if (nullptr != mLeftToolBarWdg) {
         if (VrtcUserEvent->userId.compare( QString::fromStdString( globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id ) ) == 0) {//
            mLeftToolBarWdg->SetShareType(eDestopType_Desktop);
            mLeftToolBarWdg->StartOvertimeTimer();
            mLeftToolBarWdg->slots_ScreenSharing();
         }
         else if(JOIN_ROLE_Teacher !=globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type){
            slot_BtnStopScreenClicked();
         }
      }

   }
}


void  WebRtcLiveWdg::HandleStopDesktopSharingVrtc(QEvent* event)
{
   DoubleVrtcUserEvent *VrtcUserEvent = dynamic_cast<DoubleVrtcUserEvent*>(event);
   if (VrtcUserEvent) {
         if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop)) {
            slot_BtnStopScreenClicked();
         }

         //if (nullptr != mLeftToolBarWdg) {
         //   mLeftToolBarWdg->closeScreenChoiceDlg();
         //}
   }
}

void  WebRtcLiveWdg::HandleWhiteListUserJoinRoom(QEvent* event)
{
	globalToolManager->GetClassSDK()->LeaveClassRoom();
	ui.classRoomTitleWd->hideMediaSet();
	ui.label_kickout->setText(QString::fromLocal8Bit("�����˺���ص�¼!"));
	setCurrentIndex(page_kickOut);
	ui.widget_right_tool->hide();
	if (mRenderManager) {
		mRenderManager->hide();
	}
	globalToolManager->GetWndManager()->DestoryExistWnd(WND_ID_WEBRTC_MAIN);
	ui.classRoomTitleWd->UpdateStartClassState(false);
	//���� ���������� false
	//CBaseWnd* wnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_SETTING);
	CBaseWnd* wnd = nullptr;
	CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	if (pWdg)
	{
		WebRtcLiveWdg* pMainWdg = (WebRtcLiveWdg*)pWdg;
		wnd = pMainWdg->GetPtrVhallIALiveSettingDlg();
	}

}

void  WebRtcLiveWdg::HandleEndDoubleVrtc(QEvent* event)
{
   DoubleVrtcUserEvent *VrtcUserEvent = dynamic_cast<DoubleVrtcUserEvent*>(event);
   if (VrtcUserEvent) {
      QString joinId = QString::fromStdString(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
      if (VrtcUserEvent->userId.compare(joinId) == 0)//�Լ�
      {
         bool bIsPushing = globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture);
         if (bIsPushing){
            slot_CameraDoublePush(false);
         }
         else if(globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Desktop)){
            slot_BtnStopScreenClicked();
         }
         
         if (nullptr !=mLeftToolBarWdg) {
            mLeftToolBarWdg->closeScreenChoiceDlg();
         }


      }
      else//�����Լ�
      {
      }
   }
}

void WebRtcLiveWdg::initLeftToolBarOptype()
{
   if (nullptr != mLeftToolBarWdg) {
      mLeftToolBarWdg->SetOpType((LeftToolBarWdg::OpType)mLeftToolBarWdg->CurOptype());
   }
}

void WebRtcLiveWdg::HandleDoubleVrtUsers(QEvent* event)
{
   QEventRoomEvent* msgEvent = dynamic_cast<QEventRoomEvent*>(event);
   if (msgEvent) {
      //FadeOutTip(QString::fromStdString(msgEvent->mRespMsg), Tost_Error);
      initLeftToolBarOptype();
   }
}


void WebRtcLiveWdg::HandlePushLocalStreamSuc(QEvent* event) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   QEventStream *msg = dynamic_cast<QEventStream*>(event);
   if (msg) {
      if (msg->mStreamType <= VHStreamType_AVCapture) {
         HandlePushCameraStreamSuc(event);
      }
      else if (msg->mStreamType == VHStreamType_Desktop 
         || msg->mStreamType == VHStreamType_SoftWare 
         || msg->mStreamType == VHStreamType_MediaFile 
         || VHStreamType_Stu_Desktop==msg->mStreamType) {
         HandlePushDesktopStreamSuc(event);
      }
      else if (msg->mStreamType == VHStreamType_Auxiliary_CamerCapture) {
         HandlePushAuxiliaryCameraStreamSuc(event);
      }
      else if (msg->mStreamType == VHStreamType_Stu_Desktop) {
         ui.invStateWdg->SetStateType(InvitationStateWdg::eStateType_DesktopSharing);
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void WebRtcLiveWdg::HandlePushAuxiliaryCameraStreamSuc(QEvent* event) {
   QEventStream *msg = dynamic_cast<QEventStream*>(event);
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (msg && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mLocalAuxiliaryStreamId = QString::fromStdString(msg->strStream);
      std::string stream_id = msg->strStream;
      ToolManager *callbackTool = globalToolManager;
		int layout = baseInfo.mWebinarInfo.videoPushAndMixSetting.share_desktop_mixed_flow_layout == 0 ? CANVAS_LAYOUT_PATTERN_GRID_1 : CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
      globalToolManager->GetClassSDK()->SetConfigBroadCastLayOut((LayoutMode(layout)), [&, callbackTool, stream_id](const std::string& result, const std::string& msg)->void {
         if (result == "success") {
            callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomRecvMsgEvent(CustomEvent_SetLayoutCameraAuxiliary, QString::fromStdString(stream_id)));
         }
      });
   }
}

void WebRtcLiveWdg::HandlePushCameraStreamSuc(QEvent* event) {
   QEventStream *msg = dynamic_cast<QEventStream*>(event);
   if (msg) {
      if (msg->mStreamType <= VHStreamType_AVCapture) {
		  globalToolManager->GetDataManager()->WriteLog("%s  enter", __FUNCTION__);
         mLocalStream = QString::fromStdString(msg->strStream);
         globalToolManager->GetDataManager()->SetPublished(true);

         //std::list<std::string>::iterator itorRemo = mListRemoteStream.begin();
         /*std::string strId;
         while (itorRemo != mListRemoteStream.end()) {
            strId = *itorRemo++;
            mListRemoteStream.remove(strId);
            globalToolManager->GetClassSDK()->SubScribeRemoteStream(strId);
         }*/

         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
            if (baseInfo.mWebinarInfo.type != CLASS_STATUS_IN_CLASS) {

               QString msConfPath = globalToolManager->GetConfigPath();
               int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
               globalToolManager->GetDataManager()->WriteLog("%s iQualityId %d", __FUNCTION__, iQualityId);
               globalToolManager->GetClassSDK()->StartClass(iQualityId);
            }
            else {
               ui.classRoomTitleWd->SetStartClassBtnEnable(true);
               mLeftToolBarWdg->setEnabled(true);
            }
            if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
               //����������ʱҲ��Ҫ���ô��档
               if (mMixStreamLayout.GetCurrentLayOut() == CANVAS_LAYOUT_PATTERN_GRID_1) {
                  mMixStreamLayout.SetMainViewStreamID(mLocalStream);
               }
               else {
                  mMixStreamLayout.SetMainViewStream(mLocalStream);
               }
            }
            else {
               globalToolManager->GetDataManager()->WriteLog("%s CLASS_STATUS_IN_CLASS Abnormal,Do Not mMixStreamLayout.SetMainViewStream ", __FUNCTION__);
            }

            if (mTeacherRender) {
               mTeacherRender->SetMicState(msg->mbHasAudio);
               mTeacherRender->SetCameraState(msg->mbHasVideo);
               mTeacherRender->SetStreamHasAudioFormat(msg->mbHasAudio);
               mTeacherRender->SetStreamHasVideoFormat(msg->mbHasVideo);
               mTeacherRender->SetStreamID(QString::fromStdString(msg->strStream));
            }
            if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO 
               && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
               && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
               && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
               && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Stu_Desktop)
               ) {
               setCurrentIndex(page_CenterRender);
            }
         }
         else {//ѧԱ
            //С���
            if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
               if (nullptr != mRenderManager) {
                  mRenderManager->AddLocalPublishUser(QString::fromStdString(baseInfo.mUserInfo.join_id), QString::fromStdString(msg->strStream), QString::fromStdString(baseInfo.mUserInfo.nick_name), baseInfo.mUserInfo.role_type, msg->mbHasVideo, msg->mbHasAudio);
               }

               JOIN_ROLE role = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
               bool isSpeaking = globalToolManager->GetDataManager()->GetIsSpeaking();
               globalToolManager->GetDataManager()->WriteLog("%s 6207 %d", __FUNCTION__, isSpeaking);

               if (JOIN_ROLE_Student== role && !isSpeaking) {//ѧԱ��û������


                  ChangePublishState(true);
                  globalToolManager->GetDataManager()->SetPublished(true);
                  if (mRenderManager) {
                     mRenderManager->show();
                     globalToolManager->GetDataManager()->WriteLog("%s mRenderManager->show()", __FUNCTION__);
                     mRenderManager->ShowAllRender();
                  }

                  globalToolManager->GetDataManager()->WriteLog("%s  UserPublishCallback(PushStreamEvent_Publish) 6210 ", __FUNCTION__);
                  globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_Publish);
                  //globalToolManager->GetClassSDK()->EnableSubScribeStream();
               }
               else if (JOIN_ROLE_Student == role && isSpeaking) {
                  ChangePublishState(true);
                  globalToolManager->GetDataManager()->SetPublished(true);
                  globalToolManager->GetDataManager()->SetIsSpeaking(true);
               }
               else if (JOIN_ROLE_Teacher == role){//��ʦ
                  globalToolManager->GetDataManager()->WriteLog("%s  UserPublishCallback(PushStreamEvent_Publish) 6220 ", __FUNCTION__);
                  globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_Publish);
               }
			   else {
				   globalToolManager->GetDataManager()->WriteLog("%s   5827  CLASS_TYPE_SMALL not UserPublishCallback", __FUNCTION__);
			   }
               

               ui.classRoomTitleWd->SetStudentHandsUp(true);
               if (mTeacherRender && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  mTeacherRender->ShowAllMuteCtrl(true);
               }
            }
            else {//����
               if (mTeacherRender) {
                  mTeacherRender->SetShowSmallRender(true);
                  mTeacherRender->SetSmallRenderUserPublish(true);
                  mTeacherRender->SetSmallRenderFlushEnable(false);
                  mTeacherRender->SetSmallWndUserJoinID(QString::fromStdString(baseInfo.mUserInfo.join_id));
                  mTeacherRender->SetSmallWndStreamID(QString::fromStdString(msg->strStream));
                  mTeacherRender->SetSmallRenderRole(baseInfo.mUserInfo.role_type);
                  mTeacherRender->SetSmallRenderNickName(QString::fromStdString(baseInfo.mUserInfo.nick_name));
                  mTeacherRender->SetSmallRenderCameraState(msg->mbHasVideo);
                  mTeacherRender->SetSmallRenderMicState(msg->mbHasAudio);
                  mTeacherRender->SetSmallWndStreamHasAudioFormat(msg->mbHasAudio);
                  mTeacherRender->SetSmallWndStreamHasVideoFormat(msg->mbHasVideo);
                  if (msg->mbHasVideo) {
                     mTeacherRender->SetPageIndex(TeacherRenderWdg_Page_Index_render);
                  } else {
                     mTeacherRender->SetPageIndex(TeacherRenderWdg_Page_Index_NoCamera);
                  }
                  //HWND wnd_id = nullptr;
                  std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver;
                  if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
                     //wnd_id = (HWND)(mTeacherRender->GetRenderWndID());
                     Receiver = mTeacherRender->GetRenderWnd();
                  }
                  else {
                     //wnd_id = (HWND)(mTeacherRender->GetSmallRenderRenderWndID());
                     Receiver = mTeacherRender->GetSmallRenderRenderWnd();
                  }
                  VHStreamType  iStreamType = globalToolManager->GetClassSDK()->CalcStreamType(msg->mbHasAudio, msg->mbHasVideo);
                  //globalToolManager->GetClassSDK()->StartRenderLocalStream(iStreamType, (void*)(wnd_id));
                  globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(iStreamType, Receiver);
                  ChangePublishState(true);
               }
			   

               JOIN_ROLE role = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
               bool isSpeaking = globalToolManager->GetDataManager()->GetIsSpeaking();
               if (JOIN_ROLE_Student == role && !isSpeaking) {//ѧԱ��û������
                  globalToolManager->GetDataManager()->WriteLog("%s  UserPublishCallback(PushStreamEvent_Publish)  6268 CLASS_TYPE_MULTI not UserPublishCallback", __FUNCTION__);
                  globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_Publish);
               }
               else  if (JOIN_ROLE_Teacher == role){//��ʦ
                  globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_Publish)   6272 CLASS_TYPE_MULTI not UserPublishCallback", __FUNCTION__);
                  globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_Publish);
               }
			   else {
				   globalToolManager->GetDataManager()->WriteLog("%s   5880 CLASS_TYPE_MULTI not UserPublishCallback", __FUNCTION__);
			   }

               ui.classRoomTitleWd->SetStudentHandsUp(true);
            }
         }
      }
   }
}

void WebRtcLiveWdg::SetDesktopMeidaMixStreamLayout(VHStreamType streamType)
{
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   int layout = baseInfo.mWebinarInfo.videoPushAndMixSetting.share_desktop_mixed_flow_layout == 0 ? CANVAS_LAYOUT_PATTERN_GRID_1 : CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
   //���ݺ�̨�·��Ĳ����ж����湲��ʱ�Ƿ���Ҫ����������ʾ�����˻��档
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && streamType == VHStreamType_Desktop) {
      //�����θ������ö�ȡ�������͡�
      //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
      QString msConfPath = globalToolManager->GetConfigPath();
      int desktop_layout = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_DESKTOP_SHARE_LAYOUT, ShareScreenLayout_on_camera);
      if (desktop_layout == ShareScreenLayout_camera_top) {
         layout = CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
      }
      else {
         layout = CANVAS_LAYOUT_PATTERN_GRID_1;
      }
   }

   if (streamType == VHStreamType_MediaFile) {
      layout = CANVAS_LAYOUT_PATTERN_GRID_1;
   }
   //layout = CANVAS_LAYOUT_PATTERN_GRID_2_H;

   //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mMixStreamLayout.SetMixStreamLayout(true, layout, streamType == VHStreamType_Desktop ? true : false);
   }
   globalToolManager->GetDataManager()->WriteLog("%s layout:%d", __FUNCTION__, layout);
}

void WebRtcLiveWdg::HandlePushDesktopStreamSuc(QEvent* event) {
   QEventStream *msg = dynamic_cast<QEventStream*>(event);
   if (msg) {
      mDeskTopStream = QString::fromStdString(msg->strStream);
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
         SetDesktopMeidaMixStreamLayout((VHStreamType)msg->mStreamType);
      }
      globalToolManager->GetClassSDK()->DesktopShareNotice(true,globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
   }
}

void WebRtcLiveWdg::HandleLocalStreamIdChanged(QEvent *event) {
   QEventRoomEvent *msg = dynamic_cast<QEventRoomEvent*>(event);
   if (msg) {
      if (msg->mProcessType <= VHStreamType_AVCapture) {
         mLocalStream = msg->mUserData;
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         QString userId;
         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               if (mTeacherRender) {
                  mTeacherRender->SetStreamID(mLocalStream);
               }
            }
            else {
               if (mRenderManager) {
                  mRenderManager->UpdateLocalStreamID(QString::fromStdString(baseInfo.mUserInfo.join_id), mLocalStream);
               }
            }
            userId = QString::fromStdString(baseInfo.mUserInfo.join_id);
            //QString wndId = WND_ID_FLOAT_RENDER + userId;
            CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
            WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
            if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)/*globalToolManager->GetWndManager()->IsWndExist(wndId)*/) {
               //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(wndId);
               /*FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
               CSTDMapParam param;
               param[LOCAL_STREAM_ID_CHANGED] = "";
               param[STREAM_ID] = mLocalStream;
               param[USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
               wnd->Notice(param);*/
            }

            //QString FullWndId = WND_ID_FULL_SCREEN_RENDER + userId;
            //if (globalToolManager->GetWndManager()->IsWndExist(FullWndId)) {
            //   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(FullWndId);
            //   CSTDMapParam param;
            //   param[LOCAL_STREAM_ID_CHANGED] = "";
            //   param[STREAM_ID] = mLocalStream;
            //   param[USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
            //   wnd->Notice(param);
            //}
         }
         else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               if (mTeacherRender) {
                  mTeacherRender->SetStreamID(mLocalStream);
                  HWND id = mTeacherRender->GetRenderWndID();
               }
               userId = QString::fromStdString(baseInfo.mUserInfo.join_id);

               //QString wndId = WND_ID_FLOAT_RENDER + userId;
               CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
               WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;

               if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)
                  /*globalToolManager->GetWndManager()->IsWndExist(wndId)*/) {
                  //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(wndId);
                  /*FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
                  CSTDMapParam param;
                  param[LOCAL_STREAM_ID_CHANGED] = "";
                  param[STREAM_ID] = mLocalStream;
                  param[USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
                  wnd->Notice(param);*/
               }
               //QString FullWndId = WND_ID_FULL_SCREEN_RENDER + userId;
               //if (globalToolManager->GetWndManager()->IsWndExist(FullWndId)) {
               //   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(FullWndId);
               //   CSTDMapParam param;
               //   param[LOCAL_STREAM_ID_CHANGED] = "";
               //   param[STREAM_ID] = mLocalStream;
               //   param[USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
               //   wnd->Notice(param);
               //}
            }
            else {
               if (mTeacherRender) {
                  mTeacherRender->SetSmallWndStreamID(mLocalStream);
                  //userId = mTeacherRender->GetUserJoinID();
                  //QString wndId = WND_ID_FLOAT_RENDER + userId;

                  /*CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
                  WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;*/
                  //if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)
                  //   /*globalToolManager->GetWndManager()->IsWndExist(wndId)*/) {
                  //   //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(wndId);
                  //   FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
                  //   CSTDMapParam param;
                  //   param[LOCAL_STREAM_ID_CHANGED] = "";
                  //   param[SMALL_RENDER_STEAM_ID] = mLocalStream;
                  //   param[SMALL_RENDER_USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
                  //   wnd->Notice(param);
                  //}
                  //QString FullWndId = WND_ID_FULL_SCREEN_RENDER + userId;
                  //if (globalToolManager->GetWndManager()->IsWndExist(FullWndId)) {
                  //   CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(FullWndId);
                  //   CSTDMapParam param;
                  //   param[LOCAL_STREAM_ID_CHANGED] = "";
                  //   param[SMALL_RENDER_STEAM_ID] = mLocalStream;
                  //   param[SMALL_RENDER_USER_ID] = QString::fromStdString(baseInfo.mUserInfo.join_id);
                  //   wnd->Notice(param);
                  //}
               }
            }
         }
      }
   }
}

void WebRtcLiveWdg::HandlePushLocalStreamError(QEvent* event) {
   QEventStreamError* errorMsg = dynamic_cast<QEventStreamError*>(event);
   if (errorMsg) {
      if (errorMsg->mStreamType <= VHStreamType_AVCapture) {
         if (errorMsg->mstreamId.compare(globalToolManager->GetClassSDK()->GetLocalAuxiliaryId()) != 0
            && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type != JOIN_ROLE_Teacher) {//��˫������ͷ
            globalToolManager->GetDataManager()->WriteLog("%s DisConnectRoom(%s) ", __FUNCTION__, globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id.c_str());
            DisConnectRoom(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
         }
         else{
            FadeOutTip(Str_LocalStreamPushErr, Tost_Error);
            globalToolManager->GetDataManager()->WriteLog("%s mMixStreamLayout.RePushLocalStream() ",__FUNCTION__);
            mMixStreamLayout.RePushLocalStream();
            ui.classRoomTitleWd->SetStartClassBtnEnable(true);
         }
      }
      else if (errorMsg->mStreamType == VHStreamType_Desktop) {
         globalToolManager->GetDataManager()->WriteLog("%s mMixStreamLayout.RePushLocalStream(%d) ", __FUNCTION__, errorMsg->mStreamType);
         mMixStreamLayout.RePushDesktopSream(errorMsg->mStreamType);
      }
      else if (errorMsg->mStreamType == VHStreamType_MediaFile) {
         globalToolManager->GetDataManager()->WriteLog("%s mMixStreamLayout.RePushLocalStream(%d) ", __FUNCTION__, errorMsg->mStreamType);
         mMixStreamLayout.RePushDesktopSream(errorMsg->mStreamType);
      }
      else if (errorMsg->mStreamType == VHStreamType_SoftWare) {
         FadeOutTip(Str_SourceStreamPushErr, Tost_Error);
         globalToolManager->GetDataManager()->WriteLog("%s mMixStreamLayout.RePushLocalStream(%d) ", __FUNCTION__, errorMsg->mStreamType);
         mMixStreamLayout.RePushDesktopSream(errorMsg->mStreamType);
      }
   }
}

void WebRtcLiveWdg::HandleStartLocalCapture() {
   //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
   QString configCameraId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, "");
   QString configMicId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, "");

   bool findConfigMic = false;
   std::wstring fristMicId;
   std::list<vhall::AudioDevProperty> MicList;
   globalToolManager->GetClassSDK()->GetMicDevices(MicList);
   std::list<vhall::AudioDevProperty>::iterator iterMic = MicList.begin();
   int index = -1;
   while (iterMic != MicList.end()) {//MIC List
      vhall::AudioDevProperty micInfo = (*iterMic);
      if (iterMic == MicList.begin()) {
         fristMicId = micInfo.mDevGuid;
      }
      if (CPathManager::qToStdWString(configMicId) == micInfo.mDevGuid) {
         index = micInfo.mIndex;
         findConfigMic = true;
         break;
      }
      iterMic++;
   }

   bool findConfigCamera = false;
   std::string fristCameraId;
   std::list<vhall::VideoDevProperty> CameraList;
   globalToolManager->GetClassSDK()->GetCameraDevices(CameraList);
   std::list<vhall::VideoDevProperty>::iterator iterCamera = CameraList.begin();
   while (iterCamera != CameraList.end()) {//camera list
      vhall::VideoDevProperty videoInfo = (*iterCamera);
      if (iterCamera == CameraList.begin()) {
         fristCameraId = videoInfo.mDevId;
      }
      if (configCameraId.toStdString() == videoInfo.mDevId) {
         findConfigCamera = true;
         break;
      }
      iterCamera++;
   }

   globalToolManager->GetDataManager()->WriteLog("%s camera count:%d mic count:%d", __FUNCTION__, CameraList.size() > 0, MicList.size());
   if (CameraList.size() > 0 || MicList.size() > 0) {
      int nRet = 0;
      //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	  QString msConfPath = globalToolManager->GetConfigPath();
 /*     QString configDefinition = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
      int interactiveVideoProfile = 360;
      int mixLayoutProfile = 720;
      globalToolManager->GetVideoProfileByConfig(configDefinition, interactiveVideoProfile, mixLayoutProfile);
      VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(interactiveVideoProfile);*/

      int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
      globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iQualityId);

      QString confPicPaht = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, PUSH_PIC_PATH, "");
      int openPic = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, PUSH_PIC_OPEN, 0);


      if (findConfigMic)
      {
         globalToolManager->GetClassSDK()->SetUsedMic(index, configMicId.toLatin1().data(), L"");
         globalToolManager->GetClassSDK()->SetCurrentMicVol(100);
         ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_MIC_ID, configMicId);
      }
      
      if (openPic) {
         if (globalToolManager->GetClassSDK()->CheckPicEffectiveness(confPicPaht.toStdString()) == 0) {
            globalToolManager->GetClassSDK()->StartLocalCapturePicture(confPicPaht.toStdString(), RTC_VIDEO_PROFILE_480P_4x3_L);
         }
         else {
            QString defaultPic = QApplication::applicationDirPath() + "/" + QString::fromStdString("default_pushPic.png");
            globalToolManager->GetClassSDK()->StartLocalCapturePicture(defaultPic.toStdString(), RTC_VIDEO_PROFILE_480P_4x3_L);
         }

         globalToolManager->GetDataManager()->WriteLog("StartLocalCapturePicture %s", confPicPaht.toStdString().c_str());
      }
      else {
         int iVioeProFile = 360;
         globalToolManager->GetVideoProfileByConfig(iQualityId, iVioeProFile, ToolManager::eVideoProFileTypeVideo);

         VideoProfileIndex videoIndex = globalToolManager->GetClassSDK()->GetVideoProfileByWebinarInfo(iVioeProFile);

         if (findConfigCamera) {
            nRet = globalToolManager->GetClassSDK()->StartLocalCapture(configCameraId.toStdString(), videoIndex);
            ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, configCameraId);
         }
         else if (fristCameraId.length() > 0 || fristMicId.length() >= 0) {
            nRet = globalToolManager->GetClassSDK()->StartLocalCapture(fristCameraId, videoIndex);
            ConfigSetting::writeValue(msConfPath, GROUP_DEFAULT, CAPTURE_CAMERA_ID, QString::fromStdString(fristCameraId));
         }

         if (nRet != 0) {
            ui.classRoomTitleWd->SetStartClassBtnEnable(true);
            FadeOutTip(QStringLiteral("��������Ƶ�豸ʧ��"), Tost_Error);
         }
      }
   }
   else
   {
      ui.classRoomTitleWd->SetStartClassBtnEnable(true);
   }
}

void WebRtcLiveWdg::slot_NoticeShowMsgTimeout() {
   QPoint stackedWidgetpos = ui.tabWidget->mapToGlobal(QPoint(0, 0 + 40));
   if (mNoticeShowWdg && !mNoticeShowWdg->isHidden()) {
      mNoticeShowWdg->setFixedWidth(ui.tabWidget->width());
	  ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	  if (JOIN_ROLE_Student == baseInfo.mUserInfo.role_type) {
		  mNoticeShowWdg->move(stackedWidgetpos.x(), stackedWidgetpos.y());
	  }
	  else {
		  mNoticeShowWdg->move(0, ui.tabWidget->GetTabHeight());
	  }
      
      mNoticeShowWdg->show();
      //mNoticeShowWdg->raise();
   }
   else {
      mNoticeShowMsgTimer.stop();
   }
}

bool WebRtcLiveWdg::slot_CloseClicked() {
   bool bRef = false;
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
      TipsWdg* tips = new TipsWdg();
      if (tips) {
         CSTDMapParam param;
         param[TipsMsg] = QStringLiteral("�Ƿ�ȷ���¿β��˳����ÿͻ���?");
         param[ShowCancelButton] = PARAM_TRUE;
         if (ui.classRoomTitleWd->GetStartTime() > 30) {
            param[ShowRecordSet] = "1";
         }
         int createDefaultRecord = 0;
         tips->Notice(param);
         int accept = tips->exec();
         if (accept == QDialog::Accepted) {
            globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
            tips->GetParam(param);
            delete tips;
            tips = NULL;
				CBaseWnd* AnswerWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_ANSWER_TOOL);
				if (AnswerWdg) {
					globalToolManager->GetWndManager()->DestoryWnd(WND_ID_ANSWER_TOOL);
				}
				
            std::map<QString, QString>::iterator iterShowRecordSet = param.find(QString::fromStdString(ShowRecordSet));
            if (iterShowRecordSet != param.end()) {
               createDefaultRecord = iterShowRecordSet->second.toInt() == Qt::CheckState::Checked ? 1 : 0;
            }

            globalToolManager->GetClassSDK()->StopClass(createDefaultRecord);
			mbQuitAfterUpPublish = true;

            bRef = true;
         }
         else {
            delete tips;
            tips = NULL;
         }
      }
   }
   else {//ѧԱ�˳�����
      //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND, "close");
      TipsWdg* tips = new TipsWdg();
      if (tips) {
         CSTDMapParam param;
         param[TipsMsg] = Str_IsExitVhallClassApp;
         param[ShowCancelButton] = PARAM_TRUE;
         tips->Notice(param);
         int accept = tips->exec();
         if (accept == QDialog::Accepted) {
            //ѧԱ�˳�ʱҪ����������Ϣ��
            if (globalToolManager->GetDataManager()->IsAlreadyPublished()) {//������
				mbQuitAfterUpPublish = true;
               globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish) 6635 PushStreamEvent_UnPublish", __FUNCTION__);
               globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish);
            }
			else {
				QuitLiveWdg();
			}

            bRef = true;
         }
      }
   }
   return bRef;
}

void WebRtcLiveWdg::slot_MinClicked() {
   showMinimized();
}

void WebRtcLiveWdg::slot_TurnToLogin() {
   globalToolManager->LogOut();
}

void WebRtcLiveWdg::slot_MaxClicked() {
   //if (mbIsMaxSize) {
   //   this->setGeometry(mNormalRect);
   //   mbIsMaxSize = false;
   //   int moveX = (QApplication::desktop()->availableGeometry().width() - mNormalRect.width()) / 2;
   //   int moveY = (QApplication::desktop()->availableGeometry().height() - mNormalRect.height()) / 2;
   //   move(moveX, moveY);
   //}
   //else {
   //   this->setGeometry(QApplication::desktop()->availableGeometry());
   //   mbIsMaxSize = true;
   //}

   if (mbIsMaxSize) {
      
      this->setGeometry(mNormalRect);
      mbIsMaxSize = false;
      int moveX = (QApplication::desktop()->availableGeometry().width() - mNormalRect.width()) / 2;
      int moveY = (QApplication::desktop()->availableGeometry().height() - mNormalRect.height()) / 2;
      move(moveX, moveY);

	  if (mLeftToolBarWdg)
	  {
		  AnswerWebView* wnd = mLeftToolBarWdg->AnswerWebViewDlg();
		  //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
		  if (wnd && wnd->isVisible()) {
			  //wnd->move((this->width() - wnd->width()) / 2 + this->x(), (this->height() - wnd->height()) / 2 + this->y());
		  }
	  }
   }
   else {
      QDesktopWidget * deskTop = QApplication::desktop();
      int curMonitor = deskTop->screenNumber(this); // ������һ��QWidget*
      QRect DesktopRect = deskTop->screenGeometry(curMonitor);

      QRect rect = deskTop->availableGeometry(curMonitor);
      //globalToolManager->GetDataManager()->WriteLog("DesktopRect:%d,%d,%d,%d    Rect:%d,%d,%d,%d    ",
      //   DesktopRect.x(), DesktopRect.y(), DesktopRect.width(), DesktopRect.height(),
      //   rect.x(), rect.y(), rect.width(), rect.height());
      if (rect.height() < DesktopRect.height() || rect.width() < DesktopRect.width()) {
         this->setGeometry(rect);
      }
      else {
         this->setGeometry(rect.x(), rect.y(), rect.width(), rect.height() - 1);
      }

      mbIsMaxSize = true;

   }
   ui.classRoomTitleWd->setMaxState(mbIsMaxSize);
}

void WebRtcLiveWdg::slot_btnDoc() {
   if (mstrIDPenAuth.isEmpty()) {
      if (mLeftToolBarWdg && (mLeftToolBarWdg->isMediaPlayExist()/*||globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/)) {
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
         FadeOutTip(Str_PleaseStopMedia, Tost_Warning);
         return;
      }

      if (mDocCefViewWdg) {
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
            setCurrentIndex(page_Doc);
            if (JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)
               globalToolManager->GetClassSDK()->ChangeToToolPage(ToolPage_Doc);
         }
         else if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
            FadeOutTip(Str_PleaseStartClass, Tost_Warning);
         }
      }
      
   }
   else {
      FadeOutTip(Str_PleaseBackPenAuth, Tost_Warning);
   }
}

void WebRtcLiveWdg::slot_Whiteboard(bool) {
   if (mstrIDPenAuth.isEmpty()) {

      if (mLeftToolBarWdg->isMediaPlayExist()/*||globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/) {
         mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType_NULL);
         FadeOutTip(Str_PleaseStopMedia, Tost_Warning);
         return;
      }
      if (mWhiteBoardCefViewWdg) {
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
            setCurrentIndex(page_Whiteboard);
            if (JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type)
               globalToolManager->GetClassSDK()->ChangeToToolPage(ToolPage_Board);
            if (!mbClickedWhiteBoard) {
               mbClickedWhiteBoard = true;
               std::string board = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.board;
               int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
               QString loadUrl = QString::fromStdString(board) + QString("&role_type=%1").arg(roleType);
               mWhiteBoardCefViewWdg->LoadUrl(loadUrl);
            }
         }
         else {
            FadeOutTip(Str_PleaseStartClass, Tost_Warning);
         }
      }
   }
   else {
      FadeOutTip(Str_PleaseBackPenAuth, Tost_Warning);
   }
}

void WebRtcLiveWdg::slot_Answer() {
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type != CLASS_STATUS_IN_CLASS
      && globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      mLeftToolBarWdg->SetBtnAnswerEnabled(true);
      //mLeftToolBarWdg->setAnserChecked(false);
      FadeOutTip(Str_PleaseStartClass, Tost_Warning);
      return;
   }

   if (mLeftToolBarWdg->isMediaPlayExist()/*||globalToolManager->GetWndManager()->IsWndExist(WND_ID_MEDIA_PLAY_WEBVIEW)*/) {
      //mLeftToolBarWdg->AnserDlgHide();
      FadeOutTip(Str_PleaseStopMedia, Tost_Warning);
      return;
   }

   //  if (m_pScreenShareToolWgd && !m_pScreenShareToolWgd->isHidden()) {
       //mLeftToolBarWdg->AnserDlgHide();
   //      FadeOutTip(Str_PleaseStopDesktop);
   //      return;
   //  }

   if (mLeftToolBarWdg)
   {
      AnswerWebView* wnd = mLeftToolBarWdg->AnswerWebViewDlg();
      //CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_QUESTION_UI);
      if (wnd) {
         CSTDMapParam param;
         wnd->Notice(param);
         if (nullptr != mLeftToolBarWdg)
            mLeftToolBarWdg->setMutexItemChecked(LeftToolBarWdg::OpType_Anser);

         //wnd->move((this->width() - wnd->width()) / 2 + this->x(), (this->height() - wnd->height()) / 2 + this->y());
         wnd->show();
         //mLeftToolBarWdg->SetOpType(LeftToolBarWdg::OpType::OpType_Anser);
      }
   }
}

//void WebRtcLiveWdg::slot_selectChatPage() {
//    ui.stackedWidget->setCurrentIndex(page_chat);
//    ui.pushButton_chatPage->setStyleSheet(SELECT_STYLE);
//    ui.pushButton_memberListPage->setStyleSheet(DEFAULT_STYLE);
//    ui.pushButton_noticePage->setStyleSheet(DEFAULT_STYLE);
//}
//
//void WebRtcLiveWdg::slot_selectMemberPage() {
//	ui.pushButton_memberListPage->SetHandsUpState(eHandsUpState_No);
//    ui.stackedWidget->setCurrentIndex(page_memberList);
//    ui.pushButton_memberListPage->setStyleSheet(SELECT_STYLE);
//    ui.pushButton_chatPage->setStyleSheet(DEFAULT_STYLE);
//    ui.pushButton_noticePage->setStyleSheet(DEFAULT_STYLE);
//
//}
//
//void WebRtcLiveWdg::slot_NoticePage() {
//    ui.stackedWidget->setCurrentIndex(page_notice);
//    ui.pushButton_noticePage->setStyleSheet(SELECT_STYLE);
//    ui.pushButton_chatPage->setStyleSheet(DEFAULT_STYLE);
//    ui.pushButton_memberListPage->setStyleSheet(DEFAULT_STYLE);
//}

void WebRtcLiveWdg::slot_interactionItemClicked(const int& iOpType)
{
   //�����µ�������ر�ʱ��  Ҫ�����ź�  ȥ����  LeftToolBarWdg::slot_MutexWdgClose
   switch (iOpType)
   {
      //case LeftToolBarWdg::InterActionOpType_QUESANDANS: {}
      //	break;
      //case LeftToolBarWdg::InterActionOpType_TESTPAPER: {}
      //	  break;
   case LeftToolBarWdg::InterActionOpType_SIGNIN:
      //ManageSignInDlg();
      break;
      //case LeftToolBarWdg::InterActionOpType_ANSWER: {}
      //	  break;
   case LeftToolBarWdg::InterActionOpType_TIMER:
      showTimeCountDown();
      break;
   default:
      break;
   }
}

void WebRtcLiveWdg::slotStopReDoublePush()
{
   if (InvitationStateWdg::eStateType_OverTime == ui.invStateWdg->StateType()
      || InvitationStateWdg::eStateType_ReleasWheat == ui.invStateWdg->StateType()
      || InvitationStateWdg::eStateType_ReleasVrtc == ui.invStateWdg->StateType()
      ) {
      slot_showInitPage();
   }
   
}

void WebRtcLiveWdg::slot_DocLoadFinished() {
   //["cmd","{"join_id":"69640","nick_name":"626792","online":[],"role":1,"type":"*publishStart"}"]

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {

      QJsonObject valueObj;
      QJsonArray jsonArray;
      valueObj["type"] = "*publishStart";
      valueObj["join_uid"] = QString::fromStdString(baseInfo.mUserInfo.join_id);
      valueObj["nick_name"] = QString::fromStdString(baseInfo.mUserInfo.nick_name);


      jsonArray.append("cmd");
      jsonArray.append(valueObj);

      QJsonDocument document;
      document.setArray(jsonArray);
      QByteArray byteArray = document.toJson(QJsonDocument::Compact);

      QString base64EncodeData = byteArray.toBase64();
      QString method = QString("msgByClient('%1')").arg(base64EncodeData);
      if (mDocCefViewWdg) {
         if (eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
            mDocCefViewWdg->AppCallJsFunction(method);
         mDocCefViewWdg->setEnabled(true);
      }
      if (mWhiteBoardCefViewWdg) {
         if (eClassPlayType_Flash == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().play_type)
            mWhiteBoardCefViewWdg->AppCallJsFunction(method);
      }

      if (mWhiteBoardCefViewWdg) {
         std::string board = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.board;
         int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
         QString loadUrl = QString::fromStdString(board) + QString("&role_type=%1").arg(roleType);
         mWhiteBoardCefViewWdg->LoadUrl(loadUrl);
      }

      
   }
}

//bool WebRtcLiveWdg::close()
//{
//	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
//	if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
//		int createDefaultRecord = 0;
//		globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
//
//		globalToolManager->GetClassSDK()->StopClass(createDefaultRecord);
//		if (globalToolManager) {
//			globalToolManager->SafeToExitApp();
//		}
//	}
//	else {
//		//ѧԱ�˳�ʱҪ����������Ϣ��
//		if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher && globalToolManager->GetDataManager()->IsAlreadyPublished()) {
//			globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish);
//		}
//		globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
//		if (globalToolManager) {
//			globalToolManager->SafeToExitApp();
//		}
//	}
//	return CBaseWnd::close();
//}

void WebRtcLiveWdg::closeEvent(QCloseEvent* event) {
   if (event) {
      event->ignore();
      //QWidget::closeEvent(event);
      slot_CloseClicked();

   }
}

void WebRtcLiveWdg::createTabLabel()
{
   if (nullptr == mpPaintLable)
   {
      mpPaintLable = new CPaintLabel(this);
      mpPaintLable->setAlignment(Qt::AlignCenter);
      //mpPaintLable->setText(QStringLiteral("��Ա"));
      //mpPaintLable->setStyleSheet("background-color: rgb(0, 255, 255);");
      mpPaintLable->setFixedHeight(34);
      mpPaintLable->setFixedWidth(35);
      //mpPaintLable->hide();
      //mpPaintLable->setFixedWidth(106);
   }
   if (nullptr == mpPaintLableL)
   {
      mpPaintLableL = new CPaintLabel(this);
      mpPaintLableL->setAlignment(Qt::AlignCenter);
      mpPaintLableL->setFixedHeight(34);
      mpPaintLableL->setFixedWidth(35);
   }

}

void WebRtcLiveWdg::slot_changed(int index)
{
   QString strTilte = ui.tabWidget->tabBar->tabText(index).trimmed();
   if (strTilte.compare(STRMEMBER) == 0 && nullptr != mpPaintLable)
   {
      mpPaintLable->SetHandsUpState(eHandsUpState_No);
   }
   else if(nullptr!=mMemberListWdg ) {
	   mMemberListWdg->on_btnOnLine_clicked();
   }
}

void WebRtcLiveWdg::creatTab(const bool& showMember, const bool& showNotic)
{
   ui.tabWidget->setMovable(true);

   connect(ui.tabWidget->tabBar, &QTabBar::currentChanged, this, &WebRtcLiveWdg::slot_changed);
   QString strTitle = "";
   //����
   //if (nullptr == mChatWebEngineViewWdg)
   //   mChatWebEngineViewWdg = new WebEngineViewWdg;

   if (nullptr == mChatCefViewWdg)
      mChatCefViewWdg = new LibCefViewWdg(this);

   mChatCefViewWdg->hide();
   //mChatCefViewWdg->show();
   mChatCefViewWdg->setObjectName("ChatWebEngineViewWdg");
   strTitle = STRCHAT;
   //mpChatTabItem->SetTitle(strTitle );
   //mpChatTabItem->addCenterWidget(mChatWebEngineViewWdg);
   ui.tabWidget->addTab(mChatCefViewWdg, strTitle);
   mChatCefViewWdg->setWindowTitle(strTitle);
   createTabLabel();
   //��Ա�б�

   if (nullptr == mMemberListWdg)
   {
      mMemberListWdg = new CMemberListWdg;
      mMemberListWdg->on_btnRefresh_clicked();
      //connect(mMemberListWdg, &CMemberListWdg::sig_HandsUpState, this, &WebRtcLiveWdg::slotChangeHandsUpState);//mMemberListWdg
   }
   else
   {
      mMemberListWdg->on_btnRefresh_clicked();
    }

   if (nullptr != mMemberListWdg)
   {
      connect(mMemberListWdg, &CMemberListWdg::sig_HandsUpState, this, &WebRtcLiveWdg::slotChangeHandsUpState);//mMemberListWdg
      connect(mMemberListWdg, &CMemberListWdg::sig_KickUserPublish, this, &WebRtcLiveWdg::slot_KickUserPublish);
      connect(mMemberListWdg, &CMemberListWdg::sign_AuditPublish, this, &WebRtcLiveWdg::slot_AuditPublish);
      bool bd = connect(mMemberListWdg, &CMemberListWdg::sign_AskforPublish, this, &WebRtcLiveWdg::slot_AskforPublish);
   }

   mMemberListWdg->setObjectName("MemberListWdg");
   QString msConfPath = globalToolManager->GetConfigPath();

   int debugmember = ConfigSetting::ReadInt(msConfPath, GROUP_DEFAULT, DEBUGMEMBER, 0);

   if (showMember || debugmember >0)
   {
      strTitle = STRMEMBER;
      mMemberListWdg->setWindowTitle(strTitle);
      ui.tabWidget->addTab(mMemberListWdg, strTitle);
      ui.tabWidget->setTabButton(1, QTabBar::RightSide, mpPaintLable);
      ui.tabWidget->setTabButton(1, QTabBar::LeftSide, mpPaintLableL);
   }

   //����
   if (nullptr == mNoticeSendWdg)
      mNoticeSendWdg = new NoticeSendWdg;
   mNoticeSendWdg->setObjectName("NoticeSendWdg");

   mbEjectNoticeSend = false;

   strTitle = STRNOTICE;
   ui.tabWidget->addTab(mNoticeSendWdg, strTitle);
   mNoticeSendWdg->setWindowTitle(strTitle);
   mNoticeSendWdg->showNoticWdg(showNotic);

   ui.tabWidget->setFocusPolicy(Qt::NoFocus);
   connect(ui.tabWidget->tabBar, &CTabBar::sig_tabDrag, this, &WebRtcLiveWdg::slot_tabDrag);
   connect(ui.tabWidget, &CTabWidget::tabCloseRequested, this, &WebRtcLiveWdg::slot_closeTab);
   //connect(ui.tabWidget, &CTabWidget::currentChanged, ui.tabWidget, &CTabWidget::setCurrentIndex);
}

void WebRtcLiveWdg::ChangeNoticeShow(bool bCurTool /*= true*/)
{
   if (bCurTool)
   {
      if (mNoticeSendWdg && mNoticeSendWdg->isNoticShowWdgVisible())
      {
         mNoticeSendWdg->showNotic(false);
         mNoticeShowMsgTimer.start(10);
         mNoticeShowWdg->move(0, ui.tabWidget->GetTabHeight());
         mNoticeShowWdg->show();
      }

   }
   else if (mNoticeShowWdg && mNoticeShowWdg->isVisible())
   {
      mNoticeShowWdg->hide();
      mNoticeSendWdg->showNotic(true);
   }
}

void WebRtcLiveWdg::slot_DragChat(const QPoint& posShow)
{
   /*if (nullptr != mChatWebEngineViewWdg)
      ShowDragDlg(mChatWebEngineViewWdg, posShow + QPoint(0.5*mChatWebEngineViewWdg->width(), 0));*/

   if (nullptr != mChatCefViewWdg)
      ShowDragDlg(mChatCefViewWdg, posShow + QPoint(0.5*mChatCefViewWdg->width(), 0));
}

void WebRtcLiveWdg::slot_DragMember(const QPoint& posShow)
{
   if (nullptr != mMemberListWdg)
      ShowDragDlg(mMemberListWdg, posShow + QPoint(0.5*mMemberListWdg->width(), 0));
}

void WebRtcLiveWdg::ShowDragDlg(QWidget *draged, QPoint posShow)
{
   if (nullptr == mpEjectTabItem)
   {
      mpEjectTabItem = new CTabItemDlg(this);
      connect(mpEjectTabItem, &CTabItemDlg::sig_doubleClickedTitleBar, this, &WebRtcLiveWdg::slot_tabBarDoubleClicked);
   }

   if (nullptr != mpEjectTabItem)
   {

      if (nullptr == draged) return;
      QString windowName = draged->windowTitle();


      if (!windowName.trimmed().isEmpty())
      {
         clostEjectTabItem(mpEjectTabItem);

         mpEjectTabItem->SetTitle(windowName);
         mpEjectTabItem->AddCenterWdg(draged);

         int iWidth = 325;
         int iHeight = 693;
         if (mMemberListWdg == draged)
         {
            mpPaintLable->deleteLater();
            mpPaintLable = nullptr;
            mpPaintLableL->deleteLater();
            mpPaintLableL = nullptr;

         }
         else if (mNoticeSendWdg == draged)
         {
            mbEjectNoticeSend = true;
            ChangeNoticeShow(false);
         }

         mpEjectTabItem->resize(iWidth, iHeight);

         mpEjectTabItem->move(posShow + QPoint(-0.5*iWidth, -10));
         mpEjectTabItem->show();
         draged->show();
		 mpEjectTabItem->setEnabled(true);
		 mpEjectTabItem->update();
         qDebug() << ui.tabWidget->tabBar->count() << "slot_tabDrag  " << endl;
         ui.tabWidget->setCurrentIndex(0);
         ui.tabWidget->tabBar->setCurrentIndex(0);
      }

   }
}

void WebRtcLiveWdg::slot_tabDrag(int index, QPoint point)
{
   if (index < ui.tabWidget->tabBar->count())
   {
      ui.tabWidget->setCurrentIndex(index);
      ui.tabWidget->tabBar->setCurrentIndex(index);
   }

   qDebug() << ui.tabWidget->tabBar->count() << "slot_tabDrag  " << index << endl;

   QWidget *draged = ui.tabWidget->widget(index);
   QPoint posShow = point + pos() + ui.widget_right_tool->pos() + ui.tabWidget->pos();

   ShowDragDlg(draged, posShow);

}

void WebRtcLiveWdg::clostEjectTabItem(CTabItemDlg *widget)
{
   if (nullptr == widget) return;
   QObjectList list = widget->GetChildren();
   //QWidget* pWidget = nullptr;


   for (int i = 0; i < list.count(); ++i)
   {
      if (list[i]->inherits("LibCefViewWdg"))
      {
         //mChatWebEngineViewWdg->setParent(ui.tabWidget);
         ui.tabWidget->insertTab(0, mChatCefViewWdg, widget->windowTitle());
         //ui.tabWidget->addTab( mChatWebEngineViewWdg, widget->windowTitle());
         widget->RemoveCenterWdg(mChatCefViewWdg);
         widget->hide();
         break;
      }
      else if (list[i]->inherits("CMemberListWdg"))
      {
         int index = 1;
         int iCount = ui.tabWidget->tabBar->count();
         for (i = 0; i < iCount; i++)
         {
            QString strTilte = ui.tabWidget->tabBar->tabText(i).trimmed();
            if (strTilte.compare(STRCHAT) == 0)
            {
               index = i + 1;
               break;
            }

         }
         createTabLabel();
         widget->RemoveCenterWdg(mMemberListWdg);
         ui.tabWidget->insertTab(index, mMemberListWdg, widget->windowTitle());
         ui.tabWidget->setTabButton(index, QTabBar::RightSide, mpPaintLable);
         ui.tabWidget->setTabButton(index, QTabBar::LeftSide, mpPaintLableL);
         widget->hide();
         break;
      }
      else if (list[i]->inherits("NoticeSendWdg"))
      {
         int iCount = ui.tabWidget->tabBar->count();

         ChangeNoticeShow(true);
         mbEjectNoticeSend = false;
         //mChatWebEngineViewWdg->setParent(ui.tabWidget);
         ui.tabWidget->insertTab(iCount, mNoticeSendWdg, widget->windowTitle());
         //ui.tabWidget->addTab(mNoticeSendWdg, widget->windowTitle());
         widget->RemoveCenterWdg(mNoticeSendWdg);
         widget->hide();
         break;
      }


      ui.tabWidget->setCurrentIndex(0);
      ui.tabWidget->tabBar->setCurrentIndex(0);
   }
}

void WebRtcLiveWdg::slot_tabBarDoubleClicked()
{
   CTabItemDlg *widget = qobject_cast<CTabItemDlg*>(sender());

   clostEjectTabItem(widget);
}

void WebRtcLiveWdg::slot_closeTab(int)
{

}
//void WebRtcLiveWdg::JsCallQtMsg(QString json) {
//	qDebug() << json;
//}
//
//void WebRtcLiveWdg::slot_OpenClassStudentRenderMove() {
//    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
//    if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
//        if (mOpenClassStudentRender && mOpenClassStudentRender->isVisible()) {
//            QPoint pos = ui.stackedWidget_Center->mapToGlobal(ui.stackedWidget_Center->pos());
//            mOpenClassStudentRender->move(pos.rx() + ui.stackedWidget_Center->width()-mOpenClassStudentRender->width(), pos.ry() + ui.stackedWidget_Center->height()-mOpenClassStudentRender->height());
//            mOpenClassStudentRender->show();
//            mOpenClassStudentRender->raise();
//        }
//    }
//}