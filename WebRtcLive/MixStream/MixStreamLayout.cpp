#include "MixStreamLayout.h"
#include <QApplication>
#include "ConfigSetting.h"
#include "pathmanager.h"
#include "WebRtcLiveWdg.h"

extern ToolManager *globalToolManager;

MixStreamLayout::MixStreamLayout()
{
    mLayoutTimer.setSingleShot(true);
    mMainViewTimer.setSingleShot(true);
    connect(&mLayoutTimer, SIGNAL(timeout()), this, SLOT(slot_OnSetBroadCastLayoutTimeOut()));
    connect(&mMainViewTimer, SIGNAL(timeout()), this, SLOT(slot_OnSetMainViewTimeOut()));
    connect(&mRePushLocalStreamTimer, SIGNAL(timeout()), this, SLOT(slot_OnRepushLocalStream()));
    connect(&mRePushDeskStopStreamTimer, SIGNAL(timeout()), this, SLOT(slot_OnRepushDeskTopStream()));
    connect(&mRePushSourceStreamTimer, SIGNAL(timeout()), this, SLOT(slot_OnRepushSourceStream()));
    connect(&mRePushMediaFileStreamTimer, SIGNAL(timeout()), this, SLOT(slot_OnRepushMediaFileStream()));

    mRePushMediaFileStreamTimer.setSingleShot(true);
    mRePushLocalStreamTimer.setSingleShot(true);
    mRePushDeskStopStreamTimer.setSingleShot(true);
    mRePushSourceStreamTimer.setSingleShot(true);
    //ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();

    //if (roomInfo.mWebinarInfo.linkmic_apply_type == CLASS_JOIN_MEMBER_1V4) {
    //    mMixLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_5_1D4T;
    //}
    //else if (roomInfo.mWebinarInfo.linkmic_apply_type == CLASS_JOIN_MEMBER_1V8) {
    //    mMixLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_9_1D8T;
    //}
    //else if (roomInfo.mWebinarInfo.linkmic_apply_type == CLASS_JOIN_MEMBER_1V12) {
    //    mMixLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_13_1L12R;
    //}
    //else {
    //    mMixLayoutMode = CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
    //}

}

MixStreamLayout::~MixStreamLayout()
{
}

void MixStreamLayout::StopAllRepublish() {
    mRePushLocalStreamTimer.stop();
    mRePushDeskStopStreamTimer.stop();
    mRePushSourceStreamTimer.stop();
    mRePushMediaFileStreamTimer.stop();
    mLayoutTimer.stop();
    mMainViewTimer.stop();
}

BROAD_CAST_DPI MixStreamLayout::GetBradCastIndex(int broadIndex) {
    BROAD_CAST_DPI broadCastProfile = BROAD_CAST_DPI_960_720;
    switch (broadIndex)
    {
    //case 480:
    //    broadCastProfile = BROAD_CAST_DPI_640_480;
    //    break;
    case 720:
        broadCastProfile = BROAD_CAST_DPI_960_720;
        break;
    case 960:
        broadCastProfile = BROAD_CAST_DPI_1280_960;
        break;
    case 1080:
        broadCastProfile = BROAD_CAST_DPI_1440_1080;
        break;
    default:
		broadCastProfile = BROAD_CAST_DPI_960_720;
        break;
    }
    return broadCastProfile;
}

int MixStreamLayout::iMixLayoutMode()
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCSIG__);
   int iLayoutMode = CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR;
   
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if(baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL){
      switch (baseInfo.mWebinarInfo.mixedLayout) {
      case eMixedLayoutMode_Tiled: {
         ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
         switch (roomInfo.mWebinarInfo.linkmic_apply_type)
         {
         case CLASS_JOIN_MEMBER_1V4:
            iLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_5_1T4D;
            break;
         case CLASS_JOIN_MEMBER_1V8:
            iLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_9_1L8R;
            break;
         case CLASS_JOIN_MEMBER_1V12:
            iLayoutMode = CANVAS_LAYOUT_PATTERN_TILED_13_1L12R;
            break;
         default:
            break;
         }
      }
                                    break;
      case eMixedLayoutMode_Grid: {
         if(mpWebRtcLiveWdg){
            int iRenderCount = mpWebRtcLiveWdg->iRenderCount();//学员的个数
            switch (iRenderCount) {
               case 0: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_1; break;
               case 1: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_2_H; break;
               case 2: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_3_E; break;
               case 3: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_4_M; break;
               case 4: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_5_D; break;
               case 5: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_6_E; break;
               default: iLayoutMode = CANVAS_LAYOUT_PATTERN_GRID_9_E; break;
            }
         }
      }
                                    break;
      default:break;
      }
   }
   globalToolManager->GetDataManager()->WriteLog("%s iLayoutMode %d Leave ", __FUNCSIG__, iLayoutMode);
    return iLayoutMode;
}

