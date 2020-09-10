#include "TeacherRenderWdg.h"
#include "WebRtcLiveWdg.h"
#include "FloatRender.h"
#include "FullScreenRender.h"
#include <QApplication> 
#include <QDesktopWidget> 
#include <QScreen> 
#include <QDebug>
extern ToolManager *globalToolManager;

TeacherRenderWdg::TeacherRenderWdg(QWidget *parent, bool bTeacherRender /*= false*/)
   : QWidget(parent),
	mbTeacherRender(bTeacherRender)
{
   ui.setupUi(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);

   //mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
   ui.mpVhallRenderMaskWdg->SetTeacherRenderWdg(this);
   //if (mpVhallRenderMaskWdg) {
      //ui.mpVhallRenderMaskWdg->hide();
      connect(this, &TeacherRenderWdg::sig_PicPush, ui.mpVhallRenderMaskWdg, &VhallRenderMaskWdg::sig_PicPush);
  // }
   ui.widget_small_render->hide();
   ui.mpVhallRenderMaskWdg->CheckShowChangeBigScreen(false);
   //connect(&mUserNickNameShowTimer, SIGNAL(timeout()), this, SLOT(slot_NoticeShowNickName()));
   ui.stackedWidget->installEventFilter(this);
   SetStackCurrentIndex(TeacherRenderWdg_Page_Index_classNoStart);
   mLineStateTimer = new QTimer(this);
   if (mLineStateTimer) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         connect(mLineStateTimer, SIGNAL(timeout()), this, SLOT(slot_OnUnPublishStudent()));
      }
      else  if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
         connect(mLineStateTimer, SIGNAL(timeout()), this, SLOT(slot_OnstuClickedUnPublish()));
      }
   }
   ui.mBigRenderCtrl->hide();
   //this->setUpdatesEnabled(false);
   //UpdataRendFlush(false);
}

TeacherRenderWdg::~TeacherRenderWdg()
{
   if (mLineStateTimer) {
      mLineStateTimer->stop();
   }
   //mUserNickNameShowTimer.stop();
   ui.stackedWidget->removeEventFilter(this);
   //if (nullptr != mpFullRender) {
   //   mpFullRender->deleteLater();
   //   mpFullRender = nullptr;
   //}
   //if (ui.mBigRenderCtrl) {
   //   ui.mBigRenderCtrl->installEventFilter(this);
   //}
   //if (mTeacherCtrlWdg) {
   //    mTeacherCtrlWdg->removeEventFilter(this);
   //}
   //if (mpVhallRenderMaskWdg) {
   //   delete mpVhallRenderMaskWdg;
   //   mpVhallRenderMaskWdg = NULL;
   //}
   //if (NULL != mpTrophyWdg)
   //{
   //	delete mpTrophyWdg;
   //	mpTrophyWdg = NULL;
   //}
}


void TeacherRenderWdg::ShowTrophyWdg(const bool& bshow) {
   //if (mpVhallRenderMaskWdg) {
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   ui.mpVhallRenderMaskWdg->setShowTrophyWdg(bshow && CLASS_TYPE_SMALL == baseInfo.mWebinarInfo.course_type);
   //}
}

void TeacherRenderWdg::ShowBigRenderCtrl(const bool& bShow /*= true*/)
{
   //if (mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetShowCtrlWdg(!bShow && mOperationPower);
   //}
   bShowBigRenderCtrl = bShow;
   if (bShow) {
      if (ui.mBigRenderCtrl && mWndFloatType != WndFloatType_Full && mOperationPower) {
         ui.mBigRenderCtrl->show();
      }
   }
   else {
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->hide();
      }
   }
}


bool TeacherRenderWdg::isSmallRender()
{
   return mbHasRenderSmallWnd;
}

void TeacherRenderWdg::slotTrophyAdd(const int& iCount) {
   //if (NULL != mpTrophyWdg)
   //	mpTrophyWdg->setTrophyAdd(iCount);
   //if (NULL != mpVhallRenderMaskWdg)
   {
      ui.mpVhallRenderMaskWdg->setTrophyAdd(iCount);
   }
   mTrophyRewardNum += iCount;

   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
   QString userId = GetUserJoinID();
   //if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
	  // //FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
	  // //if (wnd) {
		 // // wnd->initTrophyRewardNum(mTrophyRewardNum);
	  // //}
   //}

   //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
   //if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
	  // FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
	  // if (wnd) {
		 //  wnd->initTrophyRewardNum(mTrophyRewardNum);
	  // }
   //}

   //CreateFullRender();
   //if (mpFullRender) {
   //   mpFullRender->initTrophyRewardNum(mTrophyRewardNum);
   //}
}

//void TeacherRenderWdg::showFullRender()
//{
//   //mpFullRender->Notice(mapParam);
//   ui.widget_render;
//
//   QDesktopWidget * deskTop = QApplication::desktop();
//   int curMonitor = deskTop->screenNumber(this); // 参数是一个QWidget*
//   QRect DesktopRect = deskTop->screenGeometry(curMonitor);
//
//   QRect rect = deskTop->availableGeometry(curMonitor);
//
//   globalToolManager->GetDataManager()->WriteLog("%s DesktopRect:%d,%d,%d,%d    Rect:%d,%d,%d,%d    ",
//      __FUNCTION__,
//      DesktopRect.x(), DesktopRect.y(), DesktopRect.width(), DesktopRect.height(),
//      rect.x(), rect.y(), rect.width(), rect.height());
//   mpFullRender->move(rect.x(), rect.y());
//   if (rect.height() < DesktopRect.height() || rect.width() < DesktopRect.width()) {
//      mpFullRender->setFixedSize(rect.width(), rect.height());
//   }
//   else {
//      mpFullRender->setFixedSize(rect.width(), rect.height() - 1);
//   }
//   mpFullRender->show();
//   mpFullRender->raise();
//}

//void TeacherRenderWdg::FullRenderNotice(const CSTDMapParam & para)
//{
//   if (nullptr != mpFullRender) {
//      mpFullRender->Notice(para);
//   }
//}

//void TeacherRenderWdg::hideFullRender()
//{
//   if (nullptr != mpFullRender) {
//      mpFullRender->hide();
//   }
//
//}

//void TeacherRenderWdg::CreateFullRender()
//{
//   if (nullptr == mpFullRender) {
//      mpFullRender = new FullScreenRender(this);
//   }
//   
//}

void TeacherRenderWdg::show()
{
   mEnter = true;
   globalToolManager->GetDataManager()->WriteLog("%s uid %s smalle uid %s", __FUNCTION__, GetUserJoinID().toStdString().c_str(), GetSmallWndUserJoinID().toStdString().c_str());
	QWidget::show();
	ui.stackedWidget->resize(this->width(), this->height());
	if (ui.mBigRenderCtrl) {
		/*QPoint pos = ui.stackedWidget->mapToGlobal(ui.stackedWidget->pos());
		ui.mBigRenderCtrl->move(pos.rx() + (ui.stackedWidget->width() - ui.mBigRenderCtrl->width()) / 2, pos.ry() + ui.stackedWidget->height() - ui.mBigRenderCtrl->height() - 40);
	*/
		setGigRenderCtrlGeometry();
	}
	int iW = this->width();
	int iH = this->height();
	ui.mpVhallRenderMaskWdg->setFixedSize(iW, iH);
}

void TeacherRenderWdg::hide()
{
   QWidget::hide();
   //if (mpFullRender != nullptr) {
   //   mpFullRender->hide();
   //}
}

void TeacherRenderWdg::enterEvent(QEvent *e){
   mEnter = true;
   WebinarInfo info = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo;

   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;

   if (/*NULL != mpVhallRenderMaskWdg &&*/ info.layout != CLASS_LAYOUT_ONLY_VIDEO 
      || globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture) 
      || pWebRtcLiveWdg->isEixMediaScreen()/*globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_MediaFile)
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Desktop)
      || globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_SoftWare)*/
      /*&& mWndFloatType == WndFloatType_Normal*/){
	   ui.mpVhallRenderMaskWdg->setCtrlValid(true && ui.mBigRenderCtrl->isHidden());
	   ui.mpVhallRenderMaskWdg->showCtrlWdg(true, mWndFloatType);
      //globalToolManager->GetDataManager()->WriteLog("%s xk Enter:%d mpVhallRenderMaskWdg::Enter %d", __FUNCTION__, mEnter, mpVhallRenderMaskWdg->GetBEnter());
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s info.layout %d IsPushingStream(VHStreamType_Auxiliary_CamerCapture) %d IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)%d",
         __FUNCTION__, info.layout
         ,globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
         ,globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture));

   }
   qDebug() << __FUNCTION__ << "XK size:" << this->width() << this->height() << ui.mpVhallRenderMaskWdg->x()
	   << ui.mpVhallRenderMaskWdg->y() << ui.mpVhallRenderMaskWdg->width() << ui.mpVhallRenderMaskWdg->height();
   //globalToolManager->GetDataManager()->WriteLog("%s xk Enter:%d  mpVhallRenderMaskWdg: %x, %p", __FUNCTION__, mEnter, mpVhallRenderMaskWdg, mpVhallRenderMaskWdg);
   QWidget::enterEvent(e);
}

void TeacherRenderWdg::mousePressEvent(QMouseEvent *e)
{
   //if (ui.mpVhallRenderMaskWdg){
   //   ui.mpVhallRenderMaskWdg->setCtrlValid(false);
   //}
   QWidget::mousePressEvent(e);
}

