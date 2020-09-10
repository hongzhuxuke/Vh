#pragma once

#include <QWidget>
#include "ui_WebEngineViewWdg.h"
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>
#include <QKeyEvent>
#include <QTimer>
//#include "../RenderWdg/BigRenderCtrl.h"
#include "../RenderWdg/TeacherCtrlWdg.h"
#include "../Unility/Unility.h"
#include "FullScreenBtn.h"
#include "Menu.h"
#include "CWidget.h"

enum WebViewType {
    WebViewType_Normal = 0,
    WebViewType_LiveStream = 1,
};

enum URLType {
    URLType_normal = 0,
    URLType_LiveMixedStream = 1,
};

enum LAYOUT_POS {
    LAYOUT_POS_Normal = 0,
    LAYOUT_POS_RIGHT_TOP = 1,
    LAYOUT_POS_CENTER = 2,
};

class VHQWebEnginePage :public QWebEnginePage {
public:
    VHQWebEnginePage(QObject *parent = Q_NULLPTR);
    ~VHQWebEnginePage();

protected:
    virtual QStringList chooseFiles(QWebEnginePage::FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes);
};

class WebEngineViewWdg : public CWidget
{
    Q_OBJECT

public:
    WebEngineViewWdg(QWidget *parent = Q_NULLPTR);
    ~WebEngineViewWdg();

    void CreateWebView(int type = 0);
    void DestroyWebView();
    void LoadUrl(QString url,int urlType = 0);

    void runJavaScript(QString fun);
	bool IsLoadFinished();

    void Stop();

    //void SetFullState(bool isFull);
    void SetLayOutPos(int pos);
    int GetLayOutPos();

	void SetLoadFinished(const bool& bloadfinsh);
	bool GetWebEngineViewWdg();
signals:
	void sig_JsCallQt(QString msg);

public slots:
    void loadFinished(bool);
    void JsCallQtMsg(QString msg);
    void sendQtCloseSuccess();
    void sendQtStopAnswer();

private slots:
    void slot_OnStuClickedFloat();
    void slot_OnStuClickedFullWnd();
    void slot_WebFlush();
    void slot_fullScreen();
    void slot_featurePermissionRequested(const QUrl&, QWebEnginePage::Feature);
    void slot_fullScreenRequested(QWebEngineFullScreenRequest request);
    void slot_moveTimeOut();
    void slot_hideTimeOut();
private:
    void InitCtrl();

protected:
    virtual bool eventFilter(QObject * receiver, QEvent * event);
protected:
   virtual void enterEvent(QEvent *event);
   virtual void leaveEvent(QEvent *event);
   virtual void resizeEvent(QResizeEvent *event);

signals:
    void sig_loadFinished(bool);
    void sig_fullScreen();
    void sig_Enter();
    void sig_Leave();
    void sig_KeyEsc();
private:
    Ui::WebEngineViewWdg ui;
    QWebEngineView *mWebEngineView = NULL;
    QWebChannel* mWebChannel = NULL;
    VHQWebEnginePage *mWebEnginePage = nullptr;
    bool mbIsLoadFinished;

    int mWebViewType = 0;
    //BigRenderCtrl* mFullScreenCtrl = NULL;
    TeacherCtrlWdg* mNormalCtrl = NULL;
    QWidget* mCtrl = nullptr;
    Menu* rightMenu = nullptr;
    QAction* mActionForbidden = nullptr;
	QString mStrUrl;
    FullScreenBtn* mpFullScreenBtn = nullptr;
    int mUrlType = URLType_normal;

    //bool bIsFull = false;
    int mLayoutPos = 0;

    QTimer* mpBtnMoveTimer = nullptr;
    QTimer* mpBtnHideTimer = nullptr;
};
