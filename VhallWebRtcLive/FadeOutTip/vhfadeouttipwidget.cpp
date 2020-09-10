#include "vhfadeouttipwidget.h"
#include <QGridLayout>
#include <QPainter>
#include <QDebug>
#define TIMETICK 40

#define QLABEL_ICON_MESSAGE_PAGE    QStringLiteral("#label_icon{border-image:url(:/idention/img/idention/tost_message.png);}")

#define QLABEL_ICON_SUCCESS_PAGE    QStringLiteral("QLabel#label_icon{border-image:url(:/idention/img/idention/tost_success.png);}")

#define QLABEL_ICON_WRNING_PAGE    QStringLiteral("QLabel#label_icon{border-image:url(:/idention/img/idention/tost_wening.png);}")

#define QLABEL_ICON_ERROR_PAGE    QStringLiteral("QLabel#label_icon{border-image:url(:/idention/img/idention/tost_error.png);}")

#define QWIDGET_MESSAGE_PAGE    QStringLiteral("QWidget#widget{background:rgba(237,242,252,1);border-radius:8px;border:1px solid rgba(237,242,252,255);}")

#define QWIDGET_SUCCESS_PAGE    QStringLiteral("QWidget#widget{background:rgba(232,241,225,1);border-radius:8px;border:1px solid rgba(208,233,195,255);}")

#define QWIDGET_MWRNING_PAGE    QStringLiteral("QWidget#widget{background:rgba(253,246,237,1);border-radius:8px;border:1px solid rgba(250,236,216,255);}")

#define QWIDGET_ERROR_PAGE    QStringLiteral("QWidget#widget{background:rgba(254,240,240,1);border-radius:8px;border:1px solid rgba(251,221,221,255);}")


VHFadeOutTipWidget::VHFadeOutTipWidget(QWidget *parent) :
   QWidget(parent) {
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);
   //QGridLayout *layout = new QGridLayout(this);
   //m_tipLabel = new QLabel (this);

   ui.m_tipLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
   //layout->addWidget(m_tipLabel);
   //setLayout(layout);
   //mPoint = QPoint(0,0);
   m_timer = new QTimer(this);
   connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
   m_globalSize = QSize(898, 34);
   setWindowOpacity(0.9);

   //m_timerTest = new QTimer(this);
   //connect(m_timerTest, SIGNAL(timeout()), this, SLOT(Testtimeout()));
}

VHFadeOutTipWidget::~VHFadeOutTipWidget() {
   if (m_timer) {
      m_timer->stop();
   }

}

void VHFadeOutTipWidget::SetSize(int w,int h) {
   m_globalSize = QSize(w,h);
}

void VHFadeOutTipWidget::rePos(QWidget* mainWdg) {
   raise();
   int a = mPoint.x() + (mainWdg->width() - mMaxWidth) / 2;
   this->move(mPoint.x() + (mainWdg->width()- mMaxWidth)/2,mPoint.y());
   //resize(m_globalSize);
}

void VHFadeOutTipWidget::Tip(QWidget* mainWdg,QPoint p, QString str) {
   if (str.isEmpty()) {
      return;
   }else if (!str.isEmpty()) {
      ui.m_tipLabel->setText(str);
	  ui.m_tipLabel->setPalette(QColor(0, 0, 255));

	  ui.m_tipLabel->adjustSize();
   }

   mMaxWidth = ui.label_icon->width() + ui.m_tipLabel->width() + 50;
   this->setFixedWidth(mMaxWidth);
   mPoint = p;

   if(m_timer) {
      m_timer->stop();
      m_closeCount = 0;
      //setWindowOpacity(1.0f);
      m_timer->start(2000/*TIMETICK*/);
      mStartTime = QDateTime::currentDateTime();
   }
}

