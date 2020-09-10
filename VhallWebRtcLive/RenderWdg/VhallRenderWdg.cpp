#include "VhallRenderWdg.h"
#include "VhallUI_define.h"
#include <QDebug>
extern ToolManager *globalToolManager;
VhallRenderWdg::VhallRenderWdg(QWidget *parent)
	: QWidget(parent)
{
    ui.setupUi(this);
    mpMemberCtrlWdg = new MemberCtrlMaskWdg(this);
    if (mpMemberCtrlWdg) {
	    mpMemberCtrlWdg->hide();
	    connect(mpMemberCtrlWdg, SIGNAL(sig_clickedCamera(bool)), this, SLOT(slot_OnClickedCamera(bool)));
	    connect(mpMemberCtrlWdg, SIGNAL(sig_clickedMic(bool)), this, SLOT(slot_OnClickedMic(bool)));
	    connect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetMainView()), this, SLOT(slot_OnClickedSetMainView()));
	    connect(mpMemberCtrlWdg, SIGNAL(sig_clickedKickOutPublish()), this, SLOT(slot_OnClickedKickOutPublish()));
	    connect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetToSpeaker()), this, SLOT(slot_OnClickedSetToSpeaker()));
        connect(mpMemberCtrlWdg, SIGNAL(sig_clickedFloatRender()), this, SLOT(slot_OnClickedFloatRender()));
    }
    mpVhallRenderMaskWdg = new VhallRenderMaskWdg();
    if (mpVhallRenderMaskWdg) {
        mpVhallRenderMaskWdg->hide();
    }
    ui.widget_rendView->show();
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    SetEnableUpdateRendWnd(false);
}

VhallRenderWdg::~VhallRenderWdg()
{
    //mUserNickNameShowTimer.stop();
    if (mpVhallRenderMaskWdg) {
        delete mpVhallRenderMaskWdg;
        mpVhallRenderMaskWdg = NULL;
    }

    if (mpMemberCtrlWdg) {
        disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedCamera(bool)), this, SLOT(slot_OnClickedCamera(bool)));
        disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedMic(bool)), this, SLOT(slot_OnClickedMic(bool)));
        disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetMainView()), this, SLOT(slot_OnClickedSetMainView()));
        disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedKickOutPublish()), this, SLOT(slot_OnClickedKickOutPublish()));
        disconnect(mpMemberCtrlWdg, SIGNAL(sig_clickedSetToSpeaker()), this, SLOT(slot_OnClickedSetToSpeaker()));
	    delete mpMemberCtrlWdg;
	    mpMemberCtrlWdg = NULL;
    }
}

void VhallRenderWdg::SetNickName(QString name) {
    if (mpVhallRenderMaskWdg) {
        mpVhallRenderMaskWdg->SetUserName(name);
    }
}

void VhallRenderWdg::SetRoleType(int role) {
    if (role >= 0) {
        mJoinType = role;
    }
}

void VhallRenderWdg::InitUserInfo(const QString& userId,QString streamId,int role_type) {
    if (!userId.isEmpty()) {
        SetUserJoinID(userId);
        bool bSelf = false;
        bool bTeacher = false;
        if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id == userId.toStdString()) {
            bSelf = true;
        }
        if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == JOIN_ROLE_Teacher) {
            bTeacher = true;
        }
        //当前登录用户是主持人，根据上麦用户角色判断是否显示对应设置图标。
        if (mpMemberCtrlWdg) {
            mpMemberCtrlWdg->InitCtrlBtn(bTeacher, bSelf);
        }
    }
    if (!streamId.isEmpty()) {
        SetStreamID(streamId);
    }
    if (role_type >= 0) {
        mJoinType = role_type;
    }
}

//void VhallRenderWdg::StartShowNickNameTimer() {
//    connect(&mUserNickNameShowTimer, SIGNAL(timeout()), this, SLOT(slot_NoticeShowNickName()));
//    mUserNickNameShowTimer.start(10);
//}

void VhallRenderWdg::slot_NoticeShowNickName() {
    if (isVisible()) {
        ShowNickName(true);
    }
    else {
        ShowNickName(false);
    }
}

void VhallRenderWdg::SetEnableUpdateRendWnd(bool enable) {
   //ui.widget_rendView->setUpdatesEnabled(enable);
}

HWND VhallRenderWdg::GetRenderWndID() {
	HWND id = (HWND)(ui.widget_rendView->winId());
	return id;
}

void VhallRenderWdg::SetViewState(int state) {
	switch (state) {
	    case RenderView_NoJoin: {
            /*ui.widget_rendView->hide();*/
            ui.widget_ctrl->show();
		    ui.label_viewState->setStyleSheet("border-image: url(:/vhallActive/img/vhallactive/nojoin_2.png);");
		    ui.label_viewState->show();
            ui.label_camera->hide();
		    break;
	    }
	    case RenderView_NoCamera: {
    /*        ui.widget_rendView->hide();*/
            ui.widget_ctrl->show();
		    ui.label_camera->setStyleSheet("border-image: url(:/vhallActive/img/vhallactive/closeCamera_2.png);");
		    ui.label_camera->show(); 
            ui.label_viewState->hide();
		    break;
	    }
       default: {
            ui.widget_rendView->show();
            ui.widget_ctrl->hide();
            break;
       }
	}
	mnStateType = state;
}

