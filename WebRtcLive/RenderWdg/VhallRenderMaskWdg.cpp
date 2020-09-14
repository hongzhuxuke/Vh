#include "VhallRenderMaskWdg.h"
#include <QPainter>
#include "../Unility/Unility.h"
#include <QDebug>
#include "TeacherRenderWdg.h"
#include "pathmanager.h"

extern ToolManager *globalToolManager;

#define STYLEQLINEARGRADIENT  "#widget{\
	background-color: qlineargradient(spread : pad, x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 rgba(0, 0, 0, 0), stop : 1 rgba(0, 0, 0, 255));\
}"

#define STYLENOMAL  "#widget{\
	background-color: rgba(0, 0, 0, 204);\
}"

VhallRenderMaskWdg::VhallRenderMaskWdg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    //this->setAutoFillBackground(false);

    SetShowNoMic(false);
    SetShowPenLogo(false);
	ui.pTrophyWdg->hide();

	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedMic, this, &VhallRenderMaskWdg::sig_OnClickedMic);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedCamera, this, &VhallRenderMaskWdg::sig_OnClickedCamera);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedFullScreen, this, &VhallRenderMaskWdg::sig_OnClickedFullScreen);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedFloatRender, this, &VhallRenderMaskWdg::sig_OnClickedFloatRender);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedAllMuteCamera, this, &VhallRenderMaskWdg::sig_OnClickedAllMuteCamera);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnClickedAllMute, this, &VhallRenderMaskWdg::sig_OnClickedAllMute);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnAllUnpublish, this, &VhallRenderMaskWdg::sig_OnAllUnpublish);

	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnStuClickedCamera, this, &VhallRenderMaskWdg::sig_OnStuClickedCamera);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnStuClickedMic, this, &VhallRenderMaskWdg::sig_OnStuClickedMic);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnstuClickedUnPublish, this, &VhallRenderMaskWdg::sig_OnstuClickedUnPublish);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnStuPenClickedPen, this, &VhallRenderMaskWdg::sig_OnStuPenClickedPen);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnStuClickedFloat, this, &VhallRenderMaskWdg::sig_OnStuClickedFloat);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnStuClickedFullWnd, this, &VhallRenderMaskWdg::sig_OnStuClickedFullWnd);

	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnOpenClassStuMic, this, &VhallRenderMaskWdg::sig_OnOpenClassStuMic);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnOpenClassStudentCamera, this, &VhallRenderMaskWdg::sig_OnOpenClassStudentCamera);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnChangeCamera, this, &VhallRenderMaskWdg::sig_OnChangeCamera);
	connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_FavorStudent, this, &VhallRenderMaskWdg::sig_FavorStudent);
   connect(ui.mTeacherCtrlWdg, &TeacherCtrlWdg::sig_OnSetBigScreen, this, &VhallRenderMaskWdg::sig_OnSetBigScreen);

	connect(this, &VhallRenderMaskWdg::sig_PicPush, ui.mTeacherCtrlWdg, &TeacherCtrlWdg::slot_PicPush);
	showCtrlWdg(false);
	SetShowStuNoMic(false);
}

VhallRenderMaskWdg::~VhallRenderMaskWdg()
{
	if (nullptr != mpTimer)
	{
		if (mpTimer->isActive())
			mpTimer->stop();
		delete mpTimer;
		mpTimer = nullptr;
	}
}

void VhallRenderMaskWdg::SetTeacherRenderWdg(TeacherRenderWdg* pRenderWdg)
{
	mpRenderWdg = pRenderWdg;
}

void VhallRenderMaskWdg::SetUserName(const QString &userName) {
	globalToolManager->GetDataManager()->WriteLog("%s name : %s", __FUNCTION__, userName.toLatin1().data());
    if (!userName.isEmpty()) {
        mNickName = userName;
		QString title = QString("%1").arg(CPathManager::GetString(mNickName, 16), mNickName);
		ui.label_userName->setText(title);
    }
}

bool VhallRenderMaskWdg::GetBEnter()
{
	return mEnter;
}

QString VhallRenderMaskWdg::GetName() {
    return mNickName;
}

void VhallRenderMaskWdg::initTrophyRewardNum(const int & iNum)
{
	ui.pTrophyWdg->initTrophyRewardNum(iNum);
}

void VhallRenderMaskWdg::showTrophyWdg(const bool& bShow)
{
	if (bShow && mBShowTrophyWdg)
		ui.pTrophyWdg->show();
	else
		ui.pTrophyWdg->hide();
}
void VhallRenderMaskWdg::SetUserNamePos(const bool& bUpdown)
{
	if (bUpdown)
	{
		ui.horizontalLayout_2->setContentsMargins(10, 0, 0, 14);
	}
	else
	{	
		ui.horizontalLayout_2->setContentsMargins(10,0,0,0);
	}
}

