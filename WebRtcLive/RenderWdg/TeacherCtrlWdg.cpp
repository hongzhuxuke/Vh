#include "TeacherCtrlWdg.h"
extern ToolManager *globalToolManager;

#define BTN_CAMERA_OPEN_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/camera_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/camera_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/camera_Normal.png);}") 

#define BTN_CAMERA_CLOSE_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/disable_camera_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/disable_camera_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/disable_camera_Normal.png);}")  

#define BTN_MIC_OPEN_STATE   QStringLiteral("QPushButton{border-image: url(:/ctrl/img/ctrl/mic_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/mic_Click.png);} QPushButton:pressed{border-image: url(:/ctrl/img/ctrl/mic_Normal.png);}") 

#define BTN_MIC_CLOSE_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/mute_mic_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/mute_mic_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/mute_mic_Normal.png);}") 

#define BTN_ALL_MUTE_NOT_OPEN_STATE   QStringLiteral("QPushButton{border-image: url(:/ctrl/img/ctrl/multi_mic_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/multi_mic_Click.png);}QPushButton:pressed{border-image: url(:/ctrl/img/ctrl/multi_mic_Normal.png);}") 

#define BTN_ALL_MUTE_OPEN_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/all_mute_mic_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/all_mute_mic_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/all_mute_mic_Normal.png);}") 

#define BTN_ALL_UNPUBLISH_NOT_OPEN_STATE   QStringLiteral("QPushButton{border-image: url(:/ctrl/img/ctrl/publish_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/publish_Click.png);}QPushButton:pressed{border-image: url(:/ctrl/img/ctrl/publish_Normal.png);}") 

#define BTN_ALL_UNPUBLISH_OPEN_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/disable_publish__Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/disable_pubish_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/disable_publish__Normal.png);}") 

#define BTN_PEN_MOVE_TO_USER_STATE   QStringLiteral("QPushButton{border-image: url(:/ctrl/img/ctrl/pen_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/pen_Click.png);}QPushButton:pressed{border-image: url(:/ctrl/img/ctrl/pen_Normal.png);}") 

#define BTN_PEN_CANCEL_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/disable_pen_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/disable_pen_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/disable_pen_Normal.png);}") 

#define BTN_PEN_CANCEL_STATE   QStringLiteral("QPushButton{border-image:url(:/ctrl/img/ctrl/disable_pen_Normal.png);}\
QPushButton:hover{border-image:url(:/ctrl/img/ctrl/disable_pen_Click.png);}QPushButton:pressed{border-image:url(:/ctrl/img/ctrl/disable_pen_Normal.png);}") 



