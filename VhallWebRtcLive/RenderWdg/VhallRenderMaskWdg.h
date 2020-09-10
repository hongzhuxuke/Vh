#pragma once

#include <QWidget>
#include "ui_VhallRenderMaskWdg.h"
#include <QMouseEvent>
#include "priConst.h"
class  TeacherRenderWdg;

class VhallRenderMaskWdg : public QWidget
{
	Q_OBJECT

public:
	VhallRenderMaskWdg(QWidget *parent = Q_NULLPTR);
	~VhallRenderMaskWdg();
	void SetTeacherRenderWdg(TeacherRenderWdg* pRenderWdg);
   void SetUserName(const QString &userName);
   void SetShowNoMic(bool show/*, bool bSelf*/);
   void SetShowStuNoMic(bool show);

   void SetRenderMaskWdg(VhallRenderMaskWdg* obj);

   void SetShowPenLogo(bool show);
   bool GetBEnter();
   
   QString GetName(); 
   void initTrophyRewardNum(const int & iNum);
   void showTrophyWdg(const bool& bShow);
   void setShowTrophyWdg(const bool& bShow = true);
   void setTrophyAdd(const int& iCount);
   void showCtrlWdg(const bool& bShow, int WndFloatType = WndFloatType_Normal);
   void setCtrlValid(const bool& bValid);
   void SetSmallRenderCameraState(bool hasVideo);
   void InitBtn(QString userId);
   void ShowStuPublishBtn(bool show);
   void SetAllMuteAudioState(bool mute);
   void SetPenState(bool enable);
   void SetCameraState(bool open);
   void SetMicState(bool open);

   void SetSmallRenderMicState(bool hasAudio);
   void SetStuSmallRenderMicState(bool hasAudio);
   void ShowStudentCtrl(bool show);
   void SetUserNamePos(const bool& bUpdown);
   void CheckShowChangeBigScreen(bool show);
   void SetShowCtrlWdg(const bool& bShow);
   void SetStuSpacer(const int& iW);
   void SetFloatType(WndFloatType type);
   int GetFloatType();
signals:
	void sig_OnClickedFloatRender();
	void sig_OnClickedMic();
	void sig_OnClickedCamera();
	void sig_OnClickedAllMute();
	void sig_OnClickedFullScreen();
	void sig_OnClickedAllMuteCamera();
	void sig_OnAllUnpublish();

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
	void sig_PicPush(const bool&);
   void sig_OnSetBigScreen();

protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *e);
private slots:
	void slot_hideCtrl();
private:
	Ui::VhallRenderMaskWdg ui;
	bool mBShowTrophyWdg = false;
    QString mNickName;
	bool mbValid = false;
	bool mEnter = false;
	TeacherRenderWdg* mpRenderWdg = nullptr;
	QTimer* mpTimer = nullptr;
	//QWidget*  mWinid = nullptr;
   bool mbShowCtrle = true;
   int mWndFloatType = 0;
};
