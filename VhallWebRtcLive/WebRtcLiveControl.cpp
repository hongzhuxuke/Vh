#include "WebRtcLiveControl.h"
#include "VhallWebRtcLive.h"


CREATE_WND_FUNCTION(WebRtcLiveControl);
WebRtcLiveControl::WebRtcLiveControl(QObject *parent)
    : CBaseWnd(nullptr)
{
  
}

WebRtcLiveControl::~WebRtcLiveControl()
{

}

void WebRtcLiveControl::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find(START_WEB_RTC_LIVE);
    if (iter != mapParam.end()) {
        CBaseWnd* wnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
        if (wnd) {
            wnd->Notice(mapParam);
            wnd->show();
        }
    }
}