void MixStreamLayout::SetMixStreamLayout(bool startClass, int layout, bool hasDesktopStream) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
    ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    int setLayout = (layout == -1 ? iMixLayoutMode() : layout);
    mCurrentLayoutMode = (LayoutMode)setLayout;

    //QString msConfPath = QApplication::applicationDirPath() + "/" + QString::fromStdWString(CONFIGPATH);
	QString msConfPath = globalToolManager->GetConfigPath();
    //QString configDefinition = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY, Fluent_definition);
    //ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    //int interactiveVideoProfile = 360;
    //int mixLayoutProfile = 720;
    //globalToolManager->GetVideoProfileByConfig(configDefinition, interactiveVideoProfile, mixLayoutProfile);

   int iQualityId = ConfigSetting::ReadString(msConfPath, GROUP_DEFAULT, KEY_PUBLISH_QUALITY_NUMBER, "1").toInt();
   globalToolManager->GetDataManager()->WriteLog("%s iCurLabelNumber:%d\n", __FUNCTION__, iQualityId);
   int mixLayoutProfile = 720;
   globalToolManager->GetVideoProfileByConfig(iQualityId, mixLayoutProfile, ToolManager::eVideoProFileTypeBroadcast);

    BROAD_CAST_DPI profile = GetBradCastIndex(mixLayoutProfile);
    if (hasDesktopStream || (mixLayoutProfile == CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL)) {
       profile = BROAD_CAST_DPI_1920_1080;
    }

    ToolManager *callbackTool = globalToolManager;
    int nRet = -1;
    if (startClass) {
       nRet = globalToolManager->GetClassSDK()->StartBroadCastLayout((LayoutMode)setLayout, profile,
          [&, callbackTool, setLayout, hasDesktopStream](const std::string& result, const std::string& msg) {
          if (result == "success") {
             globalToolManager->GetDataManager()->WriteLog("%s success", __FUNCTION__);
             callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, CALL_BACK_SUC, "", setLayout, CALL_BACK_SUC, result, hasDesktopStream == true ? "has_desktop_layout" : "no_desktop_layout"));
          }
          else {
             globalToolManager->GetDataManager()->WriteLog("%s Faild", __FUNCTION__);
             callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, 0, "", setLayout, 0, result, hasDesktopStream == true ? "has_desktop_layout" : "no_desktop_layout"));
          }
       });
    }
    else {
       nRet = globalToolManager->GetClassSDK()->SetConfigBroadCastLayOut((LayoutMode)setLayout, 
          [&, callbackTool, setLayout, hasDesktopStream](const std::string& result, const std::string& msg) {
          if (result == "success") {
             globalToolManager->GetDataManager()->WriteLog("%s success", __FUNCTION__);
             callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, CALL_BACK_SUC, "", setLayout, CALL_BACK_SUC, result, hasDesktopStream == true ? "has_desktop_layout" : "no_desktop_layout"));
          }
          else {
             globalToolManager->GetDataManager()->WriteLog("%s Faild", __FUNCTION__);
             callbackTool->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_StartMixStream, 0, "", setLayout, 0, result, hasDesktopStream == true ? "has_desktop_layout" : "no_desktop_layout"));
          }
       });
    }

    if (nRet == VhallLive_SERVER_NOT_READY) {
        globalToolManager->GetDataManager()->WriteLog("HandleMixStreamLayout SetBroadCastLayout VhallLive_SERVER_NOT_READY");
        //当调用混流时服务端没有准备好，触发定时器重新进行布局设置。
        mLayoutTimer.start(RESET_LAYOUT_TIMEOUT);
    }
    else {
        globalToolManager->GetDataManager()->WriteLog("HandleMixStreamLayout SetBroadCastLayout %d nRet:%d", setLayout, nRet);
    }

    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

