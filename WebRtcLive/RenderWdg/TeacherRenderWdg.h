#pragma once

#include <QWidget>
#include "ui_TeacherRenderWdg.h"
#include "../Unility/Unility.h"
#include "VhallRenderMaskWdg.h"
#include <QTimer>
#include "CBaseRender.h"
#include "TeacherCtrlWdg.h"
#include "BigRenderCtrl.h"
#include "TrophyWdg.h"
#include "VideoRenderReceiver.h"
#include "priConst.h"

enum TeacherRenderWdg_Page_Index {
   TeacherRenderWdg_Page_Index_render = 0,     //渲染
   TeacherRenderWdg_Page_Index_classNoStart,  //未上课
   TeacherRenderWdg_Page_Index_NoCamera,      //关闭摄像头
};

enum InsertPos {
   InsertPos_Speaker = 0,//讲师
   InsertPos_WndManager, //上麦视频列表中
   InsertPos_Center,	//中间大屏幕视频
   InsertPos_FloatWnd, //浮窗
   InsertPos_FullScreen,//全屏
};

#define CHECK_ONLINE_STATE  10000 //20000  下线N秒后，将用户踢下麦
class FullScreenRender;

class TeacherRenderWdg : public QWidget, public CBaseRender
{
   Q_OBJECT

public:
   TeacherRenderWdg(QWidget *parent = Q_NULLPTR, bool bTeacherRender = false);
   ~TeacherRenderWdg();

   void setOperationPower(bool bPower);
   void InitRenderInsertPos(InsertPos pos);
   void InitRenderCtrl(QString id);

   void ChangeRenderPos(bool isCenter);
   void SetNickName(QString nickName);
   QString NickName();
   void SetRoleType(int roleType);
   void SetCameraState(bool open);
   void SetMultiCameraState(QString user_id, bool open);//公开课设置学员端

   bool GetCameraState();
   void SetMicState(bool open);
   void SetMultiMicState(bool open, bool bVideo = false);
   void ShowAllMuteCtrl(bool show);
   bool GetBEnter();
   HWND GetRenderWndID();
   std::shared_ptr<vhall::VideoRenderReceiveInterface> GetRenderWnd();
   void SetPageIndex(int index);
   void UpdataRendFlush(bool enable);

   void SetAllMuteAudioState(bool mute);
   void SetPenState(bool enable);

   void ChangeBigScreen(QString big_user_id);

   //公开课小窗显示接口
   void SetShowSmallRender(bool show);
   void SetSmallRenderNickName(QString name);
   void SetSmallRenderCameraState(bool hasVideo, bool isSelf = false);
   void setTeacherSmallRenderCameraState(bool hasVideo);
   void SetSmallRenderRole(int role);
   void SetSmallRenderMicState(bool hasAudio, bool isSelf = false);
   void SetStuSmallRenderMicState(bool hasAudio);
   bool GetSmallRenderMicState();

   HWND GetSmallRenderRenderWndID();
   std::shared_ptr<vhall::VideoRenderReceiveInterface> GetSmallRenderRenderWnd();
   void SetSmallRenderFlushEnable(bool enable);
   void SetSmallRenderUserPublish(bool publish);

   QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;

   void RenderBigStreamToWnd(QString id);
   void RenderSmallStream();

   void HandleOpenClassUserLineState(QString id, bool onLine);
   void HandleUserLineState(bool onLine);
   void initTrophyRewardNum(const int & iNum); //setTrophyNum

   void ShowStudentCtrl(bool show);
   void ShowTrophyWdg(const bool& bshow);
   void ShowBigRenderCtrl(const bool& bShow = true);

   void SetTeacherCameraCloseSheet();
   void SetTeacherCameraOpenSheet();
   void SetFloatType(WndFloatType type);
   void SetFloatTypeValue(WndFloatType type);
   int GetFloatType();
   void SetStuSpacer(const int& iW);

   bool isSmallRender();
   void CloseMic();
   void OpenMic();
   QString mStrBigScreenId;
   void setStackedSmallIndex(const int& index);
   bool MicIsOpen() { return mbMicIsOpen; }
signals:
   void sig_ClickedFloatRender(QString, QString, QString, int);
   void sig_SetFocus();
   void sig_PicPush(const bool&);
   void sig_cameraStateChange(bool bState);
public slots:
   void slotMuteSelft(bool bMute);
   void slotTrophyAdd(const int& iCount);    
   void slot_OnClickedMic();
   void slot_OnClickedCamera();
   void show();
   void hide();
   void slot_OnClickedFloatRender(bool bShowTitle = true);
protected:
   void enterEvent(QEvent *);
   void leaveEvent(QEvent *);
   void mousePressEvent(QMouseEvent *e);
   void resizeEvent(QResizeEvent *event);
   void showEvent(QShowEvent *event);
private slots:
   void slot_OnClickedAllMute();
   void slot_OnClickedFullScreen();
   void slot_OnAllUnpublish();
   void slot_OnClickedAllMuteCamera();

   void slot_OnStuClickedCamera();
   void slot_OnStuClickedMic();
   void slot_OnstuClickedUnPublish();
   void slot_OnStuPenClickedPen(bool);
   void slot_OnStuClickedFloat();
   void slot_OnStuClickedFullWnd();

   void slot_OnClickedStudentMic();
   void slot_OnClickedStudentCamera();
   void slot_OnUnPublishStudent();
   void slot_OnChangeCamera();
   void slotFavorStudent();
   void slot_OnSetBigScreen();
   
private:
   void OpenCamera();
	void setGigRenderCtrlGeometry();
   void HandleUnPublishUser(const QString& uid);

   void SetStackCurrentIndex(int idex, bool bsefl = true);

protected:


private:
   void setStackedIndex(const int& index);
	void resetSmallRender();

   Ui::TeacherRenderWdg ui;
   InsertPos mWndPos;
   QString mNickName;
   int mRoleType = -1;
   bool mbHasRenderSmallWnd = false;
   QTimer *mLineStateTimer = nullptr;
   int mTrophyRewardNum = 0;        //奖杯数量
   int mLastIndexPage = 0;
   bool mEnter = false;
   bool bShowBigRenderCtrl = false;
   int mWndFloatType = 0;
   bool mbFirstBigCtrl = false;
   bool mbTeacherRender = false;

   /////////////////////////////////////////////////
   bool mbCameraIsOpen = true;//公开课   讲师摄像头打开与否   /  小课堂   当前用户摄像头打开与否
   bool mbMicIsOpen = true;

   bool mbSmallRenderCameraIsOpen = true;//公开课   学员摄像头打开与否
   bool mbSmallRenderMicIsOpen = true;

   bool mOperationPower = true;
};
