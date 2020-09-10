#ifndef _SCREENCHOICEITEM_H_
#define _SCREENCHOICEITEM_H_
#include "ui_screenChoiceItem.h"
#include "CWidget.h"


class screenChoiceItem : public CWidget
{
	Q_OBJECT

public:

	explicit screenChoiceItem(QWidget *parent = NULL);
	~screenChoiceItem();
	
   enum eScreenIndex
   {
      eScreenIndex_First = 0,
      eScreenIndex_Second,
   };

   void SetScreenInfo(const int& sCreenIndex, int monitorIndex, VHD_WindowInfo& moniters);
   void setInvisible(const int& sCreenIndex, bool bInvisible = true);
   void setHide(const int& sCreenIndex, bool bInvisible = true);
   void setItemWdgChecked(const int64_t& id);
   void setEnabled(bool b);
signals:
   void sig_Itemclicked(int id/*, CSCreenBtnWdg**/);

private slots:

private:
   SelfDrawingButton* CurrentWdg(const int& sCreenIndex);


	Ui::screenChoiceItem ui;
};

#endif
