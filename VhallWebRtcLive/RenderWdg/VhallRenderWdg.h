#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "ui_VhallRenderWdg.h"
#include "VhallRenderMaskWdg.h"
#include "MemberCtrlMaskWdg.h"
#include "CBaseRender.h"
#include "../Unility/Unility.h"

#define SMALL_RENDER_VIEW_SIZE	QSize(175, 131)
#define MAIN_RENDER_VIEW_SIZE	QSize(294, 243)

#define MAIN_RENDER_VIEW_WIDTH 320
#define MAIN_RENDER_VIEW_HEIGHT 240

class VhallRenderWdg : public QWidget, public CBaseRender
{
	Q_OBJECT
public:
	VhallRenderWdg(QWidget *parent = Q_NULLPTR);
	~VhallRenderWdg();

	void InitUserInfo(const QString& userId, QString streamId,int role_type = 0);
    void SetNickName(QString name);
    void SetRoleType(int role);
    void ShowNickName(bool show);
	HWND GetRenderWndID();
	void SetUserCameraState(bool open);
    bool GetUserCameraState();
	void SetUserMicState(bool open);
    bool GetUserMicState();
    void SetEnableUpdateRendWnd(bool enable);
    //void StartShowNickNameTimer();

private slots:
	void slot_OnClickedCamera(bool);
	void slot_OnClickedMic(bool);
	void slot_OnClickedSetMainView();
	void slot_OnClickedKickOutPublish();
	void slot_OnClickedSetToSpeaker();
    void slot_OnClickedFloatRender();
    void slot_NoticeShowNickName();

signals:
	void sig_setInMainView(QString);
	void sig_setToSpeaker(QString);
	void sig_ClickedCamera(QString, bool);
	void sig_ClickedMic(QString, bool);
	void sig_NotToSpeak(QString);//下麦
    void sig_ClickedFloatRender(QString, QString, QString, int);

//protected:
//	virtual void enterEvent(QEvent *event);
//	virtual void leaveEvent(QEvent *event);

private:
    void SetViewState(int state);

private:
    Ui::VhallRenderWdg ui;
	HWND mRenderWndID = NULL;
	bool mIsMainView = false;	//当前窗体是否显示在主窗体。

    int mJoinType;
	MemberCtrlMaskWdg *mpMemberCtrlWdg = NULL;     
    VhallRenderMaskWdg *mpVhallRenderMaskWdg = NULL;
	int mnStateType = 0;
    bool mbCameraOpen = true;
    bool mbMicOpen = true;
    //QTimer mUserNickNameShowTimer;
};
