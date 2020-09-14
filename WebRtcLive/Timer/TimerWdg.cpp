#include "TimerWdg.h"
#include "../Unility/CustomEventType.h"
#include "../Unility/Unility.h"
#include <QtMultimedia/QMediaPlayer>
#include <QDir> 
#include "TipsWdg.h"

#define QStr_StartRecord    QString::fromWCharArray(L"开始")
#define QStr_StopRecord    QString::fromWCharArray(L"取消计时")
#define QStr_ResetRecord    QString::fromWCharArray(L"重新开始")

//#define RIGHTRADIUSQSS "#widget\
//{\
//background-color: rgb(82, 204, 144);\
//border-top-right-radius : 4px;\
//}"
//
//#define RADIUSQSSQSS "#widget\
//{\
//background-color: rgb(82, 204, 144);\
//border-top-right-radius : 4px;\
//border-top-left-radius : 4px;\
//}"

extern ToolManager *globalToolManager;
CREATE_WND_FUNCTION(TimerWdg);
TimerWdg::TimerWdg(QWidget *parent)
    : CBaseWnd(parent)
    , mbIsTeacher(false)
    , mpRecordTimerPtr(nullptr)
{
    ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
	ui.pushButton_close->setFocusPolicy(Qt::NoFocus);
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_ClickedClose()));
    connect(ui.pushButton_start, SIGNAL(clicked()), this, SLOT(slot_ClickedStart()));
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    mpRecordTimerPtr = new QTimer(this);
    if (mpRecordTimerPtr) {
        connect(mpRecordTimerPtr, SIGNAL(timeout()), this, SLOT(slot_RecordTimeout()));
    }
    ui.widget_minLeft->SetMaxSize(5);
    ui.widget_minRight->SetMaxSize(9);
    ui.widget_SecondLeft->SetMaxSize(5);
    ui.widget_SecondRight->SetMaxSize(9);


    //ui.label_4->hide();
    //ui.label_5->hide();
    //ui.label_6->hide();
    //ui.label_7->hide();
}

TimerWdg::~TimerWdg()
{
	if (nullptr != mpPlayer)
	{
		mpPlayer->stop();
		delete mpPlayer;
		mpPlayer = nullptr;
	}
}

void TimerWdg::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find(Str_Function);
    if (iter != mapParam.end() && iter->second == Str_Function_AnswerToolTeacher) {
        mbIsTeacher = true;
    }
    else if (iter != mapParam.end() && iter->second == Str_Function_AnswerToolStudent) {
        mbIsTeacher = false;
    }
}

void TimerWdg::Notice(const CSTDMapParamPtr &) {

}

void TimerWdg::slot_ClickedClose() {
    //CBaseWnd* tips = globalToolManager->GetWndManager()->FindWnd(WND_ID_TIPS_WND);
	TipsWdg* tips = new TipsWdg(this);
    if (tips) {
        CSTDMapParam param;
        param[TipsMsg] = QStringLiteral("是否确认关闭计时器?");
        param[ShowCancelButton] = PARAM_TRUE;
        tips->Notice(param);
        int accept = tips->exec();
        if (accept == QDialog::Accepted) {
            if (mpRecordTimerPtr) {
                mpRecordTimerPtr->stop();
            }
			if (accept == QDialog::Accepted) {
				globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_TIMER_TOOL));
			}
			requestStopTimer();//取消倒计时
			//close();
			//hide();
        }
    }
}
void TimerWdg::requestStartTimer()
{
	globalToolManager->GetClassSDK()->StartCoutDownTool(miTimeCount, ui.checkBox->isChecked()?1:0);
}

void TimerWdg::requestStopTimer()
{
	mbIsPlayEndSound = false;
	globalToolManager->GetClassSDK()->CancleCoutDownTool();
}

void TimerWdg::slot_ClickedStart() {//点击开始计时
	int iMinLeft = ui.widget_minLeft->GetNum();//十位  分钟数
	int iMinRight = ui.widget_minRight->GetNum();//个位分钟数

	int iSecondLeft = ui.widget_SecondLeft->GetNum();//十位秒数
	int iSecondRight = ui.widget_SecondRight->GetNum();//个位秒数

	miTimeCount = iSecondRight + iSecondLeft * 10 + iMinRight * 60 + iMinLeft * 10 * 60;
	if (mpRecordTimerPtr) {
		mpRecordTimerPtr->stop();
	}

    QString text = ui.pushButton_start->text();
    if (text == QStr_StartRecord || text == QStr_ResetRecord) { //开始/重新开始
		if (miTimeCount <= 0) return;
		enableEdit(false);
        ui.pushButton_start->setText(QStr_StopRecord);
		//请求开始倒计时
		requestStartTimer();
		mbClicked = true;
		QTimer::singleShot(5000, this, SLOT(slot_initState()));
    }
    else if (text == QStr_StopRecord) {//取消倒计时
        ui.pushButton_start->setText(QStr_ResetRecord);
		if (nullptr != mpPlayer)
		{
			mpPlayer->stop();
		}

		//请求取消倒计时
		requestStopTimer();
		enableEdit(true);
    }
}