TeacherCtrlWdg::TeacherCtrlWdg(QWidget *parent, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
   : CWidget(parent, f)
{
   ui.setupUi(this);

   //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint /*| Qt::Tool*/);
  //   setAttribute(Qt::WA_TranslucentBackground, true);
  //setWindowOpacity(0.8);
  //   this->setAutoFillBackground(false);

   connect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
   connect(ui.pushButton_camera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
   connect(ui.pushButton_fullScreen, SIGNAL(clicked()), this, SLOT(slot_OnClickedFullScreen()));
   connect(ui.pushButton_float, SIGNAL(clicked()), this, SLOT(slot_OnClickedFloatRender()));
   connect(ui.pushButton_allMute, SIGNAL(clicked()), this, SLOT(slot_OnClickedAllMute()));
   connect(ui.pushButto_allUnpublish, SIGNAL(clicked()), this, SLOT(slot_OnAllUnpublish()));
   connect(ui.pushButton_OpenClassUnPublish, SIGNAL(clicked()), this, SLOT(slot_OnAllUnpublish()));
   connect(ui.pushButton_OpenClassStudentCamera, SIGNAL(clicked()), this, SLOT(slot_OnOpenClassStudentCamera()));
   connect(ui.pushButton_OpenClassStuMic, SIGNAL(clicked()), this, SLOT(slot_OnOpenClassStuMic()));
   connect(ui.pushButton_changeCamera, SIGNAL(clicked()), this, SLOT(slot_OnChangeCamera()));

   connect(ui.pushButton_stu_camera, SIGNAL(clicked()), this, SLOT(slot_OnStuClickedCamera()));
   connect(ui.pushButton_stu_mic, SIGNAL(clicked()), this, SLOT(slot_OnStuClickedMic()));
   connect(ui.pushButton_stu_publish, SIGNAL(clicked()), this, SLOT(slot_OnstuClickedUnPublish()));
   connect(ui.pushButton_stu_pen, SIGNAL(clicked()), this, SLOT(slot_OnStuPenClickedPen()));
   connect(ui.pushButton_stu_float, SIGNAL(clicked()), this, SLOT(slot_OnStuClickedFloat()));
   connect(ui.pushButton_set_big_screen, SIGNAL(clicked()), this, SLOT(slot_OnSetBigScreen()));
   //connect(ui.pushButton_stu_full, SIGNAL(clicked()), this, SLOT(slot_OnStuClickedFullWnd()));

   ui.pushButton_stu_pen->setStyleSheet(BTN_PEN_MOVE_TO_USER_STATE);
   ui.pushButton_set_big_screen->hide();
   //mToolTips = new CToolTips(this);
   //if (mToolTips) {
   //    mToolTips->hide();
   //    mToolTips->SetOnTop();
   //}
   SetPenState(false);
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //无文档布局或是大班课，不能有画笔。
   if (baseInfo.mWebinarInfo.layout == CLASS_LAYOUT_ONLY_VIDEO /*|| baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI*/) {
      ui.pushButton_stu_pen->hide();
      //只有大班课的老师，才能进行主画面设置。
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.bigScreen != 0) {
         ui.pushButton_set_big_screen->show();
      }
   }
   SetAllMuteAudioState(false);
   ShowStudentCtrl(false);

   ui.btnReward->setFocusPolicy(Qt::NoFocus);
   ui.pushButton_stu_pen->setFocusPolicy(Qt::NoFocus);
   ui.pushButton_stu_float->setFocusPolicy(Qt::NoFocus);
   ui.pushButton_stu_camera->setFocusPolicy(Qt::NoFocus);
   ui.pushButton_stu_mic->setFocusPolicy(Qt::NoFocus);
   ui.pushButton_stu_publish->setFocusPolicy(Qt::NoFocus);
}

TeacherCtrlWdg::~TeacherCtrlWdg()
{
   //ui.pushButton_mic->removeEventFilter(this);
   //ui.pushButton_camera->removeEventFilter(this);
   //ui.pushButton_fullScreen->removeEventFilter(this);
   //ui.pushButton_float->removeEventFilter(this);
   //ui.pushButton_allMute->removeEventFilter(this);
   //ui.pushButto_allUnpublish->removeEventFilter(this);
   //ui.pushButton_changeCamera->removeEventFilter(this);

   //ui.pushButton_stu_camera->removeEventFilter(this);
   //ui.pushButton_stu_mic->removeEventFilter(this);
   //ui.pushButton_stu_publish->removeEventFilter(this);
   //ui.pushButton_stu_pen->removeEventFilter(this);
   //ui.pushButton_stu_float->removeEventFilter(this);
   ////ui.pushButton_stu_full->removeEventFilter(this);

   //ui.pushButton_OpenClassUnPublish->removeEventFilter(this);
   //ui.pushButton_OpenClassStudentCamera->removeEventFilter(this);
   //ui.pushButton_OpenClassStuMic->removeEventFilter(this);

   ui.widget_TeacherOpenClass->hide();
}

void TeacherCtrlWdg::InitBtn(QString userId) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.mUserInfo.join_id == userId.toStdString()) {
      //自己
      ui.stackedWidget->setCurrentIndex(0);
      ui.btnReward->hide();
   }
   else if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher && baseInfo.mUserInfo.join_id != userId.toStdString()) {
      //不是自己
      ui.stackedWidget->setCurrentIndex(1);
      ui.btnReward->show();
   }
   else if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher && baseInfo.mUserInfo.join_id == userId.toStdString()) {
      //是自己
      ui.stackedWidget->setCurrentIndex(1);
      ui.pushButton_stu_pen->hide();
      ui.btnReward->hide();
   }
   else if (baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher && baseInfo.mUserInfo.join_id != userId.toStdString()) {
      //不是自己
      ui.btnReward->hide();
      ui.stackedWidget->setCurrentIndex(1);
      ui.pushButton_stu_pen->hide();
      if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
         ui.widget_4->hide();
      }
   }
   ////公开课隐藏讲师
   //if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
   //    ui.widget_TeacherOpenClass->show();
   //    ui.widget_SmallClass->hide();
   //}
   ////小班课讲师
   //else if(baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher){
   //    ui.widget_TeacherOpenClass->hide();
   //    ui.widget_SmallClass->show();
   //}
}

