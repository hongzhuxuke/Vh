#include "FullScreenRender.h"
#include <QDesktopWidget>
#include "ExitFullScreenWdg.h"
#include "ESCExitNotice.h"
#include "VhallRenderMaskWdg.h"
#include <Qrect>
#define HIDE_EXIT_BUTTON    3000
extern ToolManager *globalToolManager;

#define BIG_RENDER_CLOSE_CAMERA QStringLiteral("QWidget#widget{border-image:url(:/widget/img/widget/teacher_close_camera.jpg);}")
#define SMALL_RENDER_CLOSE_CAMERA QStringLiteral("QWidget#widget_smallWidget{border-image:url(:/widget/img/widget/teacher_close_camera.jpg);}")

CREATE_WND_FUNCTION(FullScreenRender);
FullScreenRender::FullScreenRender(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
    this->setMouseTracking(true);
    ui.widget->setMouseTracking(true);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    //ui.widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    
    ui.widget->HideTitle();
    connect(ui.widget, SIGNAL(sig_move()), this, SLOT(slot_ShowExitFullScreen()));

    this->setWindowTitle(QStringLiteral("微吼课堂"));
    //mExitFullScreen = new ExitFullScreenWdg(this);
    //if (mExitFullScreen) {
    //   connect(mExitFullScreen, &ExitFullScreenWdg::sig_clicked, this, &FullScreenRender::slot_hide);
    //    mExitFullScreen->hide();
    //}
    mESCExitNotice = new ESCExitNotice(this);
    if (mESCExitNotice) {
        mESCExitNotice->hide();
    }
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();
    int screenWidth = screen.width();
    int screenHeight = screen.height();
    QSize screenSize(screenWidth, screenHeight);
    setFixedSize(screenWidth, screenHeight);
	ui.widget->setFixedSize(screenSize);
	//ui.RenderMaskWdg->setFixedSize(screenSize);

	ui.widget->setGeometry(0, 0, screenWidth, screenHeight);
	//ui.RenderMaskWdg->setGeometry(0, 0, screenWidth, screenHeight);

	ui.widget->move(0, 0);
	//ui.RenderMaskWdg->move(0, 0);

    connect(&mHideExitFullScreenTimer, SIGNAL(timeout()), this, SLOT(slot_HideExitFullScreen()));

    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    //if (baseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL) {
    //    mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
    //    if (mpVhallRenderMaskWdg) {
    //        mpVhallRenderMaskWdg->hide();
    //        mpVhallRenderMaskWdg->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    //    }
    //}
	//mpVhallRenderMaskWdg = new VhallRenderMaskWdg(this);
	//if (mpVhallRenderMaskWdg) {
	//	mpVhallRenderMaskWdg->hide();
	//	mpVhallRenderMaskWdg->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	//	mpVhallRenderMaskWdg->setFixedSize(this->width(), this->height());
	//}

    ui.widget->SetShowNoMic(true);
    grabKeyboard();
}

FullScreenRender::~FullScreenRender()
{
    //if (mpVhallRenderMaskWdg) {
    //    delete mpVhallRenderMaskWdg;
    //    mpVhallRenderMaskWdg = NULL;
    //}
}

void FullScreenRender::SetPenState(bool enable)
{
	//if (ui.RenderMaskWdg) {
	//	ui.RenderMaskWdg->SetPenState(enable);
	//	ui.RenderMaskWdg->SetShowPenLogo(enable);
	//}
   ui.widget->SetPenState(enable);
}

void FullScreenRender::initTrophyRewardNum(const int & iTrophyNum)
{
	//ui.RenderMaskWdg->initTrophyRewardNum(iTrophyNum);
}

void FullScreenRender::SetShowNoMic(bool show/*,  bool bSelf*/)
{
	/*ui.RenderMaskWdg->SetShowNoMic(show);*/
   ui.widget->SetShowNoMic(show);
}

void FullScreenRender::SetShowStuNoMic(bool hasAudio)
{
	//ui.RenderMaskWdg->SetShowStuNoMic(hasAudio);
   ui.widget->SetShowStuNoMic(hasAudio);
}

std::shared_ptr<vhall::VideoRenderReceiveInterface> FullScreenRender::GetRenderWnd()
{
   return ui.widget->GetRenderWnd();
}

void FullScreenRender::setFixedSize(int w, int h)
{
   CBaseWnd::setFixedSize( w,  h);
   ui.widget->setFixedSize(QSize(w, h));
}

void FullScreenRender::slot_hide()
{
   if (mESCExitNotice) {
      mESCExitNotice->hide();
   }
   ui.widget->SetBigRendering(false);
   CBaseWnd::hide();
}

void FullScreenRender::HandleSubScribeRender(const CSTDMapParam &mapParam) {

}

