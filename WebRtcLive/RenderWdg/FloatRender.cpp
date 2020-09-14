#include "FloatRender.h"

#include <QScreen>
#include <QDebug>
//#include <windows.h>
#include <QMouseEvent>
#include "VhallRenderMaskWdg.h"
extern ToolManager *globalToolManager;

#define BIG_RENDER_CLOSE_CAMERA QStringLiteral("QWidget#videoRender{border-image:url(:/widget/img/widget/teacher_close_camera.jpg);}")
#define SMALL_RENDER_CLOSE_CAMERA QStringLiteral("QWidget#smallWidget{border-image:url(:/widget/img/widget/teacher_close_camera.jpg);}")

CREATE_WND_FUNCTION(FloatRender);
//CBaseWnd* FloatRender::CreateFunction() {
//	CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
//	return new FloatRender(pWdg);
//}

FloatRender::FloatRender( QWidget *parent,int type) :
   CBaseWnd(parent),
   ui(new Ui::VHDesktopVideo)
{
   ui->setupUi(this);
   this->resize(800, 630);
   if (type == 0) {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::Widget);
   }
   else
   {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
   }
   
   //setAttribute(Qt::WA_TranslucentBackground, true);
   //setAutoFillBackground(false);

   this->setWindowTitle(QStringLiteral("微吼课堂"));
   isLeftPressDown = false;
   dir = NONE;
   // 追踪鼠标
   this->setMouseTracking(true);
   connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(slot_close()));

   //mpVhallRenderMaskWdg = ui->RenderMaskWdg;
   //mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
   //if (mpVhallRenderMaskWdg) {
   //   mpVhallRenderMaskWdg->hide();
   //}
   ui->smallWidget->hide();
   ui->widget->show();
}


FloatRender::~FloatRender()
{
   //if (mpVhallRenderMaskWdg) {
   //   delete mpVhallRenderMaskWdg;
   //   mpVhallRenderMaskWdg = NULL;
   //}
   delete ui;
}

void FloatRender::Notice(const CSTDMapParam &mapParam) {
   std::map<QString, QString>::const_iterator iterFunc = mapParam.find(Str_Function);
   if (iterFunc != mapParam.end() && iterFunc->second == Str_Function_SubScribe) {
      HandleSubScribeRender(mapParam);
   }
   else if (iterFunc != mapParam.end() && iterFunc->second == Str_Function_FloadAndFullWndRender) {
      HandleProcessFloadRender(mapParam);
   }
   else if (iterFunc != mapParam.end() && iterFunc->second == Str_Function_PublishStream) {
      HandlePublishStream(mapParam);
   }
   HandleSetUserType(mapParam);
}

void FloatRender::setFixedSize(const QSize &rect)
{
	CBaseWnd::setFixedSize(rect);
	ui->LayoutWidget->setFixedSize(rect);
	//if (ui->widget->isVisible())
	//{
		int iW = rect.width();
		int iH = rect.height() - 30;
		ui->RenderMaskWdg->setFixedSize(iW, iH);
		ui->videoRender->setFixedSize(iW, iH);
		ui->RenderMaskWdg->move(0, 30);
		ui->videoRender->move(0, 30);
		ui->RenderMaskWdg->setGeometry(0, 30, iW, iH);
		ui->videoRender->setGeometry(0, 30, iW, iH);

		ui->smallWidget->move(iW - ui->smallWidget->width() - 8,
			iH - ui->smallWidget->height() - 8);
	/*}
	else
	{


		int iW = rect.width();
		int iH = rect.height();

		ui->RenderMaskWdg->setFixedSize( iW, iH);
		ui->videoRender->setFixedSize( iW, iH);

		ui->RenderMaskWdg->move(0, 0);
		ui->videoRender->move(0, 0);

		ui->RenderMaskWdg->setGeometry(0, 0, iW, iH);
		ui->videoRender->setGeometry(0, 0, iW, iH);

		ui->smallWidget->move(iW - ui->smallWidget->width() - 8,
			iH - ui->smallWidget->height() - 8);
	}*/
}

void FloatRender::SetUserName(const QString & userName)
{
   ui->RenderMaskWdg->SetUserName(userName);
}

void FloatRender::SetBigRendering(const bool & big)
{
   mbBigRendering = big;
}

