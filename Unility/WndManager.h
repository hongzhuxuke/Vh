#pragma once

#include <QObject>
#include <mutex>
#include <map>
#include <QWidget>
#include <QString>
#include<map>
#include<string>
#include <QDialog>
#include <unordered_map>
#include <QEvent>
#include "WndDef.h"
#include <QHash>
#include <QString>
#include <functional>

//namespace std {
//    template<> struct hash<QString> {
//        std::size_t operator()(const QString& s) const {
//            return qHash(s);
//        }
//    };
//}

#define     WND_ABOUT               "WND_ABOUTDLG"
#define     WND_ID_LOGIN                "WND_ID_LOGIN"
#define     WND_ID_LOADING              "WND_ID_LOADING"
#define     WND_ID_WEBRTC_MAIN          "WND_ID_WEBRTC_MAIN"
#define     WND_ID_WEBRTC_COTROL        "WND_ID_WEBRTC_COTROL"
#define     WND_ID_WEBRTC_SETTING       "WND_ID_WEBRTC_SETTING"
#define     WND_ID_INVITE_TO_SPEAK      "WND_ID_INVITE_TO_SPEAK" //邀请上麦窗口
#define     WND_ID_ASK_FOR_SPEAK        "WND_ID_ASK_FOR_SPEAK"    //申请上麦窗口
#define     WND_ID_MAINUI               "WND_ID_MAINUI"

//#define     WND_ID_FLOAT_RENDER         "WND_ID_FLOAT_RENDER"
#define     WND_ID_NOTICE_SEND          "WND_ID_NOTICE_SEND"
#define     WND_ID_MEDIA_PLAY_WEBVIEW   "WND_ID_MEDIA_PLAY_WEBVIEW"
//#define     WND_ID_FULL_SCREEN_RENDER   "WND_ID_FULL_SCREEN_RENDER"
//#define     WND_ID_TIPS_WND             "WND_ID_TIPS_WND"
//#define     WND_ID_VIDEO_CHOICE_UI      "WND_ID_VIDEO_CHOICE_UI"
#define     WND_ID_QUESTION_UI          "WND_ID_QUESTION_UI"
//#define     WND_ID_CAMERA_DOUBLE_PUSH_UI          "WND_ID_CAMERA_DOUBLE_PUSH_UI"
#define     WND_ID_ANSWER_TOOL          "WND_ID_ANSWER_TOOL"
#define     WND_ID_TIMER_TOOL          "WND_ID_TIMER_TOOL"
#define     WND_ID_END                  "WND_ID_END"
//#define     WND_ID_DECICE_TESTING         "WND_ID_DECICE_TESTING"


#define Str_Function                    "Str_Function"
#define Str_Function_GetStartTime       "Str_Function_GetStartTime"
#define Str_Function_FromLogin          "Str_Function_FromLogin"
#define Str_Quality_Enabeld "Str_Quality_Enabeld"
#define Str_Quality_UnEnabeld "Str_Quality_UnEnabeld"
#define Str_Function_SubScribe          "Str_Function_SubScribe"
#define Str_Function_PublishStream      "Str_Function_PublishStream"
#define Str_Function_FloadAndFullWndRender      "Str_Function_FloadAndFullWndRender"
#define Str_Function_UserLine           "Str_Function_UserLine"

#define Str_Function_AnswerToolTeacher           "Str_Function_AnswerToolTeacher"
#define Str_Function_AnswerToolStudent           "Str_Function_AnswerToolStudent"
#define Str_Function_AnswerToolItems           "Str_Function_AnswerToolItems"

#define Str_CMD_MSG                 "Str_CMD_MSG"
#define Str_Start_Time              "Str_Start_Time"
#define Str_OnLineNum               "Str_OnLineNum"


#define START_WEB_RTC_LIVE      "StartWebRtcLive"
#define DEV_TEST "DevTest"
/*TEACHER_VIEW 浮窗、全屏时候  判断本地视频回显位置   在大画面/小画面
	1.【公开课-学员】 本地视频回显在画中画的小画面，
	2.【公开课-讲师】回显在大画面
	3.【小课堂】 只有一个画面*/
