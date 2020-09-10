#ifndef H_SOFTWARESOURCEDLG_H
#define H_SOFTWARESOURCEDLG_H
#include <QDialog>
#include <QMap>

#include "ui_softwareSourceDlg.h"
#include "../Unility/Unility.h"

class softwareSourceDlg : public CBaseWnd {
   Q_OBJECT
public:
   softwareSourceDlg(QWidget *parent = 0);
   ~softwareSourceDlg();
   virtual void Notice(const CSTDMapParam &);
   void AddDeviceItem(wchar_t* wzDeviceName, QVariant dwUserData);
   bool GetCurItem(QVariant& UserData);
   //清空软件源列表
   void Clear();
   QString GetCurShareSoftware() { return mstrCurShareSoftware; }
   int GetSelectResult(){return miSelectResult;}
public slots:
	int exec();
protected:
	//void paintEvent(QPaintEvent *);
private slots:
   void OnSave();
public slots:
   void slotReFlush();
private:
	QMap<int, DesktopCaptureSource> mSource;
	Ui::softwareSourceDlg ui;
	QString mstrCurShareSoftware;
   int miSelectResult = VhallLive_ERROR;
};

#endif // H_SOFTWARESOURCEDLG_H
