#include "BigRenderCtrl.h"
extern ToolManager *globalToolManager;


//摄像头打开
#define BTN_CAMERA_OPEN_STATE   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_render_camera_open_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_render_camera_open_Click.png);}") 

//摄像头关闭
#define BTN_CAMERA_CLOSE_STATE   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_render_camera_close_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_render_camera_close_Click.png);}") 

//单个麦克风打开
#define BTN_MIC_OPEN_STATE   QStringLiteral("QPushButton{border-image: url(:/big/img/ctrl/big/big_mic_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_mic_Click.png);}") 

//单个麦克风关闭
#define BTN_MIC_CLOSE_STATE   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_render_mic_mute_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_render_mic_mute_Click.png);}") 

//多个麦克风打开
#define BTN_DOUBLE_MIC_OPEN_STATE   QStringLiteral("QPushButton{border-image: url(:/big/img/ctrl/big/big_multi_mute_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_multi_mute_Click.png);}") 

//多个麦克风关闭
#define BTN_DOUBLE_MIC_CLOSE_STATE   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_render_all_mute_mic_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_render_all_mute_mic_Click.png);}") 

//单个用户下麦
#define BTN_UNPUBLISH   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_publish_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_publish_Click.png);}") 

//多个用户下麦
#define BTN_DOUBLE_UNPUBLISH   QStringLiteral("QPushButton{border-image:url(:/big/img/ctrl/big/big_multi_publish_Normal.png);}\
QPushButton:hover{border-image:url(:/big/img/ctrl/big/big_multi_publish_Click.png);}") 


BigRenderCtrl::BigRenderCtrl(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint/* | Qt::Tool*/);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    connect(ui.pushButton_LocalCamera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
    connect(ui.pushButton_LocalMic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
    connect(ui.pushButton_fullScreen, SIGNAL(clicked()), this, SLOT(slot_OnClickedfullScreen()));
    connect(ui.pushButton_AllMuteMic, SIGNAL(clicked()), this, SLOT(slot_OnAllMuteMic()));
    connect(ui.pushButton_AllUnPublish, SIGNAL(clicked()), this, SLOT(slot_OnAllUnPublish()));
    connect(ui.pushButton_changeCamera, SIGNAL(clicked()), this, SLOT(slot_OnChangeCamera()));
    connect(ui.pushButton_big_screen, SIGNAL(clicked()), this, SLOT(slot_OnBigScreenChanged()));
    connect(ui.pushButton_closeStuCamera, SIGNAL(clicked()), this, SLOT(slot_OncloseStuCamera()));
    connect(ui.pushButton_closeStuMic, SIGNAL(clicked()), this, SLOT(slot_OncloseStuMic()));
    connect(ui.pushButton_UnpublishStu, SIGNAL(clicked()), this, SLOT(slot_OnUnpublishStu()));

    SetCameraState(true);

    ui.pushButton_LocalCamera->installEventFilter(this);
    ui.pushButton_LocalMic->installEventFilter(this);
    ui.pushButton_fullScreen->installEventFilter(this);
    ui.pushButton_AllMuteMic->installEventFilter(this);
    ui.pushButton_AllUnPublish->installEventFilter(this);
    ui.pushButton_changeCamera->installEventFilter(this);
    ui.pushButton_closeStuCamera->installEventFilter(this);
    ui.pushButton_closeStuMic->installEventFilter(this);
    ui.pushButton_UnpublishStu->installEventFilter(this);
    ui.pushButton_big_screen->installEventFilter(this);

    mToolTips = new CToolTips(this);
    if (mToolTips) {
        mToolTips->hide();
        mToolTips->SetOnTop();
    }
}

BigRenderCtrl::~BigRenderCtrl()
{
    ui.pushButton_LocalCamera->removeEventFilter(this);
    ui.pushButton_LocalMic->removeEventFilter(this);
    ui.pushButton_fullScreen->removeEventFilter(this);
    ui.pushButton_AllMuteMic->removeEventFilter(this);
    ui.pushButton_AllUnPublish->removeEventFilter(this);
    ui.pushButton_changeCamera->removeEventFilter(this);
    ui.pushButton_closeStuCamera->removeEventFilter(this);
    ui.pushButton_closeStuMic->removeEventFilter(this);
    ui.pushButton_UnpublishStu->removeEventFilter(this);
    ui.pushButton_big_screen->removeEventFilter(this);
}  

void BigRenderCtrl::InitBtn(QString userId) {
    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    if (baseInfo.mWebinarInfo.course_type != CLASS_TYPE_MULTI || baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) { //公开课学员
      ui.pushButton_big_screen->hide();
    }
    if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {//小课堂学员
        ui.label_stu->hide();
        ui.label_teacher->hide();
        ui.pushButton_LocalCamera->hide();
        ui.pushButton_LocalMic->hide();
        ui.pushButton_AllMuteMic->hide();
        ui.pushButton_AllUnPublish->hide();
        ui.pushButton_closeStuCamera->hide();
        ui.pushButton_closeStuMic->hide();
        ui.pushButton_UnpublishStu->hide();
    }
    else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
        ui.pushButton_closeStuCamera->hide();
        ui.pushButton_closeStuMic->hide();
        ui.pushButton_UnpublishStu->hide();
    }
    else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
        ui.label_teacher->hide();
        ui.label_stu->hide();
        ui.pushButton_AllMuteMic->hide();
        ui.pushButton_AllUnPublish->hide();
        ui.pushButton_closeStuCamera->hide();
        ui.pushButton_closeStuMic->hide();
        ui.pushButton_UnpublishStu->hide();
    }
    else if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI && baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
        ui.pushButton_AllMuteMic->hide();
        ui.pushButton_AllUnPublish->hide();
        ui.pushButton_closeStuCamera->show();
        ui.pushButton_closeStuMic->show();
        ui.pushButton_UnpublishStu->show();
        OpenClassStudentPublish(false);
    }

    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
        ui.pushButton_changeCamera->show();
    }
    else {
        ui.pushButton_changeCamera->hide();
    }
}