void VhallRenderMaskWdg::CheckShowChangeBigScreen(bool show) {
   ui.mTeacherCtrlWdg->CheckShowChangeBigScreen(show);
}

void VhallRenderMaskWdg::setShowTrophyWdg(const bool& bShow/* = true*/)
{
	mBShowTrophyWdg = bShow;
	showTrophyWdg(mBShowTrophyWdg);
}

void VhallRenderMaskWdg::setTrophyAdd(const int& iCount)
{
	ui.pTrophyWdg->setTrophyAdd(iCount);
}

void VhallRenderMaskWdg::SetShowNoMic(bool show/*,  bool bSelf*/) {
    if (show) {
        ui.label_noMic->show();
    }
    else {
        ui.label_noMic->hide();
    }

	//ui.mTeacherCtrlWdg->SetSmallRenderMicState(show);
	//if (bSelf) {
	//
	//}

}

void VhallRenderMaskWdg::SetShowStuNoMic(bool show)
{
	if (show) {
		ui.label_StuNoMic->show();
	}
	else {
		ui.label_StuNoMic->hide();
	}
}

void VhallRenderMaskWdg::SetRenderMaskWdg(VhallRenderMaskWdg * obj)
{
	if (obj) {
		SetUserName(obj->GetName());
		SetShowPenLogo(obj->ui.label_pen->isVisible());
		initTrophyRewardNum(obj->ui.pTrophyWdg->iTrophyCount());
		setShowTrophyWdg(obj->mBShowTrophyWdg);
		SetShowNoMic(obj->ui.label_noMic->isVisible());
		SetShowStuNoMic(obj->ui.label_StuNoMic->isVisible());
	}
}

void VhallRenderMaskWdg::SetShowPenLogo(bool show) {
    if (show) {
        ui.label_pen->show();
    }
    else {
        ui.label_pen->hide();
    }
}

void VhallRenderMaskWdg::showCtrlWdg(const bool& bShow, int WndFloatType)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
	if (mbValid&&bShow&&WndFloatType == WndFloatType_Normal)
	{
		ui.widget->setStyleSheet(STYLENOMAL);

		ui.pageTrophyWdg->hide();
		ui.mTeacherCtrlWdg->show();
      ui.mTeacherCtrlWdg->setStyleSheet(STYLENOMAL);
		if (nullptr == mpTimer)
		{
			mpTimer = new QTimer(this);
			connect(mpTimer, &QTimer::timeout, this, &VhallRenderMaskWdg::slot_hideCtrl);
		}
		if (nullptr != mpTimer)
			mpTimer->start(300);
	}
	else
	{
      globalToolManager->GetDataManager()->WriteLog("%s mbValid %d bShow %d FloatType %d", __FUNCTION__, mbValid, bShow, WndFloatType);
		ui.widget->setStyleSheet(STYLEQLINEARGRADIENT);
		ui.mTeacherCtrlWdg->hide();
		ui.pageTrophyWdg->show();

		if (mpTimer && mpTimer->isActive())
		{
			mpTimer->stop();
		}
	}
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
   //ui.mTeacherCtrlWdg->hide();
   //ui.pageTrophyWdg->show();
}

void VhallRenderMaskWdg::slot_hideCtrl()
{
	bool bHide = false;
	if (!mEnter && mpRenderWdg && !mpRenderWdg->GetBEnter())
	{
		bHide = true;
	}
	else
	{
		QRect rect = this->rect();
		QPoint point = this->mapFromGlobal(QCursor::pos());
		bool bContains = rect.contains(point);

		if (!bContains)
		{
			bHide = true;
		}
	}

	if (bHide)
	{
		showCtrlWdg(false);
	}

	//if (mWinid && mWinid->isVisible())
	//{
	//	//mWinid->activateWindow();
	//	mWinid->raise();
	//}

}

void VhallRenderMaskWdg::mousePressEvent(QMouseEvent *e)
{
	//if (QApplication::activeWindow())
	//{
	//	if (mWinid && QApplication::activeWindow() && mWinid != QApplication::activeWindow()->window())
	//	{
	//		//if(mWinid->mapToGlobal())
	//		if (mWinid == ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN))
	//		{
	//			return;
	//		}
	//		QPoint pos_mWinid = mWinid->mapToGlobal(QPoint(0, 0));
	//		QPoint pos_this = this->mapToGlobal(QPoint(0, 0));
	//		QRect m(pos_mWinid.x(),pos_mWinid.y(), mWinid->width(), mWinid->height());
	//		QRect s(pos_this.x(), pos_this.y(), this->width(), this->height());
	//		if (m.intersected(s).width()>0 || m.intersected(s).height() > 0)
	//		{
	//			//mWinid->activateWindow();
	//			//mWinid->raise();
	//		}
	//	}
	//	
	//}
	QWidget::mousePressEvent(e);
}


