
//CTabBar.cpp
#include "CTabBar.h"
#include <QtGui>
 
CTabBar::CTabBar(QWidget *parent):QTabBar(parent),pressFlag(false)
{
 
}
 
//void CTabBar::currentChanged()
//{
//	int index = 0;
//	emit QTabBar::currentChanged(index);
//}

void CTabBar::slot_DoubleClicked()
{
	emit sig_tabDrag(currentIndex(), pos());
	qDebug() << "CTabBar::slot_DoubleClicked" << endl;
	//QTabBar::mouseDoubleClickEvent(event);
}

void CTabBar::mousePressEvent(QMouseEvent *event)
{    
    if (event->button()==Qt::LeftButton)
    {
        pressFlag = true;
		qDebug() << "CTabBar::mousePressEvent" << endl;
    }
    QTabBar::mousePressEvent(event);
}
 
void CTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&pressFlag && !mDoubleClicked)
    {
		if (!tabRect(currentIndex()).contains(event->pos()))
		{
			emit sig_tabDrag(currentIndex(), event->pos());
			qDebug() << "CTabBar::mouseReleaseEvent" << endl;
		}
        
    } 
	pressFlag = false;
	mDoubleClicked = false;
	QTabBar::mouseReleaseEvent(event);
}

void CTabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	mDoubleClicked = true;
	emit sig_tabDrag(currentIndex(), event->pos());
	qDebug() << "CTabBar::mouseDoubleClickEvent" << endl;
	QTabBar::mouseDoubleClickEvent(event);
}