#pragma once

#include "ui_AnswerWebView.h"
#include "../Unility/Unility.h"
#include <QCloseEvent>
#include "LibCefViewWdg.h"

class ShadowBoxDlg;

class AnswerWebView : public CBaseWnd, public MsgCallbackDelegate
{
    Q_OBJECT

public:
    AnswerWebView(QWidget *parent = Q_NULLPTR);
    ~AnswerWebView();

    //MsgCallbackDelegate
    virtual void OnRecvMsg(std::string msg_name, std::string msg);
    virtual void OnWebViewLoadEnd();
    virtual void OnWebViewDestoryed();

    virtual void Notice(const CSTDMapParam &);
    CREATE_WND();
	void SetLoadFinished(const bool& bloadfinish);
	//void 	setEnabled(bool enabled );
public slots:
   void JsCallQtMsg(QString name, QString param);
   void show();
   void hide();
protected:
   void mouseMoveEvent(QMouseEvent * event);
   void customEvent(QEvent* event);
   void closeEvent(QCloseEvent*);

   void resizeEvent(QResizeEvent *event);
   void showEvent(QShowEvent *event);

private slots:
    void slot_close();
    //void slot_loadFinished(bool);
    void slot_loadFinished(const QString& strTitle);
    void on_btnClose_clicked(bool checked /*= false*/);
private:
   void SetTitleChanged(const QString& strTitle);
   void sendQtStopAnswer();
   void moveXY();

	enum ePage
	{
		ePage_WEB = 0,
		ePage_local
	};
    Ui::AnswerWebView ui;
    //WebEngineViewWdg* mWebView = nullptr;
    LibCefViewWdg* mCefWebView = nullptr;
    bool mbLoadFinished = false;
    //ShadowBoxDlg* mpShaowBoxDlg = nullptr;
};
