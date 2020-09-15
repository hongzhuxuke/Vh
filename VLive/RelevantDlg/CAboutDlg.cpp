#include "CAboutDlg.h"
#include "pathmanager.h"
#include "ConfigSetting.h"
//#include "vhupdatetips.h"
#include "VH_ConstDeff.h"
//#include "vhprocessbar.h"
#include "UACUtility.h"
#include "LoginWdg.h"
#include <QScrollBar> 
#include <QJsonParseError>
#include <QtNetwork/QNetworkProxy>
#include <QDir>
//#include <QGraphicsDropShadowEffect> 

#include "../../Unility/ToolManager.h"
#include "../../ClassSDK/URLGenerator.h"
#include "TipsWdg.h"
//extern ToolManager *globalToolManager;

#pragma comment (lib, "Version.lib")
#pragma comment (lib, "ws2_32.lib")

//#define VersionHttpAPI QString("/pcv1/system/upgrade?client_type=Windows")
#define VersionHttpAPI QString("/edu/system/upgrade?client_type=Windows")

extern std::wstring gCurrentVersion;

CBaseWnd* CAboutDlg::CreateFunction() {
	return new CAboutDlg();
}

CAboutDlg::CAboutDlg(QWidget *parent)
	: CBaseWnd(parent)
	, mChkVersionAccessManager(NULL)
	, mTimeoutTimer(NULL)
	, mStartCheckTimer(NULL)
	, mDownloadAccessManager(NULL)
	, mChkVersionReply(NULL)
	, mDownloadReply(NULL)
	, mFile(NULL)
{
	ui.setupUi(this);
	ui.label_version->adjustSize();
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAutoFillBackground(false);
  
	//mProcessBar = new VHProcessBar();
	//connect(mProcessBar, &VHProcessBar::StopUpdate, this, &CAboutDlg::slot_stopUpdate);
   //ui.label_ver->hide();
   //ui.textEditMsg->hide();
   //ui.btnCancel->hide();
   //ui.tabelScrollBar->hide();
	SetValue(0);
   ui.textEditMsg->horizontalScrollBar()->hide();
   ui.textEditMsg->verticalScrollBar()->hide();

   connect(ui.textEditMsg->verticalScrollBar(), &QScrollBar::rangeChanged, this, &CAboutDlg::slot_SetRange);
   connect(ui.textEditMsg->verticalScrollBar(), &QScrollBar::valueChanged, ui.tabelScrollBar, &QScrollBar::setValue);

   connect(ui.tabelScrollBar, &QScrollBar::valueChanged, ui.textEditMsg->verticalScrollBar(), &QScrollBar::setValue);
   //connect(ui.tblWdgRoomList, &QTableWidget::cellDoubleClicked, this, &CListViewDlg::slot_Enter);



	WCHAR				szApp[1024 * 4] = L"";
	GetModuleFileNameW(NULL, szApp, 1024 * 4);
	//TRACE6("%s szApp:%ws 3\n", __FUNCTION__, szApp);
	GetCurVersion(szApp, mCurrVersionW);
	ToolManager::GetInstance()->GetDataManager()->SetVersion(QString::fromStdWString(mCurrVersionW));
	gCurrentVersion = mCurrVersionW;
	ui.btnCloseButton->setToolTip(QStringLiteral("关闭"));
	QString confPath = CPathManager::GetAppDataPath() + QString::fromStdWString(VHALL_TOOL_CONFIG);
	QString vhallHelper = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_VHALL_LIVE, VHALL_LIVE_TEXT);
	setWindowTitle(vhallHelper);
	this->installEventFilter(this);
   ui.stackedWidget->setCurrentIndex(0);
	//mpEffect = new QGraphicsDropShadowEffect(this);
	//mpEffect->setOffset(4, 6);
	//mpEffect->setColor(QColor(0, 0, 0, 40));
	//mpEffect->setBlurRadius(10);
	//ui.widget->setGraphicsEffect(mpEffect);
}

void CAboutDlg::slot_SetRange(int min, int max) {
   if (max > min) {
      ui.tabelScrollBar->show();
      ui.tabelScrollBar->setRange(min, max);
   }
   else
   {
      ui.tabelScrollBar->hide();
   }

}


