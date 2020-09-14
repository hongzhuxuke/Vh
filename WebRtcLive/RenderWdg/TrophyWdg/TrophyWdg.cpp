#include "TrophyWdg.h"
#include <QPropertyAnimation> 
#include <QTimer> 

TrophyWdg::TrophyWdg(QWidget *parent, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: QWidget(parent, f)
{
	ui.setupUi(this);
   // this->setWindowFlags(f);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);


	ui.labCount->setText(QString::number(miCurNum));
	ui.labAdd->setText("");
}

//TrophyWdg::TrophyWdg(QWidget *parent) 
//	: QWidget(parent)
//{
//	ui.setupUi(this);
//	//Qt::WindowFlags f = Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool;
//	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
//	setAttribute(Qt::WA_TranslucentBackground, true);
//	this->setAutoFillBackground(false);
//
//	ui.labCount->setText(QString::number(miCurNum));
//	ui.labAdd->setText("");
//}


TrophyWdg::~TrophyWdg()
{
	if (m_propertyAdmin != NULL) {
		delete m_propertyAdmin;
		m_propertyAdmin = NULL;
	}
}

void TrophyWdg::initTrophyRewardNum(const int& iNum)
{
	miCurNum = iNum;
	QString strNum = "";
	if (miCurNum > 99)
	{
		strNum = QStringLiteral("99+");
	}
	else
	{
		strNum = QString::number(miCurNum);
	}
	 
	ui.labCount->setText(strNum);
}

void TrophyWdg::hideLabAdd()
{
	ui.labAdd->hide();
}

int TrophyWdg::iTrophyCount()
{
	return miCurNum;
}

void TrophyWdg::SetStyleSheet(const QString& strQss)
{
	ui.widget->setStyleSheet(strQss);
}

void TrophyWdg::setTrophyAdd(const int& iNum)
{
	//miCurNum += iNum;
	initTrophyRewardNum(miCurNum + iNum);
	if (ui.labAdd->isVisible())
	{
		if (NULL==m_propertyAdmin)
		{
			m_propertyAdmin = new QPropertyAnimation(this, "geometry");
			m_propertyAdmin->setTargetObject(ui.labAdd);
			connect(m_propertyAdmin, &QPropertyAnimation::finished, this, &TrophyWdg::SlotAdminFinished);
		}

		ui.labAdd->setText(QStringLiteral("+") + QString::number(iNum));
		m_propertyAdmin->setStartValue(QRect(ui.labAdd->x(), ui.labAdd->y(), 0, 0));
		int iW = ui.labCount->height();
		m_propertyAdmin->setEndValue(QRect(ui.labAdd->x(), ui.labAdd->y(), iW, iW));
		m_propertyAdmin->setDuration(500);
		m_propertyAdmin->start();
	}

}

void TrophyWdg::SlotAdminFinished()
{
	QTimer::singleShot(1000, this, &TrophyWdg::slotHideTrophyAdd);
}

void TrophyWdg::slotHideTrophyAdd()
{
	ui.labAdd->setText("");
}