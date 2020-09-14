#pragma once

#include <QWidget>
#include <unordered_map>
#include "ui_RenderManager.h"
#include "VhallRenderWdg.h"

#define ITEM_WIDTH   160
#define ITEM_HEIGHT  120

class TeacherRenderWdg;
class WebRtcLiveWdg;
class RenderManager : public QWidget
{
    Q_OBJECT
public:
    RenderManager(WebRtcLiveWdg* pWdg, QWidget *parent = Q_NULLPTR);
    ~RenderManager();
    int iRenderSize();
    void addRender(TeacherRenderWdg* pRender);
    void AddInitReadyPublishUser(QString userid, QString nickName,int role_type,bool hasVideo, bool hasAudio);
    void AddLocalPublishUser(QString userid, QString streamId, QString nickName, int role_type, bool hasVideo, bool hasAudio);
    void RemovePublishUser(QString userid);
    void CloseFloatRender(QString userid);
    void HandleSubScribStream(QEvent* event);

    void SetUserMicState(QString userid, bool open);
    void SetAllMicState(const bool& bOpen);
    void SetUserCameraState(QString userid, bool open);
    void SetUserPenAuth(QString userId, bool hasAuth);
    void ClearAllRender();
    void UpdateLocalStreamID(QString userId,QString streamID);
    void UnPublishAllUsers();
    void ShowAllRender();
    void HideAllRender();

    int GetListWidgetWidth();
    bool IsExistRender(const QString& uid);
    void SetAllMuteState(bool open);
    void HandleUserLineState(QString user_id,bool onLine);
	void AddTrophyCount(const QString& strId, const int& iAdd);
	void initTrophyRewardNum(const QString& strId, const int& iNum);
	void SetCameraQss(const QString& strId, const bool& bOpenState);
	HWND GetRenderWndID(const QString& strUid);
   std::shared_ptr<vhall::VideoRenderReceiveInterface> GetRenderWnd(const QString& strUid);
	void UpdateInfo(const QString & strUid, const QString& strName);
   int IUserCount();

   TeacherRenderWdg* GetFloateRender(QString strId);

   void ShowRender(QStringList strListId);
public slots:
	void slot_MuteAllSubScribeAudio(const bool& bMute);
   void show();
protected:
   void showEvent(QShowEvent *event);
   bool eventFilter(QObject * obj, QEvent * event);
   void wheelEvent(QWheelEvent *event);
private:
    void InsertWidgetToListWidget(TeacherRenderWdg* render, QString userid, bool bADD = true);
private slots:
    void slot_clickedFloatRender(QString, QString, QString, int);
    //void slot_SetForces();
private:
   void showRender(TeacherRenderWdg* render);
   void setCenterManageWidth();
   void TestLayout();
   void setHidden(int iHiddenCount);
    std::unordered_map<QString, TeacherRenderWdg*> mRenderMap;//TeacherRenderWdg*
    Ui::RenderManager ui;
	bool mMuteAllAudio = false;
   bool mShowSelf = false;
   WebRtcLiveWdg* pRtcLiveWdg = nullptr;
};
