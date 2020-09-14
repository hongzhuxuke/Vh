#include "DoubleRenderWdg.h"
#include <QMouseEvent>
extern ToolManager *globalToolManager;
DoubleRenderWdg::DoubleRenderWdg(QWidget *parent)
    :QWidget(parent)
	//mVhallRenderMaskWdg(nullptr)
    //, mMemberCtrlMaskWdg(nullptr)
{
    ui.setupUi(this);
    ui.widget_student->hide();
    //mVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
    //mMemberCtrlMaskWdg = new MemberCtrlMaskWdg(this);
}

DoubleRenderWdg::~DoubleRenderWdg()
{
}

void  DoubleRenderWdg::StartRenderTeacherView(QString uid) {
    if (globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type == vlive::JOIN_ROLE_Teacher) {
        //HWND id = (HWND)(ui.widget_teacher->winId());
        //globalToolManager->GetClassSDK()->StartRenderLocalStream(VHStreamType_AVCapture, (void*)(id));

        globalToolManager->GetClassSDK()->StartRenderLocalStreamByInterface(VHStreamType_AVCapture, ui.widget_teacher->GetVideoReciver());
    }
    else {
        //HWND id = (HWND)(ui.widget_teacher->winId());
        ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
        globalToolManager->GetDataManager()->WriteLog("%s StartRenderRemoteStream big id %s  my id :%s", __FUNCTION__, QString::number(baseInfo.bigScreen).toStdString().c_str(), baseInfo.mUserInfo.join_id.c_str());
		//globalToolManager->GetClassSDK()->StartRenderRemoteStream(mTeacherJoinId.toStdWString(), VHStreamType_AVCapture, (void*)(id));
		globalToolManager->GetClassSDK()->StartRenderRemoteStreamByInterface(mTeacherJoinId.toStdWString(), VHStreamType_AVCapture, ui.widget_teacher->GetVideoReciver());
    }
}

void  DoubleRenderWdg::StartRenderStudentView(QString uid) {

}

void DoubleRenderWdg::SetTeacherJoinId(QString join_id) {
    mTeacherJoinId = join_id;
}

void DoubleRenderWdg::SetStudendJoinId(QString join_id) {
    mStudendJoinId = join_id;
}

void DoubleRenderWdg::SetTeacherCameraStreamId(QString stream_id) {
    mTeacherCameraStreamId = stream_id;
}

void DoubleRenderWdg::SetStudentCameraStreamId(QString stream_id) {
    mStudendCameraStreamId = stream_id;
}

void DoubleRenderWdg::SetTeacherNickName(QString name) {
    mTeacherNickName = name;
    if (ui.mVhallRenderMaskWdg) {
        ui.mVhallRenderMaskWdg->SetUserName(name);
    }
}

void DoubleRenderWdg::SetStudentNickName(QString name) {
    mStudendNickName = name;
}