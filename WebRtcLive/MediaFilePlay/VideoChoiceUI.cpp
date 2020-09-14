#include "stdafx.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QToolButton>
#include <vector>

#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include "TitleWidget.h"
#include "push_button.h"
//#include "VedioPlayUI.h"
#include "VideoChoiceUI.h"
#include "ToolButton.h"
#include "Push_button.h"
#include <QSpacerItem>
//#include <QGraphicsDropShadowEffect> 
#include "TipsWdg.h"
#include "VH_ConstDeff.h"
#include "NoiseTooltip.h"

//#include "MessageBox.h"

extern ToolManager *globalToolManager;
using namespace std;

//#define PLAYUI_PLAYSTATE
//#define PLAYUI_PLAYSTATE_STOPPING  -2 //结束中
//#define PLAYUI_PLAYSTATE_FAIL      -1      //失败
//#define PLAYUI_PLAYSTATE_NOTHING   0   //
//
//#define PLAYUI_PLAYSTATE_OPENING   1    //打开中
//#define PLAYUI_PLAYSTATE_BUFFERING 2   //缓冲中
//#define PLAYUI_PLAYSTATE_PALYERING 3    //播放中
//#define PLAYUI_PLAYSTATE_PAUSED    4     //暂停
//#define PLAYUI_PLAYSTATE_END       5        //结束
//#define PLAYUI_PLAYSTATE_STOP      6       //停止
//#define PLAYUI_PLAYSTATE_ERROR     7      //错误

//;; *.rmvb;; *.mp4;; *.avi;; *.wmv;; *.mkv;; *.flv;; *.mov;; "
#define STRSUPPORTFORMAT  QString("*.mp3 *.wav *.rmvb *.mp4  *.avi *.wmv *.mkv *.flv *.mov")
//#define STRSUPPORTFORMAT  QString("*;;*.mp3;; *.wav;; *.rmvb;;*.mp4;; *.avi;;*.wmv;;*.mkv;;*.flv;; *.mov;; ")
                                                     
#define CYCLETYLE_LIST  QStringLiteral("列表循环")
#define CYCLETYLE_SINGLE QStringLiteral("单视频循环")
#define UNPLAY QStringLiteral("播放")
#define PLAYING QStringLiteral("播放中")

#define UNPALAYQSS	"QPushButton{\
	border-image:url(:/button/img/button/StartPlay.png);\
	}\
	QPushButton:hover{\
		border-image:url(:/button/img/button/StartPlay.png);\
	}\
	QPushButton:pressed{\
		border-image:url(:/button/img/button/StartPlay.png);\
	}\
	QPushButton:disable{\
		border-image:url(:/button/img/button/StartPlay.png);\
	}"

#define PALAYINGQSS	"QPushButton{\
	border-image:url(:/button/img/button/suspend.png);\
	}\
	QPushButton:hover{\
		border-image:url(:/button/img/button/suspend.png);\
	}\
	QPushButton:pressed{\
		border-image:url(:/button/img/button/suspend.png);\
	}\
	QPushButton:disable{\
		border-image:url(:/button/img/button/suspend.png);\
	}"

#define  UNPLAYQSS "QPushButton{\
font: 12px \"微软雅黑\";\
  color: rgb(68, 68, 68);\
  border:1px solid rgb(231, 234, 236);\
  background:rgb(255, 255, 255);\
	  border-radius:2px;\
}\
QPushButton:hover{\
	color: rgb(255, 255, 255);\
	background:rgba(82,204,144,255);\
	border:1px solid rgba(82,204,144,0);\
}\
QPushButton:pressed{\
	color: rgb(68,68,68);\
	border:1px solid rgb(231,234,236);\
	background:rgb(255, 255, 255);\
}\
QPushButton:disabled{\
	color: rgb(153, 153, 153);\
   border:1px solid rgb(231, 234, 236);\
   background:rgb(231, 234, 236);\
}"
#define  PLAYINGQSS "QPushButton{\
font: 12px \"微软雅黑\";\
  color: rgb(68, 68, 68);\
  border:1px solid rgb(231, 234, 236);\
  background:rgb(255, 255, 255);\
	  border-radius:2px;\
}"


