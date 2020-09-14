#include "VhallWebRtcLive.h"
#include "WebRtcLiveWdg.h"
#include "WebRtcLiveControl.h"
//#include "CScreenShareToolDlg.h"
#include "VhallIALiveSettingDlg.h"
#include "InviteJoinWdg\InviteJoinWidget.h"
#include "InviteJoinWdg\ApplyJoinWdg.h"

#include "RenderWdg/FloatRender.h"
#include "./Notice/NoticeSendWdg.h"
#include "./WebView/MediaPlayWebView.h"
#include "FullScreenRender.h"
#include "Tips/TipsWdg.h"
#include "MediaFilePlay/VideoChoiceUI.h"
#include "./WebView/AnswerWebView.h"
//#include "AuxiliaryCameraChoiceWdg.h"
#include "./Answer/AnswerTool.h"
#include "./Timer/TimerWdg.h"
#include "./DeviceTesting/DeviceTestingWdg.h"

ToolManager *globalToolManager = nullptr;

VhallWebRtcLive::VhallWebRtcLive()
{
}


bool RegisterGlobalManager(void* reg) {
    if (reg == nullptr) {
        return false;
    }
    globalToolManager = (ToolManager*)reg;
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_MAIN, REGISTER_WND(WebRtcLiveWdg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_COTROL, REGISTER_WND(WebRtcLiveControl));
	//globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_MAIN, REGISTER_WND(CScreenShareToolDlg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_WEBRTC_SETTING, REGISTER_WND(VhallIALiveSettingDlg));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_INVITE_TO_SPEAK, REGISTER_WND(InviteJoinWidget));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_ASK_FOR_SPEAK, REGISTER_WND(ApplyJoinWdg));

    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_FLOAT_RENDER, REGISTER_WND(FloatRender));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_NOTICE_SEND, REGISTER_WND(NoticeSendWdg));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_MEDIA_PLAY_WEBVIEW, REGISTER_WND(MediaPlayWebView));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_FULL_SCREEN_RENDER, REGISTER_WND(FullScreenRender));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_TIPS_WND, REGISTER_WND(TipsWdg));
	//globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_VIDEO_CHOICE_UI, REGISTER_WND(VideoChoiceUI));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_QUESTION_UI, REGISTER_WND(AnswerWebView));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_CAMERA_DOUBLE_PUSH_UI, REGISTER_WND(CameraCaptureAuxiliary));
    globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_ANSWER_TOOL, REGISTER_WND(AnswerTool));
	//globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_DECICE_TESTING, REGISTER_WND(DeviceTestingWdg));
    //globalToolManager->GetWndManager()->RegisterWndFactory(WND_ID_TIMER_TOOL, REGISTER_WND(TimerWdg));
    
    return true;
}
