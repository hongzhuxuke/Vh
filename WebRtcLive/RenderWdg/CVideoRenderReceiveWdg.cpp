#include "CVideoRenderReceiveWdg.h"
#include <QStyleOption> 
#include <QPainter> 
#include <QDebug>
#include <QDateTime> 
#include "../Unility/CustomEventType.h"


void RTCVideoReciver::ReceiveVideo(const unsigned char* video, int length, int width, int height) {
   std::unique_lock<std::mutex> lock(mReciverMutex);
   if (mUIFrameReciver && mbRun) {
      mUIFrameReciver->ReceiveVideo(video, length, width, height);
   }
}
void RTCVideoReciver::SetReiver(UIFrameReciver* obj) {
   std::unique_lock<std::mutex> lock(mReciverMutex);
   obj == nullptr ? mbRun = false : mbRun = true;
   mUIFrameReciver = obj;
}

/////////////////////////////////////////////

CVideoRenderReceiveWdg::CVideoRenderReceiveWdg(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
	: CWidget(parent)
{
   ui.setupUi(this);
   setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

   mRTCVideoReciver = std::make_shared<RTCVideoReciver>();
   mRTCVideoReciver->SetReiver(this);
   //ZeroMemory(&bmi_, sizeof(bmi_));
   //bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   //bmi_.bmiHeader.biPlanes = 1;
   //bmi_.bmiHeader.biBitCount = 32;
   //bmi_.bmiHeader.biCompression = BI_RGB;
   //bmi_.bmiHeader.biWidth = 1;
   //bmi_.bmiHeader.biHeight = -1;
   //bmi_.bmiHeader.biSizeImage = 1 * 1 * (bmi_.bmiHeader.biBitCount >> 3);
}

CVideoRenderReceiveWdg::~CVideoRenderReceiveWdg()
{
   mRTCVideoReciver->SetReiver(nullptr);
   mRTCVideoReciver.reset();
}

std::shared_ptr<RTCVideoReciver> CVideoRenderReceiveWdg::GetVideoReciver()
{
   return mRTCVideoReciver;
}

void CVideoRenderReceiveWdg::on_comboBox_currentIndexChanged(int index)
{
   format = (QImage::Format)index;
}

void CVideoRenderReceiveWdg::ReceiveVideo(const unsigned char* video, int length, int width, int height)
{
   //static int i = 0;
   ////if (i == 0) {
   //i++;
   
      mutex.lock();
      //QString str = QDateTime::currentDateTime().toLocalTime();
      //miRec++;
      unsigned char* szRgbData = new unsigned char[length + 1];
      memset(szRgbData, 0, length + 1);
      memcpy(szRgbData, video, length);
      int iNum = 3;

      MyBITMAPFILEHEADER bfh;
      MyBITMAPINFOHEADER bih;

      unsigned short bfType = 0x4d42;
      bfh.bfReserved1 = 0;
      bfh.bfReserved2 = 0;
      bfh.bfSize = 2 + sizeof(MyBITMAPFILEHEADER) + sizeof(MyBITMAPINFOHEADER) + width * height * iNum;
      bfh.bfOffBits = 0x36;

      bih.biSize = sizeof(MyBITMAPINFOHEADER);
      bih.biWidth = width;
      bih.biHeight = height;
      bih.biPlanes = 1;
      bih.biBitCount = 8 * iNum;
      bih.biCompression = 0;
      bih.biSizeImage = 0;
      bih.biXPelsPerMeter = width/*5000*/;
      bih.biYPelsPerMeter = height/*5000*/;
      bih.biClrUsed = 0;
      bih.biClrImportant = 0;

      int len = sizeof(bfType) + sizeof(bfh) + sizeof(bih) + width * height * iNum;

      BmpEvent* dataEvent = nullptr;
      dataEvent = new BmpEvent(CustomEvent_ReciveBmpData, len + 1, width,  height);
      if (nullptr != dataEvent && nullptr != dataEvent->mStrData) {
         unsigned char* temp = dataEvent->mStrData;

         memcpy(temp, &bfType, sizeof(bfType));
         temp += sizeof(bfType);

         memcpy(temp, &bfh, sizeof(bfh));
         temp += sizeof(bfh);
         memcpy(temp, &bih, sizeof(bih));
         temp += sizeof(bih);


         for (int i = height - 1; i >= 0; i--)
         {
            unsigned char* rgbTemp = szRgbData;
            rgbTemp += (i * width*iNum);
            memcpy(temp, rgbTemp, width* iNum);
            temp += width * iNum;
         }

         QCoreApplication::postEvent(this, dataEvent);
      }
      delete[]szRgbData;

      //qDebug() << "Receive:" << i << endl;

      mutex.unlock();
      
   //}
   //else if (i == 5)
   //{
   //   i = -1;
   //}
   //
   //i++;
}

void CVideoRenderReceiveWdg::SetWidthHeightRatio(const int & iW, const int & iH)
{
   //miWidthRatio = iW;
   //miHeightRatio = iH;
   reseizeEffective();
}

void CVideoRenderReceiveWdg::customEvent(QEvent* event) {

   if (event) {
      switch (event->type())
      {
      case CustomEvent_ReciveBmpData:
      {
         BmpEvent* msgEvent = (BmpEvent*)event;

         if (this->isVisible()) {

            mpixmap.loadFromData(msgEvent->mStrData, msgEvent->mIlen, "BMP");
            //mpixmap.save("123.png");
            if (mPixW != mpixmap.width() || mPixH != mpixmap.height()) {
               mPixW = mpixmap.width();
               mPixH = mpixmap.height();
               reseizeEffective(); 
            }            
            update();
         }
         break;
      }
      default: break;
      }
   }
   CWidget::customEvent(event);
}

void CVideoRenderReceiveWdg::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);

   painter.setBrush(Qt::black); //设定填充颜色
   painter.setPen(Qt::black); //设定画笔颜色，到时侯就是边框颜色
   painter.drawRect(this->rect());

   //painter.drawPixmap(mEffectiveRect, mpixmap.copy(0, 0, mpixmap.width(), mpixmap.height()));
   painter.drawPixmap(mEffectiveRectPix, mpixmap.copy(0, 0, mpixmap.width(), mpixmap.height()));

   CWidget::paintEvent(event);
}