CREATE_WND_FUNCTION(VideoChoiceUI);
VideoChoiceUI::VideoChoiceUI(QWidget *parent) :
	CBaseWnd(parent) 
	, m_iVolume(100)
	, m_bIsMute(false) {
	ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
   connect(ui.addButton, &QToolButton::clicked, this, &VideoChoiceUI::slot_FileDialogChoose);
   ui.btnClose->setFocusPolicy(Qt::NoFocus);
   connect(ui.ListWidget, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(PlayListDoubleClicked(const QModelIndex &)));
   connect(ui.ListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
   ui.ListWidget->setDragEnabled(true);
   ui.ListWidget->setDragDropMode(QAbstractItemView::InternalMove);
   ui.ListWidget->setDefaultDropAction(Qt::CopyAction);
   ui.btnPlay->setToolTip(QStringLiteral("播放"));
   ui.btnPlay->setStyleSheet(UNPALAYQSS);//播放按钮

   connect(ui.ProgressSlider, SIGNAL(sigValueChange(int)), this, SLOT(OnProgressChanged(int)));
   connect(&m_qCycleTimer, &QTimer::timeout, this, &VideoChoiceUI::OnCyclePlay);
   SetTitle(QStringLiteral("当前播放：无"));

   ui.ListWidget->setFocusPolicy(Qt::NoFocus);
   SetCircleMode(CycleTyle_List);
   
   ui.FileTypeTips->installEventFilter(this);

   ui.btnMinimize->hide();

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();

   ui.backframe->setFixedSize(588, 567);
   setFixedSize(590, 569);
   miItemWidth = 518;
}

VideoChoiceUI::~VideoChoiceUI() {
   if (nullptr != m_pNoiseTips) {
      delete m_pNoiseTips;
      m_pNoiseTips = nullptr;
   }
	//if (nullptr != mpEffect)
	//{
	//	delete mpEffect;
	//	mpEffect = nullptr;
	//}
	//
	//if (nullptr != mpEffectLeft)
	//{
	//	delete mpEffectLeft;
	//	mpEffectLeft = nullptr;
	//}
}

void VideoChoiceUI::on_btnPlay_clicked(bool checked /*= false*/)
{
	if (ui.ListWidget->count() > 0)
	{
		if (nullptr == mpCurPlayItem)
		{
			bool finde = false;
			QListWidgetItem * item = ui.ListWidget->item(0);
			if (NULL != item)
			{
				 QWidget* w= ui.ListWidget->itemWidget(item);
				 if (w)
				 {
					 VideoChoiceItem* VideoW = dynamic_cast<VideoChoiceItem*>(w);
					 slot_paly(VideoW, VideoChoiceItem::ePlayState_Unplay);
					 finde = true;
				 }
			}
			
			if (!finde)
			{
				CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
				CSTDMapParam mapParam;
				mapParam[TipsMsg] = QString::fromWCharArray(L"请先选择要插播的视频");
				pWdg->Notice(mapParam);
			}
			return;
		}

		int nState = globalToolManager->GetClassSDK()->MediaGetPlayerState();
		if (nState == 7) {
			MediaFilePlayFaild();
		}
		else if (nState == PLAYUI_PLAYSTATE_PAUSED) {//当前状态暂停
			globalToolManager->GetClassSDK()->MediaFileResume();//恢复播放
			if (NULL != mpCurPlayItem)
				mpCurPlayItem->SetPlayState(VideoChoiceItem::ePlayState_Playing);
		}
		else if (nState == PLAYUI_PLAYSTATE_PALYERING) {//当前状态播放中
			globalToolManager->GetClassSDK()->MediaFilePause(); //暂停
			if (NULL != mpCurPlayItem)
				mpCurPlayItem->SetPlayState(VideoChoiceItem::ePlayState_suspend);
		}
		else {
			CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
			CSTDMapParam mapParam;
			mapParam[TipsMsg] = QString::fromWCharArray(L"请先选择要插播的视频");
			pWdg->Notice(mapParam);
		}
	}
	else
	{
		CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		CSTDMapParam mapParam;
		mapParam[TipsMsg] = QString::fromWCharArray(L"请先添加要插播视频");
		pWdg->Notice(mapParam);
	}
}

void VideoChoiceUI::MediaFilePlayFaild()
{
	CBaseWnd* pWdg= globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	CSTDMapParam mapParam;
	mapParam[TipsMsg] = QString::fromWCharArray(L"插播视频失败，请检测插播文件是否正确");
	pWdg->Notice(mapParam);
	
	globalToolManager->GetDataManager()->WriteLog("%s CustomEvent_GetLocalFileStreamErr\n", __FUNCTION__);
	//关闭循环播放定时器
	m_qCycleTimer.stop();
	////隐藏UI
	m_bPlayCurrentFile = true;
}

void VideoChoiceUI::PlaySelectedFile(bool bCurr /*= true*/) {
	m_bPlayCurrentFile = bCurr;
}

int VideoChoiceUI::GetPlayFileState(signed long long& n64Pos, signed long long& n64MaxPos) {
	int nRet = -1;
	
	nRet = globalToolManager->GetClassSDK()->MediaGetPlayerState();
	n64Pos = globalToolManager->GetClassSDK()->MediaFileGetCurrentDuration();
	n64MaxPos = globalToolManager->GetClassSDK()->MediaFileGetMaxDuration();
	if (nRet == 7) {
		MediaFilePlayFaild();
	}
	return nRet;
}


void VideoChoiceUI::UpdatePlayTime(QString curT, QString fulT) {
	ui.labCurTime->setText(curT);
	ui.labFullTime->setText(fulT);
}

void VideoChoiceUI::UpdatePlayStates(bool bPlay) {
 	if (bPlay) {
		ui.btnPlay->setToolTip(QStringLiteral("暂停"));
		ui.btnPlay->setStyleSheet(PALAYINGQSS);//暂停按钮
		//ui.btnDel->setEnabled(false);
	}
	else {
		ui.btnPlay->setToolTip(QStringLiteral("播放"));
		ui.btnPlay->setStyleSheet(UNPALAYQSS);//播放按钮
		//ui.btnDel->setEnabled(true);
	}
}

void VideoChoiceUI::SetMuteSelf(const bool& bMute)
{
	m_bChkBoxMuteSelf = bMute;
   ui.ChkBoxMuteSelf->setChecked(bMute);
}

void VideoChoiceUI::SetMuteStudent(const bool& bMute)
{
	m_bChkBoxMuteStudent = bMute;
   ui.ChkBoxMuteStudent->setChecked(m_bChkBoxMuteStudent);
}

void VideoChoiceUI::OnCyclePlay()
{
	signed long long curPos, maxPos;
	int nState = GetPlayFileState(curPos, maxPos);
	long dulation = curPos;
	long maxDulation = maxPos;

	QString strcurrDulation;
	QString strmaxDulation;
	
	//将时间计算出来 时分秒，展示当前播放到 时分秒/ 总时长：时分秒
	QString dulationStr = dulation2Str(dulation, maxDulation, strcurrDulation, strmaxDulation);
	if (nState == PLAYUI_PLAYSTATE_STOP ||//停止
		nState == PLAYUI_PLAYSTATE_END ||//结束
		nState == PLAYUI_PLAYSTATE_FAIL) {//失败
		
		QString dulationStr = dulation2Str(0, 0, strcurrDulation, strmaxDulation);
		UpdatePlayTime(strcurrDulation, strmaxDulation);
		UpdatePlayStates(FALSE);
		SetProgressValue(0, 0);
	}
	else if (nState == PLAYUI_PLAYSTATE_PAUSED) {//暂停
		UpdatePlayTime(strcurrDulation, strmaxDulation);
		UpdatePlayStates(FALSE);
		SetProgressValue(dulation, maxDulation);
	}
	else if (PLAYUI_PLAYSTATE_STOPPING == nState) {//结束中
		UpdatePlayTime(strcurrDulation, strmaxDulation);
		UpdatePlayStates(FALSE);
		SetProgressValue(dulation, maxDulation);
	}
	else {
		UpdatePlayTime(strcurrDulation, strmaxDulation);
		UpdatePlayStates(TRUE);
		SetProgressValue(dulation, maxDulation);
	}

	if (nState == PLAYUI_PLAYSTATE_END) {//当前播放结束
		switch (GetCurPlayMode()) {
		case CycleTyle_None: {				//顺序播放
			if (!IsArriveTheEnd()) {
				SwitchToNext();
				PlaySelectedFile();
			}
			else {
				DealStopPlay();
			}
		}
		break;
		case CycleTyle_List: {	//列表循环
			SwitchToNext();
			PlaySelectedFile();
		}
		break;
		case CycleTyle_Single: { //单视频循环
			PlaySelectedFile();
		}
		break;
		case CycleTyle_END:
			break;
		default:
			break;
		}
	}

	if (m_bPlayCurrentFile) {
			QString szFileName;
			bool bCanPlay = GetCurFile(szFileName);
			unsigned int iVolume = GetCurVolume();
			if (bCanPlay) {
            if(m_qCycleTimer.isActive()){
               m_qCycleTimer.stop();
            }
            
				if (OpenPlayMediaFile((char*)szFileName.toStdString().c_str()) == 0) {
					//SetPlayMeidaFileVolume(iVolume);
					m_bPlayCurrentFile = false;
				}
			}
	}
}

int VideoChoiceUI::GetCurVolume() {
	return m_bIsMute ? 0 : m_iVolume;
}

void VideoChoiceUI::on_ChkBoxMuteSelf_toggled(bool checked)
{
	int nState = globalToolManager->GetClassSDK()->MediaGetPlayerState();
	if (PLAYUI_PLAYSTATE_NOTHING< nState && nState< PLAYUI_PLAYSTATE_ERROR)
	{
		emit sigMuteSelf(checked);
	}
	
}

void VideoChoiceUI::on_ChkBoxMuteStudent_toggled(bool checked)
{
	int nState = globalToolManager->GetClassSDK()->MediaGetPlayerState();
	if (PLAYUI_PLAYSTATE_NOTHING < nState && nState < PLAYUI_PLAYSTATE_ERROR)
	{
		if (checked)
		{
			globalToolManager->GetClassSDK()->OperateAllUserMic(1);
		}
		else
		{
			globalToolManager->GetClassSDK()->OperateAllUserMic(0);
		}
	}
}

int VideoChoiceUI::OpenPlayMediaFile(const char *szMediaFile) {
	return InitMediaStream(szMediaFile);
}

void VideoChoiceUI::DealStopPlay(/*void* apData, DWORD adwLen*/) {
	//关闭循环播放定时器
	m_qCycleTimer.stop();
	//隐藏UI
	m_bPlayCurrentFile = true;
	StopPlayMediaFile();
}

void VideoChoiceUI::StopPlayMediaFile() {
	if (globalToolManager->GetClassSDK()->IsPushingStream(VHStreamType_MediaFile))
	{
		globalToolManager->GetDataManager()->WriteLog("%s :StopPushMediaFileStream", __FUNCTION__);
		globalToolManager->GetClassSDK()->StopPushMediaFileStream();
	}

	//判断是否播放中 如是 则释放播放采集中
	if (globalToolManager->GetClassSDK()->IsCapturingStream(VHStreamType_MediaFile))
	{
		globalToolManager->GetDataManager()->WriteLog("%s :StopMediaFileCapture", __FUNCTION__);
		globalToolManager->GetClassSDK()->StopMediaFileCapture();
	}

	CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	if (nullptr!=pWdg)
	{
		CSTDMapParam mapParam;
		mapParam[STOPSHOWMEDIAVIDEO] = "StopShowMediaVideo";
		pWdg->Notice(mapParam);
	}

}

int VideoChoiceUI::InitMediaStream(const char *szMediaFile) {
	QFile file(szMediaFile);
	if (!file.exists()) {//文件不存在
		//QCoreApplication::postEvent(mpMainUILogic, new QEvent(CustomEvent_GetLocalFileStreamErr));
		//return -1;
	}

	int iRef = globalToolManager->GetClassSDK()->StartPlayMediaFile(szMediaFile);
	if (VhallLive_OK == iRef)
	{
		//globalToolManager->GetClassSDK()->SetCurrentMicVol(iVolume);
	}
	else
	{
		CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
		CSTDMapParam mapParam;
		mapParam[TipsMsg] = QString::fromWCharArray(L"插播视频错误，错误码 :%1").arg(iRef);
		pWdg->Notice(mapParam);

		return -1;
	}
	return 0;
}

bool VideoChoiceUI::IsArriveTheEnd() {
	int index = ui.ListWidget->currentIndex().row();

	if (ui.ListWidget->count() <= index + 1) {
		return true;
	}
	return false;
}

int VideoChoiceUI::GetCurPlayMode() {
	return miCycleType;
}

void VideoChoiceUI::SwitchToNext() {
	int iRow = ui.ListWidget->currentIndex().row();
	if (iRow + 1 >= ui.ListWidget->count())
	{
		ui.ListWidget->setCurrentRow(0);
	}
	else
	{
		ui.ListWidget->setCurrentRow(iRow + 1);
	}
}

void VideoChoiceUI::initItem()
{
	QList<QString> MediaList= globalToolManager->GetDataManager()->GetMediaPlayFileList();

	Clear();
	m_bPaintItem = false;

	for (QString filename : MediaList){
		InsertFileList(filename, false);
		//strList += "/*/" + filename;
	}
	m_bPaintItem = true;

	FlashTips();
	repaint();

}

void VideoChoiceUI::Notice(const CSTDMapParam &mapParam)
{
	std::map<QString, QString>::const_iterator it = mapParam.begin();
	for (; it != mapParam.end(); ++it)
	{
		QString strKey = it->first;
		if (strKey.compare(INIT) == 0)
		{
			initItem();
		}
		else if (strKey.compare(DEALSTOPPLAY) == 0)
		{
			DealStopPlay();
		}
		else
		{
		}
	}
}

void VideoChoiceUI::CloseWdg()
{
   close();
	DealStopPlay();
	
	globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_LOGIN));
	if (m_qCycleTimer.isActive())
	{
		m_qCycleTimer.stop();
	}
	globalToolManager->GetClassSDK()->StopMediaFileCapture();//停止插播
	globalToolManager->GetClassSDK()->DesktopShareNotice(false, globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.join_id);
	if (mpCurPlayItem)
	{
		mpCurPlayItem->SetPlayState(VideoChoiceItem::ePlayState_Unplay);
	}
	SetTitle(QStringLiteral("当前播放：无"));
	mpCurPlayItem = NULL;
	QString strcurrDulation;
	QString strmaxDulation;

	QString dulationStr = dulation2Str(0, 0, strcurrDulation, strmaxDulation);
	UpdatePlayTime(strcurrDulation, strmaxDulation);
	SetProgressValue(0, 0);

	
   emit sig_close();
}

