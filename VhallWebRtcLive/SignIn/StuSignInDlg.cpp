#include "StuSignInDlg.h"
#include <QListWidgetItem> 
#include <QTimer>
#include "../Unility/Unility.h"

extern ToolManager *globalToolManager;

#define STRCOUNTDOWN(x)  QStringLiteral("您有 <font color=#FF0000>")  \
	+	QString::number(x) \
	+ QStringLiteral("</font>秒的时间进行签到")
							 
CREATE_WND_FUNCTION(StuSignInDlg);
StuSignInDlg::StuSignInDlg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
	setModal(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();
   ui.btnClose->hide();

   ui.widget->setFixedSize(438, 310);
   setFixedSize(440, 312);
}

StuSignInDlg::~StuSignInDlg()
{

	if (mpSignInTimer)
	{
		if (mpSignInTimer->isActive())
			mpSignInTimer->stop();
		delete mpSignInTimer;
		mpSignInTimer = nullptr;
	}
}

void StuSignInDlg::Notice(const CSTDMapParam &mapParam) {

}

void StuSignInDlg::Notice(const CSTDMapParamPtr &) {

}

void StuSignInDlg::ShowExec(const int& iTime, const QString& strUid)
{
	ui.btnStartSignIn->setEnabled(true);
	mCountDownTime = iTime;
	mStrUid = strUid;
	//ui.labCountDown->setText( QString(STRCOUNTDOWN).arg(mCountDownTime) );
	ui.labCountDown->setText(STRCOUNTDOWN(mCountDownTime));
	if (nullptr == mpSignInTimer)
	{
		mpSignInTimer = new QTimer(this);
		connect(mpSignInTimer, &QTimer::timeout, this, &StuSignInDlg::slotSignInCountDown);
	}

	if (nullptr != mpSignInTimer)
	{
		mpSignInTimer->start(1000);
	}
	//开始签到

	exec();
}

void StuSignInDlg::CloseExec()
{
	StopTimer();
}

void StuSignInDlg::on_btnClose_clicked(bool checked/* = false*/)
{
	StopTimer(); 
}


void StuSignInDlg::on_btnStartSignIn_clicked(bool checked /*= false*/)
{
	globalToolManager->GetClassSDK()->SignIn(mStrUid.toStdString());
	StopTimer();
	QTimer::singleShot(5000, this, SLOT( slot_EndSignIn()));

}

void StuSignInDlg::slot_EndSignIn()
{
	hide();
	close();
	accept();
}

void StuSignInDlg::StopTimer()
{
	if (nullptr != mpSignInTimer && mpSignInTimer->isActive())
	{
		mpSignInTimer->stop();
		ui.btnStartSignIn->setEnabled(true);
	}
	close();
	accept();
}

void StuSignInDlg::slotSignInCountDown()
{
	--mCountDownTime;
	if (mCountDownTime <= 0)
	{
		StopTimer();
	}
	else
	{
		ui.labCountDown->setText( STRCOUNTDOWN(mCountDownTime) );
	}
}