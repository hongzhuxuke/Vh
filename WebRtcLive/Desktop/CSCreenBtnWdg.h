#ifndef CSCREENBTNWDG_H
#define CSCREENBTNWDG_H

#include "CWidget.h"

#include "ui_CSCreenBtnWdg.h"

class CSCreenBtnWdg: public CWidget
{
    Q_OBJECT

public:
    explicit CSCreenBtnWdg(QWidget *parent = 0);
    ~CSCreenBtnWdg();
   //void SetIndex(const int &index);
   //void SetChecked(bool bChecked = true);
   void SetDisplayText(const QString &str);
   void SetDesktopIndex(int64_t Destopid);
   int DesktopIndex();
   void setChecked(const int64_t& Destopid);
   void setCurEnabled(const bool& bEnable);
signals:
    void sig_clicked(int id/*, CSCreenBtnWdg**/);

protected:
private slots:
   void slot_btn_clicked(bool checked = true);
private:
	int64_t m_iDestopid;
	Ui::CSCreenBtnWdg ui;
	
};

#endif // CSCREENBTNWDG_H