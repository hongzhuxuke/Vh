#pragma once

#include <QWidget>
#include "ui_MediaPlayWebView.h"
#include "../Unility/Unility.h"
#include <QCloseEvent>
#include <QMovie>
//#include "WebEngineViewWdg.h"
#include "LibCefViewWdg.h"
class ShadowBoxDlg;

class MediaPlayWebView :public CBaseWnd, public MsgCallbackDelegate
{
    Q_OBJECT

public:
    MediaPlayWebView(QWidget *parent = Q_NULLPTR);
    ~MediaPlayWebView();

    //MsgCallbackDelegate
    virtual void OnRecvMsg(std::string msg_name, std::string msg);
    virtual void OnWebViewLoadEnd();
    virtual void OnWebViewDestoryed();

    virtual void Notice(const CSTDMapParam &);
	void SetLoadFinished(const bool& bloadfinish);
    CREATE_WND();

    void move(int x, int y);

signals:
	//void sig_close();
public slots:
   void JsCallQtMsg(QString name, QString param);
   void show();
   void hide();
private slots:
    void slot_close();
    //void slot_loadFinished(bool);
    void slot_loadFinished(const QString& strTitle);
    

protected:
   void resizeEvent(QResizeEvent *event);
   void showEvent(QShowEvent *event);
   void mouseMoveEvent(QMouseEvent * event);
   void customEvent(QEvent* event);
   void closeEvent(QCloseEvent*);
    //virtual void showEvent(QShowEvent *event);
private:
   void moveXY();

    Ui::MediaPlayWebView ui;
    //WebEngineViewWdg* mWebView = nullptr;
	QMovie *movie = nullptr;
	bool mLoadFinsh = false;

   LibCefViewWdg* mpCefWebView = nullptr;

   //ShadowBoxDlg* mpShaowBoxDlg = nullptr;
};
