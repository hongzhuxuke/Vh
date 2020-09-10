#include "LiveEventListener.h"
#include <QCoreApplication>

//LiveEventListener::LiveEventListener(QObject *parent)
//    : QObject(parent)
//{
//}
//
//LiveEventListener::~LiveEventListener()
//{
//    std::unique_lock<std::mutex> lock(mEventSubListMutex);
//    mEventSubList.clear();
//}
//
//void LiveEventListener::AppentEventSubScribeObj(QObject* obj) {
//    bool bFind = false;
//    std::unique_lock<std::mutex> lock(mEventSubListMutex);
//    std::list<QObject*>::iterator iter = mEventSubList.begin();
//    while (iter != mEventSubList.end()) {
//        if (*iter == obj) {
//            bFind = true;
//            break;
//        }
//        iter++;
//    }
//    if (!bFind) {
//        mEventSubList.push_back(obj);
//    }
//}
//
//void LiveEventListener::RemoveEventSubScribeObj(QObject* obj) {
//    std::unique_lock<std::mutex> lock(mEventSubListMutex);
//    std::list<QObject*>::iterator iter = mEventSubList.begin();
//    while (iter != mEventSubList.end()) {
//        if (*iter == obj) {
//            mEventSubList.erase(iter);
//            break;
//        }
//        iter++;
//    }
//}
//
//void LiveEventListener::PostCustomEventMsg(QEvent* event) {
//    std::unique_lock<std::mutex> lock(mEventSubListMutex);
//    std::list<QObject*>::iterator iter = mEventSubList.begin();
//    while (iter != mEventSubList.end()) {
//        QCoreApplication::postEvent(*iter, event);
//        iter++;
//    }
//}
//
//void LiveEventListener::OnWebRtcRoomConnetSuc() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomConnetSuc));
//}
//
//void LiveEventListener::OnWebRtcRoomConnetErr() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomConnetErr));
//}
//
//void LiveEventListener::OnWebRtcRoomReConneting() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomReConneting));
//}
//
//void LiveEventListener::OnWebRtcRoomReConneted() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomReConneted));
//}
//
//void LiveEventListener::OnWebRtcRoomRecover() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomRecover));
//}
////网络变化
//void LiveEventListener::OnWebRtcRoomNetWorkChanged() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomNetWorkChanged));
//
//}
////网络变化恢复
//void LiveEventListener::OnWebRtcRoomNetWorkChangedCover() {
//    PostCustomEventMsg(new QEvent(CustomEvent_WebRtcRoomNetWorkChangedCover));
//
//}
//
//void LiveEventListener::OnStartLocalCapture() {
//    PostCustomEventMsg(new QEvent(CustomEvent_CameraCaptureSuc));
//}
//
//void LiveEventListener::OnCameraCaptureErr() {
//    PostCustomEventMsg(new QEvent(CustomEvent_CameraCaptureErr));
//}
//
//void LiveEventListener::OnStartPushLocalStream(std::string stream) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_PushCameraStreamSuc, stream));
//}
//
//void LiveEventListener::OnStopPushLocalStream(std::string message) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_StopPushCameraStreamSuc, message));
//}
//
//void LiveEventListener::OnLocalMicOpenErr() {
//    PostCustomEventMsg(new QEvent(CustomEvent_LocalMicOpenErr));
//}
//
//void LiveEventListener::OnReciveRemoteUserLiveStream(const std::string& user, const std::string& stream, const int type, bool hasVideo, bool hasAudio) {
//    PostCustomEventMsg(new QEventStream(CustomEvent_ReciveRemoteUserLiveStream, user, stream, type, hasVideo, hasAudio));
//}
//
//void LiveEventListener::OnLocalUserLiveStreamPushErr(const std::string& user, const std::string& stream, const int type) {
//    PostCustomEventMsg(new QEventStream(CustomEvent_LocalUserLiveStreamPushErr, user, stream, type));
//}
//
//void LiveEventListener::OnRemoveRemoteUserLiveStream(const std::string& user, const std::string& stream, const int type) {
//    PostCustomEventMsg(new QEventStream(CustomEvent_RemoveRemoteUserLiveStream, user, stream, type));
//}
//
//void LiveEventListener::OnSetBroadCastLayout(LAYOUT_MODE mode, std::string msg) {
//    PostCustomEventMsg(new QEventConfigBroadCast(CustomEvent_ConfigBroadCast, msg));
//}
//
//void LiveEventListener::OnSetMixLayoutMainView(std::string msg) {
//    PostCustomEventMsg(new QEventConfigBroadCast(CustomEvent_SetMixLayoutMainView, msg));
//}
//
//void LiveEventListener::OnLocalStreamToRestart() {
//    PostCustomEventMsg(new QEvent(CustomEvent_LocalStreamToRestart));
//}
//
///*开始桌面共享采集回调*/
//void LiveEventListener::OnStartDesktopCaptureSuc() {
//    PostCustomEventMsg(new QEvent(CustomEvent_DesktopCaptureSuc));
//}
//
///*开始桌面共享采集回调*/
//void LiveEventListener::OnStartDesktopCaptureErr() {
//    PostCustomEventMsg(new QEvent(CustomEvent_DesktopCaptureErr));
//}
//
///*开始桌面共享采集推流*/
//void LiveEventListener::OnStartPushDesktopStream(std::string stream) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_DesktopStartPushStream, stream));
//}
//
///*停止桌面共享采集推流*/
//void LiveEventListener::OnStopPushDesktopStream(std::string msg) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_DesktopStopPushStream, msg));
//}
//
//void  LiveEventListener::OnPrepareMediaFileCaptureSuc() {
//
//}
//
///*插播文件准备失败*/
//void  LiveEventListener::OnPrepareMediaFileCaptureErr() {
//
//}
//
///*开始文件插播推流*/
//void LiveEventListener::OnStartPushMediaFileStream(std::string stream) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_MediaFileStartPushStream, stream));
//}
///*停止文件插播推流*/
//void LiveEventListener::OnStopPushMediaFileStream(std::string msg) {
//    PostCustomEventMsg(new QEventPublishStream(CustomEvent_MediaFileStopPushStream, msg));
//}
//