void FloatRender::HideTitle() {
   ui->widget->hide();
   this->setMouseTracking(true);
   ui->videoRender->setMouseTracking(true);
   ui->RenderMaskWdg->move(0, 0);
   ui->videoRender->move(0, 0);

   int iW = this->width();
   int iH = this->height();

   ui->RenderMaskWdg->setGeometry(0, 0, iW, iH);
   ui->videoRender->setGeometry(0, 0, iW, iH);
   
   ui->smallWidget->move(iW - ui->smallWidget->width() - 8,
	   iH - ui->smallWidget->height() - 8);

   grabKeyboard();
   bFullType = true;
}

void FloatRender::hideRender()
{
   //ui->RenderMaskWdg->hide();
   this->hide();
}

void FloatRender::keyPressEvent(QKeyEvent *event) {
   if (bFullType) {
      switch (event->key())
      {
      case Qt::Key_Escape:
         emit sig_move();
         break;
      default:
         QDialog::keyPressEvent(event);
      }
   }
}

QPaintEngine *FloatRender::paintEngine() const {
   return NULL;
}

void FloatRender::NoticeShowNickName() {
   if (ui->RenderMaskWdg == nullptr) {
      return;
   }
   if (ui->RenderMaskWdg && isVisible()) {
      if (ui->RenderMaskWdg->width() != ui->videoRender->width()) {
         if (ui->RenderMaskWdg->width() < ui->videoRender->width())
         {
            ui->RenderMaskWdg->SetUserNamePos(true);
         }
         else if (ui->RenderMaskWdg->width() > ui->videoRender->width())
         {
            ui->RenderMaskWdg->SetUserNamePos(false);
         }
         ui->RenderMaskWdg->setFixedWidth(ui->videoRender->width());
         ui->RenderMaskWdg->setFixedHeight(ui->videoRender->height());
      }
      QPoint pos = ui->videoRender->mapToGlobal(QPoint(0, 0));
      ui->RenderMaskWdg->move(pos.rx(), pos.ry() + ui->videoRender->height() - ui->RenderMaskWdg->height());
      ui->RenderMaskWdg->SetUserName(mNickName);
      ui->RenderMaskWdg->show();
   }
   else {
      //ui->RenderMaskWdg->hide();
   }
   ui->RenderMaskWdg->setFixedSize(this->width(), this->height() - 40);
}

HWND FloatRender::GetRenderWndID()
{
   HWND id = (HWND)(ui->videoRender->winId());
   return id;
}


std::shared_ptr<vhall::VideoRenderReceiveInterface> FloatRender::GetRenderWnd()
{
	return ui->videoRender->GetVideoReciver();
}
void FloatRender::SetMicState(bool open)
{
	//mbMicIsOpen = open;
	if (ui->RenderMaskWdg) {
      ui->RenderMaskWdg->SetMicState(open);
      ui->RenderMaskWdg->SetShowNoMic(!open);
	}
}

void FloatRender::SetPenState(bool enable)
{
	if (ui->RenderMaskWdg) {
		ui->RenderMaskWdg->SetPenState(enable);
		ui->RenderMaskWdg->SetShowPenLogo(enable);
	}
}

void FloatRender::initTrophyRewardNum(const int & iTrophyNum)
{
	ui->RenderMaskWdg->initTrophyRewardNum(iTrophyNum);
}

void FloatRender::SetShowNoMic(bool show/*,  bool bSelf*/)
{
	ui->RenderMaskWdg->SetShowNoMic(show);
}

void FloatRender::SetShowStuNoMic(bool hasAudio)
{
	ui->RenderMaskWdg->SetShowStuNoMic(hasAudio);
}


void FloatRender::show()
{
	CBaseWnd::show();
}

void FloatRender::showEvent(QShowEvent *event)
{
	CBaseWnd::showEvent(event);
}

void FloatRender::resizeEvent(QResizeEvent * ev)
{
   CBaseWnd::resizeEvent( ev);
   setFixedSize(ev->size());
}

