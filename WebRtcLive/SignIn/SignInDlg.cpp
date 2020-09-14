#include "SignInDlg.h"
#include <QListWidgetItem> 
#include <QTimer>
#include "../Unility/Unility.h"

#include "TipsWdg.h"

extern ToolManager *globalToolManager;

#define STRCOUNTDOWN(x)  QStringLiteral("正在统计 : <font color=#FF3838>")  \
	+	QString::number(x) \
	+ QStringLiteral("</font>秒…")
#define STRSTATICS(x)  QStringLiteral("当前签到人数 : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>人")
					
#define TATAL(x)  QStringLiteral("在线人数 : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>人")

#define SIGNIN(x)  QStringLiteral("签到人数 : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>人")

#define SIGNINLV(x)  QStringLiteral("签到率 : <font color=#52CC90>") \
	+ QString::number(x) +  QStringLiteral(" </font>%")

#define BTN_STSRT_SIGN   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(82, 204, 144);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}\
                                         QPushButton:hover{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(66, 198, 97);\
                                         font: 10pt \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}")  
#define BTN_STSRT_SIGN_ENADLE   QStringLiteral("QPushButton{\
                                         border-top-left-radius : 15px;\
                                         border-top-right-radius : 15px;\
                                         border-bottom-left-radius : 15px;\
                                         border-bottom-right-radius : 15px;\
                                         background-color: rgb(170,230,200);\
                                         font: 10pt  \"微软雅黑\";\
                                         color: rgb(255, 255, 255);}") 
                                        
CREATE_WND_FUNCTION(SignInDlg);
SignInDlg::SignInDlg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
	QStringList list;
	addItem(SignInListItem::eSignInType::eSignInType_SignIn, list);
	addItem(SignInListItem::eSignInType::eSignInType_UnSign, list);
	//addItem(SignInListItem::eSignInType::eSignInType_NotPresent, list);
	ui.btnSignInRecord->setFocusPolicy(Qt::NoFocus);
	ui.btnSignIn->setFocusPolicy(Qt::NoFocus);
	ui.btnStartSignIn->setFocusPolicy(Qt::NoFocus);

	ui.labStatistics->setText(STRSTATICS(0));

	ui.btnClose->setFocusPolicy(Qt::NoFocus);
	ui.listWidget->setFocusPolicy(Qt::NoFocus);
	ui.listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.btnStartSignIn->setStyleSheet(BTN_STSRT_SIGN);

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();
   ui.widget->setFixedSize(482, 418);
   setFixedSize(484, 420);
}

SignInDlg::~SignInDlg()
{
	for (int i = ui.listWidget->count()-1; i >0 ; i--) {
		QListWidgetItem *item = ui.listWidget->item(i);
		if (item) {
			QWidget* tempw = ui.listWidget->itemWidget(item);
			SignInListItem *videoWidget = dynamic_cast<SignInListItem *>(tempw);
			item = ui.listWidget->takeItem(i);
			delete videoWidget;
			videoWidget = NULL;

			delete item;
			item = NULL;
		}
	}
	
	ui.listWidget->clear();
}

void SignInDlg::Notice(const CSTDMapParam &mapParam) {

}

void SignInDlg::Notice(const CSTDMapParamPtr &) {

}

void SignInDlg::SetStatistics(const int& iStatistics)
{
	ui.labStatistics->setText(STRSTATICS(iStatistics));
}

void SignInDlg::on_btnClose_clicked(bool checked/* = false*/)
{
	//StopTimer();
	TipsWdg* tips = new TipsWdg(this);
	if (tips) {
		CSTDMapParam param;
		param[TipsMsg] = QStringLiteral("是否结束签到?");
		param[ShowCancelButton] = PARAM_TRUE;
		tips->Notice(param);
		int accept = tips->exec();
		if (accept == QDialog::Accepted) {
			emit sig_close();
			if (nullptr != mpSignInTimer && mpSignInTimer->isActive())			//取消签到
			{
				globalToolManager->GetClassSDK()->SignStop(mStrSignInUi.toStdString());
			}
			StopTimer();
			hide();
			this->accept();
		}
	}

}

void SignInDlg::on_btnSignIn_clicked(bool checked /*= false*/)
{
	ui.stackedWidget->setCurrentIndex(0);
	ui.btnSignIn->setChecked(true);
	ui.btnSignInRecord->setChecked(false);
}