void CVideoRenderReceiveWdg::showEvent(QShowEvent * event)
{

   reseizeEffective();
   CWidget::showEvent(event);
}

void CVideoRenderReceiveWdg::resizeEvent(QResizeEvent * event)
{
   reseizeEffective();
   CWidget::resizeEvent(event);
}

void CVideoRenderReceiveWdg::reseizeEffective()
{
   int iCurW = this->width();
   int iCurH = this->height();

   //单位化
   //double wRatd = 1.0 * iCurW / miWidthRatio;
   //double hRatd = 1.0 * iCurH / miHeightRatio;

   
   //if (hRatd < wRatd) {
   //   miEffectiveW = hRatd * miWidthRatio;
   //   miEffectiveH = iCurH;
   //}
   //else if (hRatd = wRatd) {
      //miEffectiveW = iCurW;
      //miEffectiveH = iCurH;
   //}
   //else {
   //   miEffectiveW = iCurW;
   //   miEffectiveH = wRatd * miHeightRatio;;
   //}

   //miEffectiveX = 0.5*(iCurW- miEffectiveW);
   //miEffectiveY = 0.5*(iCurH - miEffectiveH);

   //mEffectiveRect.setRect(miEffectiveX, miEffectiveY, miEffectiveW, miEffectiveH);
   mEffectiveRect.setRect(0, 0, iCurW, iCurH);



   if (mPixW > 0 && mPixH > 0) {
      //单位化
      //double wRatd = 1.0 * mPixW / miWidthRatio;
      //double hRatd = 1.0 * mPixH / miHeightRatio;

      double wRatd = 1.0 * mPixW / iCurW;
      double hRatd = 1.0 * mPixH / iCurH;

      if (hRatd < wRatd) {
         //miEffectivePixW = miEffectiveW;
         //miEffectivePixH = mPixH * miEffectiveW / mPixW;
         miEffectivePixW = iCurW;
         miEffectivePixH = mPixH * iCurW / mPixW;
      }
      else if (hRatd == wRatd) {
         //miEffectivePixW = miEffectiveW;
         //miEffectivePixH = miEffectiveH;
         miEffectivePixW = iCurW;
         miEffectivePixH = iCurH;
      }
      else {
         //miEffectivePixW = mPixW * miEffectiveH / mPixH;
         //miEffectivePixH = miEffectiveH;
         miEffectivePixW = mPixW * iCurH / mPixH;
         miEffectivePixH = iCurH;
      }

      //miEffectivePixX = miEffectiveX + 0.5* (miEffectiveW - miEffectivePixW);
      //miEffectivePixY = miEffectiveY + 0.5 * (miEffectiveH - miEffectivePixH);
      miEffectivePixX =  0.5* (iCurW - miEffectivePixW);
      miEffectivePixY =  0.5 * (iCurH - miEffectivePixH);
       
	  mEffectiveRectPix.setRect(miEffectivePixX, miEffectivePixY, miEffectivePixW, miEffectivePixH);
   }
}