void VhallRenderWdg::SetUserCameraState(bool open) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->SetCameraState(open);
		if (open) {
			SetViewState(RenderView_None);
		}
		else {
			SetViewState(RenderView_NoCamera);
		}
        mbCameraOpen = open;
	}
}

bool VhallRenderWdg::GetUserCameraState() {
   return mbCameraOpen;
}

bool VhallRenderWdg::GetUserMicState() {
   return mbMicOpen;
}

void VhallRenderWdg::SetUserMicState(bool open) {
	if (mpMemberCtrlWdg) {
		mpMemberCtrlWdg->SetMicState(open);
        mbMicOpen = open;      
	}
}

//void VhallRenderWdg::enterEvent(QEvent *event) {
//    if (mpMemberCtrlWdg ) {
//        ui.widget_rendView->hide();
//        ui.widget_ctrl->show();
//	    mpMemberCtrlWdg->setFixedSize(this->width(), this->height());
//	    mpMemberCtrlWdg->move(0, 0);
//	    mpMemberCtrlWdg->show();
//        ui.label_viewState->hide();
//        ui.label_camera->hide();
//    }
//    QWidget::enterEvent(event);
//}

//void VhallRenderWdg::leaveEvent(QEvent *event) {
//    if (mpMemberCtrlWdg) {
//	    mpMemberCtrlWdg->hide();
//	    if (mnStateType == RenderView_NoJoin) {
//            ui.label_viewState->show();
//            ui.label_camera->hide();
//	    }
//        else if(mnStateType == RenderView_NoCamera){
//            ui.label_viewState->hide();
//            ui.label_camera->show();
//        }
//    }
//
//    if (mnStateType != RenderView_NoJoin && mnStateType != RenderView_NoCamera) {
//        ui.widget_rendView->show();
//        ui.widget_ctrl->hide();
//    }
//    QWidget::enterEvent(event);
//}

void VhallRenderWdg::slot_OnClickedCamera(bool isOpen) {
    if (isOpen) {
        globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
        mpMemberCtrlWdg->SetCameraState(false);
        globalToolManager->GetClassSDK()->CloseCamera();
        globalToolManager->GetDataManager()->SetMyCameraOpen(false);
    }
    else {
        globalToolManager->GetClassSDK()->OperateUserCamera(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
        mpMemberCtrlWdg->SetCameraState(true);
        globalToolManager->GetDataManager()->WriteLog("%s OpenCamera()", __FUNCTION__);
        bool bRet = globalToolManager->GetClassSDK()->OpenCamera();
        if (bRet) {
           globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() Succeed", __FUNCTION__);
        }
        else {
           globalToolManager->GetDataManager()->WriteLog("%s OpenCamera() failed", __FUNCTION__);
        }

        globalToolManager->GetDataManager()->SetMyCameraOpen(true);
    }
}

void VhallRenderWdg::slot_OnClickedMic(bool isOpen) {
    if (isOpen) {
        globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, true);
        mpMemberCtrlWdg->SetMicState(false);
    }
    else {
        globalToolManager->GetClassSDK()->OperateUserMic(globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id, false);
        mpMemberCtrlWdg->SetMicState(true);
    }
}

void VhallRenderWdg::slot_OnClickedSetMainView() {
    QString userId = GetUserJoinID();
	emit sig_setToSpeaker(userId);
}

void VhallRenderWdg::slot_OnClickedKickOutPublish() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);

    QString userId = GetUserJoinID();
    if (true) {
        //如果用户在线，发送下麦消息

       globalToolManager->GetDataManager()->WriteLog("%s  KickUserPublish %s", __FUNCTION__, userId.toStdString().c_str());
        globalToolManager->GetClassSDK()->KickUserPublish(userId.toStdString());
    }
    else {
        //如果用户不在线，强制踢出
       globalToolManager->GetDataManager()->WriteLog("%s UserPublishCallback(PushStreamEvent_UnPublish, %s) 237 PushStreamEvent_UnPublish", __FUNCTION__, userId.toStdString().c_str());
        globalToolManager->GetClassSDK()->UserPublishCallback(PushStreamEvent_UnPublish, userId.toStdString());
    }
    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);

}

void VhallRenderWdg::slot_OnClickedSetToSpeaker() {
    QString userId = GetUserJoinID();
	emit sig_setInMainView(userId);
}

void VhallRenderWdg::slot_OnClickedFloatRender(){
    QString userId = GetUserJoinID();
    QString streamId = GetStreamID();
    if (userId.isEmpty() || streamId.isEmpty()) {
        return;
    }
    hide();
    QString nickName = userId;
    if (mpVhallRenderMaskWdg) {
        nickName = mpVhallRenderMaskWdg->GetName();
    }
    emit sig_ClickedFloatRender(userId, streamId, nickName, mJoinType);
}

void VhallRenderWdg::ShowNickName(bool show) {
    if (show) {
        if (mpVhallRenderMaskWdg) {
            QPoint pos = ui.widget_rendView->mapToGlobal(ui.widget_rendView->pos());
            QPoint pos1 = this->mapToGlobal(QPoint(0, 0));
            int w = this->height();
            int h = this->height();
            mpVhallRenderMaskWdg->move(pos.rx(), pos.ry() + this->height()-mpVhallRenderMaskWdg->height());
            mpVhallRenderMaskWdg->show();
            mpVhallRenderMaskWdg->raise();
        }
    }
    else {
        if (mpVhallRenderMaskWdg) {
            mpVhallRenderMaskWdg->hide();
        }
    }
}
