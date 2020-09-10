#ifndef CTabItemDlg_H
#define CTabItemDlg_H
#include "..\Unility\WndManager.h"

#include "ui_CTabItemDlg.h"

class QGraphicsDropShadowEffect;
class CTabItemDlg : public CBaseWnd {
   Q_OBJECT

public:
   CTabItemDlg(QWidget *parent = 0);
   ~CTabItemDlg();
   QObjectList GetChildren();
   void Notice(const CSTDMapParam &);
   void SetTitle(const QString& strTitle);
   void AddCenterWdg(QWidget* pWdg);
   void RemoveCenterWdg(QWidget* pWdg);
signals:
	void sig_doubleClickedTitleBar();  //被双击时发射的信号
public slots:

protected:
	bool event(QEvent *);
private slots:

private:
	Ui::CTabItemDlg ui;
	QGraphicsDropShadowEffect* mpEffect = nullptr;
};

#endif // CTabItemDlg_H