void BigRenderCtrl::SetCameraState(bool open) {
    if (open) {
        ui.pushButton_LocalCamera->setStyleSheet(BTN_CAMERA_OPEN_STATE);
    }
    else {
        ui.pushButton_LocalCamera->setStyleSheet(BTN_CAMERA_CLOSE_STATE);
    }
}

void BigRenderCtrl::SetMicState(bool open) {
    if (open) {
        ui.pushButton_LocalMic->setStyleSheet(BTN_MIC_OPEN_STATE);
    }
    else {
        ui.pushButton_LocalMic->setStyleSheet(BTN_MIC_CLOSE_STATE);
    }
}

void BigRenderCtrl::SetStudentCamera(bool open) {
    if (open) {
        ui.pushButton_closeStuCamera->setStyleSheet(BTN_CAMERA_OPEN_STATE);
    }
    else {
        ui.pushButton_closeStuCamera->setStyleSheet(BTN_CAMERA_CLOSE_STATE);
    }
    mbStudentCameraOpen = open;
}

void BigRenderCtrl::SetStudentMic(bool open) {
    if (open) {
        ui.pushButton_closeStuMic->setStyleSheet(BTN_MIC_OPEN_STATE);
    }
    else {
        ui.pushButton_closeStuMic->setStyleSheet(BTN_MIC_CLOSE_STATE);
    }
    mbStudentMicOpen = open;
}

void BigRenderCtrl::OpenClassStudentPublish(bool start) {
    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
        if (start) {
            ui.label_stu->show();
            ui.pushButton_closeStuCamera->show();
            ui.pushButton_closeStuMic->show();
            ui.pushButton_UnpublishStu->show();
            if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
               ui.pushButton_big_screen->show();
            }
        }
        else {
            ui.label_stu->hide();
            ui.pushButton_closeStuCamera->hide();
            ui.pushButton_closeStuMic->hide();
            ui.pushButton_UnpublishStu->hide();
            if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
               ui.pushButton_big_screen->hide();
            }
        }
    }
}


void BigRenderCtrl::SetAllMuteVideo(bool mute) {

}

void BigRenderCtrl::SetAllMuteAudio(bool mute) {
    if (mute) {
        ui.pushButton_AllMuteMic->setStyleSheet(BTN_DOUBLE_MIC_CLOSE_STATE);
    }
    else {
        ui.pushButton_AllMuteMic->setStyleSheet(BTN_DOUBLE_MIC_OPEN_STATE);
    }
}

void BigRenderCtrl::slot_OnClickedCamera() {
    emit sig_OnClickedCamera();
}

void BigRenderCtrl::slot_OnClickedMic() {
    emit sig_OnClickedMic();
}

void BigRenderCtrl::slot_OnClickedfullScreen() {
    emit sig_OnClickedfullScreen();
}

void BigRenderCtrl::slot_OnAllMuteVideo() {
    emit sig_OnAllMuteVideo();
}

void BigRenderCtrl::slot_OnAllMuteMic() {
    emit sig_OnAllMuteMic();
}

void BigRenderCtrl::slot_OnAllUnPublish() {
    emit sig_OnAllUnPublish();
}

