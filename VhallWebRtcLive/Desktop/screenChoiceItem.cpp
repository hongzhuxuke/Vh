#include "stdafx.h"
#include "screenChoiceItem.h"
#include "../Unility/Unility.h"

extern ToolManager *globalToolManager;

screenChoiceItem::screenChoiceItem(QWidget *parent) :
	CWidget(parent) {
	ui.setupUi(this);
   connect(ui.btnFirt, &SelfDrawingButton::sigClicked, this, &screenChoiceItem::sig_Itemclicked);
   connect(ui.btnSecond, &SelfDrawingButton::sigClicked, this, &screenChoiceItem::sig_Itemclicked);

}

screenChoiceItem::~screenChoiceItem() {
}

void screenChoiceItem::SetScreenInfo(const int& sCreenIndex, int monitorIndex, VHD_WindowInfo& moniters)
{
   SelfDrawingButton* pTem = CurrentWdg( sCreenIndex);
   if (nullptr != pTem) {
      //pTem->SetDesktopIndex(id);
      pTem->SetDesktopInfo(monitorIndex, moniters);
   }
}

SelfDrawingButton* screenChoiceItem::CurrentWdg(const int& sCreenIndex)
{
   SelfDrawingButton* pTem = nullptr;
   if (eScreenIndex_First == sCreenIndex) {
      pTem = ui.btnFirt;
   }
   else if (eScreenIndex_Second == sCreenIndex) {
      pTem = ui.btnSecond;
   }
   return pTem;
}

void screenChoiceItem::setInvisible(const int& sCreenIndex, bool bInvisible )
{
   SelfDrawingButton* pTem = CurrentWdg(sCreenIndex);
   if (nullptr != pTem) {
      pTem->setEnabled(false);
   }
}

void screenChoiceItem::setHide(const int& sCreenIndex, bool bInvisible /*= true*/)
{
   SelfDrawingButton* pTem = CurrentWdg(sCreenIndex);
   if (nullptr != pTem) {
      pTem->hide();
      ui.label->hide();
      this->setFixedWidth(412);
   }
}

void screenChoiceItem::setItemWdgChecked(const int64_t& id)
{
   ui.btnFirt->setChecked(id);
   ui.btnSecond->setChecked(id);
}

void screenChoiceItem::setEnabled(bool b)
{
   CWidget::setEnabled(b);

   ui.btnFirt->setEnabled(b);
   ui.btnSecond->setEnabled(b);
}