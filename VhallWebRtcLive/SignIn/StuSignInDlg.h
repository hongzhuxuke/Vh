#pragma once
#include "..\Unility\WndManager.h"
#include "ui_StuSignInDlg.h"
#include "SignInListItem.h"

class StuSignInDlg : public CBaseWnd
{
    Q_OBJECT
public:
    StuSignInDlg(QWidget *parent = Q_NULLPTR);
    ~StuSignInDlg();
    CREATE_WND();

    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
	void ShowExec(const int& iTime, const QString& strUid);
	void CloseExec();
protected:
	//void showEvent(QShowEvent* event);
private slots:
	void on_btnClose_clicked(bool checked = false);
	void on_btnStartSignIn_clicked(bool checked = false);
	void slotSignInCountDown();
	void slot_EndSignIn();
private:
	void StopTimer();

    Ui::StuSignInDlg ui;
	QTimer* mpSignInTimer = nullptr;
	int mCountDownTime = 30;
	QString mStrUid;
};
