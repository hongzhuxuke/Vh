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
   TeacherRenderWdg_Page_Index_render = 0,     //��Ⱦ
   TeacherRenderWdg_Page_Index_classNoStart,  //δ�Ͽ�
   TeacherRenderWdg_Page_Index_NoCamera,      //�ر�����ͷ
};

enum InsertPos {
   InsertPos_Speaker = 0,//��ʦ
   InsertPos_WndManager, //������Ƶ�б���
   InsertPos_Center,	//�м����Ļ��Ƶ
   InsertPos_FloatWnd, //����
   InsertPos_FullScreen,//ȫ��
};

#define CHECK_ONLINE_STATE  10000 //20000  ����N��󣬽��û�������
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
   void SetMultiCameraState(QString user_id, bool open);//����������ѧԱ��

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

   //������С����ʾ�ӿ�
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
   int mTrophyRewardNum = 0;        //��������
   int mLastIndexPage = 0;
   bool mEnter = false;
   bool bShowBigRenderCtrl = false;
   int mWndFloatType = 0;
   bool mbFirstBigCtrl = false;
   bool mbTeacherRender = false;

   /////////////////////////////////////////////////
   bool mbCameraIsOpen = true;//������   ��ʦ����ͷ�����   /  С����   ��ǰ�û�����ͷ�����
   bool mbMicIsOpen = true;

   bool mbSmallRenderCameraIsOpen = true;//������   ѧԱ����ͷ�����
   bool mbSmallRenderMicIsOpen = true;

   bool mOperationPower = true;
};
