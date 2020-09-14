#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include "ui_FullScreenRender.h"
#include "../Unility/Unility.h"
class ExitFullScreenWdg;
class ESCExitNotice;
class VhallRenderMaskWdg;
class FullScreenRender : public CBaseWnd
{
    Q_OBJECT

public:
    FullScreenRender(QWidget *parent = Q_NULLPTR);
    ~FullScreenRender();
    virtual void Notice(const CSTDMapParam &);
    CREATE_WND();
    QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;

    void HandleFullSizeCameraChange(const CSTDMapParam &);
    void HandleMicState(const CSTDMapParam &);
    void HandleSetUserInfo(const CSTDMapParam &);

	void SetRenderMaskWdg(VhallRenderMaskWdg * obj);
	void SetPenState(bool enable);

	void initTrophyRewardNum(const int& iTrophyNum);
	void SetShowNoMic(bool show/*,  bool bSelf*/);
	void SetShowStuNoMic(bool hasAudio);
   std::shared_ptr<vhall::VideoRenderReceiveInterface> GetRenderWnd();
   void setFixedSize(int w, int h);
public slots:
   void slot_hide();
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *e);
    void NoticeShowNickName();

    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QShowEvent *event);
private:
    void HandleSubScribeRender(const CSTDMapParam &);

private slots:
    void slot_HideExitFullScreen();
    void slot_ShowExitFullScreen();

private:
    Ui::FullScreenRender ui;
    //VhallRenderMaskWdg *mpVhallRenderMaskWdg = NULL;
    //ExitFullScreenWdg* mExitFullScreen;
    ESCExitNotice* mESCExitNotice;
    QTimer mHideExitFullScreenTimer;
    QString mUserId;
    QString mStreamId;
    QString mNickName;

    QString mSmallUserId;
    QString mSmallStreamId;
    QString mSmallNickName;

    bool bStartRenderBig = false;
    bool bStartRenderSmall = false;

    bool bBigRenderCloseCamera = false;
    bool bSmallRenderCloseCamera = false;

    bool bWndShown = false;
};