void VideoChoiceUI::OnProgressChanged(int nValue) {
	globalToolManager->GetClassSDK()->MediaFileSeek(nValue);
	ui.ProgressSlider->reActive();
}


void VideoChoiceUI::SetProgressValue(int nCurValue, int nMaxValue) {
	if (ui.ProgressSlider) {
		ui.ProgressSlider->setValue(nCurValue);
		ui.ProgressSlider->setMaxValue(nMaxValue);
	}
}


void VideoChoiceUI::slot_FileDialogChoose()
{
	if (nullptr == mFileDlg)
	{
		mFileDlg = new QFileDialog(this);
		mFileDlg->setMinimumSize(900, 600);
		mFileDlg->setNameFilter(STRSUPPORTFORMAT);

		mFileDlg->setFileMode(QFileDialog::ExistingFiles);
		connect(mFileDlg, &QFileDialog::filesSelected, this,  &VideoChoiceUI::ChoiceFiles);
	}

	if (nullptr != mFileDlg)
		mFileDlg->show();
}

void VideoChoiceUI::on_btnMinimize_clicked(bool checked /*= false*/)
{
	mbMin = true;
	showMinimized();
	//hide();
}

void VideoChoiceUI::on_btnClose_clicked(bool checked /*= false*/)
{
	//CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
	TipsWdg* tips = new TipsWdg(QStringLiteral("是否确认关闭插播?"), this, QStringLiteral("确定"), QStringLiteral("取消"));
	if (tips) {
		//CSTDMapParam param;
		//param[TipsMsg] = QStringLiteral("是否确认关闭插播?");
		//param[ShowCancelButton] = PARAM_TRUE;
		//tips->Notice(param);

		if (QDialog::Accepted == tips->exec())
		{
			CloseWdg();

		}
		else
		{

		}
	}
}

