#pragma once
#include <QDialog>
#include "ui_CAboutDlg.h"
#include "../Unility/Unility.h"
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMutex>
#include <QFileInfo>
#include <QFile>

//class QGraphicsDropShadowEffect;

#define CHECKVER_JSON_ERROR -1
#define CHECKVER_DATA_ERROR -2
#define CHECKVER_JSON_SUCCESS 2
#define CHECKVER_ALL_SUCCESS 1

#ifndef	IA64
#define	IA64 __int64
#endif//IA64
using namespace std;

class VHProcessBar;

class CAboutDlg :  public CBaseWnd
{
	Q_OBJECT

public:
	CAboutDlg(QWidget *parent = Q_NULLPTR);
	~CAboutDlg();
	CREATE_WND();
	virtual void Notice(const CSTDMapParam &);
	

	wstring GetCurVersion();
	int CheckVersion();
	static void	SplitString(const string& str, const string& sKey, vector<string>& vecResult, BOOL bUrl);
public slots:
		void GetNewVersion();

protected:
	bool eventFilter(QObject *, QEvent *);
   virtual void closeEvent(QCloseEvent *event);
private slots:
	void slot_startCheck();
	void on_btnCheckAndUpdate_clicked(bool checked = false);
	//void on_btnClose_clicked(bool checked = false);
	void VersionCheckTimeout();
	int VersionReplayFinished(QNetworkReply*reply);

	void DownloadReplyFinished(QNetworkReply *reply);
	void onDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
	void onReadyRead();
	void slot_stopUpdate();
	void on_btnUpdate_clicked(bool checked = false);
	void on_btnCloseButton_clicked(bool checked = false);
   void on_btnCancel_clicked(bool checked = false);
   void slot_SetRange(int min, int max);
private:
   void SetUpgradeLevel();
	void SetValue(int v);
	void SetVersion(QString version = "");
	void AcceptClose();
	void SetNetworkProxy(QNetworkAccessManager* manager);
	int parseJson(QByteArray *bytes);
	IA64	GetLongVersion(string sVersion);
	static IA64	GetCurVersion(const wchar_t* exeFile, std::wstring &);
	void StartDownload();
	void installNewFile();

	Ui::CAboutDlg ui;
	QTimer *mStartCheckTimer = nullptr;
	QTimer      *mTimeoutTimer = nullptr;
	QNetworkAccessManager *mChkVersionAccessManager = nullptr;
	QNetworkAccessManager *mDownloadAccessManager = nullptr;
	QNetworkRequest mRequest;

	QNetworkReply *mChkVersionReply = nullptr;
	QNetworkReply *mDownloadReply = nullptr;

	QMutex      mCheckMutex;
	bool        mIsChecked;
	bool mIsForceUpdata = false;//强制更新

	string  mNewVersion;
	QString mDownloadLink; //下载链接
	//QString mDownloadTips;
	wstring mCurrVersionW;

	QString mDownloadFilePath;
	QFileInfo fileInfo;
	QFile *mFile = nullptr;
	//VHProcessBar *mProcessBar = NULL;//更新进度对话框
	//下载完 安装命令调用 参照
	//RuntimeInstance.cpp   990

	QPoint pressPoint;
	QPoint startPoint;
	bool mBStopUpdate = false;
	//QGraphicsDropShadowEffect* mpEffect = nullptr;
   //QString mChangeLog = "";
   int miUpgradeLevel = 0;
   //enum eUpgradeLevel {
   //   eUpgradeLevel_
   //};
   bool mbClose = false;
};