void TeacherCtrlWdg::InitByWebView() {
   SetPageIndex(1);
   ui.stackedWidget->setCurrentIndex(1);
   ui.pushButton_stu_pen->hide();
   ui.widget_4->hide();
}

void TeacherCtrlWdg::SetPageIndex(int index) {
   ui.stackedWidget->setCurrentIndex(index);
}

void TeacherCtrlWdg::ShowStuPublishBtn(bool show) {
   if (show) {
      ui.widget_4->show();
   }
   else {
      ui.widget_4->hide();
   }
}

void TeacherCtrlWdg::SetAllMuteAudioState(bool mute) {
   if (mute) {
      ui.pushButton_allMute->setStyleSheet(BTN_ALL_MUTE_OPEN_STATE);
      ui.pushButton_allMute->setToolTip(Str_AllCancelMuteAudio);
   }
   else {
      ui.pushButton_allMute->setStyleSheet(BTN_ALL_MUTE_NOT_OPEN_STATE);
      ui.pushButton_allMute->setToolTip(Str_AllMuteAudio);
   }
}

void TeacherCtrlWdg::SetPenState(bool enable) {
   bHasPen = enable;
   if (enable) {
      ui.pushButton_stu_pen->setStyleSheet(BTN_PEN_CANCEL_STATE);
      ui.pushButton_stu_pen->setToolTip(QStringLiteral("取消授权画笔"));
   }
   else {
      ui.pushButton_stu_pen->setStyleSheet(BTN_PEN_MOVE_TO_USER_STATE);
      ui.pushButton_stu_pen->setToolTip(QStringLiteral("授权画笔"));
   }
}

void TeacherCtrlWdg::SetCameraState(bool open) {
	
   if (open) {
      ui.pushButton_camera->setStyleSheet(BTN_CAMERA_OPEN_STATE);
      ui.pushButton_stu_camera->setStyleSheet(BTN_CAMERA_OPEN_STATE);

      ui.pushButton_camera->setToolTip(Str_MuteVideo);
      ui.pushButton_stu_camera->setToolTip(Str_MuteVideo);
   }
   else {
      ui.pushButton_camera->setStyleSheet(BTN_CAMERA_CLOSE_STATE);
      ui.pushButton_stu_camera->setStyleSheet(BTN_CAMERA_CLOSE_STATE);
      ui.pushButton_camera->setToolTip(Str_CancelMuteVideo);
      ui.pushButton_stu_camera->setToolTip(Str_CancelMuteVideo);
   }
}

