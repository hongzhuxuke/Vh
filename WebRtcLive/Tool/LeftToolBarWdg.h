#pragma once
#include <QMouseEvent>
#include <QTimer>
//#include<windows.h>

#include "CWidget.h"
#include "ui_LeftToolBarWdg.h"
#include "AuxiliaryCameraChoiceWdg.h"
#include "DoublePubStuChoiceUI.h"
#include "InvitationStateWdg.h"
#include "ChangeView.h"

class WebRtcLiveWdg;
class 	VedioPlayUI;
class VideoChoiceUI;
class Menu;
class SignInDlg;
class BlackMenuWdg;
class MediaPlayWebView;
class AnswerWebView;
class screenChoiceDlg;
class softwareSourceDlg;
class MultiClassFloatWnd;
class ChooseTestPaperDlg;
class ShadowBoxDlg;

class LeftToolBarWdg : public CWidget
{
	Q_OBJECT

public:
	LeftToolBarWdg(QWidget *parent = Q_NULLPTR);
	~LeftToolBarWdg();

   //void LeftToolBarWdgShow();
	void setEnabled(bool);
   void setEnableDoublePush(const bool& bEnable);
	VideoChoiceUI* GetVideoChoiceUi() {
		return	mpVideoChoiceUi;
	}
	void SetUserInfo(const QString& userName);
	void SetClassInfo(const QString& className);
	   
   bool IsLocalMediaPlaying();
	void SetBtnDocEnabled(bool enable = true);
	void SetBtnWhiteboardEnabled(bool enable = true);
	void SetBtnDesktopSharingEnabled(bool enable = true);
	void SetBtnMediaEnabled(bool enable = true);
	void SetBtnAnswerEnabled(bool enable = true);
   void SetDoublePushCameraEnabled(bool enable = true);
   void SetBtnDesktopSharingText(const QString& str, const bool& bSorftWare = false);
   void SetDesktoSharingChecked(const bool& bChecked = true);
   void SetDoublePushChecked(const bool& bChecked = true);

	void setWebRtcLiveWdg(WebRtcLiveWdg* pwdg);
	void setDocView(const bool bType = false);
	void initView();
	void PopDesktopSharingMenu(const QPoint& pos);
	void PopMediaMenu(const QPoint& pos);
	void PopInteractionToolMenu(const QPoint& temppos);
	void PopDoublePushMenu(const QPoint& temppos);
	int64_t GetDesktopSharingIndex() { return miIndex; }
	QString GetCurShareSoftware();
	void SetShareType(const int& iType) { miShareType = iType; }
	bool isMediaPlayExist();
	bool isVideoChoiceUiExist();
	void setAnserChecked(const bool& bChecked = true);
	void StartSignin(const int& iTime, const QString& strSignInId );
	void SomeOneSignin(const QString& strSigner, const QString& strNick);
	void RecvSignUsers( std::list<std::string> signUsers, std::list<std::string> noSignUsers);
	void SetMuteSelf(const bool& bMute );
	void SetMuteStudent(const bool& bMute);
	MediaPlayWebView* MediaPlayWebViewDlg();
	void DestoryMediaPlayWebViewDlg();

	AnswerWebView* AnswerWebViewDlg(); 
	void DestoryAnswerWebViewDlg();
	void DestoryAnswerToolDlg();
	void DestoryTimerToolDlg();
	void DestoryTestPaperDlg();

	bool  IsExitAnserWebView();
   bool IsTestPaperDlg();
	bool IsExitSignInDlg();
	void AddOnline(const int& membercount);
   void DoubleVrtcUsers(const CDoubleVrtcUsers& vrtcUser);
   void initDoubleVrtcUsers(const DoublePubStuChoiceUI::eChoiceType& type);
   void CloseVrtcUsersDlg();
   void EnableStuDoubleVrtc(const bool bEnalbe);
	void SetscreenChoiceDlgState(bool baccept);
   enum OpType
	{
		OpType_Doc,					//文档 
		OpType_Whiteboard,		//白板
		OpType_DesktopShar,		//桌面共享
		OpType_SoftWareShar,	//软件共享
		OpType_MediaMenu,		//插播
		OpType_Anser,				//答题
		OpType_Interaction,		//互动工具

      OpType_TeacherDoublCamera,//讲师双推
      OpType_StuDoublCamera,//学员双推
      OpType_StuDesktopShar,  //学员桌面共享
		OpMediaType_database,	//插播--资料库
		OpMediaType_Local,		//插播--本地


		//InterActionOpType_QUESANDANS,	//互动工具--问答
		InterActionOpType_TESTPAPER,		//互动工具--试卷
		InterActionOpType_SIGNIN,				//互动工具--签到
		InterActionOpType_ANSWER,			//互动工具--答题器
		InterActionOpType_TIMER,					//互动工具--计时器

		OpType_NULL
	};

	enum OpDesktopSharType
	{
		OpDesktopSharType_DesktopShar,
		OpDesktopSharType_SoftwareShar,
      OpDesktopSharType_StuDesktopShar,
		OpDesktopSharType_NULL
	};
   enum eDoubleType {
      eDoubleType_StuDouble = 0,
      eDoubleType_StuScreen,
      eDoubleType_Null
   };
   int DoubleType() {
      return mDoubleType;
   };
	void SetOpType(OpType type);
   int CurOptype();
	void DealTopplayChoiceUi();

	void setMutexItemChecked(const OpType& eType);
   void PreviewAuxiliaryCamera(QEvent*event);
   void EnablePreviewAuxiliary();
   bool isPreViewAuxiliaryVisible();
   void ShowPreViewAuxiliaryWdg();
   void SetDoubleCamera(const bool bDouble);
   void SetStuDesktopShare();
   DoublePubStuChoiceUI::eChoiceType GetDoublePushType();
   void closeScreenChoiceDlg();
   void StartOvertimeTimer();
   void StartMadiePlayTimer();
   void StuStartExamination();

signals:
	void sig_btnDoc();
	void sig_Whiteboard(bool bAllow);
	void sig_Answer();
	void sig_Notice();
    