void TeacherRenderWdg::resizeEvent(QResizeEvent *event)
{
	ui.stackedWidget->resize(this->width(), this->height());
	if (ui.mBigRenderCtrl) {
		/*QPoint pos = ui.stackedWidget->mapToGlobal(ui.stackedWidget->pos());
		ui.mBigRenderCtrl->move(pos.rx() + (ui.stackedWidget->width() - ui.mBigRenderCtrl->width()) / 2, pos.ry() + ui.stackedWidget->height() - ui.mBigRenderCtrl->height() - 40);
	*/
		setGigRenderCtrlGeometry();
	}
	int iW = this->width();
	int iH = this->height();
	ui.mpVhallRenderMaskWdg->setFixedSize(iW, iH);
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI/* && baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO*/) {
		resetSmallRender();
	}

	QWidget::resizeEvent(event);
}

void TeacherRenderWdg::showEvent(QShowEvent *event)
{
	ui.stackedWidget->resize(this->width(), this->height());
	if (ui.mBigRenderCtrl) {
		/*QPoint pos = ui.stackedWidget->mapToGlobal(ui.stackedWidget->pos());
		ui.mBigRenderCtrl->move(pos.rx() + (ui.stackedWidget->width() - ui.mBigRenderCtrl->width()) / 2, pos.ry() + ui.stackedWidget->height() - ui.mBigRenderCtrl->height() - 40);
	*/
		setGigRenderCtrlGeometry();
	}
	int iW = this->width();
	int iH = this->height();
	ui.mpVhallRenderMaskWdg->setFixedSize(iW, iH);
	QWidget::showEvent(event);
}

void TeacherRenderWdg::leaveEvent(QEvent * e)
{
   mEnter = false;
   QWidget::leaveEvent(e);
}

void TeacherRenderWdg::setOperationPower(bool bPower)
{
   mOperationPower = bPower;
   if (mOperationPower) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
         if (ui.mBigRenderCtrl && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
            && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
            && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
            && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Stu_Desktop)
            && mOperationPower) {
            ui.mBigRenderCtrl->show();
         }
      }
   }
   else{
      ui.mBigRenderCtrl->hide();
      
   }
   ui.mpVhallRenderMaskWdg->SetShowCtrlWdg(mOperationPower);
}

void TeacherRenderWdg::InitRenderInsertPos(InsertPos pos) {
   mWndPos = pos;
   if (pos == InsertPos_Speaker || pos == InsertPos_WndManager) {

      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnClickedMic()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnClickedCamera()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedFullScreen()), this, SLOT(slot_OnClickedFullScreen()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedFloatRender()), this, SLOT(slot_OnClickedFloatRender()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedAllMuteCamera()), this, SLOT(slot_OnClickedAllMuteCamera()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedAllMute()), this, SLOT(slot_OnClickedAllMute()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnAllUnpublish()), this, SLOT(slot_OnAllUnpublish()));

      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedCamera()), this, SLOT(slot_OnStuClickedCamera()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedMic()), this, SLOT(slot_OnStuClickedMic()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnstuClickedUnPublish()), this, SLOT(slot_OnstuClickedUnPublish()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuPenClickedPen(bool)), this, SLOT(slot_OnStuPenClickedPen(bool)));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedFloat()), this, SLOT(slot_OnStuClickedFloat()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedFullWnd()), this, SLOT(slot_OnStuClickedFullWnd()));

      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnOpenClassStuMic()), this, SLOT(slot_OnClickedStudentMic()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnOpenClassStudentCamera()), this, SLOT(slot_OnClickedStudentCamera()));
      disconnect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnChangeCamera()), this, SLOT(slot_OnChangeCamera()));
      disconnect(ui.mpVhallRenderMaskWdg, &VhallRenderMaskWdg::sig_FavorStudent, this, &TeacherRenderWdg::slotFavorStudent);
      disconnect(ui.mpVhallRenderMaskWdg, &VhallRenderMaskWdg::sig_OnSetBigScreen, this, &TeacherRenderWdg::slot_OnSetBigScreen);

      //小窗中显示此控件

      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnClickedMic()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnClickedCamera()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedFullScreen()), this, SLOT(slot_OnClickedFullScreen()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedFloatRender()), this, SLOT(slot_OnClickedFloatRender()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedAllMuteCamera()), this, SLOT(slot_OnClickedAllMuteCamera()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnClickedAllMute()), this, SLOT(slot_OnClickedAllMute()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnAllUnpublish()), this, SLOT(slot_OnAllUnpublish()));

      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedCamera()), this, SLOT(slot_OnStuClickedCamera()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedMic()), this, SLOT(slot_OnStuClickedMic()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnstuClickedUnPublish()), this, SLOT(slot_OnstuClickedUnPublish()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuPenClickedPen(bool)), this, SLOT(slot_OnStuPenClickedPen(bool)));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedFloat()), this, SLOT(slot_OnStuClickedFloat()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnStuClickedFullWnd()), this, SLOT(slot_OnStuClickedFullWnd()));

      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnOpenClassStuMic()), this, SLOT(slot_OnClickedStudentMic()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnOpenClassStudentCamera()), this, SLOT(slot_OnClickedStudentCamera()));
      connect(ui.mpVhallRenderMaskWdg, SIGNAL(sig_OnChangeCamera()), this, SLOT(slot_OnChangeCamera()));
      connect(ui.mpVhallRenderMaskWdg, &VhallRenderMaskWdg::sig_FavorStudent, this, &TeacherRenderWdg::slotFavorStudent);
      connect(ui.mpVhallRenderMaskWdg, &VhallRenderMaskWdg::sig_OnSetBigScreen, this, &TeacherRenderWdg::slot_OnSetBigScreen);

      ui.mpVhallRenderMaskWdg->setCtrlValid(true && ui.mBigRenderCtrl->isHidden());
      //mTeacherCtrlWdg->installEventFilter(this);
      this->setMouseTracking(true);

   }
   else {
      //无文档布局或窗口显示和全屏，显示此大控件。
      //if (ui.mBigRenderCtrl == nullptr) 
	   if(false==mbFirstBigCtrl){
      //   ui.mBigRenderCtrl = new BigRenderCtrl(this);
		   mbFirstBigCtrl = true;
         if (ui.mBigRenderCtrl) {
            bShowBigRenderCtrl = true;
            ui.mBigRenderCtrl->hide();
            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnClickedMic()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnClickedCamera()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnChangeCamera()), this, SLOT(slot_OnChangeCamera()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OncloseStuCamera()), this, SLOT(slot_OnClickedStudentCamera()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OncloseStuMic()), this, SLOT(slot_OnClickedStudentMic()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnUnpublishStu()), this, SLOT(slot_OnUnPublishStudent()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnBigScreenChanged()), this, SLOT(slot_OnSetBigScreen()));

               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnClickedMic()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnClickedCamera()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnChangeCamera()), this, SLOT(slot_OnChangeCamera()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OncloseStuCamera()), this, SLOT(slot_OnClickedStudentCamera()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OncloseStuMic()), this, SLOT(slot_OnClickedStudentMic()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnUnpublishStu()), this, SLOT(slot_OnUnPublishStudent()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnBigScreenChanged()), this, SLOT(slot_OnSetBigScreen()));

            }
            else {
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnStuClickedMic()));
               disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnStuClickedCamera()));

               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedMic()), this, SLOT(slot_OnStuClickedMic()));
               connect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedCamera()), this, SLOT(slot_OnStuClickedCamera()));
            }
            disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedfullScreen()), this, SLOT(slot_OnClickedFullScreen()));
            disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnAllUnPublish()), this, SLOT(slot_OnAllUnpublish()));
            disconnect(ui.mBigRenderCtrl, SIGNAL(sig_OnAllMuteMic()), this, SLOT(slot_OnClickedAllMute()));

            connect(ui.mBigRenderCtrl, SIGNAL(sig_OnClickedfullScreen()), this, SLOT(slot_OnClickedFullScreen()));
            connect(ui.mBigRenderCtrl, SIGNAL(sig_OnAllUnPublish()), this, SLOT(slot_OnAllUnpublish()));
            connect(ui.mBigRenderCtrl, SIGNAL(sig_OnAllMuteMic()), this, SLOT(slot_OnClickedAllMute()));
            ui.mBigRenderCtrl->installEventFilter(this);
         }
      }
   }
}

void TeacherRenderWdg::ChangeRenderPos(bool isCenter) {
   //if (ui.mBigRenderCtrl) {
   //   delete ui.mBigRenderCtrl;
   //   ui.mBigRenderCtrl = nullptr;
   //}

   InitRenderInsertPos(isCenter ? InsertPos_Center : InsertPos_Speaker);
   InitRenderCtrl(mUserJoinId);
}

void TeacherRenderWdg::InitRenderCtrl(QString id) {
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->InitBtn(id);
   }
   if (ui.mBigRenderCtrl) {
      ui.mBigRenderCtrl->InitBtn(id);
   }
   mUserJoinId = id;
}



void TeacherRenderWdg::SetPageIndex(int index) {
   globalToolManager->GetDataManager()->WriteLog("%s index:%d", __FUNCTION__, index);
   SetStackCurrentIndex(index);

   if (index == TeacherRenderWdg_Page_Index_render || index == TeacherRenderWdg_Page_Index_NoCamera) {
		//mUserNickNameShowTimer.start(10);
		if (ui.mpVhallRenderMaskWdg) {
			ui.mpVhallRenderMaskWdg->setFixedWidth(this->width());
      }
   }
   else {
      //mUserNickNameShowTimer.stop();
      if (ui.mpVhallRenderMaskWdg) {
         //ui.mpVhallRenderMaskWdg->hide();
      }
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->hide();
      }
   }
   if (index == TeacherRenderWdg_Page_Index_classNoStart) {
      ui.widget_small_render->hide();
      ui.mpVhallRenderMaskWdg->CheckShowChangeBigScreen(false);
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->OpenClassStudentPublish(false);
      }
   }
}

void TeacherRenderWdg::UpdataRendFlush(bool enable) {
   //ui.widget_render->setUpdatesEnabled(enable);
}

