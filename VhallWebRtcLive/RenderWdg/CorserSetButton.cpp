#include "CorserSetButton.h"

CorserSetButton::CorserSetButton(QWidget *parent /*= 0*/)
	:QPushButton(parent)
{

}

CorserSetButton::~CorserSetButton()
{

}

void CorserSetButton::enterEvent(QEvent *event)
{
	setCursor(QCursor(Qt::ArrowCursor));
	QPushButton::enterEvent(event);
}