	void signal_move(QPoint point);
	void sig_DesktopSharing();
	void sig_Media();
	void sigMuteSelf(bool checked);
    void sig_AnswerTool();//答题器
	void sig_TimerCountDown();
	void sig_InteractionTool();
	void sigInteractionItemClicked(const int& iOpType);
   void sig_CameraDoublePush(bool start);
	void sig_DoublePush();
   void sigStopScreenSharing();
   void sig_StudentDoublePush();
   void sig_stopStuDoublePush();
   void sig_inviteDoubleVrtc(QString strId, const DoublePubStuChoiceUI::eChoiceType& eType);
   void sigStopReDoublePush();
   void sig_stopStuDestopSharingPush();
	void sig_StopClass();

public slots:
	void slots_ScreenSharing();
	void slots_MenuAboutToHide();
   
	void slots_MediaMenuAboutToHide();
	void slots_InteractionHide();
	void slots_DoublePushToHide();
	void slot_OverClass();
	void slot_MutexWdgClose();
   void slot_ReSelection(const DoublePubStuChoiceUI::eChoiceType& eType);
	void slot_ShowDoublePubStuChoiceUI();
   void slot_InvitaStateChanged(const InvitationStateWdg::eStateType& type);
	void slot_DesktopCapture(bool bDesktopCapture);
	void slot_StopClass();
 public Q_SLOTS:
    void 	slot_InteractionItemClicked(const int& iOpType);
      void show();
protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private slots:
   void slot_closeTestPaper();
   void slotOvertime();
   void slot_InviteStuDoublePubTimerOut();
	
	//void slot_InteractionClicked(const int& iOpType);
	void on_btnTimerCountDown_clicked(bool checked = false);
	void on_btnDoublePushCamera_clicked(bool checked = false);
	//void on_btnAdd_clicked(bool checked = false);
	void on_btnDoc_clicked(bool checked = false);
	void on_btnWhiteboard_clicked(bool checked = false);
	void on_btnDesktopSharing_clicked(bool checked = false);
	void on_btnMedia_clicked(bool checked = false);

	void on_btnAnswer_clicked(bool checked = false);
    void slot_doublePush(const int& iOptype);
	
    void slot_answerTool();
	void slots_DesktopSharing(const int& iOptype);
	//void on_btnDoublePushCamera_clicked();

	void slots_MediaMenu(const int& iOptype);
	void on_btnInteractionTool_clicked(bool checked = false);
	void slot_VideoClose();
   void slot_startInviteTimerOut();
private:
   void ShowCEFWebTestView();

   void stopTimer();
   
   void StuDoublePush();
	void setMutexItemChecked();
   void requestVrtcUsers(const DoublePubStuChoiceUI::eChoiceType& type);
	void setInterActionItemEnabled(const bool& enabled);

	void CreateVideoChoiceUi();
	void CreateSignInDlg();
	void SetCheckedOpType(OpType type);
	void openMic(bool bOpen);
	
	bool IsPlayMediaFile();
	void localMediaPlay();
	void StopPlayFile();
	int GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos);
	void StopPlayMediaFile();
	void StopShowMediaVideo();
	void DealStopPlay(void* apData, DWORD adwLen);
	void AddPlayFiles();

   void createDoubleStuChoiceUi();
	int InitMediaStream(const char *szMediaFile);
	
	Ui::LeftToolBarWdg ui;
	int64_t miIndex = 0;
	int mTotal = 0;
	
	QPoint mPressPoint;
	bool mIsMoved = false;
	bool m_bIsEnableMove = true;
	//Menu* mPDesktopSharMenu;
	//Menu* mPMediaMenu;
	BlackMenuWdg* mpDesktopSharWdg = nullptr;
	BlackMenuWdg* mpMediaWdg = nullptr;
	BlackMenuWdg* mpDoublePushWdg = nullptr;

	WebRtcLiveWdg* mpWebRtcLiveWdg = nullptr;
	QString mstrCurShareSoftware;
	int miShareType;

    //bool mbIsCameraOpenDoublePush;
	VideoChoiceUI* mpVideoChoiceUi = nullptr;
   DoublePubStuChoiceUI* mpDoublePubStuChoiceUI = nullptr;

	SignInDlg* mpSignInDlg = nullptr;
	BlackMenuWdg* mpInteractionWdg = nullptr;
	
	
   AuxiliaryCameraChoiceWdg* mpAuxiliaryWdg = nullptr;
   QTimer* mpInviteDoublePubTimer = nullptr;
   OpType mCurType;
   QTimer* mpOvertimeTimer = nullptr;
   screenChoiceDlg* mpScreenChoiceDlg = nullptr;
   DoublePubStuChoiceUI::eChoiceType mDoublePushType = DoublePubStuChoiceUI::eChoiceType_Invalid;

   softwareSourceDlg* mpsoftwareSourceDlg = nullptr;
   bool mbRequestByManual = false;
   ChooseTestPaperDlg* mpTestPaper = nullptr;
   //ShadowBoxDlg* mpShadowBoxDlg = nullptr;

   AnswerWebView* mpAnswerWebView = nullptr;
   //ShadowBoxDlg* mpAnswerShadowBoxDlg = nullptr;

   MediaPlayWebView* mpMediaPlayWebView = nullptr;
   //ShadowBoxDlg* mpMediaPlayShadowBoxDlg = nullptr;
   eDoubleType mDoubleType = eDoubleType_Null;
};
