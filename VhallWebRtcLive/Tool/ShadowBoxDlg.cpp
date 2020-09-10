
#include "ShadowBoxDlg.h"
#include <QDesktopWidget> 
#include <QDebug>



ShadowBoxDlg::ShadowBoxDlg(QWidget *parent) :
   QDialog(parent)
{
    ui.setupUi(this);
	//setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint 
      | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
      | Qt::WindowMaximizeButtonHint);

	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(true);
}

ShadowBoxDlg::~ShadowBoxDlg()
{

}

void ShadowBoxDlg::SetTitleChanged(const QString& strTitle)
{
   setWindowTitle(strTitle);
}

void ShadowBoxDlg::setLefRightBoerderWidth(int iW)
{
   ui.labRight->setFixedWidth(iW);
   ui.labLeft->setFixedWidth(iW);
}

int ShadowBoxDlg::GetChildGlobalX()
{
   int iX = this->x() + ui.backframe->x();
   return iX;
}

int ShadowBoxDlg::GetChildGlobalY()
{
   int iX = this->y() + ui.backframe->y();
   return iX;
}

int ShadowBoxDlg::GetChildX()
{
   return ui.backframe->x();;
}

int ShadowBoxDlg::GetChildY()
{
   return ui.backframe->y();
}

void ShadowBoxDlg::moveXY()
{
	QRect rect = QApplication::desktop()->availableGeometry();

	int iDW = rect.width();
	int iDH = rect.height();
	int iW = width();
	int iH = height();

	int iX = 0.5 * (iDW - iW);
	int iY = 0.5 * (iDH - iH);

	this->move(iX, iY);
}

void ShadowBoxDlg::resizeEvent(QResizeEvent *event)
{	
   QDialog::resizeEvent(event);
	moveXY();
}

void ShadowBoxDlg::showEvent(QShowEvent *event)
{
   QDialog::showEvent(event);
	moveXY();
   //mCefTestView->LoadUrl(m_TestUrl);
}
