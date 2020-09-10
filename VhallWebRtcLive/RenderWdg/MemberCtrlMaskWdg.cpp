#include "MemberCtrlMaskWdg.h"
#include "VhallUI_define.h"

MemberCtrlMaskWdg::MemberCtrlMaskWdg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	connect(ui.pushButton_camera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
	connect(ui.pushButton_setPen, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetPen()));
	connect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
	connect(ui.pushButton_fullScreen, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetFullScreen()));
	connect(ui.pushButton_KickOutPublish, SIGNAL(clicked()), this, SLOT(slot_OnClickedKickOutPublish()));
    connect(ui.pushButton_floatRender, SIGNAL(clicked()), this, SLOT(slot_OnClickedFloatRender()));

	mpTips = new CToolTips(this);
	if (mpTips) {
		mpTips->hide();
		mpTips->setSuspension();
	}
	setAttribute(Qt::WA_TranslucentBackground, true);
}

MemberCtrlMaskWdg::~MemberCtrlMaskWdg()
{
   disconnect(ui.pushButton_camera, SIGNAL(clicked()), this, SLOT(slot_OnClickedCamera()));
   disconnect(ui.pushButton_setPen, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetPen()));
   disconnect(ui.pushButton_mic, SIGNAL(clicked()), this, SLOT(slot_OnClickedMic()));
   disconnect(ui.pushButton_fullScreen, SIGNAL(clicked()), this, SLOT(slot_OnClickedSetFullScreen()));
   disconnect(ui.pushButton_KickOutPublish, SIGNAL(clicked()), this, SLOT(slot_OnClickedKickOutPublish()));
   disconnect(ui.pushButton_floatRender, SIGNAL(clicked()), this, SLOT(slot_OnClickedFloatRender()));

   if (mpTips) {
      delete mpTips;
      mpTips = NULL;
   }
}

void MemberCtrlMaskWdg::InitCtrlBtn(bool teacher, bool bIsSelf) {
    if (!teacher && !bIsSelf) {
        ui.pushButton_camera->hide();
        ui.pushButton_KickOutPublish->hide();
        ui.pushButton_mic->hide();
        ui.pushButton_setPen->hide();
    }
    else if (!teacher && bIsSelf) {
        ui.pushButton_setPen->hide();
        ui.pushButton_KickOutPublish->hide();
    }
}

//设置麦克风状态
void MemberCtrlMaskWdg::SetMicState(bool open) {
	if (open) {
		ui.pushButton_mic->setStyleSheet(MIC_OPEN_STATE_CSS);
	}
	else {
		ui.pushButton_mic->setStyleSheet(MIC_CLOSE_STATE_CSS);
	}
	mbMicOpen = open;
}

//设置摄像头状态
void MemberCtrlMaskWdg::SetCameraState(bool open) {
	if (open) {
		ui.pushButton_camera->setStyleSheet(CAMERA_OPEN_STATE_CSS);
	}
	else {
		ui.pushButton_camera->setStyleSheet(CAMERA_CLOSE_STATE_CSS);
	}
	mbCameraOpen = open;
}

void MemberCtrlMaskWdg::hide()
{
	mpTips->hide();
	QWidget::hide();
}

void MemberCtrlMaskWdg::slot_OnClickedCamera() {
	emit sig_clickedCamera(mbCameraOpen);
}

void MemberCtrlMaskWdg::slot_OnClickedMic() {
	emit sig_clickedMic(mbMicOpen);
}

void MemberCtrlMaskWdg::slot_OnClickedSetPen() {
	emit sig_clickedSetMainView();
}

void MemberCtrlMaskWdg::slot_OnClickedKickOutPublish() {
	emit sig_clickedKickOutPublish();
}

void MemberCtrlMaskWdg::slot_OnClickedSetToSpeaker() {
	emit sig_clickedSetToSpeaker();
}

void MemberCtrlMaskWdg::slot_OnClickedFloatRender() {
    emit sig_clickedFloatRender();
}

void MemberCtrlMaskWdg::slot_OnClickedCamerafocusIn() {
   OnEnteCamera();
}
void MemberCtrlMaskWdg::slot_OnClickedSetMainViewfocusIn() {
   OnEnteMainView();
}
void MemberCtrlMaskWdg::slot_OnClickedMicfocusIn() {
   OnEnteMic();
}
void MemberCtrlMaskWdg::slot_OnClickedSetToSpeakerfocusIn() {
   OnEnteToSpeaker();
}
void MemberCtrlMaskWdg::slot_OnClickedKickOffRoomfocusIn() {
   OnEnteKickOffRoom();
}

void MemberCtrlMaskWdg::slot_OnClickedCamerafocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedSetMainViewfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedMicfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedSetToSpeakerfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}
void MemberCtrlMaskWdg::slot_OnClickedKickOffRoomfocusOut() {
   if (mpTips) {
      mpTips->hide();
   }
}

void MemberCtrlMaskWdg::leaveEvent(QEvent *event) {
	hide();
	QWidget::leaveEvent(event);
}

void MemberCtrlMaskWdg::focusOutEvent(QFocusEvent *event) {
	hide();
	QWidget::focusOutEvent(event);
}

void MemberCtrlMaskWdg::OnEnteCamera() {
	if (mpTips) {
		QPoint pos = ui.pushButton_camera->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint(( ui.pushButton_camera->width()-mpTips->width())/2, ui.pushButton_camera->height() + 4));
		if (mbCameraOpen) {
			mpTips->SetText(QString::fromWCharArray(L"关闭摄像头"));
		}
		else {
			mpTips->SetText(QString::fromWCharArray(L"打开摄像头"));
		}
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteMainView() {
	//if (mpTips) {
	//	QPoint pos = ui.pushButton_mainView->pos();
	//	QPoint tempPos = mapToGlobal(pos);
	//	mpTips->move(tempPos + QPoint((ui.pushButton_mainView->width()-mpTips->width())/2, ui.pushButton_mainView->height() + 4));
	//	mpTips->SetText(QString::fromWCharArray(L"设为主画面"));
	//	mpTips->show();
	//}
}

void MemberCtrlMaskWdg::OnEnteMic() {
	if (mpTips) {
		QPoint pos = ui.pushButton_mic->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_mic->width()-mpTips->width())/2, ui.pushButton_mic->height() + 4));
		if (mbMicOpen) {
			mpTips->SetText(QString::fromWCharArray(L"关闭麦克风"));
		}
		else {
			mpTips->SetText(QString::fromWCharArray(L"打开麦克风"));
		}
		mpTips->show();
	}
}

void MemberCtrlMaskWdg::OnEnteToSpeaker() {
	//if (mpTips) {
	//	QPoint pos = ui.pushButton_setSpeaker->pos();
	//	QPoint tempPos = mapToGlobal(pos);
	//	mpTips->move(tempPos + QPoint((ui.pushButton_setSpeaker->width()-mpTips->width())/2, ui.pushButton_setSpeaker->height() + 4));
	//	mpTips->SetText(QString::fromWCharArray(L"画面切换"));
	//	mpTips->show();
	//}
}

void MemberCtrlMaskWdg::OnEnteKickOffRoom() {
	if (mpTips) {
		QPoint pos = ui.pushButton_KickOutPublish->pos();
		QPoint tempPos = mapToGlobal(pos);
		mpTips->move(tempPos + QPoint((ui.pushButton_KickOutPublish->width()-mpTips->width())/2, ui.pushButton_KickOutPublish->height() + 4));
		mpTips->SetText(QString::fromWCharArray(L"下麦"));
		mpTips->show();
	}
}

