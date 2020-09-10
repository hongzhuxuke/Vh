#ifndef SELFDRAWINGBUTTON_H
#define SELFDRAWINGBUTTON_H

#include <QPushButton>
#include <QMap>
#include <QTimer>
#include "VHMonitorCapture.h"

class QPainter;
class QMouseEvent;

class SelfDrawingButton : public QPushButton {
   Q_OBJECT
public:
   explicit SelfDrawingButton(QWidget *parent = 0);
   ~SelfDrawingButton();
   void loadPixmap(int iState, QString pic_name);
   void loadPixmap(int iState, int id);
   void GrapPixMap(int iState, int id);

   void SetEnabled(bool);
   virtual void setEnabled(bool b);

   enum ButtonStatus { NORMAL, ENTER, PRESS, NOSTATUS };
   void setChecked(const int64_t& Destopid);


   //int DesktopIndex();
   //void SetDesktopIndex(int64_t Destopid);
   int GetCurrentIndex();
   void SetDesktopInfo(int monitorIndex, VHD_WindowInfo& moniters);
   VHD_WindowInfo GetCurrentWindowInfo();

   

signals:
   void sigClicked(int64_t iDestopid);
   void sigEnterIn();
protected:
   void enterEvent(QEvent *);
   void leaveEvent(QEvent *);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void paintEvent(QPaintEvent *);
private slots:
	void slot_ReCaptureScreen();
private:
	void setSate(int iState);
   
   ButtonStatus mStatus;
   QMap<int, QPixmap> mMapPixmap;

   QTimer* mpScreenCaptureTimer = nullptr;
   bool mMousePressed = false;
   //QScreen * mCurscreen = nullptr;
   //int mCurscreenId;
   //int64_t m_iDestopid;

   HDC mCaptureDC = nullptr;
   HDC mMemDC = nullptr;

   int mMonitorIndex;
   VHD_WindowInfo mMoniterInfo;
};

#endif // SELFDRAWINGBUTTON_H
