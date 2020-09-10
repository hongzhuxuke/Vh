#include "CTimeButton.h"
#include "../Unility/Unility.h"
#include "priConst.h"

extern ToolManager *globalToolManager;

CTimeButton::CTimeButton(QWidget *parent /*= Q_NULLPTR*/)
	:QPushButton(parent)
{

}

CTimeButton::~CTimeButton()
{

}

void CTimeButton::setHoverDisplayText(const QString& strText)
{
	mstrDisplayText = strText;
    if (mstrDisplayText.isEmpty())
    {
        setText("00:00:00");
        setEnabled(false);
    }
    else {
        setText(strText);
        setEnabled(true);
    }
}

void CTimeButton::enterEvent(QEvent *event)
{
	mbMouseHoverOn = true;
    if (!mstrDisplayText.isEmpty()) {
        setText(mstrDisplayText);
    }
	QPushButton::enterEvent(event);
}

void CTimeButton::leaveEvent(QEvent *event)
{
	mbMouseHoverOn = false;
    if (eClassType_Classing == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type)//上课中
    {
        dispalyTime();
    }
	QPushButton::leaveEvent(event);
}

void CTimeButton::startClassTime()
{
	if (eClassType_Classing == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.type)//上课中
	{
		miClassingTime = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mWebinarInfo.real_duration;//上课时长
		if (nullptr == mpClassTime)
		{
			mpClassTime = new QTimer(this);
			connect(mpClassTime, &QTimer::timeout, this, &CTimeButton::slot_UpdateClassTime);
		}

		if (nullptr != mpClassTime)
			mpClassTime->start(1000);
	}
}

void CTimeButton::stopClassTime()
{
	if (nullptr != mpClassTime && mpClassTime->isActive())
		mpClassTime->stop();
	setText(mstrDisplayText);
}

void CTimeButton::dispalyTime()
{
	if (!mbMouseHoverOn || mstrDisplayText.trimmed().isEmpty())
	{
		int iSecond = miClassingTime % 60;
		int iTemp = miClassingTime/60;
		int iBranch = iTemp % 60;
		int iHour = iTemp/60;

		setText(QString("%1:%2:%3").arg(iHour).arg(iBranch, 2, 10, QLatin1Char('0')).arg(iSecond, 2, 10, QLatin1Char('0')));
	}
}

void CTimeButton::slot_UpdateClassTime()
{
	miClassingTime++;
	dispalyTime();
}

long CTimeButton::GetClassTime() {
    return miClassingTime;
}


//////////////////////////////////////////////////////////////////////////////

CCancelCountDownBtn::CCancelCountDownBtn(QWidget *parent /*= Q_NULLPTR*/)
	:QPushButton(parent)
{
	connect(this, &QPushButton::clicked, this, &CCancelCountDownBtn::slotclicked);
}

CCancelCountDownBtn::~CCancelCountDownBtn() {
	if (nullptr != mpCountDownTimer)
	{
		if (mpCountDownTimer->isActive())
		{
			mpCountDownTimer->stop();
		}
		delete mpCountDownTimer;
		mpCountDownTimer = nullptr;
	}
}

void CCancelCountDownBtn::SetNormalText(const QString& str) {
	if(nullptr!=mpCountDownTimer && mpCountDownTimer->isActive())
		mpCountDownTimer->stop();
	mstrNormal = str;
	setText(mstrNormal);
}

void CCancelCountDownBtn::SetCountDownText(const QString& str) {
	mstrCountDown = str;
}

void CCancelCountDownBtn::SetCountDownTime(const int& iTime) {
	miInitCount = iTime;
}

void CCancelCountDownBtn::StartTimer()
{
	if (nullptr == mpCountDownTimer)
	{
		mpCountDownTimer = new QTimer(this);
		connect(mpCountDownTimer, &QTimer::timeout, this, &CCancelCountDownBtn::slotCountDonw);
	}

	if (nullptr != mpCountDownTimer)
	{
		miCountDown = miInitCount;
		mpCountDownTimer->start(1000);
	}
}

void CCancelCountDownBtn::slotCountDonw()
{
	miCountDown--;
	if (miCountDown == 0)
	{
		mpCountDownTimer->stop();
		setText(mstrNormal);
	}
	else
	{
		setText(mstrCountDown + "(" + QString::number(miCountDown) + "s)");
	}
}

void CCancelCountDownBtn::slotclicked()
{
	mbCountDownState = (nullptr != mpCountDownTimer && mpCountDownTimer->isActive() && miCountDown > 0);
	emit sigCliecked(mbCountDownState);
	if(!mbCountDownState && mstrNormal.compare(this->text()) == 0)
		StartCountDown(mbCountDownState);
}

void  CCancelCountDownBtn::StopCountDown(){
	if (nullptr != mpCountDownTimer && mpCountDownTimer->isActive())
	{
		mpCountDownTimer->stop();
	}
}
void CCancelCountDownBtn::StartCountDown(const bool& bCountDown) {
	mbCountDownState = bCountDown;
	if (mbCountDownState)
	{
		if (nullptr != mpCountDownTimer && mpCountDownTimer->isActive())
		{
			mpCountDownTimer->stop();
		}
		setText(mstrNormal);
		setToolTip(mstrNormal);
	}
	else
	{
		setToolTip(mstrCountDown);
		setText(mstrCountDown + "(" + QString::number(miInitCount) + "s)");
		StartTimer();
	}
}

