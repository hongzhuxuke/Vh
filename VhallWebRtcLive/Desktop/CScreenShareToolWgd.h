#ifndef CSCREENSHARETOOLWGD_H
#define CSCREENSHARETOOLWGD_H

#include <QWidget>
#include <QTimer>
#include <QCloseEvent> 

#include "ui_CScreenShareToolWgd.h"

#define DEF_WDG_WIDTH   67

class QPropertyAnimation;
class ShortcutWdg;

class CScreenShareToolWgd : public QWidget {
   Q_OBJECT

public:
   CScreenShareToolWgd(QWidget *parent = 0);
   ~CScreenShareToolWgd();
   void SetScreenTitle(const QString& strTile);
   void SetSCreenType(const int& iType);
   void StopRoll();

signals:
	void sigStopScreenSharing();
	void sigChangeScreen();
	void sigShowChat(const QPoint& posShow);
	void sigShowMember(const QPoint& posShow);
   void sigOpMediaType(const int& iOptype, const QPoint& posShow);
   void sigCameraClicked(bool bOpenState);
   void sigMicClicked();
public slots:
	void on_btnStop_clicked(bool checked = false);
	void on_btnRetract_clicked(bool checked = false);
	void on_btnOpen_clicked(bool checked = false);
	void on_btnQuikTool_clicked(bool checked = false);
   void on_btnCamera_clicked(bool checked = false);
   void on_btnMic_clicked(bool checked = false);

	void slots_MediaMenu(const int& iOptype);

   void slotcameraStateChange(bool bState);
   void slotMicStateChange(bool bState);

   bool close();
   void hide();
protected:
   void closeEvent(QCloseEvent*);
private slots:
  

	void slot_moveText();
private:
	void StartRoll();
	

   Ui::CScreenShareToolWgd ui;
   bool m_bEnterEvent = false;

   QTimer *mPMoveTimer = nullptr;
   ShortcutWdg* mpShortcutWdg = nullptr;
   bool mbCameraOpen = true; 
};

#endif // CSCREENSHARETOOLWGD_H