void VideoChoiceUI::on_btnCircleType_clicked(bool checked /*= false*/)
{
	if (CycleTyle_List == miCycleType)
		SetCircleMode(CycleTyle_Single);
	else
		SetCircleMode(CycleTyle_List);
}

void VideoChoiceUI::SetCircleMode(int mode) {
	miCycleType = mode;
	if (miCycleType== CycleTyle_List)
	{
		ui.btnCircleType->setText(CYCLETYLE_LIST);
	}
	else {
		ui.btnCircleType->setText(CYCLETYLE_SINGLE);
	}
}

int VideoChoiceUI::getCircleMode() {

   return miCycleType;
}

void VideoChoiceUI::SetTitle(const QString& strTitle)
{
	ui.labTitle->setText(strTitle);
}

bool VideoChoiceUI::Create() {

   return true;
}

void VideoChoiceUI::Destroy() {

}

void VideoChoiceUI::showEvent(QShowEvent *e) {
	QDialog::showEvent(e);
	ui.ListWidget->setFocus();
	//外部修改后  不会影响当前状态变化
	if (m_bChkBoxMuteSelf != ui.ChkBoxMuteSelf->isChecked())
		ui.ChkBoxMuteSelf->setChecked(m_bChkBoxMuteSelf);
		//on_ChkBoxMuteSelf_toggled(ui.ChkBoxMuteSelf->isChecked());
	if(m_bChkBoxMuteStudent!= ui.ChkBoxMuteStudent->isChecked())
		ui.ChkBoxMuteStudent->setChecked(m_bChkBoxMuteStudent);
		//on_ChkBoxMuteStudent_toggled(ui.ChkBoxMuteStudent->isChecked());

	CBaseWnd::showEvent(e);
}

