//CTabWidget.cpp
#include "CTabWidget.h"
#include <QStyleOption> 
#include <QPainter> 
#include "CPaintLabel.h"

CTabWidget::CTabWidget(QWidget *parent):QTabWidget(parent)
{
	tabBar = new CTabBar;
	setTabBar(tabBar);     //�����ǹؼ��������������Զ����CTabBar�滻ԭ����QTabBar
	//tabBar->setMinimumWidth(1000);
	//connect(this, &CTabWidget::currentChanged, this, &CTabWidget::slotChanged);
}
 
void CTabWidget::setTabButton(int index, QTabBar::ButtonPosition position, CPaintLabel *widget)
{
	tabBar->setTabButton(index, position, widget);
	connect(widget, &CPaintLabel::sig_DoubleClicked, tabBar, &CTabBar::slot_DoubleClicked);
}

void CTabWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt; // ��Ҫͷ�ļ�#include <QStyleOption>  
	opt.init(this);
	QPainter p(this); // ��Ҫͷ�ļ�#include <QPainter>  
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int CTabWidget::GetTabHeight() {
   if (tabBar) {
      return tabBar->height();
   }
   return 0;
}