void TeacherRenderWdg::SetAllMuteAudioState(bool mute) {
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetAllMuteAudioState(mute);
   }
   if (ui.mBigRenderCtrl) {
      ui.mBigRenderCtrl->SetAllMuteAudio(mute);
   }
}

void TeacherRenderWdg::SetPenState(bool enable) {
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetPenState(enable);
      ui.mpVhallRenderMaskWdg->SetShowPenLogo(enable);
   }

   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
   QString userId = GetUserJoinID();
  // if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
		//FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
		//if (wnd ) {
		//	wnd->SetPenState(enable);
		//}
  // }
   
   //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
   //if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
	  // FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
	  // if (wnd) {
		 //  wnd->SetPenState(enable);
	  // }
   //}

   //CreateFullRender();
   //if (nullptr!= mpFullRender) {
   //   mpFullRender->SetPenState(enable);
   //}
}

void TeacherRenderWdg::SetFloatType(WndFloatType type) {
   SetFloatTypeValue(type);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (mWndFloatType == WndFloatType_Float) {
      //mUserNickNameShowTimer.stop();
    //  if (ui.mpVhallRenderMaskWdg) {
		  //ui.mpVhallRenderMaskWdg->hide();
    //  }
   }
   else if (mWndFloatType == WndFloatType_Full) {
      //mUserNickNameShowTimer.stop();
      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
         if (ui.mBigRenderCtrl) {
            ui.mBigRenderCtrl->hide();
            update();
         }
      }
      else {
    //     if (ui.mpVhallRenderMaskWdg) {
			 //ui.mpVhallRenderMaskWdg->hide();
    //     }
      }
   }
   else {
      //mUserNickNameShowTimer.start(5);
      if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO) {
         if (ui.mBigRenderCtrl && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
            && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
            && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
            && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Stu_Desktop)
            && mOperationPower) {
            ui.mBigRenderCtrl->show();
         }
      }
      else {
         if (ui.mpVhallRenderMaskWdg) {
			 ui.mpVhallRenderMaskWdg->show();
			 ui.mpVhallRenderMaskWdg->setCtrlValid(true && ui.mBigRenderCtrl->isHidden());
            ui.mpVhallRenderMaskWdg->showCtrlWdg(true);
            ui.mpVhallRenderMaskWdg->raise();
            ui.mpVhallRenderMaskWdg->activateWindow();
         }
      }

      activateWindow();
   }
}

void TeacherRenderWdg::SetFloatTypeValue(WndFloatType type)
{
   mWndFloatType = type;
   globalToolManager->GetDataManager()->WriteLog(" %s %s mWndFloatType %d",
      __FUNCTION__, GetUserJoinID().toLocal8Bit().data(), mWndFloatType);
   ui.mpVhallRenderMaskWdg->SetFloatType(type);
}

int TeacherRenderWdg::GetFloatType() {
   globalToolManager->GetDataManager()->WriteLog(" %s %s mWndFloatType %d",
      __FUNCTION__, GetUserJoinID().toLocal8Bit().data(), mWndFloatType);
   ui.mpVhallRenderMaskWdg->GetFloatType();
   return mWndFloatType;
}

void TeacherRenderWdg::resetSmallRender()
{
	int iCurW = this->width();
	int iCurH = this->height();

	int iSamllW = ui.widget_small_render->width();
	int iSamllH = ui.widget_small_render->height();
	int iX = iCurW - iSamllW;
	int iY = iCurH - iSamllH;

	//ui.widget_samllRenderWidget->move(iX, iY);
	ui.widget_small_render->setGeometry(iX, iY, iSamllW, iSamllH);
}

void TeacherRenderWdg::SetShowSmallRender(bool show) {
   globalToolManager->GetDataManager()->WriteLog("%s show:%d", __FUNCTION__, show);
   if (show) {
      mbHasRenderSmallWnd = true;
      ui.widget_small_render->show();
	  resetSmallRender();
      ui.mpVhallRenderMaskWdg->CheckShowChangeBigScreen(true);
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->OpenClassStudentPublish(true);
      }
   }
   else {
      mbHasRenderSmallWnd = false;
      ui.widget_small_render->hide();
      ui.mpVhallRenderMaskWdg->CheckShowChangeBigScreen(false);
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->OpenClassStudentPublish(false);
      }
   }
}

void TeacherRenderWdg::SetSmallRenderNickName(QString name) {


}

void TeacherRenderWdg::SetSmallRenderMicState(bool hasAudio, bool isSelf) {
   mbSmallRenderMicIsOpen = hasAudio;
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetSmallRenderMicState(mbSmallRenderMicIsOpen);

	  if (mWndPos == InsertPos_WndManager) {
		  ui.mpVhallRenderMaskWdg->SetShowNoMic(!mbSmallRenderMicIsOpen);

		  CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		  WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
		  QString userId = GetUserJoinID();
		  //if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
			 // FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
			 // if (wnd) {
				//  wnd->SetShowNoMic(!mbSmallRenderMicIsOpen);
			 // }
		  //}

		  //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
		  //if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
			 // FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
			 // if (wnd) {
				//  wnd->SetShowNoMic(!mbSmallRenderMicIsOpen);
			 // }
		  //}

        //CreateFullRender();
        //if (nullptr != mpFullRender) {
        //   mpFullRender->SetShowNoMic(!mbSmallRenderMicIsOpen);
        //}

	  }
   }


   if (isSelf) {
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->SetMicState(mbSmallRenderMicIsOpen);
      }
   }
   else {
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->SetStudentMic(mbSmallRenderMicIsOpen);
      }
   }
}

void TeacherRenderWdg::SetStuSmallRenderMicState(bool hasAudio)
{
	mbSmallRenderMicIsOpen = hasAudio;
	if (ui.mpVhallRenderMaskWdg) {
		ui.mpVhallRenderMaskWdg->SetSmallRenderMicState(mbSmallRenderMicIsOpen);
		if (mWndPos == InsertPos_WndManager) {
			ui.mpVhallRenderMaskWdg->SetShowStuNoMic(!mbSmallRenderMicIsOpen);

			CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
			WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
			QString userId = GetUserJoinID();
			//if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
			//	FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
			//	if (wnd) {
			//		wnd->SetShowStuNoMic(!mbSmallRenderMicIsOpen);
			//	}
			//}

			//QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
			//if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
			//	FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
			//	if (wnd) {
			//		wnd->SetShowStuNoMic(!mbSmallRenderMicIsOpen);
			//	}
			//}

         //CreateFullRender();
         //if (nullptr != mpFullRender) {
         //   mpFullRender->SetShowStuNoMic(!mbSmallRenderMicIsOpen);
         //}

		}
	}

	if (ui.mBigRenderCtrl) {
		ui.mBigRenderCtrl->SetStudentMic(mbSmallRenderMicIsOpen);
	}
	
}

bool TeacherRenderWdg::GetSmallRenderMicState() {
   return mbSmallRenderMicIsOpen;
}

void TeacherRenderWdg::setTeacherSmallRenderCameraState(bool hasVideo)
{
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {//本人讲师在小画面
         if (hasVideo) {
            setStackedSmallIndex(0);
            OpenCamera();
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s big win", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
            //globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(QString::fromStdString(baseInfo.mUserInfo.join_id).toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());
         }
         else {
            setStackedSmallIndex(1);
            //ui.widget_render->update();
            //update();
            //globalToolManager->GetClassSDK()->StopRenderRemoteStream(stu_user_id.toStdWString(), GetStreamID().toStdString(), VHStreamType_AVCapture);
         }
      }
   }
}

void TeacherRenderWdg::SetSmallRenderCameraState(bool hasVideo, bool isSelf/* = false*/) {
   globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, hasVideo);
   mbSmallRenderCameraIsOpen = hasVideo;
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      QString stu_user_id = GetSmallWndUserJoinID();
      if (QString::number(baseInfo.bigScreen) != stu_user_id) { //学员在小画面 
         if (hasVideo) {
            setStackedSmallIndex(0);
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) 
            {
               globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s small wnd", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
			      globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(stu_user_id.toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());
            }
         }
         else {
            setStackedSmallIndex(1);
         }
      }
      else {  //学员在大画面
         //if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {//本人讲师在小画面
         //   if (hasVideo) {
         //      setStackedSmallIndex(0);
         //      globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s big win", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
         //      globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(   QString::fromStdString(baseInfo.mUserInfo.join_id).toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());
         //   }
         //   else {

         //      setStackedSmallIndex(1);
         //      ui.widget_render->update();
         //      update();
         //      //globalToolManager->GetClassSDK()->StopRenderRemoteStream(stu_user_id.toStdWString(), GetStreamID().toStdString(), VHStreamType_AVCapture);
         //   }
         //}
         //else {
            if (hasVideo) {
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render, isSelf);
               //ui.widget_render->setStyleSheet("");
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  //HWND wnd_id = GetRenderWndID();
                  globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s big win", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
                  //globalToolManager->GetClassSDK()->StartRenderRemoteStream(stu_user_id.toStdWString(), VHStreamType_AVCapture,(void*)wnd_id);
                  globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(stu_user_id.toStdWString(), VHStreamType_AVCapture, GetRenderWnd());
               }
            }
            else {
               //ui.widget_render->setStyleSheet(CLOSE_CAMERA_SHEET);
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
               ui.widget_render->update();
               update();
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  globalToolManager->GetClassSDK()->StopRenderRemoteStream(stu_user_id.toStdWString(), GetSmallWndStreamID().toStdString(), VHStreamType_AVCapture);
               }
            }
         //}

      }
   }
   else {
      if (hasVideo) {
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
      }
      else {
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
      }
   }

   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetSmallRenderCameraState(mbSmallRenderCameraIsOpen);
   }
   if (isSelf) {
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->SetCameraState(mbSmallRenderCameraIsOpen);
      }
   }
   else {
      if (ui.mBigRenderCtrl) {
         ui.mBigRenderCtrl->SetStudentCamera(mbSmallRenderCameraIsOpen);
      }
   }
}



