#ifndef BaiduPaperDlg_H
#define BaiduPaperDlg_H

#include <QDialog> 
#include <QTimer>

#include "ui_BaiduPaperDlg.h"
#include "../Unility/Unility.h"
#include "LibCefViewWdg.h"
//class LibCefViewWdg;

class BaiduPaperDlg : public CBaseWnd,public MsgCallbackDelegate
{
    Q_OBJECT

public:
    explicit BaiduPaperDlg(QWidget *parent = 0);
    ~BaiduPaperDlg();
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
private slots:
	void slot_TitleChanged(const QString& strTitle);
	void slot_LoadTest();
	void on_btnClose_clicked(bool checked = false);
	void on_btnMin_clicked(bool checked = false);
   void slot_timeOut();
private:
   void SetTitleChanged(const QString& strTitle);
	void moveXY();


	Ui::BaiduPaperDlg ui;
	bool mbIsMaxSize = false;

   QString m_TestUrl = "http://e.vhall.com/";
   LibCefViewWdg *mCefTestView = nullptr;

	//QRect mNormalRect;
	//QList<CListViewItemWdg*> mListItem ;
	//int miCurrentPage = 1;
	//int miReqPage = 1;
	////QMap<QString, RoomListInfo> mMapRoomInfoList;

	//bool mbGust = false;
	//int miLiveStatus = -1;

   QTimer* mpTimer = nullptr;
   bool mbLoadEnd = false;
};

#endif // BaiduPaperDlg_H