bool VideoChoiceUI::IsFileInList(const QString &fullFilename) {

	for (int index = 0; index < ui.ListWidget->count(); index++) {
		QListWidgetItem *item = ui.ListWidget->item(index);
		if (item) {

			QString currFile;
			QListWidgetItem *item = ui.ListWidget->item(index);
			if (item) {
				QWidget *w = ui.ListWidget->itemWidget(item);
				if (w) {
					VideoChoiceItem *itemWidget = dynamic_cast<VideoChoiceItem *>(w);
					if (itemWidget) {
						currFile = itemWidget->getFilename();
					}
				}
			}
			if (currFile.isEmpty()) {
				continue;
			}

			if (currFile.toLower() == fullFilename.toLower()) {
				return true;
			}

			QFileInfo currFileInfo;
			currFileInfo.setFile(currFile);
			QFileInfo inputFileInfo;
			inputFileInfo.setFile(fullFilename);
			if (currFileInfo == inputFileInfo && currFileInfo.filePath() == inputFileInfo.filePath()) {
				return true;
			}

		}
	}
	return false;
};

bool VideoChoiceUI::InsertFileList(const QString &filename, const bool& bSave)
{
	if (IsFileInList(filename)) {
		return false;
	}

	QFileInfo mfi(filename);
	QString strFix = mfi.suffix().trimmed();
	
	if(!STRSUPPORTFORMAT.contains(strFix))
	{
		TipsWdg* tips = new TipsWdg(QStringLiteral("不支持选中文件"), this, QStringLiteral("知道了"), "");
		tips->exec();
		return false;
	}

	if (ui.ListWidget->count() >= MAXFILECOUNT) {
		TipsWdg* tips = new TipsWdg(QStringLiteral("最多可添加20个音视频文件"), this, QStringLiteral("知道了"), "");
		tips->exec();
		return false;
	}

	VideoChoiceItem *itemWidget = new VideoChoiceItem();
	if (itemWidget)
	{
		itemWidget->setFileName(filename);
		itemWidget->setMaximumWidth(miItemWidth);
		connect(itemWidget, SIGNAL(closeClicked(VideoChoiceItem *)), this, SLOT(DeleteItem(VideoChoiceItem *)));
		connect(itemWidget, &VideoChoiceItem::sig_paly, this, &VideoChoiceUI::slot_paly);

	}

	QListWidgetItem *item = new QListWidgetItem(ui.ListWidget);
	ui.ListWidget->addItem(item);
	item->setSizeHint(itemWidget->size());
	item->setFlags(Qt::NoItemFlags);
	item->setFlags(Qt::ItemIsEnabled);
	ui.ListWidget->setItemWidget(item, itemWidget);

	if (bSave)
	{
		globalToolManager->GetDataManager()->AddMediaPlayFile(filename);
	}

	return true;
}

void VideoChoiceUI::AppendFileList(const QStringList &fileList, int currentIndex, const bool& bSave/* = false*/) {
   //bool isMoreThanMaxFileCount = false;
   m_bPaintItem = false;

	//QString strList;
   //for_each(fileList.begin(), fileList.end(), [&](const QString &filename) {
   //   ;	
   //});

   QStringList::const_iterator it = fileList.begin();
   bool bContinue = true;
   for ( ; it!=fileList.end(); it++)
   {
	   if (!InsertFileList(*it, bSave))
	   {
		   break;
	   }
   }


   if (currentIndex >= 0 && currentIndex < ui.ListWidget->count()) {
      ui.ListWidget->setCurrentRow(currentIndex);
   }

   m_bPaintItem = true;
   FlashTips();
   repaint();
}

