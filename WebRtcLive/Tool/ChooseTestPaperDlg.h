#ifndef CHOOSETESTPAPERDLG_H
#define CHOOSETESTPAPERDLG_H


#include <QTimer>

#include "ui_ChooseTestPaperDlg.h"
#include "../Unility/Unility.h"
#include "LibCefViewWdg.h"

class ShadowBoxDlg;

class ChooseTestPaperDlg : public CBaseWnd,public MsgCallbackDelegate
{
    Q_OBJECT

public:
    explicit ChooseTestPaperDlg(QWidget *parent = 0);
    ~ChooseTestPaperDlg();
	CREATE_WND();
	virtual void Notice(const CSTDMapParam &);

   //MsgCallbackDelegate
   virtual void OnRecvMsg(std::string msg_name, std::string msg);
   virtual void OnWebViewLoadEnd();
   virtual void OnWebViewDestoryed();

   void ClientCallCloseExamCallback();
signals:
   void sig_close();

public slots:
   void JsCallQtMsg(QString name, QString param);
protected:
	void customEvent(QEvent* event);
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
   void mouseMoveEvent(QMouseEvent *);
private slots:
	void slot_TitleChanged(const QString& strTitle);
	void slot_LoadTest();
	void on_btnClose_clicked(bool checked = false);
	void on_btnMin_clicked(bool checked = false);
   void slot_timeOut();
private:
   void SetTitleChanged(const QString& strTitle);
	void moveXY();


	Ui::ChooseTestPaperDlg ui;
	bool mbIsMaxSize = false;

   QString m_TestUrl = "http://e.vhall.com/";
   LibCefViewWdg *mCefTestView = nullptr;

   QTimer* mpTimer = nullptr;
   bool mbLoadEnd = false;
   //ShadowBoxDlg* mpShaowBoxDlg = nullptr;
};

#endif // CHOOSETESTPAPERDLG_H