void TeacherRenderWdg::SetSmallRenderRole(int role) {

}

void TeacherRenderWdg::SetSmallRenderFlushEnable(bool enable) {
   //ui.widget_samllRenderWidget->setUpdatesEnabled(enable);
}

void TeacherRenderWdg::SetSmallRenderUserPublish(bool publish) {
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->ShowStuPublishBtn(publish);
   }
}

void TeacherRenderWdg::RenderBigStreamToWnd(QString id) {
   //HWND wnd_id = (HWND)(GetRenderWndID());//取大画面的用户ID
   std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = GetRenderWnd();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.join_id == id.toStdString()) {
      if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
         //wnd_id = GetSmallRenderRenderWndID();
         Receiver = GetSmallRenderRenderWnd();
      }
      //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(wnd_id));
      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
      if (!mbCameraIsOpen) {
         globalToolManager->GetClassSDK()->CloseCamera();
         globalToolManager->GetDataManager()->SetMyCameraOpen(false);
      }
   }
   else {
      //HWND wnd_id = (HWND)(GetRenderWndID());//取大画面的用户ID
      std::shared_ptr<vhall::VideoRenderReceiveInterface> pwd = GetRenderWnd();
      QString stream_id = GetStreamID();
      if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != id.toStdString()) {
		//wnd_id = GetSmallRenderRenderWndID();
		pwd = GetSmallRenderRenderWnd();
         globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream id %s small", __FUNCTION__, id.toStdString().c_str());
      }
      globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
	  //globalToolManager->GetClassSDK()->StartRenderRemoteStream(id.toStdWString(), VHStreamType_AVCapture, (void*)(wnd_id));
	  globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(id.toStdWString(), VHStreamType_AVCapture, pwd);
	  globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);

	  if (id == globalToolManager->GetDataManager()->GetTeacherJoinId()) {
         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);
      }
      else {
         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);
      }
   }
}

void TeacherRenderWdg::RenderSmallStream() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (!ui.stackedWidget_small->isHidden()) {
      //HWND id = (HWND)(GetSmallRenderRenderWndID());
      std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = GetSmallRenderRenderWnd();
      if (baseInfo.mUserInfo.join_id == GetSmallWndUserJoinID().toStdString()) {
         if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
            //id = GetRenderWndID();
            Receiver = GetRenderWnd();
         }
         //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(id));
         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
         if (!mbSmallRenderCameraIsOpen) {
            globalToolManager->GetClassSDK()->CloseCamera();
            globalToolManager->GetDataManager()->SetMyCameraOpen(false);
         }
      }
      else {
         //HWND id = (HWND)(GetSmallRenderRenderWndID());
         std::shared_ptr<vhall::VideoRenderReceiveInterface> pInterFace = GetSmallRenderRenderWnd();
         QString stream_id = GetSmallWndStreamID();
         QString userId = GetSmallWndUserJoinID();
         if (!GetSmallWndUserJoinID().isEmpty() && !GetSmallWndUserJoinID().isEmpty()) {
            if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == userId.toStdString()) {
				//id = GetRenderWndID();
				pInterFace = GetRenderWnd();
               globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream id %s big", __FUNCTION__, userId.toStdString().c_str());
            }
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s",__FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());

			//globalToolManager->GetClassSDK()->StartRenderRemoteStream(userId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
			globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(userId.toStdWString(), VHStreamType_AVCapture, pInterFace);
            globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(userId.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);
         }                                                                                                              
      }
   }
}

HWND TeacherRenderWdg::GetSmallRenderRenderWndID() {
   HWND id = (HWND)(ui.widget_samllRenderWidget->winId());
   return id;
}

std::shared_ptr<vhall::VideoRenderReceiveInterface> TeacherRenderWdg::GetSmallRenderRenderWnd()
{
   return ui.widget_samllRenderWidget->GetVideoReciver();
}

void TeacherRenderWdg::SetNickName(QString nickName) {
   if (nickName.isEmpty()) {
      return;
   }
   
   mNickName = nickName;
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetUserName(mNickName);
   }
}

QString TeacherRenderWdg::NickName()
{
   return mNickName;
}

void TeacherRenderWdg::SetRoleType(int roleType) {
   mRoleType = roleType;
}

void TeacherRenderWdg::SetMultiCameraState(QString user_id,bool open){
	mbCameraIsOpen = open;
   emit sig_cameraStateChange(mbCameraIsOpen);
	globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, mbCameraIsOpen);
	ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	if (mbCameraIsOpen) {
      //HWND big_wnd_id = GetRenderWndID(); 
      //HWND small_wnd_id = GetSmallRenderRenderWndID();
      std::shared_ptr<vhall::VideoRenderReceiveInterface> big_wnd_id  = GetRenderWnd();
      std::shared_ptr<vhall::VideoRenderReceiveInterface> small_wnd_id = GetSmallRenderRenderWnd();

		//SetTeacherCameraOpenSheet();
     //学员接收到讲师打开摄像头。
      if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
         //如果学员在大画面,讲师在小窗。把小窗关闭。
         setStackedSmallIndex(0);
         //globalToolManager->GetClassSDK()->StartRenderRemoteStream(user_id.toStdWString(),VHStreamType_AVCapture,(void*)small_wnd_id) ;
         globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(user_id.toStdWString(), VHStreamType_AVCapture, small_wnd_id);
      }
      else {
         //globalToolManager->GetClassSDK()->StartRenderRemoteStream(user_id.toStdWString(), VHStreamType_AVCapture, (void*)big_wnd_id);
         globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(user_id.toStdWString(), VHStreamType_AVCapture, big_wnd_id);
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
      }
	}
	else {
		//SetTeacherCameraCloseSheet();
      if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
         //如果学员在大画面,讲师在小窗。把小窗关闭。
         setStackedSmallIndex(1);
      }
      else {
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
      }
	}
}

void TeacherRenderWdg::SetCameraState(bool open) {
   
   mbCameraIsOpen &= open;
   emit sig_cameraStateChange(mbCameraIsOpen);
   globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, mbCameraIsOpen);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (mbCameraIsOpen) {
      SetTeacherCameraOpenSheet();
   }
   else {
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
         SetTeacherCameraCloseSheet();
      }
      else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && open) {
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
      }
      else {
         SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
      }
   }
   
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->SetCameraState(mbCameraIsOpen);
   }
   if (ui.mBigRenderCtrl) {
      ui.mBigRenderCtrl->SetCameraState(mbCameraIsOpen);
   }
}

void TeacherRenderWdg::SetTeacherCameraCloseSheet() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.bigScreen == 0 
      || (QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id  && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher)
      || (QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id  && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student)) {
      //ui.widget_render->setStyleSheet(CLOSE_CAMERA_SHEET);
      SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
      ui.widget_render->update();
      update();
   }
   else {
      setStackedSmallIndex(1);
   }
}

void TeacherRenderWdg::SetTeacherCameraOpenSheet() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL || //小班课
      (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && //公开课
      (baseInfo.bigScreen == 0 || (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id))//当前用户在大画面
         )) {
      SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
      globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, TeacherRenderWdg_Page_Index_render);
      //ui.widget_render->setStyleSheet("");
   }
   else {//当前用户在小画面
      setStackedSmallIndex(0);   
   }

}

bool TeacherRenderWdg::GetCameraState() {
   return mbCameraIsOpen;
}

void TeacherRenderWdg::slot_OnStuClickedCamera() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      //大班课时只有学员自己显示此按键，所以是针对自己操作。
      if (HasSmallWndVideoFormat()) {
         if (globalToolManager->GetDataManager()->GetMuteCameraByTeacherState()) {
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_CannotOpearte));
            return;
         }
         globalToolManager->GetDataManager()->WriteLog("slot_OnStuClickedCamera big %s  --- my %s", QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());

         if (mbSmallRenderCameraIsOpen) {
            globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
            globalToolManager->GetClassSDK()->CloseCamera();
            globalToolManager->GetDataManager()->SetMyCameraOpen(false);
            mbSmallRenderCameraIsOpen = false;
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
               //学员是在小画面
               setStackedSmallIndex(1);
            }
            else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera, true);
            }
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
            
            globalToolManager->GetDataManager()->WriteLog("%s OpenCamera()", __FUNCTION__);
            bool bRet = globalToolManager->GetClassSDK()->OpenCamera();
            if (bRet) {
               globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() Succeed", __FUNCTION__);
            }
            else {
               globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() failed", __FUNCTION__);
            }

            globalToolManager->GetDataManager()->SetMyCameraOpen(true);

            mbSmallRenderCameraIsOpen = true;
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
               //学员是在小画面
               setStackedSmallIndex(0);
               globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetSmallRenderRenderWnd());
            }
            else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render, true);
            }
         }
         if (ui.mpVhallRenderMaskWdg) {
            ui.mpVhallRenderMaskWdg->SetSmallRenderCameraState(mbSmallRenderCameraIsOpen);
         }
         if (ui.mBigRenderCtrl) {
            ui.mBigRenderCtrl->SetCameraState(mbSmallRenderCameraIsOpen);
         }
      }
   }
   else {
      //小班课时员自己显示此按键。 主持人也可以显示此按键。
      if (baseInfo.mUserInfo.join_id == GetUserJoinID().toStdString()) {
         //学员自己执行
         if (HasVideoFormat()) {
            if (globalToolManager->GetDataManager()->GetMuteCameraByTeacherState() && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
               globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_CannotOpearte));
               return;
            }
            //讲师
            globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, mbCameraIsOpen);
            if (mbCameraIsOpen) {
               globalToolManager->GetClassSDK()->OperateUserCamera(baseInfo.mUserInfo.join_id, true);
               globalToolManager->GetClassSDK()->CloseCamera();
               globalToolManager->GetDataManager()->SetMyCameraOpen(false);
               mbCameraIsOpen = false;
               emit sig_cameraStateChange(mbCameraIsOpen);
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
            }
            else {
               globalToolManager->GetClassSDK()->OperateUserCamera(baseInfo.mUserInfo.join_id, false);
               globalToolManager->GetDataManager()->WriteLog("%s OpenCamera()", __FUNCTION__);
               bool bRet = globalToolManager->GetClassSDK()->OpenCamera();
               if (bRet) {
                  globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() Succeed", __FUNCTION__);
               }
               else {
                  globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() failed", __FUNCTION__);
               }

               globalToolManager->GetDataManager()->SetMyCameraOpen(true);

               mbCameraIsOpen = true;
               emit sig_cameraStateChange(mbCameraIsOpen);
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);

               WebinarInfo info = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo;
               if (info.course_type == CLASS_TYPE_SMALL)
                  OpenCamera();
            }
            if (ui.mpVhallRenderMaskWdg) {
               ui.mpVhallRenderMaskWdg->SetSmallRenderCameraState(mbSmallRenderCameraIsOpen);
            }
         }
      }
      else {
         if (HasVideoFormat()) {
            if (mbCameraIsOpen) {
               globalToolManager->GetClassSDK()->OperateUserCamera(GetUserJoinID().toStdString(), true);
            }
            else {
               globalToolManager->GetClassSDK()->OperateUserCamera(GetUserJoinID().toStdString(), false);
            }
         }
      }
   }
}