void FullScreenRender::Notice(const CSTDMapParam &mapParam) {
	globalToolManager->GetDataManager()->WriteLog("  Enter %s", __FUNCTION__);
    std::map<QString, QString>::const_iterator iterFunc = mapParam.find(Str_Function);
    if (iterFunc != mapParam.end() && iterFunc->second == Str_Function_SubScribe) {
        ui.widget->HandleSubScribeRender(mapParam);
    }
    else if (iterFunc != mapParam.end() && iterFunc->second == Str_Function_FloadAndFullWndRender) {
       ui.widget->show();
        ui.widget->HandleProcessFloadRender(mapParam);
    }

	std::map<QString, QString>::const_iterator iterNickID = mapParam.find(SHOW_NICK_NAME);
	if (iterNickID != mapParam.end() && !iterNickID->second.isEmpty()) {
		mNickName = iterNickID->second;
		/*if (mpVhallRenderMaskWdg) {
			mpVhallRenderMaskWdg->SetUserName(mNickName);
		}*/
	/*	if (ui.RenderMaskWdg) {
			ui.RenderMaskWdg->SetUserName(mNickName);
		}*/
      ui.widget->SetUserName(mNickName);
	}
    std::map<QString, QString>::const_iterator iterUserID = mapParam.find(USER_ID);
    if (iterUserID != mapParam.end() && !iterUserID->second.isEmpty()) {
        mUserId = iterUserID->second;
        //if (mExitFullScreen) {
        //    mExitFullScreen->SetUserID(mUserId);
        //}
    }

    ui.widget->HandleSetUserType(mapParam);
    setFocusPolicy(Qt::WheelFocus);
    setFocus();
	globalToolManager->GetDataManager()->WriteLog("  Leave %s", __FUNCTION__);
}

void FullScreenRender::NoticeShowNickName() {

  //  if (/*mpVhallRenderMaskWdg &&*/ isVisible()) {
  ////      mpVhallRenderMaskWdg->setFixedWidth(this->width());
		////mpVhallRenderMaskWdg->setFixedHeight(this->height());
  //      //QPoint pos = ui.widget->mapToGlobal(ui.widget->pos());
  //      //mpVhallRenderMaskWdg->move(pos.rx(), pos.ry() + this->height()-mpVhallRenderMaskWdg->height());
		//ui.RenderMaskWdg->show();
  //      //mpVhallRenderMaskWdg->raise();
  //  }
  //  else {
  //      if (ui.RenderMaskWdg) {
		//	ui.RenderMaskWdg->hide();
  //      }
  //  }
  //  setFocusPolicy(Qt::WheelFocus);
  //  setFocus();
}

void FullScreenRender::SetRenderMaskWdg(VhallRenderMaskWdg * obj)
{
	ui.widget->SetRenderMaskWdg(obj);
}

QPaintEngine *FullScreenRender::paintEngine() const {
    return NULL;
}

void FullScreenRender::mousePressEvent(QMouseEvent *e) {
    slot_ShowExitFullScreen();
    QDialog::mousePressEvent(e);
}

void FullScreenRender::mouseMoveEvent(QMouseEvent *e) {
    slot_ShowExitFullScreen();
    QDialog::mouseMoveEvent(e);
}

void FullScreenRender::mouseReleaseEvent(QMouseEvent *e) {
    QDialog::mouseReleaseEvent(e);
}

void FullScreenRender::keyPressEvent(QKeyEvent *event) {
    switch (event->key())
    {
    case Qt::Key_Escape:
        //if (mExitFullScreen) {
        //    mExitFullScreen->slot_OnClicked();
        //}
        break;
    default:
       CBaseWnd::keyPressEvent(event);
    }
}

void FullScreenRender::slot_HideExitFullScreen() {
    //if (mExitFullScreen) {
    //    mExitFullScreen->hide();
    //}
    if (mESCExitNotice) {
        mESCExitNotice->hide();
    }
}

void FullScreenRender::showEvent(QShowEvent *event) { 
   CBaseWnd::showEvent(event);
}

void FullScreenRender::hideEvent(QShowEvent * event)
{
   ui.widget->SetBigRendering(false);
   CBaseWnd::showEvent(event);
}

void FullScreenRender::HandleSetUserInfo(const CSTDMapParam &mapParam) {
 
}

void FullScreenRender::HandleFullSizeCameraChange(const CSTDMapParam &mapParam) {
 
}

void FullScreenRender::slot_ShowExitFullScreen() {
    //if (mExitFullScreen) {
    //    mHideExitFullScreenTimer.start(HIDE_EXIT_BUTTON);
    //    mExitFullScreen->show();
    //    QDesktopWidget *desktop = QApplication::desktop();
    //    QRect screen = desktop->screenGeometry();
    //    mExitFullScreen->move(screen.width() / 2 - mExitFullScreen->width() / 2, screen.height() - mExitFullScreen->height() - 40);
    //}

    if (mESCExitNotice) {
        mHideExitFullScreenTimer.start(HIDE_EXIT_BUTTON);
        mESCExitNotice->show();
        QDesktopWidget *desktop = QApplication::desktop();
        QRect screen = desktop->screenGeometry();
        mESCExitNotice->move(screen.width() / 2 - mESCExitNotice->width() / 2, 100);
    }
}

void FullScreenRender::HandleMicState(const CSTDMapParam &mapParam) {

    //参数用于更新麦克风状态
    std::map<QString, QString>::const_iterator iterBigRenderMicIsClose = mapParam.find(BIG_RENDER_MIC_IS_CLOSE);
    if (iterBigRenderMicIsClose != mapParam.end()) {
        //if (iterBigRenderMicIsClose->second.compare(PARAM_TRUE) == 0) {
        //    if (mpVhallRenderMaskWdg) {
        //        mpVhallRenderMaskWdg->SetShowNoMic(true);
        //    }
        //}
        //else {
        //    if (mpVhallRenderMaskWdg) {
        //        mpVhallRenderMaskWdg->SetShowNoMic(false);
        //    }
        //}
		if (iterBigRenderMicIsClose->second.compare(PARAM_TRUE) == 0) {
			if (ui.widget) {
				ui.widget->SetShowNoMic(true);
			}
		}
		else {
			if (ui.widget) {
				//ui.widget->SetShowNoMic(false);
			}
		}
		
    }
}