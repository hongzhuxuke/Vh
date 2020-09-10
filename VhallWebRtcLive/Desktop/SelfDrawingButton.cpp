#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget> 
#include "SelfDrawingButton.h"
#include <QGuiApplication> 
#include <QScreen> 
#include <QtWin>
#include "../Unility/Unility.h"

extern ToolManager *globalToolManager;
SelfDrawingButton::SelfDrawingButton(QWidget *parent)
: QPushButton(parent) {
   mStatus = NORMAL;
   mMousePressed = false;
   this->setFocusPolicy(Qt::StrongFocus);
}

SelfDrawingButton::~SelfDrawingButton() {
	if (nullptr != mpScreenCaptureTimer)
	{
		if (mpScreenCaptureTimer->isActive())
		{
			mpScreenCaptureTimer->stop();
			delete mpScreenCaptureTimer;
			mpScreenCaptureTimer = nullptr;
		}
	}
}


//int SelfDrawingButton::DesktopIndex()
//{
//   return m_iDestopid;
//}
//
//void SelfDrawingButton::SetDesktopIndex(int64_t Destopid)
//{
//   m_iDestopid = Destopid;
//   loadPixmap(SelfDrawingButton::NORMAL, Destopid);
//   loadPixmap(SelfDrawingButton::ENTER, ":/button/img/button/DesktopSharping_select.png");
//}

int SelfDrawingButton::GetCurrentIndex()
{
   return mMonitorIndex;
}

void SelfDrawingButton::SetDesktopInfo(int monitorIndex, VHD_WindowInfo& moniters)
{
   mMonitorIndex = monitorIndex;
   mMoniterInfo = moniters;

   //setSate(iState);
   if (nullptr == mpScreenCaptureTimer)
   {
      mpScreenCaptureTimer = new QTimer;
      connect(mpScreenCaptureTimer, &QTimer::timeout, this, &SelfDrawingButton::slot_ReCaptureScreen);
   }

   if (nullptr != mpScreenCaptureTimer)
   {
      mpScreenCaptureTimer->start(100);
   }

   loadPixmap(SelfDrawingButton::ENTER, ":/button/img/button/DesktopSharping_select.png");
}

VHD_WindowInfo SelfDrawingButton::GetCurrentWindowInfo()
{
   return mMoniterInfo;
}

void SelfDrawingButton::setChecked(const int64_t& Destopid)
{
   setCheckable(true);
   QPushButton::setChecked(mMonitorIndex == (int)Destopid);
}

void SelfDrawingButton::loadPixmap(int iState, QString pic_name) {
	QPixmap  Pixmap(pic_name);
	mMapPixmap[iState] = Pixmap;
}

void SelfDrawingButton::GrapPixMap(int iState, int id)
{
   globalToolManager->GetDataManager()->WriteLog("%s Enter", __FUNCTION__);
   QScreen * Curscreen = nullptr;
   QList < QScreen * > list = QGuiApplication::screens();
   //mCurscreenId = id;
   if (id < list.count())
   {
      Curscreen = list.at(id);
   }
   else
   {
      Curscreen = list.at(0);
   }
   mMapPixmap[iState] = Curscreen->grabWindow(0);
   globalToolManager->GetDataManager()->WriteLog("%s Leave", __FUNCTION__);
}

void SelfDrawingButton::loadPixmap(int iState, int id)
{
   //GrapPixMap(iState, id);
	//QString filePathName = QString("%1.jpg").arg(id);
	//bool  bS = mMapPixmap[iState].save(filePathName, "jpg");

	setSate(iState);
	if (nullptr == mpScreenCaptureTimer)
	{
		mpScreenCaptureTimer = new QTimer;
		connect(mpScreenCaptureTimer, &QTimer::timeout, this, &SelfDrawingButton::slot_ReCaptureScreen);
	}

	if (nullptr!=mpScreenCaptureTimer)
	{
		mpScreenCaptureTimer->start(100);
	}
}

