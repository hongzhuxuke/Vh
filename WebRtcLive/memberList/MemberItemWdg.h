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
/*�����б��Ա��*/
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

	//������״̬
	enum eForbiddenState
	{
		eForbiddenState_normal,		//����״̬
		eForbiddenState_Forbidden,   //����
	};

	enum eKickOutState
	{
		eKickOutState_normal,		//����״̬
		eKickOutState_KickOut,      //�߳�
	};

	//����״̬
	enum eRaiseHandsState
	{
		eRaiseHandsState_normal = -1,		//����״̬
		eRaiseHandsState_RaiseHands = 0,   //������
		eRaiseHandsState_Speaking = 1,      //������
	};

    //enum eUserState {
    //    eUserState_OnLine = 0,    //�����û�
    //    eUserState_KickOut = 1,   //�߳��û�
    //    eUserState_ChatForbid = 2,  //�����û�
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

	//int miCurLimitedState;//���ఴť��ʾ״̬


	int miRaiseHandsState = -1; //����/����/�������� ״̬��ʶ

	QString mStrId;
	int miRole;
	int mINoMic = 0; //�Ƿ������������  0 ����������mic��������ͷ�ȣ��� 1 ��������û��mic���豸��
	
	bool mbForbiddenState = false;//����״̬
	bool mbKickOutState = false;//�߳�״̬

	int mOnlineStatus = 1;            // ����״̬��Ĭ��Ϊ0�����ߣ�1Ϊ����
	//Menu* mPMoreOpMenu = nullptr;
	MenuWdg* mPMoreOpMenu = nullptr;
    //bool mbIsInviting = false;//��������/������  true

};

#endif//H_MEMBERITEMWDG_H