void FloatRender::HandlePlayBigRender() {
   if (bStartRenderBig) {

      //HWND id = (HWND)(ui->videoRender->winId());
      std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = ui->videoRender->GetVideoReciver();
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (baseInfo.mUserInfo.join_id == mUserId.toStdString()) {
         if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
            //在小屏幕显示
            //id = (HWND)ui->widget_smallWidget->winId();
            Receiver = ui->widget_smallWidget->GetVideoReciver();
         }

         //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(id));
         globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
      }
      else {
         //HWND id;
         globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(mUserId.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);
         if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != mUserId.toStdString()) {
            //在小屏幕显示
            //id = (HWND)ui->widget_smallWidget->winId();
            //globalToolManager->GetClassSDK()->StartRenderRemoteStream(mUserId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
            //
			 Receiver = ui->widget_smallWidget->GetVideoReciver();
         }

         globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
         //globalToolManager->GetClassSDK()->StartRenderRemoteStream(mUserId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
		 globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(mUserId.toStdWString(), VHStreamType_AVCapture, Receiver);
      }
      bStartRenderBig = false;
   }
}


void FloatRender::HandleBigRender(const CSTDMapParam &mapParam) {
   std::map<QString, QString>::const_iterator iterStreamID = mapParam.find(STREAM_ID);
   std::map<QString, QString>::const_iterator iterUserID = mapParam.find(USER_ID);
   std::map<QString, QString>::const_iterator iterBigHasVideo = mapParam.find(BIG_RENDER_CAMERA_IS_CLOSE);
   std::map<QString, QString>::const_iterator iterBigHasMic = mapParam.find(BIG_RENDER_MIC_IS_CLOSE);
   if (iterStreamID != mapParam.end() && iterUserID != mapParam.end() && iterBigHasVideo != mapParam.end() && iterBigHasMic != mapParam.end()) {
      QString user_id = iterUserID->second;
      mUserId = iterUserID->second;
      QString stream_id = iterStreamID->second;
      bool closeCamera = iterBigHasVideo->second == PARAM_TRUE ? true : false;
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (closeCamera) {
         if (mbBigRendering) {
            if (baseInfo.mUserInfo.join_id != user_id.toStdString()) {
               globalToolManager->GetClassSDK()->StopRenderRemoteStream(user_id.toStdWString(), stream_id.toStdString(), VHStreamType_AVCapture);
            }
            else {
               globalToolManager->GetClassSDK()->CloseCamera();
               globalToolManager->GetDataManager()->SetMyCameraOpen(false);
            }
            mbBigRendering = false;
            ui->videoRender->setStyleSheet(BIG_RENDER_CLOSE_CAMERA);
            repaint();
         }
         if (!mbBigRenderInit && closeCamera) {
            mbBigRenderInit = true;
            ui->videoRender->setStyleSheet(BIG_RENDER_CLOSE_CAMERA);
            repaint();
         }
      }
      else {
         if (!mbBigRendering) {
            //HWND id = (HWND)(ui->videoRender->winId());
  
            CVideoRenderReceiveWdg * Receiver = ui->videoRender;
            if (baseInfo.mUserInfo.join_id == user_id.toStdString()) {
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student){
                  ui->RenderMaskWdg->setShowTrophyWdg(true);
               }else{
                  ui->RenderMaskWdg->setShowTrophyWdg(false);
               }
               if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
                  //如果不是在大屏，则在小屏幕显示
                  //id = (HWND)ui->widget_smallWidget->winId();
                  Receiver = ui->widget_smallWidget;
               }
               //ui.stackedWidget->
               //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(id));
               globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver->GetVideoReciver()/*(void*)(id)*/);
            }
            else {
               //HWND id = (HWND)(ui->videoRender->winId());
				CVideoRenderReceiveWdg* render = ui->videoRender;
               if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student){
                  ui->RenderMaskWdg->setShowTrophyWdg(false);
               }else{
                  ui->RenderMaskWdg->setShowTrophyWdg(true);
               }

               globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(mUserId.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);
               if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != mUserId.toStdString()) {
                  //如果不是在大屏，则在小屏幕显示
                  //id = (HWND)ui->widget_smallWidget->winId();
                  //globalToolManager->GetClassSDK()->StartRenderRemoteStream(mUserId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
                  
				  render = ui->widget_smallWidget;
               }

               globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
               //globalToolManager->GetClassSDK()->StartRenderRemoteStream(mUserId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
			   globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(mUserId.toStdWString(), VHStreamType_AVCapture, render->GetVideoReciver());

            }
            mbBigRendering = true;
            ui->videoRender->setStyleSheet("");
            repaint();
         }
      }
   }
}

