#ifndef H_CMemberListWdg_H
#define H_CMemberListWdg_H


#include <QWidget>
#include <QMouseEvent> 
#include <QMap>
#include <QString>
#include <list>
#include  <QDateTime>
#include "ui_CMemberListWdg.h"
//#include "pub.Const.h"
#include "priConst.h"
#include "MemberItemWdg.h"
#include "../../Unility/CustomEventType.h"
#include "CWidget.h"

class COnlineItemWdg;
class QTimer;
class QPainter;


bool compareOnlineData(const COnlineItemWdg& pwdg1, const COnlineItemWdg& pwdg2);

class CMemberListWdg : public CWidget
{
	Q_OBJECT

public:
	CMemberListWdg(QWidget *parent = Q_NULLPTR);
	~CMemberListWdg();
	void SetCurrentListTab(const eTabMember& tabM);
	void NtfUserOffLine(const QString& strId);
	void setHansUpStates(const bool& bChecked);
	void SetHansUpShow(const bool& bshow);
	bool GetHansUpShow();
	void clearData();
	void setTotalMemberCount(const int& iTotal);
	void SetUpperStatesById(const QString& strId, const bool& bUpper);//true 显示下麦 false显示邀请上麦
	void SetInviteList(const std::string& invite_id);
	void initUpperStates();
	bool addOnlineMember(const CMember &info);
	bool addKickOutMember(const CMember &info, const bool& bKickOut = true, const bool& bForAll = true);
	void addKickOutMember(const QString& user_id, const int& state);

	bool addForbidChatMember(const CMember &inf, const bool& bForbid = true, const bool& bForAll = true);
	void addForbidChatMember(const QString& user_id, const int& forbid);
	bool addRaiseHandsMember(const CMember &info/*, const int& raiseHandsState = true, const bool& bForAll = true*/);
	bool addRaiseHandsMember(QString strUserId, const int& raiseHandsState);
	void updatePublishAbility(const QString& strId, const int& iStatus);
	void updateUpperState();
	int getRolType(const QString& strId);

	void showAllowRaiseHands(const bool& bShow = true);
	bool GetUserSpeaking();
    //设置用户上下麦状态
    void SetUpperStates(const QString& strUserId, int iRaiseHandsState);
    //设置在线用户禁言状态
    void SetForbiddenState(const QString& strUserId, const bool& bState);
	void UpdateMemberCount();
    CMember GetMemberPersonalInfo(const QString& strUserId);
	bool GetMember(const QString& strId, CMember& info);
	int GetMemberCount();
	const std::list<std::string>& GetInvitelist();
	enum eForbidden
	{
		eForbidden_cancle = 0,//取消禁言
		eForbidden_Forbidden, //禁言
	};

    int GetOnLineMemberSize();
    void ClearListRaiseHans();
    bool StartInvitationSpeech();
    bool IsUserSpeaking(QString strId);
signals:
	void sig_AgreeUpper(const QString& strId);
	void signal_Refresh();
	void signal_Upper(const QString& strId, const bool& bOperation = true);
	void sig_ReqApointPageUserList(const int& iApoint);
	void sig_HandsUpState(const int& iState);
	void sig_UpdateMemberCount(const int& membercount);
   void sig_KickUserPublish(QString strId);
   void sign_AuditPublish(QString strId);
   void sign_AskforPublish(QString strId);
public slots:
	//void show();
	void on_btnRefresh_clicked(bool checked = false);
	void slot_JsCallQtMsg(QString msg);
	void on_btnOnLine_clicked(bool checked = false);
protected:

private slots:
	void on_btnClear_clicked(bool checked = false);
	void on_btnLookUp_clicked(bool checked = false);
	//void slot_UserSpeaking(bool speak);
	void on_btnRaiseHands_clicked(bool checked = false);
	void on_btnProhibitSpeaking_clicked(bool checked = false);	
	void on_tbtnAllowRaiseHands_clicked(bool checked = false);
	void slotEnableRefresh();
	void on_ledtLookUp_textChanged(const QString &text);

   //void slot_KickUserPublish(QString strId);
   //void slot_AuditPublish(QString strId);
   //void slot_AskforPublish(QString strId);

private:	
	void startRefreshUpdateTime();

	COnlineItemWdg* getOnlineItemWdt(const QString& strId);
   COnlineItemWdg* getForbidItemWdt(const QString& strId);
   COnlineItemWdg* getRaiseHandsItemWdt(const QString& strId);

	void RemoveOnLineItemWdt(const QString& strId);
	//void updateReplyInvite(const CMember &info, const bool& bUpperSuc);
	//void calcListNum();
	int calcWdgHeight(const int& iRowCount) const;
	void dealNotSpeak(const QString& strId);
	void addSpeaker(const QString& strId);

	//void DealSwitchHandsUp(const Event& e);
	
	bool RemoveInfo(QListWidget *pListWdt, const QString &strId);
	void clearInfoList(QListWidget *pListWdt);
	Ui::CMemberListWdg ui;

	bool m_bTitlePressed = false;
	QPoint pressPoint;
	QPoint startPoint;
	QMap< QString, COnlineItemWdg* > mOnlineItemMap; //在线展示列表
	QMap< QString, COnlineItemWdg* > mForbidItemMap;
	QMap< QString, COnlineItemWdg* > mRaiseHandsItemMap;
   QList<QString> mListRaiseHans;
	std::list<std::string> mInvitelist;         //已上麦成员列表

	QString mStrMainSpeakerId;
	QTimer* mpRefreshTime = NULL; //刷新按钮定时器

	int mReqPage = 1;
	int miCount = 0;
	bool mbIsUserSpeaking = false;   //是否有学员已上麦或正在上麦（限制公开课只允许一人上麦）

   QDateTime mDateTimeInvitationSpeech;//上麦邀请时间
};
#endif