#ifndef VHALLWEBVIEWFREE_H
#define VHALLWEBVIEWFREE_H

#include <QDialog>
#include <QPoint>
#include "cbasedlg.h"

class TitleButton;
class QWebEngineView;
class QWebChannel;
class VhallLiveMainDlg;
class VhallIALive;

namespace Ui {
	class VhallLivePlugInUnitDlg;
}

class VhallLivePlugInUnitDlg : public CBaseDlg
{
    Q_OBJECT

public:
    explicit VhallLivePlugInUnitDlg(QWidget *parent = 0);
    ~VhallLivePlugInUnitDlg();
    void SetWindowTitle(QString);
    void InitPluginUrl(QString url,QObject *obj);
    bool Create();
    void Destory();
    void CenterWindow(QWidget* parent);

    void executeJSCode(QString);
    void ReloadPluginUrl();
    bool IsLoadUrlFinished();  
    void JSCallQtResize();
    void StartLive(bool start);
    void RemoveWebViewToMainView(VhallLiveMainDlg *mainDlg);
    void RemoveWebViewToMainView(VhallIALive *vliveDlg);
    void ReleasePluginUrl();
    void AddWebViewToPlguin(QWebEngineView* webView, VhallLiveMainDlg *mainDlg);
    void AddWebViewToPlguin(QWebEngineView* webView, VhallIALive *vliveDlg);

signals:
    void SigJsCode(QString);
    void SigClose();
public slots:
	void show(const QRect& rect);
protected:
    bool eventFilter(QObject *, QEvent *);    
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
   void AddWebEngineView();
   void Load(QString url, QObject *obj);

public slots:
    void loadFinished(bool);    
    void SlotJsCode(QString);
    void SlotMaxClicked();
    void SlotMinClicked();
    void SlotRefresh();
    void JsCallQtDebug(QString msg);
    void slotLiveToolChoise(const int& iOpType);
private slots:
   void SlotClose();
private:
	 Ui::VhallLivePlugInUnitDlg *ui;
    QPoint m_PressPoint;
    QPoint m_StartPoint;
    TitleButton *m_pBtnRefresh = NULL;
    TitleButton *m_pBtnMin = NULL;
    TitleButton *m_pBtnMax = NULL;  
    TitleButton *m_pBtnClose = NULL;

    QObject *m_obj= NULL;
    bool m_bInit = false;
    //bool m_bFirstShow = true;
    QRect m_lastGeometry;
    bool mIsLoadUrlFinished = true;
    QRect m_startRect;
    QPoint m_startPoint;

    QWebEngineView *m_pWebEngineView = NULL;
    QWebChannel* m_pWebChannel = NULL;

    QWebEngineView *m_pH5WebEngineViewTemp = NULL;
    QWebChannel* m_pH5WebChannel = NULL;

    QString m_pluginUrl;   
    QString mStreamID;
    bool mFirstLoad = true;
    bool bResize = false;
    bool mbStartLive = false;

    VhallLiveMainDlg *mpMainDlg = nullptr; 
    VhallIALive* mpVliveDlg = nullptr;
	QRect mNormalRect;
};

#endif // VHALLWEBVIEWFREE_H