void TimerWdg::StartCountDown(const QString& strId, const int& miPlay, const int& iCount)
{
	mbClicked = false;
	int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
   globalToolManager->GetDataManager()->WriteLog("%s roleType:%d iCount: %d", __FUNCTION__, roleType, iCount);
	if (-1 == iCount)//取消倒计时
	{
      
		if (mpRecordTimerPtr) {
			mpRecordTimerPtr->stop();
		}

		
		if (JOIN_ROLE_Teacher == roleType)
		{
			SetState(QStr_StartRecord);
		}
		else
		{
			close();
			hide();
		}
	}
	else//开始倒计时
	{
		mbIsPlayEndSound = (ePlayEndSound_play == miPlay && iCount>3);
		globalToolManager->GetDataManager()->WriteLog("%s  PLAY mbIsPlayEndSound %d miPlay %d iCount %d", __FUNCTION__, mbIsPlayEndSound, miPlay, iCount);
		miTimeCount = iCount;
		calcDisplayTime();
		if (JOIN_ROLE_Teacher != roleType)
		{
			//ui.leftWdg->show();
			//ui.widget->setStyleSheet(RIGHTRADIUSQSS);
			ui.endWdg->hide();
			//ui.label_7->hide();
			showEdit(false);
			ui.pushButton_close->hide();
			ui.pushButton_start->hide();
			ui.checkBox->hide();
			ui.CountdownWdg->show();
			//setFixedSize(420, 270);
			show();
		}
		SetState(QStr_StopRecord);
	}
}

void  TimerWdg::showWdg(const int& roleType)
{
	if (JOIN_ROLE_Teacher == roleType)//讲师
	{
		//ui.leftWdg->show();
		//ui.widget->setStyleSheet(RIGHTRADIUSQSS);
		ui.endWdg->hide();
		//ui.label_7->hide();
		//setFixedSize(420, 350);
	}
	else
	{
		//setFixedSize(420, 270);
	}
	show();
}

void TimerWdg::enableEdit(const bool & enable /*= false*/)
{
	ui.widget_SecondRight->enableEdit(enable);
	ui.widget_SecondLeft->enableEdit(enable);
	ui.widget_minRight->enableEdit(enable);
	ui.widget_minLeft->enableEdit(enable);
	ui.checkBox->setEnabled(enable);
}

void	TimerWdg::showEdit(const bool & enable /*= false*/)
{
	if (enable)
	{
		ui.widget_SecondRight->showEdit(true);
		ui.widget_SecondLeft->showEdit(true);
		ui.widget_minRight->showEdit(true);
		ui.widget_minLeft->showEdit(true);
	}
	else
	{
		ui.widget_SecondRight->showEdit(false);
		ui.widget_SecondLeft->showEdit(false);
		ui.widget_minRight->showEdit(false);
		ui.widget_minLeft->showEdit(false);
	}
}

void TimerWdg::SetState(const QString& strState)
{
	ui.pushButton_start->setText(strState);//重新开始
	if (strState.compare(QStr_StartRecord) == 0 || strState.compare(QStr_ResetRecord) == 0)
	{
		if (mpRecordTimerPtr && mpRecordTimerPtr->isActive()) {
			mpRecordTimerPtr->stop();
		}

		ui.widget_SecondRight->SetNum(0);
		ui.widget_SecondLeft->SetNum(0);
		ui.widget_minRight->SetNum(0);
		ui.widget_minLeft->SetNum(0);

		miTimeCount = 0;
		calcDisplayTime();
		enableEdit(true);
		showEdit(true);
	}
	else//开始倒计时
	{
		showEdit(false);
		if (mpRecordTimerPtr) {
			mpRecordTimerPtr->start(1000);
		}
	}
}

void TimerWdg::slot_initState()
{
	if (mbClicked)
	{
		enableEdit(true);
		//SetState(QStr_StartRecord);
	}
}

void TimerWdg::calcDisplayTime()
{
	int iMin = miTimeCount / 60;//分钟数
	int iSec = miTimeCount % 60;//秒数

	int iTenDigitsM = iMin / 10;
	int iNumberOfDigitsM = iMin % 10;

	int iTenDigitsS = iSec / 10;
	int iNumberOfDigitsS = iSec % 10;

	ui.widget_minLeft->SetNum(iTenDigitsM);
	ui.widget_minRight->SetNum(iNumberOfDigitsM);

	ui.widget_SecondLeft->SetNum(iTenDigitsS);
	ui.widget_SecondRight->SetNum(iNumberOfDigitsS);
	if (2 >= miTimeCount && mbIsPlayEndSound)////播放声音
	{
		mbIsPlayEndSound = false;
		if(nullptr == mpPlayer)
			mpPlayer = new QMediaPlayer(this);
		QString str = QDir::currentPath();
		QString strPath = QApplication::applicationDirPath();

		strPath.append("/timeEnd.mp3");
		globalToolManager->GetDataManager()->WriteLog("%s  PLAY %s", __FUNCTION__, strPath.toLatin1().data());
		mpPlayer->setMedia(QUrl::fromLocalFile(strPath));

		mpPlayer->setVolume(50);
		mpPlayer->play();
	}
	globalToolManager->GetDataManager()->WriteLog("%s  PLAY mbIsPlayEndSound %d", __FUNCTION__, mbIsPlayEndSound);
}

void TimerWdg::slot_RecordTimeout() {
	miTimeCount--;

	if (miTimeCount > 0)
	{
		calcDisplayTime();
	}
	else
	{
		ui.widget_minLeft->SetNum(0);
		ui.widget_minRight->SetNum(0);
		ui.widget_SecondLeft->SetNum(0);
		ui.widget_SecondRight->SetNum(0);

		int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
		if (JOIN_ROLE_Teacher == roleType)//讲师
		{
			SetState(QStr_ResetRecord);
		}
		else//学员
		{
         
         if(miTimeCount <0) {
            mpRecordTimerPtr->stop();
            close();
            hide();
         }
         else {
            calcDisplayTime();
         }

			//ui.CountdownWdg->hide();
			//ui.endWdg->show();
			//ui.label_7->hide();
			////ui.leftWdg->hide();
			//ui.widget->setStyleSheet(RADIUSQSSQSS);
		} 
	}
}
