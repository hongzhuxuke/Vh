#include "CTabItemDlg.h"

#include <QPropertyAnimation>
#include <QDebug>
#include <QGraphicsDropShadowEffect> 

CTabItemDlg::CTabItemDlg(QWidget *parent)
: CBaseWnd(parent)/* ,
mbTencentSDK(true)*/
{
   ui.setupUi(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);
   //setAttribute(Qt::WA_TranslucentBackground);
   //setAutoFillBackground(false);
   connect(ui.btnClose, &QPushButton::clicked, this, &CTabItemDlg::sig_doubleClickedTitleBar);

   //mpEffect = new QGraphicsDropShadowEffect(this);
   //mpEffect->setOffset(4, 6);
   //mpEffect->setColor(QColor(0, 0, 0, 40));
   //mpEffect->setBlurRadius(10);
   //ui.frame->setGraphicsEffect(mpEffect);
}

CTabItemDlg::~CTabItemDlg() {
	if (nullptr != mpEffect)
	{
		delete mpEffect;
		mpEffect = nullptr;
	}
}


QObjectList CTabItemDlg::GetChildren()
{
	return ui.frame->children();
}

void CTabItemDlg::Notice(const CSTDMapParam &)
{

}

void CTabItemDlg::SetTitle(const QString& strTitle)
{
	setWindowTitle(strTitle);
	ui.labTitle->setText( strTitle);
}


void CTabItemDlg::RemoveCenterWdg(QWidget* pWdg)
{
	if (nullptr != pWdg)
		ui.verticalLayout->removeWidget(pWdg);
}

void CTabItemDlg::AddCenterWdg(QWidget* pWdg)
{
	if (nullptr != pWdg)
	{
		ui.verticalLayout->addWidget(pWdg);
		//QString str = QString("#%1{border-bottom-left-radius : 10px; \
		//	border-bottom-right-radius: 10px; }").arg(pWdg->objectName());
		//pWdg->setStyleSheet(str);
	}

}

bool CTabItemDlg::event(QEvent * event)
{
#ifdef unix
	if (event->type() == QEvent::MouseButtonDblClick) {   //标题栏单击没反应，就单击窗口边缘替代吧
		emit sig_doubleClickedTitleBar();
		return true;
	}
#endif   
	if (event->type() == QEvent::MouseButtonDblClick)  
	{
		emit sig_doubleClickedTitleBar();
		return true;
	}
	return QWidget::event(event);
}