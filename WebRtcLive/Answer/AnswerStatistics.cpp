#include "AnswerStatistics.h"
#include <QPainter>

#define GreenLeftFill   QString("border-bottom:1px solid #C4C4C4; \
border-top:1px solid #C4C4C4;   \
border-left:1px solid #C4C4C4;   \
border-top-left-radius:20px; \
border-bottom-left-radius:20px; \
background-color:rgb(82, 204, 144);");

#define GreenRightFill   QString("border-bottom:1px solid #C4C4C4; \
border-top:1px solid #C4C4C4;   \
border-right:1px solid #C4C4C4;  \
border-top-right-radius:20px; \
border-bottom-right-radius:20px; \
background-color:rgb(82, 204, 144); ");


#define GrayLeftFill   QString("border-bottom:1px solid #C4C4C4; \
border-top:1px solid #C4C4C4;   \
border-left:1px solid #C4C4C4;   \
border-top-left-radius:20px; \
border-bottom-left-radius:20px; \
background-color:rgb(221, 221, 221); ");

#define GrayRightFill   QString("border-bottom:1px solid #C4C4C4; \
border-top:1px solid #C4C4C4;   \
border-right:1px solid #C4C4C4;  \
border-top-right-radius:20px; \
border-bottom-right-radius:20px; \
background-color:rgb(221, 221, 221); ");


#define LeftNoramlFill   QString("border-bottom:1px solid #C4C4C4; \
border-top:1px solid #C4C4C4;   \
border-left:1px solid #C4C4C4;  \
border-top-left-radius:20px; \
border-bottom-left-radius:20px; \
background-color: rgb(255, 255, 255); ");


AnswerStatistics::AnswerStatistics(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

AnswerStatistics::~AnswerStatistics()
{
}

void AnswerStatistics::SetAnswerPerson(int selectPer, int total) {
    mSelectPer = selectPer;
	total<=0 ? mTotal=1 : mTotal=total;
    int showStat = mSelectPer * 100 / mTotal;
    QString text = QString::fromWCharArray(L"%1% (%2人)").arg(showStat).arg(selectPer);
    ui.label_stat->setText(text);
}

void AnswerStatistics::SetAnswerIndex(QString text) {
    ui.label->setText(text);
}

void AnswerStatistics::SetAnswerCorrect(bool correct) {
    mbCorrect = correct;
}

void AnswerStatistics::paintEvent(QPaintEvent* painter) {
    QPainter paint;
    paint.begin(this);
    QColor whiteColor(255, 255, 255);
    paint.setPen(QPen(whiteColor, 1, Qt::SolidLine));//设置画笔形式 
    paint.setBrush(QBrush(whiteColor, Qt::SolidPattern));//设置画刷形式 
    paint.drawRect(QRect(0, 0, this->width(), this->height()));  

    if (mSelectPer > 0) {
        if (mbCorrect) {
            QString sheet = GreenLeftFill;
            ui.label_leftFill->setStyleSheet(sheet);

            QColor color(82, 204, 144);
            paint.setPen(QPen(color, 1, Qt::SolidLine));//设置画笔形式 
            paint.setBrush(QBrush(color, Qt::SolidPattern));//设置画刷形式 
            int paintWidth = ((float)mSelectPer / (float)mTotal) * this->width();
            if (paintWidth < this->width()-60 - ui.label_leftFill->width() - ui.label_rightFill->width()) {
                paint.drawRect(QRect(ui.label_leftFill->width()+30, 10, paintWidth, this->height() - 21));
            }
            else {
                paint.drawRect(QRect(ui.label_leftFill->width()+30, 10, ui.widget_progress->width(), this->height() - 21));
                if (mSelectPer == mTotal) {
                    QString sheet = GreenRightFill;
						  if (ui.label_rightFill->styleSheet() != sheet) {
							  ui.label_rightFill->setStyleSheet(sheet);
						  }									 
                }
            }
        }
        else {
            QString sheet = GrayLeftFill;
            ui.label_leftFill->setStyleSheet(sheet);

            QColor color(221, 221, 221);
            paint.setPen(QPen(color, 1, Qt::SolidLine));//设置画笔形式 
            paint.setBrush(QBrush(color, Qt::SolidPattern));//设置画刷形式 
            int paintWidth = ((float)mSelectPer / (float)mTotal) * this->width();
            if (paintWidth < this->width()-60 - ui.label_leftFill->width() - ui.label_rightFill->width()) {
                paint.drawRect(QRect(ui.label_leftFill->width()+30, 10, paintWidth, this->height() - 21));
            }
            else {
                paint.drawRect(QRect(ui.label_leftFill->width()+30, 10, ui.widget_progress->width(), this->height() - 21));
                if (mSelectPer == mTotal) {
                    QString sheet = GrayRightFill;
						  if (ui.label_rightFill->styleSheet() != sheet) {
							  ui.label_rightFill->setStyleSheet(sheet);
						  }
					               
                }
            }
        }

    }
    else {
        QString sheet = LeftNoramlFill;		
		  if (ui.label_leftFill->styleSheet() != sheet) {
			  ui.label_leftFill->setStyleSheet(sheet);
		  }		
    }
    paint.end();
	 update();
	 QWidget::paintEvent(painter);
}