void VideoChoiceUI::PlayListDoubleClicked(const QModelIndex &index) {
   qDebug()<<"########################VideoChoiceUI::PlayListDoubleClicked####################";
   if (!ui.ListWidget) {
      return;
   }

   int row = index.row();
   if (row < 0 || row >= ui.ListWidget->count()) {
      qDebug()<<"########################VideoChoiceUI::PlayListDoubleClicked#################### row < 0 || row >= ui.ListWidget->count()";
      return;
   }

   ui.ListWidget->setCurrentRow(row);
   m_bPlayCurrentFile = true;
   if (!m_qCycleTimer.isActive()) {
	   m_qCycleTimer.start(400);
   }
}

void VideoChoiceUI::StartMeadiePlayTime()
{
   if (!m_qCycleTimer.isActive()) {
      m_qCycleTimer.start(400);
   }
}

void VideoChoiceUI::slot_paly(VideoChoiceItem *itemWidget, const int& iState) {
	if (!itemWidget) {
		return;
	}

	if (VideoChoiceItem::ePlayState_suspend == iState)
	{
		on_btnPlay_clicked();
	}
	else
	{
		if(m_bChkBoxMuteSelf!=ui.ChkBoxMuteSelf->isChecked())
		{
			m_bChkBoxMuteSelf = (!m_bChkBoxMuteSelf);
			emit sigMuteSelf(m_bChkBoxMuteSelf);
		}

		if(m_bChkBoxMuteStudent!=ui.ChkBoxMuteStudent->isChecked())
		{
			m_bChkBoxMuteStudent = (!m_bChkBoxMuteStudent);
			if (m_bChkBoxMuteStudent)
			{
				globalToolManager->GetClassSDK()->OperateAllUserMic(1);
			}
			else
			{
				globalToolManager->GetClassSDK()->OperateAllUserMic(0);
			}
		}

		//添加静音设置
		SetPlayState(itemWidget);
		m_bPlayCurrentFile = true;
		if (!m_qCycleTimer.isActive()) {
			m_qCycleTimer.start(400);
		}
	}

}

bool VideoChoiceUI::GetCurFile(QString& selectFile) {
	bool bRef = false;
	QListWidgetItem* item = ui.ListWidget->currentItem();

	if (NULL != mpCurPlayItem)
	{
		mpCurPlayItem->SetPlayState(VideoChoiceItem::ePlayState_Unplay);
		mpCurPlayItem = NULL;
	}

	if (item)
	{
		QWidget *w = ui.ListWidget->itemWidget(item);
		if (w) {
			VideoChoiceItem *videoWidget = dynamic_cast<VideoChoiceItem *>(w);
			QString qStrFileName = videoWidget->getFilename();

			SetTitle(QStringLiteral("当前播放：")+ qStrFileName);
			selectFile = qStrFileName.replace("/", "\\");

			mpCurPlayItem = videoWidget;
			mpCurPlayItem->SetPlayState(VideoChoiceItem::ePlayState_Playing);
			bRef = true;
		}
	}

	
	return bRef;
}

void VideoChoiceUI::SetPlayState(VideoChoiceItem *itemWidget)
{
	for (int i = 0; i < ui.ListWidget->count(); i++) {
		QListWidgetItem *item = ui.ListWidget->item(i);
		if (item) {
			QWidget* tempw = ui.ListWidget->itemWidget(item);
			VideoChoiceItem *videoWidget = dynamic_cast<VideoChoiceItem *>(tempw);
			if (tempw == itemWidget) {
				ui.ListWidget->setCurrentItem(item);
				//videoWidget->SetPlayState(VideoChoiceItem::ePlayState_Playing);
				break;
			}
			else {
				//videoWidget->SetPlayState(VideoChoiceItem::ePlayState_Unplay);
			}
		}
	}
}

void VideoChoiceUI::ChoiceFiles(const QStringList &fileNameList) {
   if (!ui.ListWidget) {
      return;
   }
   if (fileNameList.isEmpty()) {
      return;
   }
   int width = 0;
   int height = 0;
   for (int i = 0; i < fileNameList.size(); i++) {
      QString fileName = fileNameList.at(i);
      if (!fileName.contains(".mp3") && !fileName.contains(".MP3") && !fileName.contains(".WAV") && !fileName.contains(".wav")) {
         //VH::CComPtr<IOBSControlLogic> pObsControlLogic = NULL;
         //DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_IOBSControl, IID_IOBSControlLogic, pObsControlLogic, return, ASSERT(FALSE));
         //if (pObsControlLogic) {
         //   pObsControlLogic->GetMediaFileWidthAndHeight(fileName.toStdString().c_str(), width, height);
         //}
         //产品确定最大支持视频分辨率为1080P;
       /*  if (mLiveType == eLiveType_Live && width > 1920 && height > 1080) {
			ui.tipLab->setText(QString::fromWCharArray(L"插播视频分辨率最大支持1080P"));
			return;
         }
		 else */if (/*mLiveType == eLiveType_VhallActive && */width > 1280 && height > 720)
		 {
			 ui.tipLab->setText(QString::fromWCharArray(L"插播视频分辨率最大支持720"));
			 return;
		 }
      }
   }

   int lastFileNameListCount = ui.ListWidget->count();
   AppendFileList(fileNameList, -1, true);
   if (ui.ListWidget->count() > lastFileNameListCount) {
      ui.ListWidget->setCurrentRow(lastFileNameListCount);
   }
}