void TeacherRenderWdg::ChangeBigScreen(QString bigScreenId) {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);
   mStrBigScreenId = bigScreenId.trimmed();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();//当前用户被设置为大画面
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {   //  讲师   （自己）   
      QString small_user_id = GetSmallWndUserJoinID(); //订阅学员id
      if (bigScreenId.toStdString() == baseInfo.mUserInfo.join_id) { //讲师（自己） 将设置为大画面

         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetRenderWnd());
         if (globalToolManager->GetDataManager()->IsMyCameraOpen()) {
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
         }
         else {
            //globalToolManager->GetClassSDK()->CloseCamera();
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
            ui.widget_render->update();
            update();
         }

         if(!ui.widget_small_render->isHidden()){
            globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(small_user_id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);
            globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(small_user_id.toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());  //学员小窗
            if (mbSmallRenderCameraIsOpen) {
               setStackedSmallIndex(0);
               globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s small", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
				//globalToolManager->GetClassSDK()->StartRenderRemoteStream(small_user_id.toStdWString(), VHStreamType_AVCapture, small_wnd_id);  //学员小窗
				
			}
            else {
               //globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(small_user_id.toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());  //学员小窗
               setStackedSmallIndex(1);
            }
         }


      } else{//讲师（自己） 将设置为小画面     学员到大画面
         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(small_user_id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);

         globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(small_user_id.toStdWString(), VHStreamType_AVCapture, GetRenderWnd()); //学员大窗
         if (mbSmallRenderCameraIsOpen) {//学员是打开状态
            //SetTeacherCameraOpenSheet();
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render, false);
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s big", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
			//globalToolManager->GetClassSDK()->StartRenderRemoteStream(small_user_id.toStdWString(), VHStreamType_AVCapture, big_wnd_id); //学员大窗
			
         }
         else {
            //globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(small_user_id.toStdWString(), VHStreamType_AVCapture, GetRenderWnd()); //学员大窗
            //globalToolManager->GetClassSDK()->StopRenderRemoteStream(small_user_id.toStdWString(),"", VHStreamType_AVCapture); //学员大窗
            //ui.widget_render->setStyleSheet(CLOSE_CAMERA_SHEET);
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
            ui.widget_render->update();
            update();
         }

         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetSmallRenderRenderWnd());
         if (globalToolManager->GetDataManager()->IsMyCameraOpen()) {
            setStackedSmallIndex(0);
            //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, small_wnd_id);   //主持人小窗
            
         }
         else {
            //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, small_wnd_id);   //主持人小窗
            setStackedSmallIndex(1);
         }
      }
   } 
   else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {//////         学员
      QString big_user_id = GetUserJoinID();
      if (bigScreenId.toStdString() == baseInfo.mUserInfo.join_id) {//学员（自己）   即被设置为了大屏。
         if (globalToolManager->GetDataManager()->IsMyCameraOpen()) {//学员（自己） 
            //SetTeacherCameraOpenSheet(); 
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
         }
         else {
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
            //globalToolManager->GetClassSDK()->CloseCamera();
            //SetTeacherCameraCloseSheet();
         }
         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetRenderWnd());  //学员大窗

         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(big_user_id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_SmallStream);
         globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(big_user_id.toStdWString(), VHStreamType_AVCapture, GetSmallRenderRenderWnd());   //主持人小窗
         if (globalToolManager->GetDataManager()->IsMultiClassOtherUserCameraOpen()) {//讲师（对方） 摄像头打开
            setStackedSmallIndex(0);
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream small id %s  my id :%s small", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
         }
         else {//讲师（对方） 摄像头关闭
            //globalToolManager->GetClassSDK()->StopRenderRemoteStream(big_user_id.toStdWString(),"",VHStreamType_AVCapture);
            setStackedSmallIndex(1);
         }
      }
      else {//学员即被设置为了小屏
         globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(big_user_id.toStdWString(), VHStreamType_AVCapture, GetRenderWnd());  //主持人大窗
         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(big_user_id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);

         if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI)//公开课
         {
            if (globalToolManager->GetDataManager()->IsMultiClassOtherUserCameraOpen()) {//判断(讲师)对方  摄像头打开
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);

               globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s small", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
            }
            else {
               //globalToolManager->GetClassSDK()->StopRenderRemoteStream(big_user_id.toStdWString(), "", VHStreamType_AVCapture);
               SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
               ui.widget_render->update();
               update();
            }
         }

         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetSmallRenderRenderWnd());
         if (globalToolManager->GetDataManager()->IsMyCameraOpen()) {
            setStackedSmallIndex(0);
         }
         else {
            //globalToolManager->GetClassSDK()->CloseCamera();
            setStackedSmallIndex(1);
         }
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);
}

void TeacherRenderWdg::slot_OnStuClickedMic() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      //大班课时只有学员自己显示此按键，所以是针对自己操作。
      if (HasSmallWndAudioFormat()) {
         if (globalToolManager->GetDataManager()->GetMuteMicByTeacherState()) {
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_CannotOpearte));
            return;
         }
         if (mbSmallRenderMicIsOpen) {
            globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
            globalToolManager->GetClassSDK()->CloseMic();
            mbSmallRenderMicIsOpen = false;
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
            globalToolManager->GetClassSDK()->OpenMic();
            mbSmallRenderMicIsOpen = true;
         }

         if (ui.mpVhallRenderMaskWdg) {
            ui.mpVhallRenderMaskWdg->SetSmallRenderMicState(mbSmallRenderMicIsOpen);
         }
         if (ui.mBigRenderCtrl) {
            ui.mBigRenderCtrl->SetMicState(mbSmallRenderMicIsOpen);
         }
      }
   }
   else {
      //小班课时员自己显示此按键。 主持人也可以显示此按键。
      if (baseInfo.mUserInfo.join_id == GetUserJoinID().toStdString()) {
         if (HasAudioFormat()) {
            //学员被静音不能自己打开。

            if (globalToolManager->GetDataManager()->GetMuteMicByTeacherState() && baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) {
               globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_CannotOpearte));
               return;
            }
            if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student && baseInfo.mutingAll && !mbMicIsOpen)
            {
               globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, Str_CannotOpearte));
               return;
            }
			if (ui.mpVhallRenderMaskWdg) {
				ui.mpVhallRenderMaskWdg->SetMicState(mbMicIsOpen);
			}

            if (mbMicIsOpen) {
               globalToolManager->GetClassSDK()->OperateUserMic(baseInfo.mUserInfo.join_id, true);
               globalToolManager->GetClassSDK()->CloseMic();
               mbMicIsOpen = false;
            }
            else {
               globalToolManager->GetClassSDK()->OperateUserMic(baseInfo.mUserInfo.join_id, false);
               globalToolManager->GetClassSDK()->OpenMic();
               mbMicIsOpen = true;
            }

         }
      }
      else {
         if (HasAudioFormat()) {
            if (mbMicIsOpen) {
               globalToolManager->GetClassSDK()->OperateUserMic(GetUserJoinID().toStdString(), true);
            }
            else {
               globalToolManager->GetClassSDK()->OperateUserMic(GetUserJoinID().toStdString(), false);
            }
         }
      }
   }
}

void TeacherRenderWdg::slot_OnstuClickedUnPublish() {
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      //讲师下麦用户，如果用户在线，发送消息。不在线强制下麦
      QString userId = GetUserJoinID();
      HandleUnPublishUser(userId);
   }
   else {
      QString userId;
	  ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	  if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
		  userId = GetUserJoinID();
	  }
	  else {
		  userId = QString::fromStdString( baseInfo.mUserInfo.join_id);
	  } 
	  
      globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish, %s) 1404 PushStreamEvent_UnPublish", __FUNCTION__, userId.toStdString().c_str());
      globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish, userId.toStdString());
   }
}

void TeacherRenderWdg::slot_OnStuPenClickedPen(bool hasPen) {
   if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
      //讲师下麦用户，如果用户在线，发送消息。不在线强制下麦
      QString userId = GetUserJoinID();
      if (hasPen) {
         globalToolManager->GetClassSDK()->ChangeUserPenAuth(userId.toStdString(), false);
      }
      else {
         globalToolManager->GetClassSDK()->ChangeUserPenAuth(userId.toStdString(), true);//授权画笔
      }
   }
}

