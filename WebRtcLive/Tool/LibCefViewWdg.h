#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "VHWebView.h"
#include <QTimer>

class MsgCallbackDelegate {
public:
   virtual void OnRecvMsg(std::string msg_name, std::string msg) = 0;
   virtual void OnWebViewLoadEnd() = 0;
   virtual void OnWebViewDestoryed() = 0;
};

class LibCefViewWdg : public QWidget, public vhall_paas::VHWebEventCb
{
   Q_OBJECT

public:
    LibCefViewWdg(MsgCallbackDelegate* delegate, QWidget *parent = nullptr);
    ~LibCefViewWdg();

    /*创建WebView成功时的回调。
    *参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
    *用户根据返回的id，可操作指定的webview*/
    virtual void OnCreateWebView(int id, const char* url);
    /*webview销毁事件通知*/
    virtual void OnWebViewDestoryed(int id);

    virtual void OnRecvMsg(const int id, std::string fun_name, std::string);
	virtual void OnTitleChanged(const int id, std::string title_name);

    void InitLibCef();
    void Create(QString url);
    void ChangeViewSize(int width, int height);
    void AppCallJsFunction(QString function);
    void LoadUrl(QString url);
    void CloseWeb();
    void SetLayOutPos(int pos) {
       mLayoutPos = pos;
    }

    int GetLayOutPos() {
       return mLayoutPos;
    }

signals:
	void sig_TitleChanged(const QString& strTitle);
protected:
   virtual void showEvent(QShowEvent *e);
   virtual void resizeEvent(QResizeEvent *event);
   virtual void keyPressEvent(QKeyEvent *event);
   virtual void keyReleaseEvent(QKeyEvent *event);
   virtual void customEvent(QEvent* event);
   virtual void enterEvent(QEvent *event);

private slots:
   void slot_OnChnageWebView();

signals:
   void sig_PressKeyEsc();

private:
   vhall_paas::VHWebViewInterface* mDocCefView = nullptr;
   QString mUrl = "http://e.vhall.com";
   int mDocIndex = 0;
   bool mbShown = false;
   MsgCallbackDelegate* mDelegate = nullptr;
   QTimer* mShowTimer = nullptr;
   int mLayoutPos = 0;
};