void FloatRender::HandlePublishStream(const CSTDMapParam &mapParam) {
   std::map<QString, QString>::const_iterator iterStreamID = mapParam.find(STREAM_ID);
   std::map<QString, QString>::const_iterator iterUserID = mapParam.find(USER_ID);
   std::map<QString, QString>::const_iterator iterHasVideo = mapParam.find(BIG_RENDER_CAMERA_IS_CLOSE);
   std::map<QString, QString>::const_iterator iterTeacherView = mapParam.find(TEACHER_VIEW);
   if (iterStreamID != mapParam.end() && iterUserID != mapParam.end() && iterHasVideo != mapParam.end() && iterTeacherView != mapParam.end()) {
      mUserId = iterUserID->second;
      QString uid = iterUserID->second;
      QString stream_id = iterStreamID->second;
      bool hasVideo = iterHasVideo->second == PARAM_TRUE ? true : false;
      bool showInBigRender = iterTeacherView->second == PARAM_TRUE ? true : false;
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
      if (showInBigRender) {//讲师
         if (mbBigRendering) {
            std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = ui->videoRender->GetVideoReciver();
            //HWND id = (HWND)(ui->videoRender->winId());
            if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() != baseInfo.mUserInfo.join_id) {
               //在小屏幕显示
               //id = (HWND)ui->widget_smallWidget->winId();
               Receiver = ui->widget_smallWidget->GetVideoReciver();
            }
            //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)id);
            globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
         }
      }
      else {//学员
         if (mbSmallRendering) {
            //HWND id = (HWND)ui->widget_smallWidget->winId();
            std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = ui->widget_smallWidget->GetVideoReciver();
            if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
               //在大屏幕显示
               //id = (HWND)(ui->videoRender->winId());
               Receiver = ui->videoRender->GetVideoReciver();
            }
            //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)id);
            globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
         }
      }
   }
}

void FloatRender::HandleSmallRender(const CSTDMapParam &mapParam) {
   std::map<QString, QString>::const_iterator iterStreamID = mapParam.find(SMALL_RENDER_STEAM_ID);
   std::map<QString, QString>::const_iterator iterUserID = mapParam.find(SMALL_RENDER_USER_ID);
   std::map<QString, QString>::const_iterator iterBigHasVideo = mapParam.find(SMALL_RENDER_CAMERA_IS_CLOSE);
   std::map<QString, QString>::const_iterator iterBigHasMic = mapParam.find(SMALL_RENDER_MIC_IS_CLOSE);
   if (iterStreamID != mapParam.end() && iterUserID != mapParam.end() && iterBigHasVideo != mapParam.end() && iterBigHasMic != mapParam.end()) {
      if (iterStreamID->second.isEmpty() && iterUserID->second.isEmpty()) {
         ui->smallWidget->hide();
      }
      else {
         ui->smallWidget->show();
         QString user_id = iterUserID->second;
         QString stream_id = iterStreamID->second;
         bool closeCamera = iterBigHasVideo->second == PARAM_TRUE ? true : false;
         ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         if (closeCamera) {
            if (mbSmallRendering) {
               if (baseInfo.mUserInfo.join_id != user_id.toStdString()) {
                  globalToolManager->GetClassSDK()->StopRenderRemoteStream(user_id.toStdWString(), stream_id.toStdString(), VHStreamType_AVCapture);
               }
               else {
                  globalToolManager->GetClassSDK()->CloseCamera();
                  globalToolManager->GetDataManager()->SetMyCameraOpen(false);
               }
               mbSmallRendering = false;
               ui->stackedWidget->setCurrentIndex(1);
               repaint();
            }
            if (!mbSmallRenderInit && closeCamera) {
               mbSmallRenderInit = true;
               //HWND id = (HWND)ui->widget_smallWidget->winId();
               //globalToolManager->GetDataManager()->WriteLog("StartRenderRemoteStream big id %s  my id :%s", QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());

               //globalToolManager->GetClassSDK()->StartRenderRemoteStream(user_id.toStdWString(), VHStreamType_AVCapture, (void*)(id));

               globalToolManager->GetClassSDK()->StopRenderRemoteStream(user_id.toStdWString(), stream_id.toStdString(), VHStreamType_AVCapture);
               ui->stackedWidget->setCurrentIndex(1);
            }
         }
         else {
            if (!mbSmallRendering) {
               //HWND id = (HWND)ui->widget_smallWidget->winId();
               std::shared_ptr<vhall::VideoRenderReceiveInterface> Receiver = ui->widget_smallWidget->GetVideoReciver();
               if (baseInfo.mUserInfo.join_id == user_id.toStdString()) {
                  if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == baseInfo.mUserInfo.join_id) {
                     //在大屏幕显示
                     //id = (HWND)(ui->videoRender->winId());
                     Receiver = ui->videoRender->GetVideoReciver();
                  }
                  //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(id));
                  globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, Receiver);
               }
               else {
                  //HWND id = (HWND)ui->widget_smallWidget->winId();
                  CVideoRenderReceiveWdg* pWdg = ui->widget_smallWidget;
                  if (baseInfo.bigScreen != 0 && QString::number(baseInfo.bigScreen).toStdString() == user_id.toStdString()) {
                     //在大屏幕显示
                     //id = (HWND)(ui->videoRender->winId());
                     pWdg = ui->videoRender;
                  }
                  globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
                  globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(user_id.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);
                  //globalToolManager->GetClassSDK()->StartRenderRemoteStream(user_id.toStdWString(), VHStreamType_AVCapture, (void*)(id));
                  globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(user_id.toStdWString(), VHStreamType_AVCapture, pWdg->GetVideoReciver());
               }
               mbSmallRendering = true;
               ui->stackedWidget->setCurrentIndex(0);
            }
         }
      }
   }
}