void BigRenderCtrl::slot_OnChangeCamera() {
    emit sig_OnChangeCamera();
}

void BigRenderCtrl::slot_OnBigScreenChanged() {
   emit sig_OnBigScreenChanged();
}

void BigRenderCtrl::slot_OncloseStuCamera() {
    emit sig_OncloseStuCamera();
}

void BigRenderCtrl::slot_OncloseStuMic() {
    emit sig_OncloseStuMic();
}

void BigRenderCtrl::slot_OnUnpublishStu() {
    emit sig_OnUnpublishStu();
}

bool BigRenderCtrl::eventFilter(QObject* receiver, QEvent * event) {
    if (event->type() == QEvent::Enter) {
        if (receiver == ui.pushButton_LocalMic) {
            if (mToolTips) {
                if (ui.pushButton_LocalMic->styleSheet() == BTN_MIC_OPEN_STATE) {
                    mToolTips->SetText(Str_MuteAudio);
                }
                else {
                    mToolTips->SetText(Str_CancelMuteAudio);
                }
                QPoint pos = ui.pushButton_LocalMic->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_LocalMic->width())/2), pos.ry() + ui.pushButton_LocalMic->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_changeCamera) {
            if (mToolTips) {
                mToolTips->SetText(Str_ChangeCamera);
                QPoint pos = ui.pushButton_changeCamera->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_changeCamera->width()) / 2), pos.ry() + ui.pushButton_changeCamera->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_LocalCamera) {
            if (mToolTips) {
                if (ui.pushButton_LocalCamera->styleSheet() == BTN_CAMERA_OPEN_STATE) {
                    mToolTips->SetText(Str_MuteVideo);
                }
                else {
                    mToolTips->SetText(Str_CancelMuteVideo);
                }

                QPoint pos = ui.pushButton_LocalCamera->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_LocalCamera->width())/2), pos.ry() + ui.pushButton_LocalCamera->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_fullScreen) {
            if (mToolTips) {
                mToolTips->SetText(QStringLiteral("全屏"));
                QPoint pos = ui.pushButton_fullScreen->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_fullScreen->width())/2), pos.ry() + ui.pushButton_fullScreen->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_AllUnPublish) {
            if (mToolTips) {
                mToolTips->SetText(Str_AllUnPublish);
                QPoint pos = ui.pushButton_AllUnPublish->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_AllUnPublish->width())/2), pos.ry() + ui.pushButton_AllUnPublish->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_AllMuteMic) {
            if (mToolTips) {
                mToolTips->SetText(Str_AllMuteAudio);
                QPoint pos = ui.pushButton_AllMuteMic->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx()-((mToolTips->width()-ui.pushButton_AllMuteMic->width())/2), pos.ry() + ui.pushButton_AllMuteMic->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_big_screen) {
           if (mToolTips) {
              mToolTips->SetText(Str_ChangeBigScreen);
              QPoint pos = ui.pushButton_big_screen->mapToGlobal(QPoint(0, 0));
              mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_big_screen->width()) / 2), pos.ry() + ui.pushButton_big_screen->height() + 5);
              mToolTips->show();
              mToolTips->raise();
           }
        }
        else if (receiver == ui.pushButton_closeStuCamera) {
            if (mToolTips) {
                if (ui.pushButton_closeStuCamera->styleSheet() == BTN_CAMERA_OPEN_STATE) {
                    mToolTips->SetText(Str_MuteVideo);
                }
                else {
                    mToolTips->SetText(Str_CancelMuteVideo);
                }

                QPoint pos = ui.pushButton_closeStuCamera->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_closeStuCamera->width()) / 2), pos.ry() + ui.pushButton_closeStuCamera->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
        else if (receiver == ui.pushButton_closeStuMic) {
            if (mToolTips) {
                if (ui.pushButton_closeStuMic->styleSheet() == BTN_CAMERA_OPEN_STATE) {
                    mToolTips->SetText(Str_MuteAudio);
                }
                else {
                    mToolTips->SetText(Str_CancelMuteAudio);
                }

                QPoint pos = ui.pushButton_closeStuMic->mapToGlobal(QPoint(0, 0));
                mToolTips->move(pos.rx() - ((mToolTips->width() - ui.pushButton_closeStuMic->width()) / 2), pos.ry() + ui.pushButton_closeStuMic->height() + 5);
                mToolTips->show();
                mToolTips->raise();
            }
        }
    }
    else if (event->type() == QEvent::Leave) {
        if (mToolTips) {
            mToolTips->hide();
        }
    }
    return QWidget::eventFilter(receiver, event);
}