void TeacherRenderWdg::slot_OnStuClickedFloat() {
   //公开课自己是老师，
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      //公开课浮窗都显示在单独窗体里。
      //globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEvent(CustomEvent_FloatType));
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_FloatType, "") /*new QEvent(CustomEvent_FloatType)*/);
      return;
   }

   QString join_id = GetUserJoinID();
   QString stream_id = GetStreamID();
   if (join_id.isEmpty() /*|| stream_id.isEmpty()*/) {
      return;
   }
   else {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_FloatType_Renders, join_id));
      
   }
    
   //return;
   //  if (mTeacherCtrlWdg) {
       //globalToolManager->GetDataManager()->WriteLog("%s 829 showCtrlWdg(false)", __FUNCTION__);
   //      showCtrlWdg(false);
   //      //qDebug() << "hide";
   //  }
     //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FLOAT_RENDER, join_id);
   //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   //WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
   ////FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);

   //if (wnd) {
   //   CSTDMapParam mapParam;
   //   mapParam[Str_Function] = Str_Function_FloadAndFullWndRender;
   //   mapParam[USER_ID] = join_id;
   //   mapParam[STREAM_ID] = stream_id;
   //   mapParam[NICK_NAME] = mNickName;
   //   mapParam[ROLE_TYPE] = QString::number(mRoleType);
   //   mapParam[TROPHY_COUNT] = QString::number(mTrophyRewardNum);
   //   mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = mbCameraIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
   //   mapParam[BIG_RENDER_MIC_IS_CLOSE] = mbMicIsOpen == false ? PARAM_TRUE : PARAM_FALSE;

	  //wnd->SetRenderMaskWdg(ui.mpVhallRenderMaskWdg);
   //   wnd->Notice(mapParam);
	  //wnd->SetMicState(mbMicIsOpen);
   //   wnd->show();
   //   wnd->raise();
   //}

   //hide();

   CBaseWnd *mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   if (mainwnd) {
      CSTDMapParam mapParam;
      mapParam[TEACHER_VIEW] = PARAM_TRUE;
      mapParam[USER_ID] = join_id;
      mainwnd->Notice(mapParam);
   }
   emit sig_ClickedFloatRender(join_id, stream_id, mNickName, mRoleType);
}

void TeacherRenderWdg::slot_OnStuClickedFullWnd() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
      //公开课浮窗都显示在单独窗体里。
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_FullType));
      return;
   }

   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI || (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && (mWndPos == InsertPos_Speaker || mWndPos == InsertPos_WndManager))) {
      QString join_id = GetUserJoinID();
      QString stream_id = GetStreamID();
      if (join_id.isEmpty() || stream_id.isEmpty()) {
         return;
      }
     // hide();
      //     if (mTeacherCtrlWdg) {
            //globalToolManager->GetDataManager()->WriteLog("%s 872 showCtrlWdg(false)", __FUNCTION__);
      //         showCtrlWdg(false);
      //         //qDebug() << "hide";
      //     }
      //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, join_id);
      //if (mpFullRender) {
      //   CSTDMapParam mapParam;
      //   mapParam[Str_Function] = Str_Function_FloadAndFullWndRender;
      //   mapParam[USER_ID] = join_id;
      //   mapParam[STREAM_ID] = stream_id;
      //   mapParam[NICK_NAME] = mNickName;
      //   mapParam[ROLE_TYPE] = QString::number(mRoleType);
      //   mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = mbCameraIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
      //   mapParam[BIG_RENDER_MIC_IS_CLOSE] = mbMicIsOpen == false ? PARAM_TRUE : PARAM_FALSE;

      //   if (mbHasRenderSmallWnd && !GetSmallWndUserJoinID().isEmpty() && !GetSmallWndStreamID().isEmpty()) {
      //      mapParam[SMALL_RENDER_USER_ID] = GetSmallWndUserJoinID();
      //      mapParam[SMALL_RENDER_STEAM_ID] = GetSmallWndStreamID();
      //   }

      //   mpFullRender->Notice(mapParam);
      //   showFullRender();
      //   //wnd->show();
      //   //wnd->raise();
      //}
   }
   else {
      QString join_id = GetSmallWndUserJoinID();
      QString stream_id = GetSmallWndStreamID();
      if (join_id.isEmpty() || stream_id.isEmpty()) {
         return;
      }
      //hide();
      //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, join_id);
      //if (mpFullRender) {
      //   CSTDMapParam mapParam;
      //   mapParam[Str_Function] = Str_Function_FloadAndFullWndRender;
      //   mapParam[USER_ID] = join_id;
      //   mapParam[STREAM_ID] = stream_id;
      //   mapParam[NICK_NAME] = mNickName;
      //   mapParam[ROLE_TYPE] = QString::number(mRoleType);
      //   mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = mbCameraIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
      //   mapParam[BIG_RENDER_MIC_IS_CLOSE] = mbMicIsOpen == false ? PARAM_TRUE : PARAM_FALSE;

      //   if (mbHasRenderSmallWnd && !GetSmallWndUserJoinID().isEmpty() && !GetSmallWndStreamID().isEmpty()) {
      //      mapParam[SMALL_RENDER_USER_ID] = GetSmallWndUserJoinID();
      //      mapParam[SMALL_RENDER_STEAM_ID] = GetSmallWndStreamID();
      //   }
      //   mpFullRender->Notice(mapParam);
      //   //wnd->show();
      //   showFullRender();

      //}
   }
}

void TeacherRenderWdg::slot_OnClickedStudentMic() {
   QString user_id = GetSmallWndUserJoinID();
   if (!user_id.isEmpty()) {
      if (ui.mBigRenderCtrl) {
         if (ui.mBigRenderCtrl->mbStudentMicOpen) {
            globalToolManager->GetClassSDK()->OperateUserMic(user_id.toStdString(), true);
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserMic(user_id.toStdString(), false);
         }
      }
      else {
         if (mbSmallRenderMicIsOpen) {
            globalToolManager->GetClassSDK()->OperateUserMic(user_id.toStdString(), true);
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserMic(user_id.toStdString(), false);
         }
      }
   }
}

void TeacherRenderWdg::slot_OnClickedStudentCamera() {
   QString user_id = GetSmallWndUserJoinID();
   if (!user_id.isEmpty()) {
      if (ui.mBigRenderCtrl) {
         if (ui.mBigRenderCtrl->mbStudentCameraOpen) {
            globalToolManager->GetClassSDK()->OperateUserCamera(user_id.toStdString(), true);
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserCamera(user_id.toStdString(), false);
         }
      }
      else {
         if (mbSmallRenderCameraIsOpen) {
            globalToolManager->GetClassSDK()->OperateUserCamera(user_id.toStdString(), true);
         }
         else {
            globalToolManager->GetClassSDK()->OperateUserCamera(user_id.toStdString(), false);
         }
      }
   }
}

void TeacherRenderWdg::slot_OnUnPublishStudent() {
   QString user_id = GetSmallWndUserJoinID();
   if (!user_id.isEmpty()) {
      HandleUnPublishUser(user_id);
   }
}

void TeacherRenderWdg::slot_OnChangeCamera() {
   globalToolManager->GetDataManager()->WriteLog(" Enter %s", __FUNCTION__);
   globalToolManager->GetClassSDK()->ChangeCamera();
   globalToolManager->GetDataManager()->WriteLog(" Leave %s", __FUNCTION__);
}

void TeacherRenderWdg::SetMicState(bool open) {
   this;
   mbMicIsOpen = open;
   //if (ui.mpVhallRenderMaskWdg) {
   //}
   if (ui.mBigRenderCtrl) {
      ui.mBigRenderCtrl->SetMicState(mbMicIsOpen);
   }
   //QString join_id = GetUserJoinID();
   //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   //WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
   //if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(join_id)) {
	  // FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);
	  // if (wnd) {
		 //  //wnd->hide();
		 //  wnd->SetMicState(mbMicIsOpen);
	  // }
   //}

   if (ui.mpVhallRenderMaskWdg) {
	   ui.mpVhallRenderMaskWdg->SetMicState(mbMicIsOpen);
	   ui.mpVhallRenderMaskWdg->SetShowNoMic(!open);

	   CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	   WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
	   QString userId = GetUserJoinID();
	/*   if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
		   FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);
		   if (wnd) {
			   wnd->SetShowNoMic(!open);
		   }
	   }*/

	   //QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
	   //if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
		  // FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
		  // if (wnd) {
			 //  wnd->SetShowNoMic(!open);
		  // }
	   //}

      //CreateFullRender();
      //if (nullptr != mpFullRender) {
      //   mpFullRender->SetShowNoMic(!open);
      //}
   }
}

void TeacherRenderWdg::SetMultiMicState(bool open, bool bVideo/* = false*/)
{
	mbMicIsOpen = open;
	//if (mpVhallRenderMaskWdg) {
	//}
	if (!bVideo)
	{
		if (ui.mBigRenderCtrl) {
			ui.mBigRenderCtrl->SetMicState(mbMicIsOpen);
		}
		/*QString join_id = GetUserJoinID();
		CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;*/
		//if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(join_id)) {
		//	FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);
		//	if (wnd) {
		//		//wnd->hide();
		//		wnd->SetMicState(mbMicIsOpen);
		//	}
		//}
	}

	if (ui.mpVhallRenderMaskWdg) {
		ui.mpVhallRenderMaskWdg->SetShowNoMic(!open);

		CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
		QString userId = GetUserJoinID();
		//if (pWebRtcLiveWdg && pWebRtcLiveWdg && pWebRtcLiveWdg->IsExitFloatRender(userId)) {
		//	FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(userId);
		//	if (wnd) {
		//		wnd->SetShowNoMic(!open);
		//	}
		//}

		//QString fullWndID = WND_ID_FULL_SCREEN_RENDER + userId;
		//if (globalToolManager->GetWndManager()->IsWndExist(fullWndID)) {
		//	FullScreenRender *wnd = (FullScreenRender*)globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, userId);
		//	if (wnd) {
		//		wnd->SetShowNoMic(!open);
		//	}
		//}

      //CreateFullRender();
      //mpFullRender->SetShowNoMic(!open);
	}
}

