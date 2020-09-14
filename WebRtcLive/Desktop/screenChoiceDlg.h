#pragma once

#include <QDialog>
#include <QGridLayout> 
#include <QPushButton>
#include <QLabel>
#include "../Unility/Unility.h"
#include "ui_screenChoiceDlg.h"
#include "CSCreenBtnWdg.h"
#include <vector>
#include "VHMonitorCapture.h"
using namespace std;

class screenChoiceDlg : public CBaseWnd
{
	Q_OBJECT

public:
	screenChoiceDlg(QWidget *parent = Q_NULLPTR);
	~screenChoiceDlg();
   void setFixedSize(int w, int h);

	int GetIndex();
	int64_t GetDesktopIndex();
	virtual void Notice(const CSTDMapParam &);
	void SetDesktopCapture(bool bDesktopCaptur);
	CREATE_WND();
   void initList();
   void setListWidgetEnabled(bool bEnable = true);
   void setEnabled(bool b);
	void Accept();
signals:
	void sig_btnclicked(int index);
	void sig_DesktopCapture(bool);
public slots:
	int exec();
   void on_btnClose_clicked(bool bChecked = false);
	void slot_OpenDesktopAudio(bool checked);
   void on_btnOk_clicked(bool bChecked = false);
   void on_btnCancle_clicked(bool bChecked = false);
protected:
	void paintEvent(QPaintEvent *e);
	bool eventFilter(QObject * watched, QEvent * event);
private slots:
	
	void slotBtnClicked(int index/*, CSCreenBtnWdg* pdlg*/);
private:
   bool GetScreenList(vector<VHD_WindowInfo>& allWindows);

   void clearList();

	//QList<CSCreenBtnWdg*> mListBtn;
	int mindex;
	int64_t miDesktopId;
	int miCount = 1;
	Ui::screenChoiceDlg ui;

	QPixmap* mPixmap = nullptr;
	CSCreenBtnWdg* mPCureBtn = nullptr;
};
