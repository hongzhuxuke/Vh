
#include "CSCreenBtnWdg.h"
#include "../Unility/Unility.h"

extern ToolManager *globalToolManager;

CSCreenBtnWdg::CSCreenBtnWdg(QWidget *parent) :
	CWidget(parent)
{
    ui.setupUi(this);
	//ui.btn->setFocusPolicy(Qt::NoFocus);
	ui.label->hide();
   connect(ui.btn, &SelfDrawingButton::clicked, this, &CSCreenBtnWdg::slot_btn_clicked);
   this->setEnabled(true);
}

CSCreenBtnWdg::~CSCreenBtnWdg()
{

}

void CSCreenBtnWdg::SetDisplayText(const QString &str)
{
	ui.label->setText(str);
}

//void CSCreenBtnWdg::SetChecked(bool bChecked /*= true*/)
//{
//	//if (bChecked)
//	//	ui.btn->setStyleSheet(SELECT_STYLE);
//	//else
//	//	ui.btn->setStyleSheet(DEFAULT_STYLE);
//}

void CSCreenBtnWdg::SetDesktopIndex(int64_t Destopid)
{
	m_iDestopid = Destopid;
	ui.btn->loadPixmap(SelfDrawingButton::NORMAL, Destopid);
	ui.btn->loadPixmap(SelfDrawingButton::ENTER, ":/button/img/button/DesktopSharping_select.png");
}

int CSCreenBtnWdg::DesktopIndex()
{
	return m_iDestopid;
}

void CSCreenBtnWdg::setChecked(const int64_t& Destopid)
{
   ui.btn->setCheckable(true);
   ui.btn->setChecked(m_iDestopid== Destopid);
}

void CSCreenBtnWdg::setCurEnabled(const bool& bEnable)
{
   this->setEnabled(bEnable);
   ui.btn->setEnabled(bEnable);
}

void CSCreenBtnWdg::slot_btn_clicked(bool checked /*= true*/)
{

	emit sig_clicked(m_iDestopid/*, this*/);
}