void SignInDlg::on_btnSignInRecord_clicked(bool checked /*= false*/)
{
	ui.btnSignInRecord->setChecked(true);
	ui.btnSignIn->setChecked(false);
	ui.stackedWidget->setCurrentIndex(1);
}

void SignInDlg::on_btnStartSignIn_clicked(bool checked /*= false*/)
{
	ui.btnStartSignIn->setEnabled(false);
	mSignInedCount = 0;
	//QTimer::singleShot(5000, this, SLOT(slot_EnabledSignIn()));
	//开始签到
	globalToolManager->GetClassSDK()->AddSign();
	clearList();
	ui.labStatistics->setText(STRSTATICS(0));
	ui.btnStartSignIn->setStyleSheet(BTN_STSRT_SIGN_ENADLE);
	ui.btnStartSignIn->setEnabled(false);
}

void SignInDlg::slot_EnabledSignIn()
{
	ui.btnStartSignIn->setEnabled(true);
}

void SignInDlg::startSignIn(const int& iTime, const QString& strSignInId)
{
	mCountDownTime = iTime;
	mStrSignInUi = strSignInId;
	//ui.labCountDown->setText( QString(STRCOUNTDOWN).arg(mCountDownTime) );
	ui.labCountDown->setText(STRCOUNTDOWN(mCountDownTime));
	if (nullptr == mpSignInTimer)
	{
		mpSignInTimer = new QTimer(this);
		connect(mpSignInTimer, &QTimer::timeout, this, &SignInDlg::slotSignInCountDown);
	}

	if (nullptr != mpSignInTimer)
	{
		mpSignInTimer->start(1000);
	}

	if (nullptr != mpSignInTimer && mpSignInTimer->isActive())
	{
		clearList();
	}

	mSignIn = true;
	globalToolManager->GetClassSDK()->GetSignRecord(mStrSignInUi.toStdString());
	
}

void SignInDlg::clearList()
{
	if (mSignIn) return;
	bool is_clear = false;

	for (int i = mListSignListItem.size() - 1; i >= 0; i--) {
		//QListWidgetItem *item = ui.listWidget->item(i);
		if (mListSignListItem.at(i)) {
			//QWidget* tempw = ui.listWidget->itemWidget(item);
			//SignInListItem *videoWidget = dynamic_cast<SignInListItem *>(ui.listWidget->itemWidget(item));
			//SignInListItem *videoWidget = static_cast<SignInListItem *>(ui.listWidget->itemWidget(item));
			mListSignListItem.at(i)->clearData();
			//ui.listWidget->removeItemWidget(dynamic_cast<QListWidgetItem *>(mListSignListItem.at(i)));
			mListSignListItem.at(i)->setFixedHeight(80);
		
		}
	}
	ui.listWidget->item(0)->setSizeHint(QSize(ui.listWidget->width()-10, 80));
	ui.listWidget->item(1)->setSizeHint(QSize(ui.listWidget->width()-10, 80));
	if (mTotalList.size()>0)
	{
		mTotalList.clear();
	}

}

void SignInDlg::AddonlineCount(const int& membercount)
{
	mTotal = membercount;
	globalToolManager->GetDataManager()->WriteLog("%s mTotal %d", __FUNCTION__, mTotal);
	ui.label_tatal->setText(TATAL(mTotal));
}

void SignInDlg::SetSignInfo(int total, int SignCount)
{
	if (mCountDownTime > 0)
	{
		mTotal = total;
		globalToolManager->GetDataManager()->WriteLog("%s mTotal %d", __FUNCTION__, mTotal);
	}
	globalToolManager->GetDataManager()->WriteLog("%s total %d", __FUNCTION__, total);
	globalToolManager->GetDataManager()->WriteLog("%s SignCount %d", __FUNCTION__, SignCount);
	ui.label_tatal->setText(TATAL(total));
    ui.label_sigin->setText(SIGNIN(SignCount));
	double sign = (double)SignCount / (double)total * 100;
	if (SignCount == 0)
	{
		sign = 0;
	}
	ui.label_startTimeCount->setText(SIGNINLV(sign));
}