void SelfDrawingButton::slot_ReCaptureScreen()
{
	////if (nullptr != mCurscreen)
	////{
	////	mMapPixmap[NORMAL] = mCurscreen->grabWindow(0);
	////	repaint();
	////}

 //  GrapPixMap(NORMAL, mCurscreenId);
 //  repaint();

   if (mCaptureDC == nullptr) {
      mCaptureDC = GetDC(NULL);
   }
   if (mMemDC == nullptr) {
      mMemDC = CreateCompatibleDC(mCaptureDC);
   }

   int width = mMoniterInfo.rect.right - mMoniterInfo.rect.left;
   int height = mMoniterInfo.rect.bottom - mMoniterInfo.rect.top;

   int bytes_per_row = width * 4;
   int buffer_size = bytes_per_row * height;
   // Describe a device independent bitmap (DIB) that is the size of the desktop.
   //BITMAPINFO bmi = {};
   //bmi.bmiHeader.biHeight = -height;
   //bmi.bmiHeader.biWidth = width;
   //bmi.bmiHeader.biPlanes = 1;
   //bmi.bmiHeader.biBitCount = 4 * 8;
   //bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
   //bmi.bmiHeader.biSizeImage = bytes_per_row * height;
   //void* data = nullptr;
   //HANDLE hMySharedMapFile = CreateFileMapping(((HANDLE)0xFFFFFFFF), NULL, PAGE_READWRITE, 0, buffer_size, L"MySharedMem");
   //HBITMAP hBmp = CreateDIBSection(mCaptureDC, &bmi, DIB_RGB_COLORS, &data, hMySharedMapFile, 0);
   HGDIOBJ previous_object = nullptr;

   if (mMemDC) {
      HBITMAP hBmp = CreateCompatibleBitmap(mCaptureDC, width, height);
      if (hBmp) {
         previous_object = SelectObject(mMemDC, hBmp);
         BitBlt(mMemDC, 0, 0, width, height, mCaptureDC, mMoniterInfo.rect.left, mMoniterInfo.rect.top, CAPTUREBLT | SRCCOPY);
         SelectObject(mMemDC, previous_object);
         mMapPixmap[NORMAL] = QtWin::fromHBITMAP(hBmp);
         DeleteObject(hBmp);
      }
      DeleteObject(mMemDC);
      mMemDC = nullptr;
   }
   if (mCaptureDC) {
      DeleteObject(mCaptureDC);
      mCaptureDC = nullptr;
   }
   repaint();
}

void SelfDrawingButton::SetEnabled(bool ok) {
   if(ok) {
	   setSate(NORMAL);
   }
   else {
	   setSate(NOSTATUS);
   }
   update();
   this->repaint();
}

void SelfDrawingButton::setEnabled(bool b)
{
   QPushButton::setEnabled(b);
   globalToolManager->GetDataManager()->WriteLog("%s b:%d isEnabled:%d", __FUNCTION__, b, isEnabled());
}


void SelfDrawingButton::enterEvent(QEvent *) {
   if(mStatus==NOSTATUS) {
      return ;
   }
   setSate(ENTER);
   update();
   this->repaint();
   bool bEnable =  this->isEnabled();
   setEnabled(true);
   bEnable = this->isEnabled();
   emit sigEnterIn();
}

void SelfDrawingButton::setSate(int iState)
{
	mStatus = (ButtonStatus)iState;
	//mPpixmap = &mMapPixmap[iState];
	//mBtnWidth = mPpixmap->width();
	//mBtnHeight = mPpixmap->height();
}
void SelfDrawingButton::leaveEvent(QEvent *) {
   
   if(mStatus==NOSTATUS) {
      return ;
   }
   
   setSate(NORMAL);
   update();
   
   this->repaint();
}

void SelfDrawingButton::mousePressEvent(QMouseEvent *event) {
   if(mStatus==NOSTATUS) {
      return ;
   }
   if (event->button() == Qt::LeftButton) {
      mMousePressed = true;
      //mStatus = PRESS;
	  setSate(NORMAL);
      update();
   }
}

void SelfDrawingButton::mouseReleaseEvent(QMouseEvent *) {
   if (mMousePressed) {
      mMousePressed = false;
      //mStatus = ENTER;
	  setSate(ENTER);
      update();
      emit clicked();
   }
   emit sigClicked(mMonitorIndex);
}
#define BORDERWITH 10
void SelfDrawingButton::paintEvent(QPaintEvent * e) {
   QPushButton::paintEvent(e);
   QPainter painter(this);
   setEnabled(true);
   
	if (isEnabled() && ENTER == mStatus || this->isChecked()){
		painter.drawPixmap(rect(), mMapPixmap[ENTER].copy(0, 0, mMapPixmap[ENTER].width(), mMapPixmap[ENTER].height()));
		painter.drawPixmap(rect().x()+ BORDERWITH, rect().y() +BORDERWITH, rect().width() - 2*BORDERWITH, rect().height() - 2*BORDERWITH,
			mMapPixmap[NORMAL].copy(0, 0, mMapPixmap[NORMAL].width(), mMapPixmap[NORMAL].height()));
	}
	else if(isEnabled()){
		painter.drawPixmap(rect(),
			mMapPixmap[NORMAL].copy(0, 0, mMapPixmap[NORMAL].width(), mMapPixmap[NORMAL].height()));
	}
   else {
      painter.drawPixmap(rect(), mMapPixmap[ENTER].copy(0, 0, mMapPixmap[ENTER].width(), mMapPixmap[ENTER].height()));
      painter.drawPixmap(rect().x() + BORDERWITH, rect().y() + BORDERWITH, rect().width() - 2 * BORDERWITH, rect().height() - 2 * BORDERWITH,
         mMapPixmap[NORMAL].copy(0, 0, mMapPixmap[NORMAL].width(), mMapPixmap[NORMAL].height()));
   }

   
}