void FloatRender::HandleProcessFloadRender(const CSTDMapParam &mapParam) {
   HandleBigRender(mapParam);
   HandleSmallRender(mapParam);
}

void FloatRender::SetRenderMaskWdg(VhallRenderMaskWdg * obj)
{
	ui->RenderMaskWdg->SetRenderMaskWdg(obj);
}

void FloatRender::HandleSubScribeRender(const CSTDMapParam &mapParam) {
   //std::map<QString, QString>::const_iterator iterStreamID = mapParam.find(STREAM_ID);
   std::map<QString, QString>::const_iterator iterUserID = mapParam.find(USER_ID);
   std::map<QString, QString>::const_iterator iterHasVideo = mapParam.find(BIG_RENDER_CAMERA_IS_CLOSE);
   std::map<QString, QString>::const_iterator iterTeacherView = mapParam.find(TEACHER_VIEW);
   if (/*iterStreamID != mapParam.end() &&*/ iterUserID != mapParam.end() && iterHasVideo != mapParam.end() && iterTeacherView != mapParam.end()) {
      mUserId = iterUserID->second;
      ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

      QString uid = iterUserID->second;
      //QString stream_id = iterStreamID->second;
      bool hasVideo = iterHasVideo->second == PARAM_TRUE ? true : false;
      bool teacherView = iterTeacherView->second == PARAM_TRUE ? true : false;
      if (teacherView) {
         if (mbBigRendering) {
            //HWND id = (HWND)(ui->videoRender->winId());
            globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(uid.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);
            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
			//globalToolManager->GetClassSDK()->StartRenderRemoteStream(uid.toStdWString(), VHStreamType_AVCapture, (void*)(id));
			globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(uid.toStdWString(), VHStreamType_AVCapture, ui->videoRender->GetVideoReciver());
         }
      }
      else {
         if (mbSmallRendering) {
            //HWND id = (HWND)ui->widget_smallWidget->winId();
            globalToolManager->GetClassSDK()->ChangeSubScribeUserSimulCast(uid.toStdWString(), VHStreamType_AVCapture, VHSimulCastType_BigStream);

            globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s small ", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
			//globalToolManager->GetClassSDK()->StartRenderRemoteStream(uid.toStdWString(), VHStreamType_AVCapture, (void*)(id));
			globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(uid.toStdWString(), VHStreamType_AVCapture, ui->widget_smallWidget->GetVideoReciver());
         }
      }
   }
}

