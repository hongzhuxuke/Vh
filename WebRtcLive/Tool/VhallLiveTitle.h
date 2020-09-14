#pragma once

#include <QMouseEvent>
#include <QTimer>
#include <QDateTime>

#include "CWidget.h"
#include "ui_VhallLiveTitle.h"


class MoreSettingWdg;
enum TitleType
{
	eTitleType_OpenClassLecturer = 0,//公开课讲师端
	eTitleType_SmallClassLecturer,		//小课堂讲师端
	eTitleType_Student,						//学员端
};

class VhallLiveTitle : public CWidget
{
	Q_OBJECT

public:
	VhallLiveTitle(QWidget *parent = Q_NULLPTR);
	~VhallLiveTitle();

	void SetTitleType(TitleType type);
   void ShowHandsUpBtn();
    void UpdateStartClassState(bool start);
	void setRaiseHandsState(const int& iState);
	void startClassTime();
	void stopClassTime();

    void SetStartClassBtnEnable(bool enable);
    void SetHandsUpBtnEnable(bool enable);
    void SetStudentHandsUp(bool isPublish);
	void ShowBeginClass(const bool& bShow = true);
	void hideMediaSet();

   void setMaxState(const bool& bMax);
    int GetStartTime();
	void CancleRaiseHands();
	//void EnabledRaiseHands();
	//void UpperSucces();
	//void ShowAllChildren();
	void AddTrophyCount(const int& iAdd);
	int iTrophyCount();
signals:
   void sig_changeLayout();

signals:
   void sig_NetState();
   void sig_AllowRaiseHands(bool bAllow);
   void sig_RaiseHands();
   void sig_BeginClass();
   void sig_More();

   void sig_min();
   void sig_normal();
   void sig_max();
   void sig_close();
   void sig_OverClass();
   void sig_StopClass();
public  slots:
   void slot_StopClass();
private slots:
   void slot_changeLayout();
private slots:
	void on_btnNetState_clicked(bool checked = false);
	void on_btnAllowRaiseHands_clicked(bool checked = false);

	void on_btnMore_clicked(bool checked = false);
    void slot_BeginClass();

    void slot_HandsUp(bool bCountDown);
    void slot_AllowHandsUp();

	void on_btnMin_clicked(bool checked = false);
	void on_btnNomal_clicked(bool checked = false);
	void on_btnClose_clicked(bool checked = false);
	
	void slot_EnabledBeginClass();

	//void slot_HansUpTimeOut();
private:
    bool IsEnableJoinActive();
	void creatMoerSettingWdg();
	void NoDevShowTips();
private:
	Ui::VhallLiveTitle ui;

	QPoint mPressPoint;
	bool mIsMoved = false;
	bool m_bIsEnableMove = true;
	QTimer* mpBeginClassTimer = nullptr;
	MoreSettingWdg* mpMoerSettingWdg = nullptr;
	//QTimer* mpRaiseHandsTimer = nullptr;
	//int miResponseTime; //等待响应时间
};
