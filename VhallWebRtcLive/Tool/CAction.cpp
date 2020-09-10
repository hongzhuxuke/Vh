#include "CAction.h"
#include <QEvent>

CAction::CAction(QObject *parent/* = nullptr*/)
	:QAction(parent){
	connect(this, &CAction::hovered, this, &CAction::hover);
}

CAction::CAction(const QString &text, QObject *parent/* = nullptr*/)
	: QAction(text, parent) {
	connect(this, &CAction::hovered, this, &CAction::hover);
}

CAction::CAction(const QIcon &icon, const QString &text, QObject *parent/* = nullptr*/)
	: QAction(icon, text, parent) {
	connect(this, &CAction::hovered, this, &CAction::hover);
}

CAction::~CAction(){

}

void CAction::setstrIcon(QString strFile) {
	mstrIcon = strFile;
	setIcon(QIcon(strFile));
}

void CAction::showIcon()
{
	setIcon(QIcon(mstrIcon));
}

void CAction::setstrHoverIcon(QString strFile){
	mstrHoverIcon = strFile;
}

void CAction::hover()
{
	setIcon(QIcon(mstrHoverIcon));
	
	emit sig_Hover(this);
}

bool CAction::eventFilter(QObject *watched, QEvent *event){

	if (QEvent::Enter == event->type())
	{
		int i = 0;
	}
	else if (QEvent::Leave == event->type())
	{
		int i = 0;
	}
	return QAction::eventFilter(watched, event);
}