int MixStreamLayout::GetCurrentLayOut() {
    return mCurrentLayoutMode;
}

void MixStreamLayout::SetMainViewStreamID(const QString& stream_id) {
    mStreamId = stream_id;
    globalToolManager->GetDataManager()->WriteLog("%s stream_Id:%s", __FUNCTION__, stream_id.toStdString().c_str());
}

void MixStreamLayout::SetMainViewStream(QString streamId) {
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Student) return;

    mStreamId = streamId;
    globalToolManager->GetDataManager()->WriteLog("%s stream_Id:%s", __FUNCTION__, streamId.toStdString().c_str());
    int nRet = globalToolManager->GetClassSDK()->SetMainView(streamId.toStdString());
    if (nRet == VhallLive_SERVER_NOT_READY) {
        globalToolManager->GetDataManager()->WriteLog("SetMainViewStream VhallLive_SERVER_NOT_READY");
        //当调用时服务端没有准备好，触发定时器重新进行布局设置。
        mMainViewTimer.start(RESET_LAYOUT_TIMEOUT);
    }
    else {
        globalToolManager->GetDataManager()->WriteLog("SetMainViewStream streamId %s nRet:%d", streamId.toStdString().c_str(), nRet);
    }
}


void MixStreamLayout::slot_OnSetBroadCastLayoutTimeOut() {
    SetMixStreamLayout(false, mReSetLayoutType, bResetHasDesktop);
    mLayoutTimer.stop();
}

void MixStreamLayout::SetRetLiveWdg(WebRtcLiveWdg * pWebRtcLiveWdg)
{
   mpWebRtcLiveWdg = pWebRtcLiveWdg;
}

void MixStreamLayout::slot_OnSetMainViewTimeOut() {
    SetMainViewStream(mStreamId);
    mMainViewTimer.stop();
}

void MixStreamLayout::RestartSetLayout(int layout,bool hasDesktop) {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
    mLayoutTimer.start(RESET_LAYOUT_TIMEOUT); 
    mReSetLayoutType = layout;
    bResetHasDesktop = hasDesktop;   
    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void MixStreamLayout::RestartSetMainView() {
    mMainViewTimer.start(RESET_LAYOUT_TIMEOUT);
}

void MixStreamLayout::RePushLocalStream() {
   //QDateTime dateTime = QDateTime::currentDateTime();
    globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
    mRePushLocalStreamTimer.start(RESET_LAYOUT_TIMEOUT);
}

void MixStreamLayout::RePushDesktopSream(int type) {
    globalToolManager->GetDataManager()->WriteLog("%s  type %d ", __FUNCTION__, type);
    if (type == VHStreamType_SoftWare) {   
        mRePushSourceStreamTimer.start(RESET_LAYOUT_TIMEOUT);
    }
    else {
        mRePushDeskStopStreamTimer.start(RESET_LAYOUT_TIMEOUT);
    }
}

void MixStreamLayout::SetAlreadyMixed(bool mixed) {
    mbAlreadySetMixed = mixed;
}

void MixStreamLayout::RePushMediaFileStream() {
   mRePushMediaFileStreamTimer.start(RESET_LAYOUT_TIMEOUT);
}

void MixStreamLayout::slot_OnRepushLocalStream() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
    int nRet = globalToolManager->GetClassSDK()->StartPushLocalStream();
    if (VhallLive_ROOM_DISCONNECT == nRet) {
        RePushLocalStream();
    }
    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void MixStreamLayout::slot_OnRepushDeskTopStream() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
    int nRet = globalToolManager->GetClassSDK()->StartPushDesktopStream();
    if (VhallLive_ROOM_DISCONNECT == nRet) {
        RePushDesktopSream(VHStreamType_Desktop);
    }
    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void MixStreamLayout::slot_OnRepushSourceStream() {
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);

    int nRet = globalToolManager->GetClassSDK()->StartPushSoftWareStream();
    if (VhallLive_ROOM_DISCONNECT == nRet) {
        RePushDesktopSream(VHStreamType_SoftWare);
    }
    globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}



void MixStreamLayout::slot_OnRepushMediaFileStream() {
   int nRet = globalToolManager->GetClassSDK()->StartPushMediaFileStream();
   if (VhallLive_ROOM_DISCONNECT == nRet) {
      RePushMediaFileStream();
   }
}