void TeacherRenderWdg::ShowAllMuteCtrl(bool show) {
   if (ui.mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->ShowStudentCtrl(show);
   }
}

bool TeacherRenderWdg::GetBEnter()
{
   return mEnter;
}

HWND TeacherRenderWdg::GetRenderWndID() {
   return (HWND)(ui.widget_render->winId());
}

std::shared_ptr<vhall::VideoRenderReceiveInterface> TeacherRenderWdg::GetRenderWnd()
{
   //if (mpFullRender && mpFullRender->isVisible()) {
   //   return mpFullRender->GetRenderWnd();
   //}
   //else {
      return ui.widget_render->GetVideoReciver();
   //}
   
}

//bool TeacherRenderWdg::eventFilter(QObject * receiver, QEvent * event) {
//    //if (receiver == ui.stackedWidget) {
//    //    if (event->type() == QEvent::Enter) {
//    //        //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
//    //        /*if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS && mTeacherCtrlWdg != nullptr) {
//    //            int w = this->width();
//    //            int h = this->height();
//    //            if (mTeacherCtrlWdg && mTeacherCtrlWdg->width() != w && mTeacherCtrlWdg->height() != h) {
//    //                mTeacherCtrlWdg->setMinimumWidth(this->width());
//    //                mTeacherCtrlWdg->setMinimumHeight(this->height());
//    //                mTeacherCtrlWdg->setMaximumWidth(this->width());
//    //                mTeacherCtrlWdg->setMaximumHeight(this->height());
//    //            }
//    //            if (mTeacherCtrlWdg) {
//				//	globalToolManager->GetDataManager()->WriteLog("%s 1019 showCtrlWdg(true)", __FUNCTION__);
//    //              showCtrlWdg(true);
//    //            }
//    //        }*/
//    //    }
//    //}
//    /*else if (receiver == mTeacherCtrlWdg) {
//        if (event->type() == QEvent::Leave || event->type() == QEvent::FocusOut) {
//            ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo(); 
//            globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, mbCameraIsOpen);
//            if (baseInfo.mWebinarInfo.type == CLASS_STATUS_IN_CLASS) {
//                if (mbCameraIsOpen) {
//                    SetStackCurrentIndex(TeacherRenderWdg_Page_Index_render);
//                    ui.widget_render->setStyleSheet("");
//                }
//                else {
//                    if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
//                        ui.widget_render->setStyleSheet(CLOSE_CAMERA_SHEET);
//                        ui.widget_render->update();
//                        update();
//                    }
//                    else {
//                        SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
//                    }
//                }
//            }
//            else {
//                SetStackCurrentIndex(TeacherRenderWdg_Page_Index_classNoStart);
//            }
//            if (mTeacherCtrlWdg) {
//				globalToolManager->GetDataManager()->WriteLog("%s 1049 showCtrlWdg(false)", __FUNCTION__);
//                showCtrlWdg(false);
//            }
//        }
//    }*/
//    return QWidget::eventFilter(receiver, event);
//}

//void TeacherRenderWdg::showEven(QShowEvent* e)
//{
//	//showTrophyWdg();
//	QWidget::showEvent(e);
//}

//void TeacherRenderWdg::hideEvent(QHideEvent *e)
//{
//	if (NULL != mpTrophyWdg)
//	{
//		mpTrophyWdg->hide();
//	}
//	QWidget::hideEvent(e);
//}

void TeacherRenderWdg::slot_OnClickedFloatRender(bool bShowTitle /*= true*/) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) 
   {
      //公开课浮窗都显示在单独窗体里。
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_FloatType, "", bShowTitle) /*new QEvent(CustomEvent_FloatType)*/);
      return;
   }

   QString join_id = GetUserJoinID();
   QString stream_id = GetStreamID();
   
   if (join_id.isEmpty() || stream_id.isEmpty()) {
      return;
   }
   else {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_FloatType_Renders, join_id, bShowTitle));

   }

   //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FLOAT_RENDER, join_id);
   //CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   //WebRtcLiveWdg* pWebRtcLiveWdg = (WebRtcLiveWdg*)pWdg;
   //FloatRender* wnd = pWebRtcLiveWdg->GetFloatRender(join_id);
   //if (wnd) {
   //   //if (NULL != mTeacherCtrlWdg && mTeacherCtrlWdg->isVisible())
   //   //{
   //   //	mTeacherCtrlWdg->hide();
   //   //}
   //   CSTDMapParam mapParam;
   //   mapParam[USER_ID] = join_id;
   //   mapParam[STREAM_ID] = stream_id;
   //   mapParam[NICK_NAME] = mNickName;
   //   mapParam[ROLE_TYPE] = QString::number(mRoleType);
   //   wnd->Notice(mapParam);
	  //wnd->SetMicState(mbMicIsOpen );
	  //wnd->SetRenderMaskWdg( ui.mpVhallRenderMaskWdg );
   //   wnd->show();
   //}
   //hide();
   CBaseWnd *mainwnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   if (mainwnd) {
      CSTDMapParam mapParam;
      mapParam[TEACHER_VIEW] = PARAM_TRUE;
      mapParam[USER_ID] = join_id;
      mainwnd->Notice(mapParam);
   }

   //slot_NoticeShowNickName();
}

void TeacherRenderWdg::slotMuteSelft(bool bMute) {
   bMute ? CloseMic() : OpenMic();
}

void TeacherRenderWdg::CloseMic() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
      globalToolManager->GetClassSDK()->CloseMic();
      mbMicIsOpen = false;
      //if (ui.mpVhallRenderMaskWdg) {
         ui.mpVhallRenderMaskWdg->SetMicState(mbMicIsOpen);
      //}
   }
}

void TeacherRenderWdg::OpenMic()
{
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
      globalToolManager->GetClassSDK()->OpenMic();
      mbMicIsOpen = true;
      //if (mpVhallRenderMaskWdg) {
         ui.mpVhallRenderMaskWdg->SetMicState(mbMicIsOpen);
      //}
   }
}



void TeacherRenderWdg::slot_OnClickedMic() {
   if (mbMicIsOpen) {
      CloseMic();
   }
   else {
      OpenMic();
   }
}

void TeacherRenderWdg::slot_OnClickedCamera() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      if (mbCameraIsOpen) {
         
         if (globalToolManager->GetClassSDK()->CloseCamera()) {
            globalToolManager->GetDataManager()->SetMyCameraOpen(false);
            globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
         }
         
         mbCameraIsOpen = false;
         emit sig_cameraStateChange(mbCameraIsOpen);
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
            SetTeacherCameraCloseSheet();
         }
         else {
            SetStackCurrentIndex(TeacherRenderWdg_Page_Index_NoCamera);
            globalToolManager->GetDataManager()->WriteLog("%s hasVideo:%d", __FUNCTION__, TeacherRenderWdg_Page_Index_NoCamera);
         }
      }
      else {
         OpenCamera();
      }
      SetCameraState(mbCameraIsOpen);
   }
}
void TeacherRenderWdg::OpenCamera()
{
   globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
   globalToolManager->GetDataManager()->WriteLog("%s OpenCamera()", __FUNCTION__);

   bool bRet = globalToolManager->GetClassSDK()->OpenCamera();
   if (bRet) {
      globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() Succeed", __FUNCTION__);
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() failed", __FUNCTION__);
   }

   globalToolManager->GetDataManager()->SetMyCameraOpen(true);

   mbCameraIsOpen = true;
   emit sig_cameraStateChange(mbCameraIsOpen);
   SetTeacherCameraOpenSheet();
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
      //HWND big_wnd_id = GetRenderWndID();
      //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, big_wnd_id);
      globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, GetRenderWnd());
   }
}

void TeacherRenderWdg::slot_OnClickedAllMute() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
      //小班课全员静音
      bool mute = baseInfo.mutingAll == 1 ? false : true;
      globalToolManager->GetClassSDK()->OperateAllUserMic(mute);
   }
   else {
      //公开课指定下麦
      QString userId = GetSmallWndUserJoinID();
      bool isOpen = GetSmallRenderMicState();
      globalToolManager->GetClassSDK()->OperateUserMic(userId.toStdString(), !isOpen);
   }
}

void TeacherRenderWdg::slot_OnAllUnpublish() {
	globalToolManager->GetDataManager()->WriteLog("%s XK", __FUNCTION__);

   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
      //小班课全员下麦
      globalToolManager->GetClassSDK()->KickAllUnPublish();
   }
   else {
      //公开课指定下麦
      QString userId = GetSmallWndUserJoinID();
      HandleUnPublishUser(userId);
   }
}

void TeacherRenderWdg::setGigRenderCtrlGeometry()
{
	/*int iW = this->width();
	ui.mBigRenderCtrl->setFixedWidth(iW);*/
	QPoint pos = ui.stackedWidget->mapToGlobal(ui.stackedWidget->pos());
	int ix =/* pos.rx() +*/ (ui.stackedWidget->width() - ui.mBigRenderCtrl->width()) / 2;
	int iy = /*pos.ry() +*/ ui.stackedWidget->height() - ui.mBigRenderCtrl->height() - 60;
	ui.mBigRenderCtrl->move(ix, iy);
	//ui.mBigRenderCtrl->setGeometry(ix, iy, 746, 60);
}