#define TEACHER_VIEW            "TEACHER_VIEW"
#define TipsMsg                 "TipsMsg"
#define WEBRTC_ROOM_HOST        "WEBRTC_ROOM_HOST"
#define PARAM_TRUE              "TRUE"
#define PARAM_FALSE             "FALSE"
#define USER_ID                 "user_id"  
#define USER_LINE_STATE         "USER_LINE_STATE"   
#define WND_MANAGER_WIDTH       "WND_MANAGER_WIDTH"

#define WND_CENTER_WIDTH        "WND_CENTER_WIDTH"
#define WND_CENTER_HEIGHT       "WND_CENTER_HEIGHT"
#define STREAM_ID               "stream_id" 
#define LOCAL_STREAM_ID_CHANGED "LOCAL_STREAM_ID_CHANGED" 
#define NICK_NAME               "nick_name"
#define ROLE_TYPE               "role_type"
#define TROPHY_COUNT            "trophy_count"
#define STOPSHOWMEDIAVIDEO      "StopShowMediaVideo"
#define DEALSTOPPLAY            "DealStopPlay"
#define ShowCancelButton        "ShowCancelButton"
#define ShowTipsSize        "ShowTipsSize"
#define ShowRecordSet           "ShowRecordSet"
#define SHOW_NICK_NAME           "ShowNickName"
#define BIG_RENDER_CAMERA_IS_CLOSE  "BIG_RENDER_CAMERA_IS_CLOSE"
#define BIG_RENDER_MIC_IS_CLOSE     "BIG_RENDER_MIC_IS_CLOSE"
#define SMALL_RENDER_CAMERA_IS_CLOSE  "SMALL_RENDER_CAMERA_IS_CLOSE"
#define SMALL_RENDER_MIC_IS_CLOSE     "SMALL_RENDER_MIC_IS_CLOSE"

#define SMALL_RENDER_USER_ID        "SMALL_RENDER_USER_ID"
#define SMALL_RENDER_STEAM_ID       "SMALL_RENDER_STEAM_ID"
#define MEDIA_FILE_CTRL_MSG       "MEDIA_FILE_CTRL_MSG"
#define INIT               "init" 
#define TROPHYREWARDNUM               "TrophyRewardNum" 
#define CLOSE_MEDIA_FILE "CLOSE_MEDIA_FILE"
#define CHOOSE_TEST_PAPER_DLG "CHOOSE_TEST_PAPER_DLG"


#define CSTDMapParam  std::map<QString, QString> 
#define CSTDMapParamPtr  std::map<QString, void*> 

class CBaseWnd : public QDialog
{
public:
    CBaseWnd(QWidget *parent = Q_NULLPTR);
    virtual ~CBaseWnd();
    virtual void Notice(const CSTDMapParam &) = 0;
    virtual void Notice(const CSTDMapParamPtr &) {};
    virtual void GetParam(CSTDMapParam& ) {};

    virtual void Notice(QEvent* event) {};

protected:
    virtual void showEvent(QShowEvent *event);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);

	bool m_bIsEnableMove = true;
   QPoint mPressPoint;
   bool mIsMoved = false;
private:
    
    
};


typedef CBaseWnd* (*CreateFunction)();
#define CREATE_WND()    static CBaseWnd* CreateFunction();
#define REGISTER_WND(T) T::CreateFunction
#define CREATE_WND_FUNCTION(T) \
CBaseWnd* T::CreateFunction() { \
    return new T();             \
}                               \


class WndManager : public QObject
{
   Q_OBJECT

public:
    WndManager(QObject *parent = nullptr);
    ~WndManager();
    void RegisterWndFactory(QString wndId, CreateFunction);
    CBaseWnd* FindWnd(QString wndId, QString strWndIndex = QString());
    bool IsWndExist(QString wndId );
    bool HideWnd(QString wndId );
    bool ShowWnd(QString wndId );
    bool DestoryWnd(QString wndId );
    void DestoryAllWnd();
    bool PostEventToWnd(QString wndId, QEvent* event);
    bool DestoryExistWnd(QString wndId);
private:
    std::mutex mWndMutex;
    std::unordered_map<QString, CreateFunction> mWndFactory;
    std::unordered_map<QString, CBaseWnd*> mWndMap;

    QPoint mPressPoint;
    bool mIsMoved = false;
    bool m_bIsEnableMove = true;
};
