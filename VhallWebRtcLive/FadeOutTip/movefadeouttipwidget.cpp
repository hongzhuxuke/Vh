#include "MoveFadeoutTipWidget.h"
#include <QGridLayout>
#include <QPainter>
#include <QDebug>
#define TIMETICK 40
MoveFadeoutTipWidget::MoveFadeoutTipWidget(QWidget *parent) :
   QWidget(parent) {
	m_pParentWdg = parent;
	setWindowFlags(Qt::FramelessWindowHint |/* Qt::WindowStaysOnTopHint |*/ Qt::Tool);
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);
   
   QGridLayout *layout = new QGridLayout(this);
   mpTipText = new QLabel (this);
   mpTipText->setStyleSheet("color:rgb(150,150,150);");
   mpTipText->setAlignment(Qt::AlignCenter);
   layout->addWidget(mpTipText);
   setLayout(layout);
   mPoint = QPoint(0,0);
   mpTipText->setFixedHeight(34);
   
   m_globalSize = QSize(898,34);
   setFixedSize(QSize(898, 34));
   mpTipText->setGeometry(0,0, 0, 34);
}

MoveFadeoutTipWidget::~MoveFadeoutTipWidget() {

}

void MoveFadeoutTipWidget::SetTilte(const QString& str)
{
	if (nullptr!=mPMoveTimer)
	{
		if (mPMoveTimer->isActive())
			mPMoveTimer->stop();
	}
	int totalShowMsgLen = fontMetrics().width(str);
	mpTipText->setText(str);
	mpTipText->setFixedWidth(totalShowMsgLen);
	mpTipText->move(0, 0);


	StartRoll();
}

void MoveFadeoutTipWidget::SetTilteSheetStyle(const QString& str)
{
	mpTipText->setStyleSheet(str);
	
}

//void MoveFadeoutTipWidget::rePos(QWidget* mainWdg) {
//   int xPoint = 0;
//   int yPoint = 0;
//   if (mainWdg != NULL) {
//      xPoint = mainWdg->x();
//      yPoint = mainWdg->y();
//   }
//   raise();
//   this->move(xPoint + mPoint.x(), yPoint + mPoint.y());
//   resize(m_globalSize);
//}
//
//void MoveFadeoutTipWidget::Tip(QWidget* mainWdg,QPoint p, QString str) {
//   if (str.isEmpty()) {
//      return;
//   }else if (mpTipText && !str.isEmpty()) {
//      mpTipText->setText(str);
//   }
//   mpTipText->move(0, 0);
//   mPoint = p;
//   rePos(mainWdg);
//   show();
//   m_pParentWdg = mainWdg;
//   setParent(m_pParentWdg);
//   if (nullptr == mPMoveTimer)
//   {
//	   mPMoveTimer = new QTimer(this);
//	   connect(mPMoveTimer, &QTimer::timeout, this, &MoveFadeoutTipWidget::slot_moveText);
//   }
//   mPMoveTimer->start(30);
//}

void MoveFadeoutTipWidget::StartRoll()
{
	if (nullptr == mPMoveTimer)
	{
		mPMoveTimer = new QTimer(this);
		connect(mPMoveTimer, &QTimer::timeout, this, &MoveFadeoutTipWidget::slot_moveText);
	}
	mPMoveTimer->start(30);
}

void MoveFadeoutTipWidget::paintEvent(QPaintEvent *e) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing, true);
   //painter.fillRect(this->rect(), QColor(67, 71, 74, 230));
}


void MoveFadeoutTipWidget::mousePressEvent(QMouseEvent *event) {
   if (m_pParentWdg) {
      m_pParentWdg->raise();
      this->show();
      this->raise();
   }
}

void MoveFadeoutTipWidget::slot_moveText()
{
	int iX = mpTipText->x();
	int iWidth = mpTipText->width();
	if (iX + iWidth <= 0)
	{
		mpTipText->move(width(), 0);
	}
	else
	{
		mpTipText->move(iX-5, 0);
	}
}
