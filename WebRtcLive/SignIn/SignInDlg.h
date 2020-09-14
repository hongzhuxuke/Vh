#pragma once
#include "..\Unility\WndManager.h"
#include "ui_SignInDlg.h"
#include "SignInListItem.h"

class SignInDlg : public CBaseWnd
{
    Q_OBJECT
public:
    SignInDlg(QWidget *parent = Q_NULLPTR);
    ~SignInDlg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
	void SetStatistics(const int& iStatistics);//收到后台推送的数据统计 
	void addItem(const SignInListItem::eSignInType& etype, const QStringList& strList);
	void startSignIn(const int& iTime, const QString& strSignInId);
	void SomeOneSignin(const QString& strSigner, const QString& strNick);
	void clearList();

	void addSignUsers(const SignInListItem::eSignInType& etype, std::list<std::string> signUsers);
	void SetSignInfo(int total,int SignCount);
	void AddonlineCount(const int& membercount);
signals:
	void sig_close();
protected:
	void showEvent(QShowEvent* event);
private slots:
	void on_btnClose_clicked(bool checked = false);
	void on_btnSignIn_clicked(bool checked = false);
	void on_btnSignInRecord_clicked(bool checked = false);
	void on_btnStartSignIn_clicked(bool checked = false);
	void slot_EnabledSignIn();
	void slotSignInCountDown();
private:
	void StopTimer();

    Ui::SignInDlg ui;
	QMap<int, SignInListItem*> mMapListItem;
	QMap<int, QListWidgetItem*> mMapListWidgetItem;
	QTimer* mpSignInTimer = nullptr;
	int mCountDownTime = 30;
	QString mStrSignInUi;//发起签到ID
	int mSignInedCount = 0;
	int mTotal;   //签到总人数
	std::list<std::string> mTotalList;
	bool mSignIn = false;

	QList<SignInListItem*> mListSignListItem;
};
