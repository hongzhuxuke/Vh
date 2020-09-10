#include "MenuWdg.h"
#include "MenuItem.h"
#include <QGraphicsDropShadowEffect> 

MenuWdg::MenuWdg(QWidget *parent /*= Q_NULLPTR*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);

	mpEffect = new QGraphicsDropShadowEffect(this);
	mpEffect->setOffset(4, 6);
	mpEffect->setColor(QColor(0, 0, 0, 40));
	mpEffect->setBlurRadius(10);
	ui.widget->setGraphicsEffect(mpEffect);
}

MenuWdg::~MenuWdg()
{
	if (nullptr != mpEffect)
	{
		delete mpEffect;
		mpEffect = nullptr;
	}
	QMap<int, MenuItem*>::iterator it = mlistMenuItem.begin();
	for (; it!= mlistMenuItem.end(); it++)
	{
		MenuItem* pTem = it.value();
		if (nullptr != pTem)
		{
			delete pTem;
			pTem = nullptr;
			
		}
	}
	mlistMenuItem.clear();
}

void MenuWdg::SetShowItemCount(const int& iCount)
{
	miCount = iCount;
	QMargins margins = ui.vLayout->contentsMargins();
	setFixedHeight(30* miCount+ margins.top() + margins.bottom() + 20);
}

void MenuWdg::show(int x, int y)
{
	move(x, y);
	setFocus(); 	
    QDialog::show();
}

void MenuWdg::addItem(const QString& strText,
	const QString& strTextHover,
	const QString& strTextLeave,
	const int& iOpType,
	const QString& strIcoHover,
	const QString& strIcoLeave)
{
	MenuItem* pItem = nullptr; 
	pItem = new MenuItem(this);

	if (nullptr != pItem)
	{
		ui.vLayout->addWidget(pItem);
		connect(pItem, &MenuItem::sigClicked, this, &MenuWdg::sigClicked);
		connect(pItem, &MenuItem::sigClicked, this, &MenuWdg::close);
		mlistMenuItem.insert(iOpType, pItem);

		pItem->setHoverStyle(strIcoHover, MenuItem::eBtnId_icon);
		pItem->setLeaveStyle(strIcoLeave, MenuItem::eBtnId_icon);

		pItem->setHoverStyle(strTextHover, MenuItem::eBtnId_text);
		pItem->setLeaveStyle(strTextLeave, MenuItem::eBtnId_text);

		pItem->SetText(strText);
		pItem->SetOpType(iOpType);
	}
}

void MenuWdg::SetItemShow(const int& iOpType, const bool& bShow/* = true*/){
	QMap<int, MenuItem*>::iterator it = mlistMenuItem.find(iOpType);
	if(nullptr != it.value())
		bShow? it.value()->show(): it.value()->hide();
}

void MenuWdg::showEvent(QShowEvent *e) {
	setFocus();
	QDialog::showEvent(e);
}

void MenuWdg::enterEvent(QEvent *e) {
	QDialog::enterEvent(e);
	m_bEnter = true;
}
void MenuWdg::leaveEvent(QEvent *e) {
	QDialog::leaveEvent(e);
	m_bEnter = false;
}

void MenuWdg::focusOutEvent(QFocusEvent *e) {
	if (!m_bEnter) {
		hide();
		this->close();
		return;
	}

	QWidget *focusWidget = this->focusWidget();
	bool focusThis = false;

	while (focusWidget) {
		if (focusWidget == this) {
			focusThis = true;
			break;
		}

		focusWidget = focusWidget->parentWidget();
	}

	if (!focusThis) {
		this->close();
		m_bEnter = false;
	}
	else {
		this->setFocus();
	}
}
