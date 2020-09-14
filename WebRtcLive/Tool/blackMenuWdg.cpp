#include "BlackMenuWdg.h"
#include "cmapButton.h"

CPushButton::CPushButton(QWidget *parent /*= Q_NULLPTR*/)
	:QPushButton(parent)
{
	connect(this, &QPushButton::clicked, this, &CPushButton::slot_Clicked);
	
}

CPushButton::CPushButton(const QString &text, QWidget *parent /*= Q_NULLPTR*/)
	:QPushButton(text, parent)
{
	connect(this, &QPushButton::clicked, this, &CPushButton::slot_Clicked);

}

CPushButton::CPushButton(const QIcon &icon, const QString &text, QWidget *parent/* = Q_NULLPTR*/)
	: QPushButton(icon,  text, parent)
{
	connect(this, &QPushButton::clicked, this, &CPushButton::slot_Clicked);
}

CPushButton::~CPushButton()
{

}

void  CPushButton::SetOpType(const int& iOp)
{ 
	miOpType = iOp; 
}

void CPushButton::slot_Clicked()
{
	emit sigClicked(miOpType);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
BlackMenuWdg::BlackMenuWdg(QWidget *parent /*= Q_NULLPTR*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
	setFixedHeight(miFixHeight);
}

BlackMenuWdg::~BlackMenuWdg()
{
	QMap<int, CPushButton*>::iterator it = mlistMenuItem.begin();
	for (; it!= mlistMenuItem.end(); it++)
	{
		CPushButton* pTem = it.value();
		if (nullptr != pTem)
		{
			delete pTem;
			pTem = nullptr;
			
		}
	}
	mlistMenuItem.clear();
}

void BlackMenuWdg::show(int x, int y)
{
	move(x, y);
	setFocus(); 	
    QDialog::show();
}

void BlackMenuWdg::addItem(
	const int& iOpType,
	const QString& strStyleSheet, const bool& bCheckedable /*= true*/)
{
	miFixHeight += 30;
	setFixedHeight(miFixHeight);
	CPushButton* pItem = nullptr; 
	pItem = new CPushButton(this);

	if (nullptr != pItem)
	{
		pItem->setCheckable(bCheckedable);
		pItem->setFixedSize(95, 30);
		ui.vLayout->addWidget(pItem);
		connect(pItem, &CPushButton::sigClicked, this, &BlackMenuWdg::sigClicked);
		connect(pItem, &CPushButton::sigClicked, this, &BlackMenuWdg::close);
		mlistMenuItem.insert(iOpType, pItem);

		pItem->setStyleSheet(strStyleSheet);
		pItem->SetOpType(iOpType);
	}
}

void BlackMenuWdg::SetItemShow(const int& iOpType, const bool& bShow/* = true*/){
	QMap<int, CPushButton*>::iterator it = mlistMenuItem.find(iOpType);
	if(nullptr != it.value())
		bShow? it.value()->show(): it.value()->hide();
}

void BlackMenuWdg::SetItemChecked(const int& iOpType, const bool& bChecked/* = false*/)
{
	QMap<int, CPushButton*>::iterator it = mlistMenuItem.find(iOpType);
	if (it!= mlistMenuItem.end() && nullptr != it.value())
	{
		it.value()->setChecked(bChecked);
	}
		
}

void BlackMenuWdg::addTrigger(CMapButton* btnMap)
{
   if (nullptr != btnMap) {
      mlistMapBtn.append(btnMap);
      connect(btnMap, &CMapButton::sigfocusOut, this, &BlackMenuWdg::slotFoucusOutEvent);
   }
}

void BlackMenuWdg::setItemEnabled(const int& iOpType, const bool& bEnabled/* = true*/)
{
	QMap<int, CPushButton*>::iterator it = mlistMenuItem.find(iOpType);
	if (nullptr != it.value())
	{
		//it.value()->setChecked(false);
		it.value()->setEnabled(bEnabled);
	}
		
}

void BlackMenuWdg::showEvent(QShowEvent *e) {
	setFocus();
	QDialog::showEvent(e);
}

void BlackMenuWdg::enterEvent(QEvent *e) {
	QDialog::enterEvent(e);
	m_bEnter = true;
}
void BlackMenuWdg::leaveEvent(QEvent *e) {
	QDialog::leaveEvent(e);
	m_bEnter = false;
}

void BlackMenuWdg::slotFoucusOutEvent()
{
   bool bLeav = !m_bEnter;
   QList<CMapButton*>::iterator it = mlistMapBtn.begin();
   while (it != mlistMapBtn.end()) {
      if (*it) {
         bLeav &= (!((*it)->bEnter()));
      }
      it++;
   }

   if (bLeav) {
      hide();
      emit sigClose();
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
      emit sigClose();
      this->close();
      m_bEnter = false;
   }
   else {
      this->setFocus();
   }
}

void BlackMenuWdg::focusOutEvent(QFocusEvent *e) {
   slotFoucusOutEvent();
   QDialog::focusOutEvent(e);
}
