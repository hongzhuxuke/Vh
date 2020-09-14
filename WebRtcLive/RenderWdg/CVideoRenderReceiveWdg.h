#ifndef H_CVIDEORENDERRECEIVEWDG_H
#define H_CVIDEORENDERRECEIVEWDG_H

#include "CWidget.h"
#include "VideoRenderReceiver.h"
#include "ui_CVideoRenderReceiveWdg.h"
#include <QMutex>
#include <mutex>

class UIFrameReciver {
public:
   virtual void ReceiveVideo(const unsigned char* video, int length, int width, int height) = 0;
};


class RTCVideoReciver : public vhall::VideoRenderReceiveInterface {
public:
   virtual void ReceiveVideo(const unsigned char* video, int length, int width, int height);
   void SetReiver(UIFrameReciver* obj);
private:
   UIFrameReciver* mUIFrameReciver = nullptr;
   std::mutex mReciverMutex;
   std::atomic_bool mbRun;
};

class CVideoRenderReceiveWdg : public CWidget, public UIFrameReciver
{
	Q_OBJECT

public:
   CVideoRenderReceiveWdg(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~CVideoRenderReceiveWdg();

   std::shared_ptr<RTCVideoReciver> GetVideoReciver();

   typedef struct                       /**** BMP file header structure ****/
   {
      unsigned int   bfSize;           /* Size of file */
      unsigned short bfReserved1;      /* Reserved */
      unsigned short bfReserved2;      /* ... */
      unsigned int   bfOffBits;        /* Offset to bitmap data */
   } MyBITMAPFILEHEADER;
   typedef struct                       /**** BMP file info structure ****/
   {
      unsigned int   biSize;           /* Size of info header */
      int            biWidth;          /* Width of image */
      int            biHeight;         /* Height of image */
      unsigned short biPlanes;         /* Number of color planes */
      unsigned short biBitCount;       /* Number of bits per pixel */
      unsigned int   biCompression;    /* Type of compression to use */
      unsigned int   biSizeImage;      /* Size of image data */
      int            biXPelsPerMeter;  /* X pixels per meter */
      int            biYPelsPerMeter;  /* Y pixels per meter */
      unsigned int   biClrUsed;        /* Number of colors used */
      unsigned int   biClrImportant;   /* Number of important colors */
   } MyBITMAPINFOHEADER;
   virtual void ReceiveVideo(const unsigned char* video, int length, int width, int height);
   //virtual void ReceiveVideoBuff(BITMAPINFO* bmi);
   void SetWidthHeightRatio(const int& iW, const int& iH);
protected:
   void customEvent(QEvent* event);
   void paintEvent(QPaintEvent *event);

   void showEvent(QShowEvent *event);
   void resizeEvent(QResizeEvent *event);
private slots:
   void on_comboBox_currentIndexChanged(int index);
private:
   void reseizeEffective();
   Ui::VideoRenderReceiveWdg ui;
   QPixmap mpixmap;

   QImage::Format format = QImage::Format_Invalid;
   //BITMAPINFO* mbmi = nullptr;
   QMutex mutex;
   
   //int64_t miRec = 0;
   //int64_t mDisp = 0;
   //int miWidthRatio = 4;
   //int miHeightRatio = 3;

   //int miEffectiveW = 0;
   //int miEffectiveH = 0;

   //int miEffectiveX = 0;
   //int miEffectiveY = 0;

   int mPixW = 0;
   int mPixH = 0;

   int miEffectivePixW;
   int miEffectivePixH;

   int miEffectivePixX = 0;
   int miEffectivePixY = 0;

   QRect mEffectiveRect;
   QRect mEffectiveRectPix;

   std::shared_ptr<RTCVideoReciver> mRTCVideoReciver = nullptr;
};

#endif // !H_CVIDEORENDERRECEIVEWDG_H