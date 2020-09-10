#pragma once
#include "CWidget.h"
#include "ui_TeacherCtrlWdg.h"
#include "../Unility/Unility.h"
#include "CToolTips.h"
class TeacherCtrlWdg : public CWidget
{
    Q_OBJECT

public:
    TeacherCtrlWdg(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~TeacherCtrlWdg();

    void InitBtn(QString userId);
    void InitByWebView();
    void SetPageIndex(int index);
    void ShowStuPublishBtn(bool show);

    void SetAllMuteAudioState(bool mute);
    void SetPenState(bool enable);
    void SetCameraState(bool open);
    void SetMicState(bool open);
    void SetSmallRenderCameraState(bool hasVideo);
    void SetSmallRenderMicState(bool hasAudio);
    void CheckShowChangeBigScreen(bool show);
    void ShowStudentCtrl(bool show);

private slots:
    void slot_OnClickedFloatRender();
    void slot_OnClickedMic();
    void slot_OnClickedCamera();
    void slot_OnClickedAllMute();
    void slot_OnClickedFullScreen();
    void slot_OnClickedAllMuteCamera();
    void slot_OnAllUnpublish();

    void slot_OnStuClickedCamera();
    void slot_OnStuClickedMic();
    void slot_OnstuClickedUnPublish();
    void slot_OnStuPenClickedPen();
    void slot_OnStuClickedFloat();
   void slot_OnSetBigScreen();
    void slot_OnStuClickedFullWnd();
    void slot_OnOpenClassStuMic();
    void slot_OnOpenClassStudentCamera();
    void slot_OnChangeCamera();
	void on_btnReward_clicked(bool checked=false);//½±Àø
public slots:
	void slot_PicPush(const bool& iPicPush);
signals:
    void sig_OnClickedFloatRender();
    void sig_OnClickedMic();
    void sig_OnClickedCamera();
    void sig_OnClickedAllMute();
    void sig_OnClickedFullScreen();
    void sig_OnClickedAllMuteCamera();
    void sig_OnAllUnpublish();
    void sig_OnSetBigScreen();
    void sig_OnStuClickedCamera();
    void sig_OnStuClickedMic();
    void sig_OnstuClickedUnPublish();
    void sig_OnStuPenClickedPen(bool);
    void sig_OnStuClickedFloat();
    void sig_OnStuClickedFullWnd();

    void sig_OnOpenClassStuMic();
    void sig_OnOpenClassStudentCamera();

    void sig_OnChangeCamera();
	void sig_FavorStudent();
protected:
    //virtual bool eventFilter(QObject* receiver, QEvent * event);
    //virtual void hideEvent(QHideEvent *event);
	//virtual void showEvent(QShowEvent *event);
private:
    Ui::TeacherCtrlWdg ui;
    bool bHasPen = false;
    //CToolTips* mToolTips;
};