void FloatRender::HandleSetUserType(const CSTDMapParam &mapParam) {
   std::map<QString, QString>::const_iterator iterName = mapParam.find(NICK_NAME);
   std::map<QString, QString>::const_iterator iterRole = mapParam.find(ROLE_TYPE);
   std::map<QString, QString>::const_iterator iterTrophyRewardNum = mapParam.find(TROPHY_COUNT);

   //if (iterName != mapParam.end() && iterRole != mapParam.end() && iterTrophyRewardNum != mapParam.end()) {
   if (iterName != mapParam.end() && iterRole != mapParam.end()) {
      mNickName = iterName->second;
      mRoleType = iterRole->second.toInt();
      //mTrophyRewardNum = iterTrophyRewardNum->second.toInt();
      switch (mRoleType)
      {
      case JOIN_ROLE_Teacher: {
         ui->label_name->setText(QStringLiteral("讲师"));
         break;
      }
      case JOIN_ROLE_Student: {
         ui->label_name->setText(QStringLiteral("学员"));
         break;
      }
      case JOIN_ROLE_Assistant: {
         ui->label_name->setText(QStringLiteral("助教"));
         break;
      }
      case JOIN_ROLE_Guest: {
         ui->label_name->setText(QStringLiteral("嘉宾"));
         break;
      }
      case JOIN_ROLE_Supervise: {
         ui->label_name->setText(QStringLiteral("监课"));
         break;
      }
      case JOIN_ROLE_Admin: {
         ui->label_name->setText(QStringLiteral("管理员"));
         break;
      }
      default:
         break;
      }

      if (ui->RenderMaskWdg) {
         ui->RenderMaskWdg->SetUserName(mNickName);
         if (iterTrophyRewardNum != mapParam.end())
         {
            mTrophyRewardNum = iterTrophyRewardNum->second.toInt();
            ui->RenderMaskWdg->initTrophyRewardNum(mTrophyRewardNum);
         }
      }
   }

   std::map<QString, QString>::const_iterator iterShowNickName = mapParam.find(SHOW_NICK_NAME);

   if (iterShowNickName != mapParam.end()) {
      if (!iterShowNickName->second.isEmpty())
      {
         mNickName = iterShowNickName->second;
      }
      NoticeShowNickName();
   }
}

void FloatRender::judgeRegionSetCursor(const QPoint& currentPoint)
{
   if (bFullType) {
      return;
   }
   // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
   QRect rect = this->rect();
   QPoint tl = mapToGlobal(rect.topLeft());
   QPoint rb = mapToGlobal(rect.bottomRight());

   int x = currentPoint.x();
   int y = currentPoint.y();

   qDebug() << x << y << tl.x() << tl.y() << rb.x() << rb.y() << endl;
   if (tl.x() + Padding < x && x < rb.x() - Padding && tl.y() + Padding < y && y < rb.y() - Padding)
   {
      // 默认
      dir = NONE;
      this->setCursor(QCursor(Qt::ArrowCursor));
   }
   else if (tl.x() + Padding >= x && tl.x() <= x && tl.y() + Padding >= y && tl.y() <= y) {
      // 左上角
      dir = LEFTTOP;
      this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
   }
   else if (x >= rb.x() - Padding && x <= rb.x() && y >= rb.y() - Padding && y <= rb.y()) {
      // 右下角
      dir = RIGHTBOTTOM;
      this->setCursor(QCursor(Qt::SizeFDiagCursor));
   }
   else if (x <= tl.x() + Padding && x >= tl.x() && y >= rb.y() - Padding && y <= rb.y()) {
      //左下角
      dir = LEFTBOTTOM;
      this->setCursor(QCursor(Qt::SizeBDiagCursor));
   }
   else if (x <= rb.x() && x >= rb.x() - Padding && y >= tl.y() && y <= tl.y() + Padding) {
      // 右上角
      dir = RIGHTTOP;
      this->setCursor(QCursor(Qt::SizeBDiagCursor));
   }
   else if (x <= tl.x() + Padding && x >= tl.x()) {
      // 左边
      dir = LEFT;
      this->setCursor(QCursor(Qt::SizeHorCursor));
   }
   else if (x <= rb.x() && x >= rb.x() - Padding) {
      // 右边
      dir = RIGHT;
      this->setCursor(QCursor(Qt::SizeHorCursor));
   }
   else if (y >= tl.y() && y <= tl.y() + Padding) {
      // 上边
      dir = UP;
      this->setCursor(QCursor(Qt::SizeVerCursor));
   }
   else if (y <= rb.y() && y >= rb.y() - Padding) {
      // 下边
      dir = DOWN;
      this->setCursor(QCursor(Qt::SizeVerCursor));
   }
   else {
      // 默认
      dir = NONE;
      this->setCursor(QCursor(Qt::ArrowCursor));
   }
}