void VhallRenderMaskWdg::setCtrlValid(const bool& bValid)
{
	mbValid = bValid;
}

void VhallRenderMaskWdg::InitBtn(QString userId)
{
	ui.mTeacherCtrlWdg->InitBtn(userId);
}

void VhallRenderMaskWdg::ShowStuPublishBtn(bool show)
{
	ui.mTeacherCtrlWdg->ShowStuPublishBtn(show);
}

void VhallRenderMaskWdg::SetFloatType(WndFloatType type) {
   mWndFloatType = type;
   globalToolManager->GetDataManager()->WriteLog(" %s mWndFloatType %d",
      __FUNCTION__, mWndFloatType);
}

int VhallRenderMaskWdg::GetFloatType()
{
   globalToolManager->GetDataManager()->WriteLog(" %s mWndFloatType %d",
      __FUNCTION__, mWndFloatType);
   return mWndFloatType;
}

void VhallRenderMaskWdg::enterEvent(QEvent *e)
{
	mEnter = true;
	//CBaseWnd* mainWnd = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
	//if (QApplication::activeWindow() && QApplication::activeWindow() != this && QApplication::activeWindow() != mainWnd)
	//{
	//	mWinid = QApplication::activeWindow()->window();
	//}
	
	showCtrlWdg(mbShowCtrle, mWndFloatType);
	//globalToolManager->GetDataManager()->WriteLog("%s xk mpRenderWdg: %x, %p", __FUNCTION__, mpRenderWdg, mpRenderWdg);
	//if(mpRenderWdg)
	//	globalToolManager->GetDataManager()->WriteLog("%s xk Enter:%d mpRenderWdg::Enter %d", __FUNCTION__, mEnter, mpRenderWdg->GetBEnter(), mpRenderWdg, mpRenderWdg);
	QWidget::enterEvent(e);
}

void VhallRenderMaskWdg::SetShowCtrlWdg(const bool& bShow)
{
   mbShowCtrle = bShow;
}

void VhallRenderMaskWdg::SetStuSpacer(const int& iW)
{
	ui.labStatSpacer->setFixedWidth(iW);
}

void VhallRenderMaskWdg::leaveEvent(QEvent * e)
{
	mEnter = false;
	//if (QApplication::activeWindow())
	//{
	//	mWinid = QApplication::activeWindow()->window();
	//}
	//if(!mEnter && mpRenderWdg && !mpRenderWdg->GetBEnter())
	//	showCtrlWdg(false);
	//if (mpRenderWdg)
	//	globalToolManager->GetDataManager()->WriteLog("%s xk Enter:%d mpRenderWdg::Enter %d", __FUNCTION__, mEnter, mpRenderWdg->GetBEnter());
	//globalToolManager->GetDataManager()->WriteLog("%s xk mpRenderWdg: %x, %p", __FUNCTION__, mpRenderWdg, mpRenderWdg);
	//
	QWidget::leaveEvent(e);
}

void VhallRenderMaskWdg::SetAllMuteAudioState(bool mute)
{
	ui.mTeacherCtrlWdg->SetAllMuteAudioState( mute);
}

void VhallRenderMaskWdg::SetPenState(bool enable)
{
	ui.mTeacherCtrlWdg->SetPenState( enable);
}

void VhallRenderMaskWdg::SetCameraState(bool open)
{
	ui.mTeacherCtrlWdg->SetCameraState( open);
}

void VhallRenderMaskWdg::SetMicState(bool open)
{
	ui.mTeacherCtrlWdg->SetMicState( open);
}

void VhallRenderMaskWdg::SetSmallRenderCameraState(bool hasVideo)
{
	ui.mTeacherCtrlWdg->SetSmallRenderCameraState(hasVideo);
}

void VhallRenderMaskWdg::ShowStudentCtrl(bool show)
{
	ui.mTeacherCtrlWdg->ShowStudentCtrl(show);
}

void VhallRenderMaskWdg::SetSmallRenderMicState(bool hasAudio)
{
	ui.mTeacherCtrlWdg->SetSmallRenderMicState(hasAudio);////È¡Ïû¾²Òô
	SetShowStuNoMic(!hasAudio);
}