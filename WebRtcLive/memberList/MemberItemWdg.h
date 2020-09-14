#ifndef H_MEMBERITEMWDG_H
#define H_MEMBERITEMWDG_H


#include "CWidget.h"
#include "ui_OnlineItemWdg.h"
//#include <QAction>

#include "../../Unility/CustomEventType.h"
class QTimer;
class QListWidgetItem;
//class Menu;
class MenuWdg;
//class CAction;
class WebRtcLiveWdg;
/*在线列表成员项*/
class COnlineItemWdg : public CWidget
{
	Q_OBJECT

public:
	COnlineItemWdg(QWidget *parent = Q_NULLPTR);
	COnlineItemWdg( COnlineItemWdg* pitem,QWidget *parent = Q_NULLPTR);
	~COnlineItemWdg();
	int GetNoMic();
	void SetNoMic(const int& iNoMic);
	int GetRaiseHandsState();
	virtual void SetUserInfo(const CMember& info);
	void updateInfo(const CMember& info);
	virtual QString GetName();
	void SetUpperStates(const int& iRaiseHandsState);
	int GetUpperStates() { return miRaiseHandsState; }
	
	void SetForbiddenState(const bool& bForbidden);
	bool GetForbiddenState() { return mbForbiddenState; }

	void SetKickOutState(const bool& bstate);
	bool GetKickOutState() { return mbKickOutState;}

	virtual int GetOnlineStatusStates();
	
	virtual int GetRole();
	virtual QString GetId();
	bool isCurrentUser(const QString& strRole);

	void SetItem(QListWidgetItem* pItem) {
		mpItem = pItem;
	}
	int GetWeight();
	int CalcWeight();
	bool operator < (const COnlineItemWdg&);
	bool operator <= (const COnlineItemWdg&);
	bool operator > (const COnlineItemWdg&);
	bool operator >= (const COnlineItemWdg&);
	bool operator == (const COnlineItemWdg&) ;

	friend bool operator< (const COnlineItemWdg& obj1, const COnlineItemWdg& obj2)
	{
		return  obj1.mIWeight < obj2.mIWeight;
	}

	int m_iStaticId;

	enum eOptype
	{
		eOptype_Forbidden = 0,
		eOptype_CancleForbidden,
		eOptype_KickOut,
		eOptype_CancleKickOut
	};

	//受限制状态
	enum eForbiddenState
	{
		eForbiddenState_normal,		//正常状态
		eForbiddenState_Forbidden,   //禁言
	};

	enum eKickOutState
	{
		eKickOutState_normal,		//正常状态
		eKickOutState_KickOut,      //踢出
	};

	//举手状态
	enum eRaiseHandsState
	{
		eRaiseHandsState_normal = -1,		//正常状态
		eRaiseHandsState_RaiseHands = 0,   //举手中
		eRaiseHandsState_Speaking = 1,      //连麦中
	};

    //enum eUserState {
    //    eUserState_OnLine = 0,    //在线用户
    //    eUserState_KickOut = 1,   //踢出用户
    //    eUserState_ChatForbid = 2,  //禁言用户
    //};
signals:
	//void sig_UserSpeaking(bool);

   void sig_KickUserPublish(QString strId);
   void sign_AuditPublish(QString strId);
   void sign_AskforPublish(QString strId);
protected:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private slots:
	void slotCliecked(const int& iOpType);
	void on_btnUpper_clicked(bool checked = false);

	void slot_upperTimeOut();

	void slots_Forbidden();
	void slots_CancleForbidden();
	void slots_KickOut();
	void slots_CancleKickOut();

	void on_btnMoreOp_clicked(bool checked = false);

   
private:
   void startUpperTime(const int& iTime);
	void KickUserPublish();

	void initUserRoleId();
	void createAction();

	Ui::OnlineItemWdg ui;
	int mIWeight;
	QTimer* mpUpperTime = NULL;
	QListWidgetItem* mpItem = NULL;

	//int miCurLimitedState;//更多按钮显示状态


	int miRaiseHandsState = -1; //上麦/下麦/邀请上麦 状态标识

	QString mStrId;
	int miRole;
	int mINoMic = 0; //是否具有上麦能力  0 有能力（有mic或者摄像头等）。 1 无能力（没有mic等设备）
	
	bool mbForbiddenState = false;//禁言状态
	bool mbKickOutState = false;//踢出状态

	int mOnlineStatus = 1;            // 在线状态，默认为0不在线，1为在线
	//Menu* mPMoreOpMenu = nullptr;
	MenuWdg* mPMoreOpMenu = nullptr;
    //bool mbIsInviting = false;//邀请上麦/连麦中  true

};

#endif//H_MEMBERITEMWDG_H