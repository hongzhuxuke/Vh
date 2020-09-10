#include "CWidget.h"
#include <QStyleOption> 
#include <QPainter> 

CWidget::CWidget(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
{
}

CWidget::~CWidget()
{
}

void CWidget::paintEvent(QPaintEvent *event)
{
	QStyleOption opt; // 需要头文件#include <QStyleOption>  
	opt.init(this);
	QPainter p(this); // 需要头文件#include <QPainter>  
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}