CAboutDlg::~CAboutDlg()
{
	//if (nullptr != mpEffect)
	//{
	//	delete mpEffect;
	//	mpEffect = nullptr;
	//}
	if (NULL != mStartCheckTimer)
	{
		if (mStartCheckTimer->isActive())
		{
			mStartCheckTimer->stop();
		}
		delete mStartCheckTimer;
		mStartCheckTimer = NULL;
	}

	if (NULL != mTimeoutTimer)
	{
		if (mTimeoutTimer->isActive())
		{
			mTimeoutTimer->stop();
		}
		delete mTimeoutTimer;
		mTimeoutTimer = NULL;
	}

	if (NULL != mChkVersionAccessManager)
	{
		mChkVersionAccessManager->deleteLater();
		mChkVersionAccessManager = NULL;
	}

	if (NULL != mDownloadAccessManager)
	{
		mDownloadAccessManager->deleteLater();
		mDownloadAccessManager = NULL;
	}

	if (NULL != mFile)
	{
		mFile->deleteLater();
		mFile = NULL;
	}
}

void CAboutDlg::SetValue(int v) {
	ui.progressBar->setValue(v);
	ui.labPerc->setText(QString("%1%").arg(v));
}

bool CAboutDlg::eventFilter(QObject *o, QEvent *e) {
	if (o == this) {
		if (e->type() == QEvent::MouseButtonPress) {
			this->pressPoint = this->cursor().pos();
			this->startPoint = this->pos();
		}
		else if (e->type() == QEvent::MouseMove) {
			int dx = this->cursor().pos().x()-this->pressPoint.x();
			int dy = this->cursor().pos().y()-this->pressPoint.y();
			this->move(this->startPoint.x() + dx, this->startPoint.y() + dy);
		}
	}
	return QDialog::eventFilter(o, e);
}

void CAboutDlg::closeEvent(QCloseEvent * event)
{
   if (mbClose) {
      mbClose = false;
      CBaseWnd::closeEvent(event);
   }
   else {
      event->ignore(); // 忽略关闭事件
   }
   
}