void SignInDlg::addSignUsers(const SignInListItem::eSignInType& etype, std::list<std::string> signUsers)
{
	//if (etype == SignInListItem::eSignInType_SignIn && signUsers.size() < 0 && mCountDownTime > 0) return;
	//if (etype == SignInListItem::eSignInType_UnSign && signUsers.size() == mTotal && mCountDownTime > 0) return;
	if (mSignIn) return;
	QListWidgetItem* pItem = NULL;
	SignInListItem* pWdg = NULL;
	if (mMapListItem.contains(etype) && NULL != mMapListItem[etype]) {
		pWdg = mMapListItem[etype];
		pItem = mMapListWidgetItem[etype];
	}
	else
	{
		pItem = new QListWidgetItem(ui.listWidget);
		if (NULL != pItem)
		{
			pWdg = new SignInListItem(ui.listWidget);
			mListSignListItem << pWdg;
			if (NULL != pWdg)
			{
				ui.listWidget->addItem(pItem);
				pWdg->setTitle(etype);
				mMapListItem[etype] = pWdg;
				mMapListWidgetItem[etype] = pItem;
				ui.listWidget->setItemWidget(pItem, pWdg);
			}
		}
	}

	if (NULL != pWdg)
	{
		
		for (auto it = signUsers.begin(); it!=signUsers.end(); it++)
		{
			QString strNames = QString::fromStdString(*it);
			pWdg->addStudent(strNames);
		}

		if (NULL != pItem)
		{
			pItem->setSizeHint(QSize(0, pWdg->height()));
		}
	}
}

void SignInDlg::SomeOneSignin(const QString& strSigner, const QString& strNick)
{
	mSignInedCount++;
	ui.labStatistics->setText(STRSTATICS(mSignInedCount));

	//刷新签到记录
	ui.label_tatal->setText(TATAL(mTotal));
	globalToolManager->GetDataManager()->WriteLog("%s mTotal %d", __FUNCTION__, mTotal);
	ui.label_sigin->setText(SIGNIN(mSignInedCount));
	if (mTotal != 0) {
		double sign = (double)mSignInedCount / (double)mTotal * 100;
		ui.label_startTimeCount->setText(SIGNINLV(sign));
	}
	else {
		ui.label_startTimeCount->setText(SIGNINLV(0));
	}

	mTotalList.clear();
	mTotalList.push_back(strNick.toStdString());
	if (mSignIn)
	{
		mSignIn = false;
	}
	addSignUsers(SignInListItem::eSignInType_SignIn, mTotalList);
}

void SignInDlg::StopTimer()
{
	if (nullptr != mpSignInTimer && mpSignInTimer->isActive())
	{
		mpSignInTimer->stop();
		ui.btnStartSignIn->setEnabled(true);
		ui.btnStartSignIn->setText(QStringLiteral("重新签到"));
		ui.labCountDown->setText(QStringLiteral("<font color=#FF3838>签到结束！</font>"));
		ui.btnStartSignIn->setStyleSheet(BTN_STSRT_SIGN);
		ui.btnStartSignIn->setEnabled(true);
	}
}

void SignInDlg::slotSignInCountDown()
{
	--mCountDownTime;
	if (mCountDownTime <= 0)
	{
		StopTimer();
		mSignIn = false;
		//请求签到结果
		globalToolManager->GetClassSDK()->GetSignRecord(mStrSignInUi.toStdString());
	}
	else
	{
		ui.labCountDown->setText( STRCOUNTDOWN(mCountDownTime) );
	}
}

void SignInDlg::addItem(const SignInListItem::eSignInType& etype, const QStringList& strList)
{
	QListWidgetItem* pItem = NULL;
	SignInListItem* pWdg = NULL;
	if(mMapListItem.contains(etype) && NULL != mMapListItem[etype]){
		pWdg = mMapListItem[etype];
		pItem = mMapListWidgetItem[etype];
	}
	else
	{
		pItem = new QListWidgetItem(ui.listWidget);

		if (NULL != pItem)
		{
			pWdg = new SignInListItem(ui.listWidget);
			mListSignListItem << pWdg;
			if (NULL != pWdg)
			{
				pWdg->setTitle(etype);
				mMapListItem[etype] = pWdg;
				mMapListWidgetItem[etype] = pItem;
				ui.listWidget->setItemWidget(pItem, pWdg);				
			}
		}
	}	
	
	if (NULL != pWdg)
	{
		for (int i = 0; i < strList.size(); i++)
		{
			pWdg->addStudent(strList[i]);
		}

		if (NULL != pItem)
		{
			pItem->setSizeHint(QSize(0, pWdg->height()));
		}
	}
}

void SignInDlg::showEvent(QShowEvent* event)
{
	on_btnSignIn_clicked();
	CBaseWnd::showEvent(event);
}