void VideoChoiceUI::currentItemChanged(QListWidgetItem* it1, QListWidgetItem* it2) {
   for (int i = 0; i < ui.ListWidget->count(); i++) {
      QListWidgetItem *item = ui.ListWidget->item(i);
      if (item) {
         QWidget *w = ui.ListWidget->itemWidget(item);
         if (w) {
            VideoChoiceItem *videoWidget = dynamic_cast<VideoChoiceItem *>(w);
            if (videoWidget) {
               videoWidget->setChecked(i == ui.ListWidget->currentRow());
            }
         }
      }
   }

}

void VideoChoiceUI::DeleteItem(VideoChoiceItem *itemWidget) {
   if (!ui.ListWidget) {
      return;
   }

   if (!itemWidget) {
      return;
   }

   for (int i = 0; i < ui.ListWidget->count(); i++) {
      QListWidgetItem *item = ui.ListWidget->item(i);
      if (item) {
		  QWidget* w = ui.ListWidget->itemWidget(item);
		  if (w) {
			  VideoChoiceItem *videoWidget = dynamic_cast<VideoChoiceItem *>(w);
			  QString selectFile = videoWidget->getFilename();
			  globalToolManager->GetDataManager()->RemoveMediaPlayFile(selectFile);
		  }

         if (ui.ListWidget->itemWidget(item) == itemWidget) {

            ui.ListWidget->removeItemWidget(item);
            delete item;
            break;
         }
      }
   }

   FlashTips();
   ui.ListWidget->setFocus();
}

void VideoChoiceUI::Clear() {
   if (!ui.ListWidget) {
      return;
   }
   mpCurPlayItem = NULL;
   //ui.ListWidget->clear();
   while (ui.ListWidget->count() > 0) {
      QListWidgetItem* item = ui.ListWidget->item(0);
      if (NULL != item) {
         QWidget* w = ui.ListWidget->itemWidget(item);
         if (NULL != item) {
            VideoChoiceItem* itemWidget = dynamic_cast<VideoChoiceItem*>(w);

            item = ui.ListWidget->takeItem(0);

            delete itemWidget;
            itemWidget = NULL;

            delete item;
            item = NULL;
         }
      }
   }

}

QStringList VideoChoiceUI::GetPlayList(int &currentIndex) {
   QStringList playList;
   if (ui.ListWidget) {
      for (int i = 0; i < ui.ListWidget->count(); i++) {
         QListWidgetItem *item = ui.ListWidget->item(i);
         if (item) {
            QWidget *w = ui.ListWidget->itemWidget(item);
            if (w) {
               VideoChoiceItem *itemWidget = dynamic_cast<VideoChoiceItem *>(w);
               if (itemWidget) {
                  QString fileName = itemWidget->getFilename();
                  playList.append(fileName);
               }
            }
         }
      }

      currentIndex = ui.ListWidget->currentRow();
   }

   return playList;
}
void VideoChoiceUI::CenterWindow(QWidget* parent) {
   int x = 0;
   int y = 0;
   if (NULL == parent) {
      const QRect rect = QApplication::desktop()->availableGeometry();
      x = rect.left() + (rect.width()-width())/2;
      y = rect.top() + (rect.height()-height())/2;
   } else {
      QPoint point(0, 0);
      point = parent->mapToGlobal(point);
      x = point.x() + (parent->width()-width())/2;
      y = point.y() + (parent->height()-height())/2;
   }
   move(x, y);
}

void VideoChoiceUI::FlashTips() {

   if (!ui.ListWidget) {
      return;
   }

   if (ui.ListWidget->count() == 0) {
      ui.tipLab->setText(QString::fromWCharArray(L"请添加要插播的视频"));
   } else if (ui.ListWidget->count() >= MAXFILECOUNT) {
      ui.tipLab->setText(QString::fromWCharArray(L"最多添加20个视频"));
   } else {
      ui.tipLab->setText("");
   }
}
void VideoChoiceUI::FlushItems() {
   if (!m_bPaintItem) {
      return;
   }
   if (ui.ListWidget) {
      for (int i = 0; i < ui.ListWidget->count(); i++) {
         QListWidgetItem *item = ui.ListWidget->item(i);
         if (item) {
            QWidget *w = ui.ListWidget->itemWidget(item);
            if (w) {
               VideoChoiceItem *itemWidget = dynamic_cast<VideoChoiceItem *>(w);
               if (itemWidget) {
                  itemWidget->setEndLine(i != (ui.ListWidget->count()-1));
               }
            }
         }
      }
   }

}
void VideoChoiceUI::FLushFileList(bool accept) {//保存当前加载视频列表
   int currentIndex;
   QStringList fileList = GetPlayList(currentIndex);
   if(!accept) {
      currentIndex = -1;
   }
   emit this->SigSyncFileList(fileList, currentIndex);
}

bool VideoChoiceUI::eventFilter(QObject *obj, QEvent *ev) {
   if (ev) {
      if (ev->type() == QEvent::Enter) {
         if (obj == ui.FileTypeTips ) {
            if (nullptr == m_pNoiseTips) {
               m_pNoiseTips = new NoiseTooltip(this);
            }

            m_pNoiseTips->show();
            QPoint pos = this->mapToGlobal(ui.FileTypeTips->pos());
            m_pNoiseTips->move(pos + QPoint(32, 22-(m_pNoiseTips->height()/2) ));
            //m_pNoiseTips->setFixedHeight(125);

         }
      }
      else if (ev->type() == QEvent::Leave) {
         if (obj == ui.FileTypeTips && m_pNoiseTips != NULL && !m_pNoiseTips->isHidden()) {
            m_pNoiseTips->close();
         }
      }
   }
   return QWidget::eventFilter(obj,ev);
}