void CAboutDlg::on_btnCheckAndUpdate_clicked(bool checked/* = false*/)
{
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
	if (NULL == mStartCheckTimer)
	{
		mStartCheckTimer = new QTimer(this);
		if (NULL != mStartCheckTimer)
		{
			connect(mStartCheckTimer, SIGNAL(timeout()), this, SLOT(slot_startCheck()));
			mStartCheckTimer->setSingleShot(true);
		}
	}

	if (NULL != mStartCheckTimer)
	{	
		//更新url
		GetNewVersion();
		mStartCheckTimer->start(300);
	}
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void CAboutDlg::slot_startCheck()
{
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
	mStartCheckTimer->stop();
	//TRACE6("%s new QVBoxLayout \n", __FUNCTION__);
	if (NULL == mTimeoutTimer)
	{
		mTimeoutTimer = new QTimer(this);
		if (NULL != mTimeoutTimer)
		{
			connect(mTimeoutTimer, SIGNAL(timeout()), this, SLOT(VersionCheckTimeout()));
		}
	}

	if (NULL != mTimeoutTimer)
	{
		mTimeoutTimer->start(10000);
	}

	if (NULL == mChkVersionAccessManager)
	{
		mChkVersionAccessManager = new QNetworkAccessManager(this);

		if (NULL != mChkVersionAccessManager)
		{
			connect(mChkVersionAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(VersionReplayFinished(QNetworkReply*)));
			connect(mChkVersionAccessManager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SLOT(slot_authenticationRequired(QNetworkReply *, QAuthenticator *)));
			connect(mChkVersionAccessManager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(slot_proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
			//TRACE6("%s new mChkVersionAccessManager \n", __FUNCTION__);
		}
	}
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s GetNewVersion", __FUNCTION__);
	GetNewVersion();
	//TRACE6("%s\n", __FUNCTION__);
	QUrl url = mRequest.url();

	if (NULL != mChkVersionAccessManager)
	{
		SetNetworkProxy(mChkVersionAccessManager);
		mBStopUpdate = false;

		if (mChkVersionAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
			mChkVersionAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
		}

		mChkVersionReply = mChkVersionAccessManager->get(mRequest);
	}
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
	//TRACE6("%s mChkVersionAccessManager->get(mRequest) url:%s\n", __FUNCTION__, url.toString().toStdString().c_str());
}

wstring CAboutDlg::GetCurVersion() {
	return mCurrVersionW;
}

void CAboutDlg::Notice(const CSTDMapParam &mapParam) {
   //QString class_conf = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   QString class_conf = ToolManager::GetInstance()->GetToolConfigPath();
   int hideLogo = ConfigSetting::ReadInt(class_conf, GROUP_DEFAULT, KEY_VHALL_LOGO_HIDE, 0);
   if (hideLogo == 1) {
      AcceptClose();
   }
   else {
      on_btnCheckAndUpdate_clicked();
      //ui.label->hide();
      //ui.labPerc->hide();
      //ui.progressBar->hide();
      //ui.labProgressBar->hide();
      //ui.btnCloseButton->hide();
      //ui.label_version->hide();
      //ui.btnUpdate->hide();
      ui.stackedWidget->setCurrentIndex(0);
      show();
   }
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void CAboutDlg::SetVersion(QString version) {
   ui.stackedWidget->setCurrentIndex(1);
	if (version.trimmed().isEmpty())
	{
		ui.label_version->setText(QStringLiteral("已是最新版本"));
		ui.btnUpdate->hide();
      ui.btnCancel->hide();
      ui.label_ver->hide();
      ui.textEditMsg->hide();
      AcceptClose();
	}
	else
	{
      ui.label_ver->setText(QStringLiteral("客户端v") + version);
		ui.label_version->setText(QStringLiteral("课堂有新版本可以更新"));
		ui.btnUpdate->show();
	}
}

void CAboutDlg::AcceptClose()
{
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   mbClose = true;
	close();
   	CBaseWnd* logInWnd = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
   	if (logInWnd) {
		ToolManager::GetInstance()->GetDataManager()->WriteLog("%s", __FUNCTION__);
   		((LoginWdg*)logInWnd)->show();
   	}
      ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void CAboutDlg::GetNewVersion() {
    //QString confPath = QApplication::applicationDirPath() + QString("/") + QString::fromStdWString(VHALL_CLASS_TOOL_CONFIG);
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
	QString confPath = ToolManager::GetInstance()->GetToolConfigPath();
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s confPath %s", __FUNCTION__, confPath.toStdString().c_str());
    QString domainURL = ConfigSetting::ReadString(confPath, GROUP_DEFAULT, KEY_DOMAIN, defaultDomainURL);
    ToolManager::GetInstance()->GetDataManager()->WriteLog("%s domainURL %s", __FUNCTION__, domainURL.toStdString().c_str());
	QUrl versionUrl(domainURL + VersionHttpAPI);
	mRequest.setUrl(versionUrl);
   ToolManager::GetInstance()->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

int CAboutDlg::VersionReplayFinished(QNetworkReply *reply) {
	int ret = CHECKVER_JSON_SUCCESS;
	if (mCheckMutex.tryLock()) {
		bool isShowAlert = !mIsChecked;
		mIsChecked = true;
		mCheckMutex.unlock();
		if (true) {
			mTimeoutTimer->stop();
			QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
			if (reply->error() == QNetworkReply::NoError) {
				QByteArray bytes = reply->readAll();
				if ((ret = parseJson(&bytes)) != CHECKVER_JSON_SUCCESS) {
					Sleep(3000);
					AcceptClose();
					return ret;
				}
				else {
					//TRACE6("CCheckVersionDlg::VersionReplayFinished will CheckVersion");
					CheckVersion();
				}
			}
			else {
				//TRACE6("%s reply->error() %d\n", __FUNCTION__, reply->error());
				Sleep(1000);
				AcceptClose();
				return CHECKVER_DATA_ERROR;
			}
		}
	}

	reply->deleteLater();
	return ret;

}

IA64	 CAboutDlg::GetCurVersion(const wchar_t* exeFile, std::wstring &currentVersion) {
	IA64				_i64Ver = 0;
	DWORD				dwVerHnd = 0;
	VS_FIXEDFILEINFO*	pFileInfo = NULL;
	unsigned int		dwInfoSize = 0;
	char	*			pVersionInfo = NULL;
	DWORD				dwVerInfoSize = GetFileVersionInfoSizeW(exeFile, &dwVerHnd);

	if (dwVerInfoSize) {
		pVersionInfo = new char[dwVerInfoSize];

		GetFileVersionInfoW(exeFile, dwVerHnd, dwVerInfoSize, pVersionInfo);
		VerQueryValue(pVersionInfo, (LPTSTR)("\\"), (void**)&pFileInfo, &dwInfoSize);
		QString strAppVer = QString("%1.%2.%3.%4").arg(HIWORD(pFileInfo->dwFileVersionMS)).arg(LOWORD(pFileInfo->dwFileVersionMS)).arg(HIWORD(pFileInfo->dwFileVersionLS)).arg(LOWORD(pFileInfo->dwFileVersionLS));

		currentVersion = strAppVer.toStdWString();

		_i64Ver = pFileInfo->dwProductVersionMS;
		_i64Ver = _i64Ver << 32;
		_i64Ver |= pFileInfo->dwProductVersionLS;

		delete[] pVersionInfo;
		pVersionInfo = NULL;
	}
	return _i64Ver;
}


void CAboutDlg::SetUpgradeLevel()
{
   string newVerStr = this->mNewVersion;
   WCHAR				szApp[1024 * 4] = L"";
   IA64				i64NewVersion = 0;
   IA64				i64CurVersion = 0;
   GetModuleFileNameW(NULL, szApp, 1024 * 4);
   i64CurVersion = GetCurVersion(szApp, mCurrVersionW);
   gCurrentVersion = mCurrVersionW;
   ToolManager::GetInstance()->GetDataManager()->SetVersion(QString::fromStdWString(mCurrVersionW));
   //hide();
   //ui.labTips->hide();
   ui.stackedWidget->setCurrentIndex(0);
   mIsForceUpdata ? ui.btnCloseButton->hide() : ui.btnCloseButton->show();
   ui.label_version->show();
   ui.btnUpdate->show();

   //VHUpdateTips tips(NULL, mIsForceUpdata);
   if (newVerStr.empty() || newVerStr.length() == 0) {//无法获取当前线上版本
      //TRACE6("newVerStr.empty() || newVerStr.length() == 0\n");
      SetVersion();
   }
   else {
      i64NewVersion = GetLongVersion(newVerStr);
      //TRACE6("%s i64NewVersion:%I64d  i64CurVersion:%I64d\n", __FUNCTION__, i64NewVersion, i64CurVersion);
      if (i64NewVersion > i64CurVersion) {//有版本更新
         //TRACE6("A new version is detected\n");
         SetVersion(QString::fromStdString(this->mNewVersion));
      }
      else {//没有版本更新
         //TRACE6("%s accept\n", __FUNCTION__);
         SetVersion();
      }
   }
   
}
int CAboutDlg::CheckVersion() 
{
   ui.stackedWidget->setCurrentIndex(1);
   if(3 == miUpgradeLevel) {
      SetUpgradeLevel();
      ui.btnCancel->hide();
      ui.btnCloseButton->hide();
      ui.label_ver->show();
      ui.textEditMsg->show();
   }
   else if (1 == miUpgradeLevel) {
         SetUpgradeLevel();
         ui.btnCancel->show();
         ui.btnCloseButton->show();
         ui.label_ver->show();
         ui.textEditMsg->show();
   }
   else{
      Sleep(1000);
      AcceptClose();
     
   }
   return 0;
}

void CAboutDlg::on_btnUpdate_clicked(bool checked/* = false*/) {
   //ui.btnCloseButton->show();
	StartDownload();
}

void CAboutDlg::on_btnCloseButton_clicked(bool checked/* = false*/)
{
	if (ui.progressBar->isVisible())
	{
		//CBaseWnd* tips = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_TIPS_WND, "close");
		TipsWdg* tips = new TipsWdg(this);
		if (tips) {
			CSTDMapParam param;
			param[TipsMsg] = QStringLiteral("您尚未完成升级微吼课堂客户端，是否确认结束升级程序?");
			param[ShowCancelButton] = PARAM_TRUE;
			tips->Notice(param);
			int accept = tips->exec();
			if (accept == QDialog::Accepted) {
				slot_stopUpdate();
			}
		}
	}
	else
		AcceptClose();
}

void CAboutDlg::on_btnCancel_clicked(bool checked)
{
   on_btnCloseButton_clicked(checked);
}

void CAboutDlg::installNewFile()
{
	if (mDownloadFilePath.length() > 0) {
		WCHAR tmp[1024] = L"";
		mDownloadFilePath.toWCharArray(tmp);
		//SaveCommand();
		//TRACE6("Will run update [%ws]", tmp);
		//执行新的客户端程序
		RunProcess(tmp, NULL);
	}

}
void CAboutDlg::StartDownload() {
	if (!mDownloadLink.isEmpty() && !mDownloadLink.isNull()) {
		/*ui.label_version->hide();
		ui.btnUpdate->hide();
      ui.label_ver->hide();
		ui.btnCloseButton->show();
      ui.textEditMsg->hide();
      ui.btnCancel->hide();
      ui.tabelScrollBar->hide();
		ui.label->show();
		ui.labPerc->show();
		ui.progressBar->show();
      ui.labProgressBar->show();*/
      ui.stackedWidget->setCurrentIndex(2);
		mDownloadLink = mDownloadLink.replace("https", "http");
		QUrl downloadURL(mDownloadLink);
		qDebug() << mDownloadLink;
		fileInfo = downloadURL.path();
		//TRACE6("CCheckVersionDlg::StartDownload will download file to %s\n", fileInfo.fileName().toStdString().c_str());
		mDownloadFilePath = CPathManager::GetAppDataPath() + QString("updates\\");
		//TRACE6("%s mDownloadFilePath  to %s\n", __FUNCTION__, mDownloadFilePath.toStdString().c_str());
		QDir updateDir(mDownloadFilePath);
		if (updateDir.exists() == false) {
			bool mksuc = updateDir.mkpath(mDownloadFilePath);
			//TRACE6("%s mksuc %d\n", __FUNCTION__, mksuc);
		}
		mDownloadFilePath += "VhallClassUpdate.exe";
		//TRACE6("%s mDownloadFilePath  to %s\n", __FUNCTION__, mDownloadFilePath.toStdString().c_str());
		mFile = new QFile(mDownloadFilePath);
		if (mFile->exists())
		{
			mFile->remove();
		}

		mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);

		mRequest.setUrl(downloadURL);
		//TRACE6("CCheckVersionDlg::StartDownload downloadURL %s\n", downloadURL.toString().toStdString().c_str());
		if (NULL == mDownloadAccessManager)
		{
			mDownloadAccessManager = new QNetworkAccessManager(this);
			if (NULL != mDownloadAccessManager)
			{
				connect(mDownloadAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(DownloadReplyFinished(QNetworkReply*)));
		    }
		}

        QString strRange = QString("bytes=%1-").arg(0);
        mRequest.setRawHeader("Range", strRange.toLatin1());

		SetNetworkProxy(mDownloadAccessManager);

		if (mDownloadAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
			mDownloadAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
		}

		mDownloadReply = mDownloadAccessManager->get(mRequest);
		if (mDownloadReply) {
			connect(mDownloadReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
			connect((QObject *)mDownloadReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		}
		//this->mProcessBar->show();
	}
}

void CAboutDlg::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal) {
	if (bytesTotal > 0) {
		qint64 persent = bytesSent * 100;
		persent /= bytesTotal;
		SetValue(persent);
	}
}

void CAboutDlg::onReadyRead() {
	QIODevice::OpenMode mode = mFile->openMode();
	mode = (mode & QIODevice::WriteOnly);
	if(mode)
		mFile->write(mDownloadReply->readAll());
}

void CAboutDlg::slot_stopUpdate()
{
	mBStopUpdate = true;
	mFile->close();

   if (3 == miUpgradeLevel) {
      ToolManager::GetInstance()->SafeToExitApp();
      return;

   }

	if (mDownloadReply) {
		disconnect(mDownloadReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
		disconnect((QObject *)mDownloadReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		mDownloadReply->abort();
		mDownloadReply->deleteLater();
		mDownloadReply = NULL;
	}
	//reject();
}

void CAboutDlg::DownloadReplyFinished(QNetworkReply *reply2) {
	hide();
	QVariant status_code = reply2->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply2->error() == QNetworkReply::NoError && !mBStopUpdate) {
		//TRACE6("CheckVersion() download client success [%s]", mDownloadLink.toStdString().c_str());
		mFile->close();
		installNewFile();
        ToolManager::GetInstance()->GetDataManager()->WriteLog("%s installNewFile", __FUNCTION__);
        ToolManager::GetInstance()->SafeToExitApp();
        return;
	}
	else {
		qDebug() << "CCheckVersionDlg::DownloadReplyFinished:" << reply2->errorString();
        ToolManager::GetInstance()->GetDataManager()->WriteLog("%s error code:%d :%s", __FUNCTION__, reply2->error(), reply2->errorString().toStdString().c_str());
		//TRACE6("CheckVersion() download client failed [%s]", mDownloadLink.toStdString().c_str());
		if (3 == miUpgradeLevel) {
         reply2->abort();
         reply2->deleteLater();
         reply2 = nullptr;
         mDownloadAccessManager->deleteLater();
         ToolManager::GetInstance()->SafeToExitApp();
         return;

      }
      else {
         AcceptClose();
      }
        
	}
	reply2->deleteLater();
    reply2 = nullptr;
}

IA64	CAboutDlg::GetLongVersion(string sVersion) {
	int				i = 0;
	vector<string>	vers;
	IA64			i64Ret = 0;
	SplitString(sVersion, ".", vers, false);
	if (vers.size() != 4)
		return 0;
	for (i = 0; i < 4; i++) {
		if (i == 0)
			i64Ret = atol(vers[i].c_str());
		else {
			i64Ret = i64Ret << 16;
			i64Ret |= atol(vers[i].c_str());
		}
	}
	return i64Ret;
}

void CAboutDlg::VersionCheckTimeout() {
	if (mCheckMutex.tryLock()) {

		ui.labTips->setText(QStringLiteral( "更新检查超时"));
		mTimeoutTimer->stop();
		bool isShowAlert = !mIsChecked;
		mIsChecked = true;
		mCheckMutex.unlock();
		//此处提示更新检查超时
		Sleep(1000); 
		AcceptClose();
	}
}

void CAboutDlg::SetNetworkProxy(QNetworkAccessManager* manager) {
	QNetworkProxy proxy;
	QString configPath = CPathManager::GetConfigPath();
	int isOpenProxy = ConfigSetting::ReadInt(configPath, GROUP_DEFAULT, PROXY_OPEN, 0);
	if (isOpenProxy == 1) {
		QString ip = ConfigSetting::ReadString(configPath, GROUP_DEFAULT, PROXY_HOST, "");
		int port = ConfigSetting::ReadInt(configPath, GROUP_DEFAULT, PROXY_PORT, 80);
		proxy.setHostName(ip);
		proxy.setPort(port);
		proxy.setType(QNetworkProxy::HttpProxy);
	}
	else {
		proxy.setType(QNetworkProxy::NoProxy);
	}
	manager->setProxy(proxy);
}

int CAboutDlg::parseJson(QByteArray *bytes) {
	int iRef = CHECKVER_JSON_ERROR;
	QJsonParseError error;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(*bytes, &error);
	if (error.error == QJsonParseError::NoError) {
		if (jsonDocument.isObject()) {
			QVariantMap result = jsonDocument.toVariant().toMap();

			int code = result["code"].toInt();
			QString msg = result["msg"].toString();
			if (code == 200) {
				QVariantMap dataMap = result["data"].toMap();

				if (dataMap["client_type"].toString().compare("Windows") == 0)
				{

					mNewVersion = (dataMap["version"].toString()).toStdString();
					mDownloadLink = dataMap["download_url"].toString();
					//强制更新
					QString forceUpdata = dataMap["upgrade_level"].toString();
               miUpgradeLevel = forceUpdata.toInt();

               QString strChangeLog = dataMap["change_log"].toString();
               ui.textEditMsg->setText(strChangeLog);
					if (forceUpdata.compare("3") == 0) {
						mIsForceUpdata = true;
					}
					iRef = CHECKVER_JSON_SUCCESS;
				}
				else {		
				}
			}
			else {
			}
		}
		else {
		}
	}
	else {
	}
	return iRef;
}

void	CAboutDlg::SplitString(const string& str, const string& sKey, vector<string>& vecResult, BOOL bUrl) {
	string::size_type	npos = 0;
	string::size_type	nPrev = 0;

	while (true) {
		nPrev = npos;
		npos = str.find(sKey, npos);

		if (nPrev != 0 && bUrl == TRUE) {
			//if it is url, not check the url's key
			npos = string::npos;
		}

		if (npos != string::npos) {
			if (npos != nPrev) {
				//				sLine = str.substr(nPrev,npos-nPrev);
				vecResult.push_back(str.substr(nPrev, npos-nPrev));
			}
			npos += sKey.size();
			if (npos == nPrev)
				break;
		}
		else {
			if (nPrev < str.size())
				vecResult.push_back(str.substr(nPrev));
			break;
		}
	}
}