void FloatRender::mouseReleaseEvent(QMouseEvent *event)
{
   if (bFullType) {
      emit sig_move();
      return;
   }
   if (event->button() == Qt::LeftButton) {
      isLeftPressDown = false;
      this->releaseMouse();
      this->setCursor(QCursor(Qt::ArrowCursor));
      dir = NONE;
   }
}
void FloatRender::mousePressEvent(QMouseEvent *event)
{
   if (bFullType) {
      emit sig_move();
      return;
   }
   switch (event->button()) {
   case Qt::LeftButton:
      isLeftPressDown = true;
      if (dir != NONE) {
         this->mouseGrabber();
      }
      else {
         dragPosition = event->globalPos() - this->frameGeometry().topLeft();
      }
      break;
   default:
      QWidget::mousePressEvent(event);
   }
}

void FloatRender::mouseMoveEvent(QMouseEvent *event)
{
   if (bFullType) {
      emit sig_move();
      return;
   }
   qDebug() << "mouseMoveEvent" << this->cursor().pos().x() << this->cursor().pos().y() << ui->pushButton_close->rect().x() << ui->pushButton_close->rect().y() << ui->pushButton_close->rect().x() + ui->pushButton_close->width() << ui->pushButton_close->rect().y() + ui->pushButton_close->height() << endl;
   if (ui->pushButton_close->rect().contains(this->cursor().pos()))
   {
      // 默认
      dir = NONE;
      this->setCursor(QCursor(Qt::ArrowCursor));
   }
   else
   {
      QPoint gloPoint = event->globalPos();
      QRect rect = this->rect();
      QPoint tl = mapToGlobal(rect.topLeft());
      QPoint rb = mapToGlobal(rect.bottomRight());

      if (!isLeftPressDown) {
         this->judgeRegionSetCursor(gloPoint);
      }
      else {
         if (dir != NONE) {
            QRect rMove(tl, rb);
            switch (dir) {
            case LEFT:
               if (rb.x() - gloPoint.x() <= this->minimumWidth())
                  rMove.setX(tl.x());
               else
                  rMove.setX(gloPoint.x());
               break;
            case RIGHT:
               rMove.setWidth(gloPoint.x() - tl.x());
               break;
            case UP:
               if (rb.y() - gloPoint.y() <= this->minimumHeight())
                  rMove.setY(tl.y());
               else
                  rMove.setY(gloPoint.y());
               break;
            case DOWN:
               rMove.setHeight(gloPoint.y() - tl.y());
               break;
            case LEFTTOP:
               if (rb.x() - gloPoint.x() <= this->minimumWidth())
                  rMove.setX(tl.x());
               else
                  rMove.setX(gloPoint.x());
               if (rb.y() - gloPoint.y() <= this->minimumHeight())
                  rMove.setY(tl.y());
               else
                  rMove.setY(gloPoint.y());
               break;
            case RIGHTTOP:
               rMove.setWidth(gloPoint.x() - tl.x());
               rMove.setY(gloPoint.y());
               break;
            case LEFTBOTTOM:
               rMove.setX(gloPoint.x());
               rMove.setHeight(gloPoint.y() - tl.y());
               break;
            case RIGHTBOTTOM:
               rMove.setWidth(gloPoint.x() - tl.x());
               rMove.setHeight(gloPoint.y() - tl.y());
               break;
            default:
               break;
            }
            this->setGeometry(rMove);
         }
         else {
            move(event->globalPos() - dragPosition);
            event->accept();
         }
      }

   }
   raise();
   QWidget::mouseMoveEvent(event);
}

void FloatRender::slot_close() {
   if (bFullType) {
      return;
   }
   //QString closeWnd = WND_ID_FLOAT_RENDER + mUserId;
   if (globalToolManager) {
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_CloseFloatRender, 0, mUserId));
   }
}

void FloatRender::closeEvent(QCloseEvent* event) {
   if (bFullType) {
      return;
   }
   if (event) {
      event->ignore();
      QWidget::closeEvent(event);
      slot_close();
   }
}