void VHFadeOutTipWidget::SetStyleSheet(int style)
{
	switch (style)
	{
	case Tost_Message:
		ui.m_tipLabel->setStyleSheet("font: 14px \"Î¢ÈíÑÅºÚ\";color:rgb(144,147,153);");
		ui.label_icon->setStyleSheet(QLABEL_ICON_MESSAGE_PAGE);
		ui.widget->setStyleSheet(QWIDGET_MESSAGE_PAGE);
		mcolor = QColor(237, 242, 252,230);
		break;
	case Tost_Success:
		ui.m_tipLabel->setStyleSheet("font: 14px \"Î¢ÈíÑÅºÚ\";color:rgb(103,194,58);");
		ui.label_icon->setStyleSheet(QLABEL_ICON_SUCCESS_PAGE);
		ui.widget->setStyleSheet(QWIDGET_SUCCESS_PAGE);
		mcolor = QColor(232, 241, 225, 230);
		break;
	case Tost_Warning:
		ui.m_tipLabel->setStyleSheet("font: 14px \"Î¢ÈíÑÅºÚ\";color:rgb(230,162,61);");
		ui.label_icon->setStyleSheet(QLABEL_ICON_WRNING_PAGE);
		ui.widget->setStyleSheet(QWIDGET_MWRNING_PAGE);
		mcolor = QColor(253, 246, 237, 230);
		break;
	case Tost_Error:
		ui.m_tipLabel->setStyleSheet("font: 14px \"Î¢ÈíÑÅºÚ\";color:rgb(245,108,108);");
		ui.label_icon->setStyleSheet(QLABEL_ICON_ERROR_PAGE);
		ui.widget->setStyleSheet(QWIDGET_ERROR_PAGE);
		mcolor = QColor(254, 240, 240, 230);
		break;
	default:
		break;
	}
}

void VHFadeOutTipWidget::setDispalyMsg(QString str)
{
   ui.m_tipLabel->setText(str);
}

QString VHFadeOutTipWidget::DisplayMsg()
{
   QString strText = ui.m_tipLabel->text().trimmed();
   return  strText;
}

bool VHFadeOutTipWidget::isVisible()
{
   QDateTime curTime = QDateTime::currentDateTime();
   qint64 iValue = mStartTime.msecsTo(curTime);
   //QString str = curTime.toString("yyyy-MM-dd hh:mm:ss.z");
   //QString strStart = mStartTime.toString("yyyy-MM-dd hh:mm:ss.z");
   if (iValue > 2500)
   {
      timeout();
      return false;
   }
   return QWidget::isVisible();
}

void VHFadeOutTipWidget::Stop(){
   if(m_timer) {
      m_timer->stop();
   }
   ui.m_tipLabel->setText("");
   hide();
}

//void VHFadeOutTipWidget::Testtimeout()
//{
//   if (m_timer)
//   {
//      int iff = m_timer->elapsed();
//   }
//}

void VHFadeOutTipWidget::paintEvent(QPaintEvent *e) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
   painter.setBrush(mcolor);
   painter.setPen(mcolor);
  // painter.drawRoundedRect(ui.widget->rect(), 8, 8);
   //painter.fillRect(ui.widget->rect(), );

}
void VHFadeOutTipWidget::timeout() {
   /*m_closeCount+=TIMETICK;
   if(m_closeCount<1000) {
      return ;
   }
   else if(m_closeCount<2000) {
      float opacity=1.0f;
      opacity=2000;
      opacity-=m_closeCount;
      if (TIMETICK * 2 + 1 >= opacity)
      {
         ui.m_tipLabel->setText("");
      }

      opacity/=2000;
      setWindowOpacity(opacity);
   }
   else {
      m_timer->stop();
      m_closeCount=0;
      close();
   }*/
   //m_timer->stop();
   Stop();
   m_closeCount = 0;
   //hide();
   close();
} 

void VHFadeOutTipWidget::mousePressEvent(QMouseEvent *event) {
   if (m_pParentWdg) {
      m_pParentWdg->raise();
      this->show();
      this->raise();
   }
}