QString VideoChoiceUI::dulation2Str(long long currDulation, long long maxDulation,
	QString& strcurrDulation, QString& strmaxDulation) {
	QString dulationStr = "";
	if (currDulation > maxDulation) {
		currDulation = maxDulation;
	}

	dulationStr = dulation2StrPri(currDulation) + "/" + dulation2StrPri(maxDulation);
	strcurrDulation = dulation2StrPri(currDulation);
	strmaxDulation = dulation2StrPri(maxDulation);

	return dulationStr;
}

//时间戳转换
QString VideoChoiceUI::dulation2StrPri(long long dulation) {
	dulation /= 1000;
	int H = (dulation/3600) % 60;
	int M = (dulation/60) % 60;
	int S = dulation % 60;

	QString sH = QString::number(H);
	if (H < 10) {
		sH = "0" + sH;
	}
	QString sM = QString::number(M);
	if (M < 10) {
		sM = "0" + sM;
	}
	QString sS = QString::number(S);
	if (S < 10) {
		sS = "0" + sS;
	}
	return QString("%1:%2:%3").arg(sH, sM, sS);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

VideoChoiceItem::VideoChoiceItem(QWidget *parent) :
	CWidget(parent) {
	ui.setupUi(this);
	this->setFixedHeight(40);

	ui.btnPlay->setText(QStringLiteral("播放"));
	ui.btnPlay->setMoveStr(QStringLiteral("暂停"));
	ui.btnDel->setEnabled(true);
	ui.btnPlay->SetImgPath(":/common/img/playgif.gif", 22, 15);
	connect(ui.btnDel, SIGNAL(clicked()), this, SLOT(closeBtnClicked()));
	connect(ui.btnPlay, &QPushButton::clicked, this, &VideoChoiceItem::slot_Play);
	ui.btnDel->setStyleSheet(UNPLAYQSS);
}

VideoChoiceItem::~VideoChoiceItem() {
}

void VideoChoiceItem::slot_Play()
{
	if (ui.btnPlay &&	(ePlayState_Unplay==miPlayState  || ePlayState_suspend == miPlayState))
	{
		emit sig_paly(this, miPlayState);//
	}

}

void VideoChoiceItem::SetPlayState(int iState)
{
	miPlayState = iState;
	if (ePlayState_Unplay == iState && NULL!=ui.btnPlay)
	{
		ui.btnPlay->setStyleSheet(UNPLAYQSS);
		ui.btnPlay->setText(QStringLiteral("播放"));
		ui.btnPlay->setToolTip(QStringLiteral("播放"));
		ui.btnDel->setEnabled(true);
		//ui.btnPlay->setFixedSize(ui.btnDel->size());
	}
	else if(ePlayState_Playing == iState && NULL != ui.btnPlay)
	{
		ui.btnPlay->setStyleSheet(PLAYINGQSS);
		ui.btnDel->setEnabled(false);
		ui.btnPlay->setToolTip(PLAYING);
		ui.btnPlay->startMove();
	}
	else if (NULL != ui.btnPlay)
	{
		ui.btnPlay->setStyleSheet(PLAYINGQSS);
		ui.btnDel->setEnabled(false);
		ui.btnPlay->setPaused();

	}
}


void VideoChoiceItem::closeBtnClicked() {
	emit this->closeClicked(this);
}

void VideoChoiceItem::setFileName(const QString &text) {
	m_strFilePath = text;
	if (!ui.labFile) {
		return;
	}

	QFileInfo textInfo;
	textInfo.setFile(text);
	QString tStr = textInfo.fileName();
	//QString str = ui.labFile->fontMetrics().elidedText(tStr, Qt::ElideRight, 360);
	ui.labFile->setText(tStr);
	this->setToolTip(tStr);
}

QString VideoChoiceItem::getFilename() {
	return m_strFilePath;
}

void VideoChoiceItem::setEndLine(bool ok) {
	m_bDrawLine = ok;
}

void VideoChoiceItem::FlushLabelColor(bool pressed) {
	if (pressed) {
	}
	else {
		if (m_bChecked) {//播放中进入后 鼠标离开出发
			ui.labFile->setStyleSheet(
				"QLabel{color:rgb(82,204,144);font: 14px \"微软雅黑\";}"
			);
		}
		else {
			ui.labFile->setStyleSheet(//未播放鼠标离开
				"QLabel{color:rgb(68,68,68);font: 14px \"微软雅黑\";}"
				"QLabel:hover{color:rgb(102,102,102);}"
			);
		}
	}
}

void VideoChoiceItem::setChecked(bool ok) {
	m_bChecked = ok;
	FlushLabelColor(false);
}
void VideoChoiceItem::leaveEvent(QEvent *e) {
	QWidget::leaveEvent(e);
	FlushLabelColor(false);
}

void VideoChoiceItem::mouseReleaseEvent(QMouseEvent *e) {
	QWidget::mouseReleaseEvent(e);
	FlushLabelColor(false);
}

void VideoChoiceItem::mousePressEvent(QMouseEvent *e) {
	QWidget::mousePressEvent(e);
	FlushLabelColor(true);
}

void VideoChoiceItem::setWidth(int width) {
	this->setFixedWidth(width);
}