void TeacherCtrlWdg::SetMicState(bool open) {
   if (open) {//取消静音
      ui.pushButton_mic->setStyleSheet(BTN_MIC_OPEN_STATE);
      ui.pushButton_stu_mic->setStyleSheet(BTN_MIC_OPEN_STATE);

      ui.pushButton_mic->setToolTip(Str_MuteAudio);
      ui.pushButton_stu_mic->setToolTip(Str_MuteAudio);
   }
   else {//静音
      ui.pushButton_mic->setToolTip(Str_CancelMuteAudio);
      ui.pushButton_stu_mic->setToolTip(Str_CancelMuteAudio);
      ui.pushButton_mic->setStyleSheet(BTN_MIC_CLOSE_STATE);
      ui.pushButton_stu_mic->setStyleSheet(BTN_MIC_CLOSE_STATE);
      //ui.pushButton_mic->setToolTip(Str_CancelMuteAudio);
      //ui.pushButton_stu_mic->setToolTip(Str_CancelMuteAudio);
   }
}

void TeacherCtrlWdg::SetSmallRenderCameraState(bool hasVideo) {
   if (hasVideo) {
      ui.pushButton_OpenClassStudentCamera->setStyleSheet(BTN_CAMERA_OPEN_STATE);
   }
   else {
      ui.pushButton_OpenClassStudentCamera->setStyleSheet(BTN_CAMERA_CLOSE_STATE);
   }
}
void TeacherCtrlWdg::SetSmallRenderMicState(bool hasAudio) {
   if (hasAudio) {//取消静音
      ui.pushButton_OpenClassStuMic->setStyleSheet(BTN_MIC_OPEN_STATE);
      ui.pushButton_OpenClassStuMic->setToolTip(Str_MuteAudio);

	  ui.pushButton_stu_mic->setStyleSheet(BTN_MIC_OPEN_STATE);
	  ui.pushButton_stu_mic->setToolTip(Str_MuteAudio);
   }
   else {//静音
      ui.pushButton_OpenClassStuMic->setStyleSheet(BTN_MIC_CLOSE_STATE);
      ui.pushButton_OpenClassStuMic->setToolTip(Str_AllMuteAudio);

	  ui.pushButton_stu_mic->setStyleSheet(BTN_MIC_CLOSE_STATE);
	  ui.pushButton_stu_mic->setToolTip(Str_MuteAudio);
   }
}

void TeacherCtrlWdg::CheckShowChangeBigScreen(bool show) {
   show == true ? ui.pushButton_set_big_screen->show() : ui.pushButton_set_big_screen->hide();;
}

void TeacherCtrlWdg::ShowStudentCtrl(bool show) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   //公开课隐藏讲师
   if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      ui.widget_SmallClass->hide();
      if (show) {
         ui.widget_TeacherOpenClass->show();
      }
      else {
         ui.widget_TeacherOpenClass->hide();
      }
   }
   //小班课讲师
   else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
      ui.widget_TeacherOpenClass->hide();
      if (show) {
         ui.widget_SmallClass->show();
      }
      else {
         ui.widget_SmallClass->hide();
      }
   }
}



void TeacherCtrlWdg::slot_OnClickedFloatRender() {
   emit sig_OnClickedFloatRender();
}

void TeacherCtrlWdg::slot_OnClickedMic() {
   emit sig_OnClickedMic();
}

void TeacherCtrlWdg::slot_OnClickedCamera() {
   emit sig_OnClickedCamera();
}

void TeacherCtrlWdg::slot_OnClickedAllMute() {
   emit sig_OnClickedAllMute();
}

void TeacherCtrlWdg::slot_OnAllUnpublish() {
   emit sig_OnAllUnpublish();
}

void TeacherCtrlWdg::slot_OnClickedFullScreen() {
   globalToolManager->GetDataManager()->WriteLog(" %s", __FUNCTION__);
   emit sig_OnClickedFullScreen();
}

void TeacherCtrlWdg::slot_OnClickedAllMuteCamera() {
   emit sig_OnClickedAllMuteCamera();
}

void TeacherCtrlWdg::slot_OnStuClickedCamera() {
   emit sig_OnStuClickedCamera();
}

void TeacherCtrlWdg::slot_OnStuClickedMic() {
   emit sig_OnStuClickedMic();
}

void TeacherCtrlWdg::slot_OnstuClickedUnPublish() {
   emit sig_OnstuClickedUnPublish();
}