void TeacherRenderWdg::HandleUnPublishUser(const QString& userId) {
   globalToolManager->GetDataManager()->WriteLog("%s  Enter", __FUNCTION__);

   bool userOnline = true;
   CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
   if (wnd) {
      CSTDMapParam param;
      param[USER_ID] = userId;
      param[USER_LINE_STATE];
      wnd->GetParam(param);
      QString onLine = param[USER_LINE_STATE];
      userOnline = onLine.toInt() == 1 ? true : false;
   }
   if (userOnline) {

      globalToolManager->GetDataManager()->WriteLog("%s  KickUserPublish %s", __FUNCTION__, userId.toStdString().c_str());
      globalToolManager->GetClassSDK()->KickUserPublish(userId.toStdString());
   }
   else {
      globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish, %s) 1984 PushStreamEvent_UnPublish", __FUNCTION__, userId.toStdString().c_str());
      globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish, userId.toStdString());
   }
   globalToolManager->GetDataManager()->WriteLog("%s  Leave", __FUNCTION__);

}

void TeacherRenderWdg::slot_OnClickedFullScreen() {
   globalToolManager->GetDataManager()->WriteLog(" Enter %s", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) 
   {
      //公开课浮窗都显示在单独窗体里。
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEvent(CustomEvent_FullType));
      return;
   }

   
 //  QString join_id = GetUserJoinID();
 //  QString stream_id = GetStreamID();
 //  //CBaseWnd *wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_FULL_SCREEN_RENDER, join_id);
 //  //CreateFullRender();

 //  if (mpFullRender) {
 //     CSTDMapParam mapParam;
 //     mapParam[Str_Function] = Str_Function_FloadAndFullWndRender;
 //     mapParam[SHOW_NICK_NAME] = ui.mpVhallRenderMaskWdg->GetName();
 //     mapParam[USER_ID] = join_id;
 //     mapParam[STREAM_ID] = stream_id;
 //     mapParam[BIG_RENDER_CAMERA_IS_CLOSE] = mbCameraIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
 //     mapParam[BIG_RENDER_MIC_IS_CLOSE] = mbMicIsOpen == false ? PARAM_TRUE : PARAM_FALSE;

 //     globalToolManager->GetDataManager()->WriteLog("  %s mbHasRenderSmallWnd %d GetSmallWndUserJoinID [%s] GetSmallWndStreamID [%s]", __FUNCTION__,
 //        mbHasRenderSmallWnd, GetSmallWndUserJoinID().toStdString().c_str(), GetSmallWndStreamID().toStdString().c_str());

 //     if (mbHasRenderSmallWnd && !GetSmallWndUserJoinID().isEmpty() && !GetSmallWndStreamID().isEmpty()) {
 //        mapParam[SMALL_RENDER_CAMERA_IS_CLOSE] = mbSmallRenderCameraIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
 //        mapParam[SMALL_RENDER_MIC_IS_CLOSE] = mbSmallRenderMicIsOpen == false ? PARAM_TRUE : PARAM_FALSE;
 //        mapParam[SMALL_RENDER_USER_ID] = GetSmallWndUserJoinID();
 //        mapParam[SMALL_RENDER_STEAM_ID] = GetSmallWndStreamID();
 //     }
 //     else {
 //        mapParam[SMALL_RENDER_USER_ID] = "";
 //        mapParam[SMALL_RENDER_STEAM_ID] = "";
 //        globalToolManager->GetDataManager()->WriteLog("  %s empty", __FUNCTION__);
 //     }


	///*  if (wnd) {
	//	  FullScreenRender* pWdg =  (FullScreenRender*)wnd;
	//	  if (pWdg) {
	//		  pWdg->SetRenderMaskWdg(ui.mpVhallRenderMaskWdg);
	//	  }
	//  }*/
 //     mpFullRender->SetRenderMaskWdg(ui.mpVhallRenderMaskWdg);
 //     mpFullRender->Notice(mapParam);

 //     //showFullRender();
 //     //wnd->show();
 //     //QDesktopWidget * deskTop = QApplication::desktop();
 //     //int curMonitor = deskTop->screenNumber(this); // 参数是一个QWidget*
 //     //QRect DesktopRect = deskTop->screenGeometry(curMonitor);

 //     //QRect rect = deskTop->availableGeometry(curMonitor);
 //     //globalToolManager->GetDataManager()->WriteLog("%s DesktopRect:%d,%d,%d,%d    Rect:%d,%d,%d,%d    ",
 //     //   __FUNCTION__,
 //     //   DesktopRect.x(), DesktopRect.y(), DesktopRect.width(), DesktopRect.height(),
 //     //   rect.x(), rect.y(), rect.width(), rect.height());
 //     //wnd->move(rect.x(), rect.y());
 //     //if (rect.height() < DesktopRect.height() || rect.width() < DesktopRect.width()) {
 //     //   wnd->setFixedSize(rect.width(), rect.height());
 //     //}
 //     //else {
 //     //   wnd->setFixedSize(rect.width(), rect.height() - 1);
 //     //}
 //     
 //     
 //     //hide();
 //  }
 //  else {
 //     globalToolManager->GetDataManager()->WriteLog("  %s wnd NULL ", __FUNCTION__);
 //  }
   globalToolManager->GetDataManager()->WriteLog(" Leave %s", __FUNCTION__);
}

void TeacherRenderWdg::slot_OnClickedAllMuteCamera() {

}

QPaintEngine *TeacherRenderWdg::paintEngine() const {
   return NULL;
}

void TeacherRenderWdg::HandleOpenClassUserLineState(QString id, bool onLine) {
   if (id == GetSmallWndUserJoinID()) {
      HandleUserLineState(onLine);
   }
}

void TeacherRenderWdg::HandleUserLineState(bool onLine) {
   if (mLineStateTimer) {
      mLineStateTimer->setSingleShot(true);
      if (onLine) {
         mLineStateTimer->stop();
      }
      else {
         mLineStateTimer->start(CHECK_ONLINE_STATE);
      }
   }
}

void TeacherRenderWdg::initTrophyRewardNum(const int & iNum) {
   //if (NULL != mpVhallRenderMaskWdg) {
      ui.mpVhallRenderMaskWdg->initTrophyRewardNum(iNum);
   //}
   mTrophyRewardNum = iNum;
}

//QString TeacherRenderWdg::UserId() {
//   return mUserId.trimmed();
//}


void TeacherRenderWdg::slot_OnSetBigScreen() {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   QString user_id;
   if (baseInfo.bigScreen == 0 || baseInfo.mUserInfo.join_id == QString::number(baseInfo.bigScreen).toStdString()) {
      //当前主持人在大画面，切到小画面
      user_id = GetSmallWndUserJoinID();

   } else {
     //主持人在小画面，切到大画面
      user_id = GetUserJoinID();
   }

   if (!user_id.isEmpty()) {
      globalToolManager->GetClassSDK()->SetBigScreen(user_id.toStdString(), [&](const std::string& user, int code, const std::string& msg) {
         globalToolManager->GetDataManager()->WriteLog(" code %d msg %s", code, msg.c_str());
         if (code != 0 && code != 200) {
            globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowFadOutTipMsg, QString::fromStdString(msg)));
            return;
         }
         globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ChangeBigScreen, QString::fromStdString(user)));
      });
   }
}

void TeacherRenderWdg::slotFavorStudent() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   std::string strId = GetUserJoinID().toStdString();
   if (JOIN_ROLE_Teacher == baseInfo.mUserInfo.role_type && baseInfo.mUserInfo.join_id != strId) {
      globalToolManager->GetClassSDK()->RewardStudent(strId);
   }
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void TeacherRenderWdg::SetStuSpacer(const int& iW)
{
	ui.mpVhallRenderMaskWdg->SetStuSpacer(iW);
}

//bool TeacherRenderWdg::IsFullRenderVisible()
//{
//   return mpFullRender && mpFullRender->isVisible();
//}

void TeacherRenderWdg::SetStackCurrentIndex(int idex, bool bsefl)
{
   globalToolManager->GetDataManager()->WriteLog("%s index %d", __FUNCTION__, idex);
   ui.stackedWidget->setCurrentIndex(idex);

   if (TeacherRenderWdg_Page_Index_render == idex && !mbCameraIsOpen && bsefl)
      OpenCamera();
   WebinarInfo info = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo;


   if (TeacherRenderWdg_Page_Index_classNoStart == idex) {
      ui.mpVhallRenderMaskWdg->hide();
   }
   else {
      ui.mpVhallRenderMaskWdg->show();
   }

   

   if ((info.course_type == CLASS_TYPE_MULTI && info.layout == CLASS_LAYOUT_ONLY_VIDEO)
      || (info.course_type == CLASS_TYPE_SMALL && info.layout == CLASS_LAYOUT_ONLY_VIDEO && mbTeacherRender)
      && ui.mBigRenderCtrl
      && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Auxiliary_CamerCapture)
      && !globalToolManager->GetClassSDK()->IsExitSubScribeStream(VHStreamType_Stu_Desktop)
      && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Auxiliary_CamerCapture)
      && !globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_Stu_Desktop)) {
      //QPoint pos = ui.stackedWidget->mapToGlobal(ui.stackedWidget->pos());
      //ui.mBigRenderCtrl->move(pos.rx() + (ui.stackedWidget->width() - ui.mBigRenderCtrl->width()) / 2, pos.ry() + ui.stackedWidget->height() - ui.mBigRenderCtrl->height() - 40);
      setGigRenderCtrlGeometry();
      if ( mWndFloatType != WndFloatType_Full && mOperationPower) {
         ui.mBigRenderCtrl->show();
      }
   }

}

void TeacherRenderWdg::setStackedSmallIndex(const int & index)
{
   globalToolManager->GetDataManager()->WriteLog("%s index %d", __FUNCTION__, index);
   ui.stackedWidget_small->setCurrentIndex(index);
}