#include "MultiClassFloatWnd.h"
#include "WebRtcLiveWdg.h"
#include "ESCExitNotice.h"
#include "ExitFullScreenWdg.h"

#include <QDesktopWidget>

extern ToolManager *globalToolManager;

#define NORMAL_SIZE  QSize(720,570)

MultiClassFloatWnd::MultiClassFloatWnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_close()));
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->setMouseTracking(true);
    mESCExitNotice = new ESCExitNotice(this);
    if (mESCExitNotice) {
       mESCExitNotice->hide();
    }

    mExitFullScreen = new ExitFullScreenWdg(this);
    if (mExitFullScreen) {
       mExitFullScreen->hide();
       connect(mExitFullScreen, &ExitFullScreenWdg::sig_clicked, this, &MultiClassFloatWnd::slot_close);
    }

    connect(&mHideExitFullScreenTimer, SIGNAL(timeout()), this, SLOT(slot_HideExitFullScreen()));
    mHideExitFullScreenTimer.setSingleShot(true);
    //720  570
}

MultiClassFloatWnd::~MultiClassFloatWnd()
{
}

void MultiClassFloatWnd::setStaysOnTop(bool bTop)
{
   if (bTop) {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
      
   }
   else {
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
      
   }
}


void MultiClassFloatWnd::AddRenderWidget(QWidget* widget,bool isFull) {
   mbIsFullType = isFull;
   if (mbIsFullType) {
      ui.widget->hide();
      grabKeyboard();
   }
   else {
      ui.widget->show();
      releaseKeyboard();
   }
   mpRender = widget;
   ui.verticalLayout->addWidget(widget);
}

QWidget* MultiClassFloatWnd::RenderWidget(bool bRemove /*= true*/)
{
   if (bRemove) {
      ui.verticalLayout->removeWidget(mpRender);
   }
   
   return mpRender;
}

void MultiClassFloatWnd::RemoveRenderWidget(QWidget* widget) {
   mstrId = "";
   releaseKeyboard();
}

void MultiClassFloatWnd::keyPressEvent(QKeyEvent *event) {
   switch (event->key())
   {
   case Qt::Key_Escape:
      slot_close();
      break;
   default:
      QWidget::keyPressEvent(event);
   }
}

void MultiClassFloatWnd::slot_close() {
   if (mESCExitNotice) {
      mESCExitNotice->hide();
   }
   
   if (mExitFullScreen) {
      mExitFullScreen->hide();
   }
   
   emit sig_close(mstrId);
}

bool MultiClassFloatWnd::IsFullType() {
   return mbIsFullType;
}

void MultiClassFloatWnd::SetUserid(QString id)
{
   mstrId = id;
}

void MultiClassFloatWnd::show(bool bShowTitle)
{
   if (bShowTitle) {
      ui.widget->show();
      //this->setFixedSize(720 , 570);
   }
   else {
      ui.widget->hide();
      //this->setFixedSize(720, 570);
   }
   QWidget::show();
}

void MultiClassFloatWnd::closeEvent(QCloseEvent *event) {
   event->ignore();
   emit sig_close(mstrId);
}


void MultiClassFloatWnd::mousePressEvent(QMouseEvent *event) {
   if (event) {
      showESCExitNotice();
      mPressPoint = this->pos() - event->globalPos();
      if (!mbIsFullType) {
         mIsMoved = true;
      }
   }
}

void MultiClassFloatWnd::mouseMoveEvent(QMouseEvent *event) {
   if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
      this->move(event->globalPos() + mPressPoint);
      this->raise();
   }
}

void MultiClassFloatWnd::mouseReleaseEvent(QMouseEvent *) {
   if (mIsMoved) {
      mIsMoved = false;
   }
}

void MultiClassFloatWnd::slot_HideExitFullScreen() {
   if (mESCExitNotice) {
      mESCExitNotice->hide();
   }
}

void MultiClassFloatWnd::slot_ShowExitFullScreen() {

}
void MultiClassFloatWnd::showESCExitNotice()
{
   //int iW = this->width();
   //int iH = this->height();
   if (mESCExitNotice && mbIsFullType) {
      mHideExitFullScreenTimer.start(5000);
      mESCExitNotice->show();
      

      //QDesktopWidget *desktop = QApplication::desktop();
      //QRect screen = desktop->screenGeometry(this);

      QDesktopWidget * deskTop = QApplication::desktop();
      int curMonitor = deskTop->screenNumber(this); // 参数是一个QWidget*
      //QRect DesktopRect = deskTop->screenGeometry(curMonitor);

      QRect screen = deskTop->availableGeometry(curMonitor);

      mESCExitNotice->move(screen.x() + screen.width() / 2 - mESCExitNotice->width() / 2, 100);
      if (mExitFullScreen) {
         mExitFullScreen->show();
         mExitFullScreen->move(screen.x() + screen.width() / 2 - mExitFullScreen->width() / 2, screen.height() - 100);
		
      }

   }
}
void MultiClassFloatWnd::showEvent(QShowEvent *event) {
   showESCExitNotice();
   QWidget::showEvent(event);
}