void TeacherCtrlWdg::slot_OnStuPenClickedPen() {
   emit sig_OnStuPenClickedPen(bHasPen);
}

void TeacherCtrlWdg::slot_OnStuClickedFloat() {
   emit sig_OnStuClickedFloat();
}

void TeacherCtrlWdg::slot_OnSetBigScreen() {
   emit sig_OnSetBigScreen();
}

void TeacherCtrlWdg::slot_OnStuClickedFullWnd() {
   emit sig_OnStuClickedFullWnd();
}

void TeacherCtrlWdg::slot_OnOpenClassStuMic() {
   emit sig_OnOpenClassStuMic();
}

void TeacherCtrlWdg::slot_OnOpenClassStudentCamera() {
   emit sig_OnOpenClassStudentCamera();
}

void TeacherCtrlWdg::slot_OnChangeCamera() {
   globalToolManager->GetDataManager()->WriteLog("xk %s", __FUNCTION__);
   emit sig_OnChangeCamera();
}

void TeacherCtrlWdg::on_btnReward_clicked(bool checked /*= false*/)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   emit sig_FavorStudent();
}

void TeacherCtrlWdg::slot_PicPush(const bool& iPicPush)
{
   if (iPicPush)
   {
      ui.pushButton_changeCamera->hide();
   }
   else
   {
      ui.pushButton_changeCamera->show();
   }
}
//bool TeacherCtrlWdg::eventFilter(QObject* receiver, QEvent * event) {
//	return QWidget::eventFilter(receiver, event);
//    if (event->type() == QEvent::Enter) {
//        if (receiver == ui.pushButton_mic) {
//            if (mToolTips) {
//                if (ui.pushButton_mic->styleSheet() == BTN_MIC_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteAudio);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteAudio);
//                }
//                QPoint pos = ui.pushButton_mic->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_mic->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_changeCamera) {
//            if (mToolTips) {
//                mToolTips->SetText(Str_ChangeCamera);
//                QPoint pos = ui.pushButton_changeCamera->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_changeCamera->width()) / 2), pos.ry() + ui.pushButton_changeCamera->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_OpenClassStuMic) {
//            if (mToolTips) {
//                if (ui.pushButton_OpenClassStuMic->styleSheet() == BTN_MIC_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteAudio);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteAudio);
//                }
//                QPoint pos = ui.pushButton_OpenClassStuMic->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_OpenClassStuMic->width()) / 2), pos.ry() + ui.pushButton_OpenClassStuMic->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_camera) {
//            if (mToolTips) {
//                if (ui.pushButton_camera->styleSheet() == BTN_CAMERA_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteVideo);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteVideo);
//                }
//                QPoint pos = ui.pushButton_camera->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_camera->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_OpenClassStudentCamera) {
//            if (mToolTips) {
//                if (ui.pushButton_OpenClassStudentCamera->styleSheet() == BTN_CAMERA_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteVideo);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteVideo);
//                }
//                QPoint pos = ui.pushButton_OpenClassStudentCamera->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_mic->width()) / 2), pos.ry() + ui.pushButton_OpenClassStudentCamera->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_fullScreen) {
//            if (mToolTips) {
//                mToolTips->SetText(QStringLiteral("全屏"));
//                QPoint pos = ui.pushButton_fullScreen->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_fullScreen->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_float) {
//            if (mToolTips) {
//                mToolTips->SetText(QStringLiteral("窗口显示"));
//                QPoint pos = ui.pushButton_float->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_float->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_allMute) {
//            if (mToolTips) {
//                if (ui.pushButton_allMute->styleSheet() == BTN_ALL_MUTE_NOT_OPEN_STATE) {
//                    mToolTips->SetText(Str_AllMuteAudio);
//                }
//                else {
//                    mToolTips->SetText(Str_AllCancelMuteAudio);
//                }
//                QPoint pos = ui.pushButton_allMute->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_allMute->width())/2), pos.ry() + ui.pushButton_allMute->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButto_allUnpublish) {
//            if (mToolTips) {
//                mToolTips->SetText(Str_AllUnPublish);
//                QPoint pos = ui.pushButto_allUnpublish->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButto_allUnpublish->width())/2), pos.ry() + ui.pushButto_allUnpublish->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_stu_camera) {
//            if (mToolTips) {
//                if (ui.pushButton_stu_camera->styleSheet() == BTN_CAMERA_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteVideo);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteVideo);
//                }
//                QPoint pos = ui.pushButton_stu_camera->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_stu_camera->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_stu_mic) {
//            if (mToolTips) {
//                if (ui.pushButton_stu_mic->styleSheet() == BTN_MIC_OPEN_STATE) {
//                    mToolTips->SetText(Str_MuteAudio);
//                }
//                else {
//                    mToolTips->SetText(Str_CancelMuteAudio);
//                }
//                QPoint pos = ui.pushButton_stu_mic->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_stu_mic->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_stu_publish) {
//            if (mToolTips) {
//                mToolTips->SetText(QStringLiteral("下麦"));
//                QPoint pos = ui.pushButton_stu_publish->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_stu_publish->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_OpenClassUnPublish) {
//            if (mToolTips) {
//                mToolTips->SetText(QStringLiteral("下麦"));
//                QPoint pos = ui.pushButton_OpenClassUnPublish->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_mic->width()) / 2), pos.ry() + ui.pushButton_OpenClassUnPublish->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_stu_pen) {
//            if (mToolTips) {
//                if (ui.pushButton_stu_pen->styleSheet() == BTN_PEN_MOVE_TO_USER_STATE) {
//                    mToolTips->SetText(QStringLiteral("授权画笔"));
//                }
//                else {
//                    mToolTips->SetText(QStringLiteral("取消授权画笔"));
//                }
//                QPoint pos = ui.pushButton_stu_pen->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_stu_pen->width())/2), pos.ry() + ui.pushButton_stu_pen->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//        else if (receiver == ui.pushButton_stu_float) {
//            if (mToolTips) {
//                mToolTips->SetText(QStringLiteral("窗口显示"));
//                QPoint pos = ui.pushButton_stu_float->mapToGlobal(QPoint(0, 0));
//                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_mic->width())/2), pos.ry() + ui.pushButton_stu_float->height() + 5);
//                mToolTips->show();
//                mToolTips->raise();
//            }
//        }
//		else if (receiver == ui.btnReward) {
//			if (mToolTips) {
//				mToolTips->SetText(QStringLiteral("奖励"));
//				QPoint pos = ui.btnReward->mapToGlobal(QPoint(0, 0));
//				mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_mic->width()) / 2), pos.ry() + ui.btnReward->height() + 5);
//				mToolTips->show();
//				mToolTips->raise();
//			}
//		}
//    }
//    else if (event->type() == QEvent::Leave) {
//        if (mToolTips) {
//            mToolTips->hide();
//			
//        }
//		ui.pushButton_mic->repaint();
//		ui.pushButton_camera->repaint();
//		ui.pushButton_fullScreen->repaint();
//		ui.pushButton_float->repaint();
//		ui.pushButton_allMute->repaint();
//		ui.pushButto_allUnpublish->repaint();
//		ui.pushButton_changeCamera->repaint();
//
//		ui.pushButton_stu_camera->repaint();
//		ui.pushButton_stu_mic->repaint();
//		ui.pushButton_stu_publish->repaint();
//		ui.pushButton_stu_pen->repaint();
//		ui.pushButton_stu_float->repaint();
//		ui.btnReward->repaint();
//    }
//    return QWidget::eventFilter(receiver, event);
//}

//void TeacherCtrlWdg::hideEvent(QHideEvent *event) {
//    if (mToolTips) {
//        mToolTips->hide();
//    }
//
//    return QWidget::hideEvent(event);
//}
