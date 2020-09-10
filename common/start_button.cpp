#include <QPainter>
#include <QMouseEvent>
#include "start_button.h"
#include "pub.Const.h"
#if _MSC_VER >= 1600  
#pragma execution_character_set("utf-8")  
#endif  

StartButton::StartButton(QWidget *parent)
: QPushButton(parent) {
   mStatus = NORMAL;
   mMousePressed = false;
   mIsStarting = false;
   setStyleSheet(TOOLTIPQSS);
}

StartButton::~StartButton() {

}

void StartButton::loadPixmap(QString startPicPath, QString stopPicPath) {
   mStartPixmap.load(startPicPath);
   mStopPixmap.load(stopPicPath);
   mBtnWidth = mStartPixmap.width() / 3;
   mBtnHeight = mStartPixmap.height();
   setFixedSize(mBtnWidth, mBtnHeight);
}

void StartButton::updateLiveStatus(bool liveStatus) {
   mIsStarting = liveStatus;
   update();
}

bool StartButton::GetLiveStatus() {
   return mIsStarting;
}

void StartButton::enterEvent(QEvent * e) {
   mStatus = ENTER;
   update();
   QPushButton::enterEvent(e);
}

void StartButton::leaveEvent(QEvent *e) {
   mStatus = NORMAL;
   update();
   QPushButton::leaveEvent(e);
}

void StartButton::mousePressEvent(QMouseEvent *event) {
   if (event->button() == Qt::LeftButton) {
      mMousePressed = true;
      mStatus = PRESS;
      update();
   }
   QPushButton::mousePressEvent(event);
}

void StartButton::mouseReleaseEvent(QMouseEvent * e) {
   if (mMousePressed) {
      mMousePressed = false;
      mStatus = ENTER;
      update();
      emit clicked();
   }
   QPushButton::mouseReleaseEvent(e);
}

void StartButton::paintEvent(QPaintEvent * e) {
   if (!mIsStarting) {
      this->setToolTip(QStringLiteral("开始直播"));
      QPainter painter(this);
      painter.drawPixmap(rect(), mStartPixmap.copy(mBtnWidth * mStatus, 0, mBtnWidth, mBtnHeight));
   } else if(ENTER == mStatus){
      this->setToolTip(QStringLiteral("暂停直播"));
      QPainter painter(this);
      painter.drawPixmap(rect(), mStopPixmap.copy(mBtnWidth * mStatus, 0, mBtnWidth, mBtnHeight));
   }
   
   QPushButton::paintEvent(e);
}