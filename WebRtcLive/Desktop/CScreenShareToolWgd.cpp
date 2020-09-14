#include "CScreenShareToolWgd.h"

#include <QPropertyAnimation>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "priConst.h"
#include <QWidgetAction>
//#include "MainUIIns.h"
#include "Msg_MainUI.h"
#include "ShortcutWdg.h"
#include "../Unility/Unility.h"
#define CHATSHAR  QStringLiteral("ÌÖÂÛ")
#define MEMBERSHAR QStringLiteral("³ÉÔ±")

#define QSS_STYLE_NET_BAD "#label_flag{\
border-image: url(:/button/img/button/netState_difference.png);\
      }"

#define QSS_STYLE_NET_EXELLENT "#label_flag{\
border-image: url(:/button/img/button/netState_excellent.png);\
      }"

#define QSS_STYLE_NET_GOOD "#label_flag{\
border-image: url(:/button/img/button/netState_good.png);\
      }"

#define QSS_STYLE_CAMERA_OPEN "#btnCamera{\
color: rgb(255, 255, 255);\
   border-image: url(:/menu/img/Menu/cameraOpen.png);\
border 1px solid;\
}\
#btnCamera:hover{\
background-color:rgb(0,0,0,180);\
   border-image: url(:/menu/img/Menu/cameraOpenHover.png);\
}\
#btnCamera : pressed{\
background-color: rgb(61, 61, 61);\
}"

#define QSS_STYLE_CAMERA_CLOSE "#btnCamera{\
color: rgb(255, 255, 255);\
   border-image: url(:/menu/img/Menu/cameraClose.png);\
border 1px solid;\
}\
#btnCamera:hover{\
background-color:rgb(0,0,0,180);\
   border-image: url(:/menu/img/Menu/cameraCloseHover.png);\
}\
#btnCamera : pressed{\
background-color: rgb(61, 61, 61);\
}"

#define QSS_STYLE_MIC_OPEN "#btnMic{\
color : rgb(255, 255, 255);\
border-image: url(:/menu/img/Menu/mic.png);\
border 1px solid;\
}\
#btnMic:hover{\
background-color:rgb(0,0,0,180);\
   border-image: url(:/menu/img/Menu/micHover.png);\
}\
#btnMic : pressed{\
background-color: rgb(61, 61, 61);\
}"

#define QSS_STYLE_MIC_CLOSE "#btnMic{\
color : rgb(255, 255, 255);\
border-image: url(:/menu/img/Menu/MicClose.png);\
border 1px solid;\
}\
#btnMic:hover{\
background-color:rgb(0,0,0,180);\
   border-image: url(:/menu/img/Menu/MicCloseHover.png);\
}\
#btnMic : pressed{\
background-color: rgb(61, 61, 61);\
}"

extern ToolManager *globalToolManager;

CScreenShareToolWgd::CScreenShareToolWgd(QWidget *parent)
: QWidget(parent)/* ,
mbTencentSDK(true)*/
{
   ui.setupUi(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool | Qt::WindowStaysOnTopHint);
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);

   ui.btnOpen->hide();
   setAttribute(Qt::WA_TranslucentBackground);
   ui.btnStop->setFocusPolicy(Qt::NoFocus);
   ui.btnChangeScreen->setFocusPolicy(Qt::NoFocus);
   ui.btnQuikTool->setFocusPolicy(Qt::NoFocus);
   ui.btnOpen->setFocusPolicy(Qt::NoFocus);
   ui.btnRetract->setFocusPolicy(Qt::NoFocus);
   connect(ui.btnChangeScreen, &QPushButton::clicked, this, &CScreenShareToolWgd::sigChangeScreen);
  
   mpShortcutWdg = new ShortcutWdg(this);
   if (nullptr != mpShortcutWdg)
   {
	   connect(mpShortcutWdg, SIGNAL(sig_click(const int&)), this, SLOT(slots_MediaMenu(const int&)));
   }
   setWindowTitle(QStringLiteral("Î¢ºð¿ÎÌÃ"));
   setFixedWidth(680);
   if (JOIN_ROLE_Teacher == globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type) {
      ui.widget->setFixedWidth(160);
   }
   else {
      ui.widget->setFixedWidth(100);
   }
}

CScreenShareToolWgd::~CScreenShareToolWgd() {

}

void CScreenShareToolWgd::slots_MediaMenu(const int& iOptype)
{
	QPoint posTem = ui.btnQuikTool->pos();
	QPoint posShow = mapToGlobal(posTem);
	posShow += QPoint(0, height() + 15);
	mpShortcutWdg->hide();
   switch (iOptype)
   {
   case ShortcutWdg::OpMediaType_ChatShare: emit sigShowChat(posShow); break;
   case ShortcutWdg::OpMediaType_Member: emit sigShowMember(posShow); break;
   //case  ShortcutWdg::OpMediaType_Testpaper: emit sigOpMediaType(iOptype, posShow);; break;
   //case  ShortcutWdg::OpMediaType_Signin: emit sigOpMediaType(iOptype, posShow); break;
   //case  ShortcutWdg::OpMediaType_answer: emit sigOpMediaType(iOptype, posShow); break;
   //case  ShortcutWdg::OpMediaType_time: emit sigOpMediaType(iOptype, posShow); break;
   default:
      emit sigOpMediaType(iOptype, posShow);
      break;
   }

}

void CScreenShareToolWgd::slotcameraStateChange(bool bState)
{
   mbCameraOpen = bState;
   if (bState) {
      ui.btnCamera->setStyleSheet(QSS_STYLE_CAMERA_OPEN);
   }
   else{
      ui.btnCamera->setStyleSheet(QSS_STYLE_CAMERA_CLOSE);
   }
}

void CScreenShareToolWgd::slotMicStateChange(bool bState)
{
   if (bState) {
      ui.btnMic->setStyleSheet(QSS_STYLE_MIC_OPEN);
   }
   else {
      ui.btnMic->setStyleSheet(QSS_STYLE_MIC_CLOSE);
   }
}

bool CScreenShareToolWgd::close()
{
   return QWidget::close();
}

void CScreenShareToolWgd::hide()
{
   QWidget::hide();
}

void CScreenShareToolWgd::SetScreenTitle(const QString& strTile)
{
	//ui.labTitle->setText( strTile );
	StopRoll();
	ui.titleLab->setText(strTile);
	int totalShowMsgLen = ui.titleLab->fontMetrics().width(strTile);

	ui.titleLab->setFixedWidth(totalShowMsgLen);

	StartRoll();
}
void CScreenShareToolWgd::StartRoll()
{
	if (nullptr == mPMoveTimer)
	{
		mPMoveTimer = new QTimer(this);
		connect(mPMoveTimer, &QTimer::timeout, this, &CScreenShareToolWgd::slot_moveText);
	}
	if (nullptr != mPMoveTimer)
	{
		StopRoll();
		mPMoveTimer->start(300);
	}
	
}

void CScreenShareToolWgd::StopRoll()
{
	if (nullptr != mPMoveTimer && mPMoveTimer->isActive())
	{
		mPMoveTimer->stop();
	}
}

void CScreenShareToolWgd::SetSCreenType(const int& iType)
{
	if (eDestopType_Desktop == iType)//µçÄÔÆÁÄ»ÁÐ±í
	{
		std::vector<DesktopCaptureSource> vertSource = globalToolManager->GetClassSDK()->GetDesktops(eDestopType_Desktop);
		int iCount = vertSource.size();
		if (iCount>1)//100
		{
	//		ui.btnChangeScreen->setText(QStringLiteral("ÇÐ»»ÆÁÄ»"));
			ui.btnChangeScreen->setToolTip(QStringLiteral("ÇÐ»»ÆÁÄ»"));
			ui.btnChangeScreen->show();
		}
		else//480
		{
			ui.btnChangeScreen->hide();
		}
		
	}
	else//Èí¼þÁÐ±í
	{
		//ui.btnChangeScreen->setText(QStringLiteral("ÇÐ»»Èí¼þ"));
		ui.btnChangeScreen->setToolTip(QStringLiteral("ÇÐ»»Èí¼þ"));
		ui.btnChangeScreen->show();
	}
}



void CScreenShareToolWgd::slot_moveText()
{
	int iX = ui.titleLab->x();
	int iWidth = ui.titleLab->width();
	if (iX + iWidth <= 0)
	{
		ui.titleLab->move(ui.title->width(), 0);
	}
	else
	{
		ui.titleLab->move(iX - 5, 0);
	}
   static int i = 0;
   i++;
   if (i > 10) {
      i = 0;
      double lostRate = globalToolManager->GetClassSDK()->GetPushDesktopVideoLostRate();
      if (lostRate < 4.0) {
         ui.label_flag->setStyleSheet(QSS_STYLE_NET_EXELLENT);
      }
      else if (lostRate < 10.0) {
         ui.label_flag->setStyleSheet(QSS_STYLE_NET_GOOD);
      }
      else {

         ui.label_flag->setStyleSheet(QSS_STYLE_NET_BAD);
      }
   }

}

void CScreenShareToolWgd::on_btnStop_clicked(bool checked /*= false*/)
{
	emit sigStopScreenSharing();
}

void CScreenShareToolWgd::on_btnRetract_clicked(bool checked /*= false*/)
{
	ui.title->hide();
	ui.btnStop->hide();
	ui.btnRetract->hide();
	ui.btnChangeScreen->hide();
	ui.label_flag->hide();
	ui.label_left->hide();
   ui.label_right->hide();
   ui.widget->hide();
   ui.widgetFlag->hide();
	ui.btnQuikTool->hide();
	ui.btnOpen->show();
}

void CScreenShareToolWgd::on_btnOpen_clicked(bool checked/* = false*/)
{
	ui.btnOpen->hide();
	ui.title->show();
	ui.label_left->show();
	ui.btnStop->show();
	ui.btnRetract->show();
	ui.label_flag->show();
   ui.label_right->show();
   ui.widget->show();
   ui.widgetFlag->show();
	std::vector<DesktopCaptureSource> vertSource = globalToolManager->GetClassSDK()->GetDesktops(eDestopType_Desktop);
	int iCount = vertSource.size();
	if (iCount > 1)
		ui.btnChangeScreen->show();

	ui.btnQuikTool->show();
//	ui.line1->show();
//	ui.line2->show();
//	ui.line_startLive->show();
}

void CScreenShareToolWgd::closeEvent(QCloseEvent* event) {
   if (event) {
      event->ignore();
      //QWidget::closeEvent(event);
      //on_btnStop_clicked();         //ÐÞ¸´×ÀÃæ¹²ÏíÇÐ»»ÆÁÄ»ÍË³öµÄbug

   }
}


void CScreenShareToolWgd::on_btnQuikTool_clicked(bool checked /*= false*/)
{
	if (nullptr != mpShortcutWdg)
	{
		QPoint pos = mapToGlobal(/*temppos +*/ ui.widget->pos());

		int iX = pos.x();
		int iY = pos.y();
      
         
		//QRect  rect = ;
		//qDebug() << iX << iY << ui.btnDesktopSharing->width() << rect.x() << rect.y() << rect.width() << rect.height();
		mpShortcutWdg->move(iX + ui.btnQuikTool->geometry().x(), iY + ui.widget->geometry().height());
		mpShortcutWdg->setFocus();
		mpShortcutWdg->show();
	}

}

void CScreenShareToolWgd::on_btnCamera_clicked(bool checked)
{
   emit sigCameraClicked(!mbCameraOpen);
   slotcameraStateChange(!mbCameraOpen);
}

void CScreenShareToolWgd::on_btnMic_clicked(bool checked)
{
   emit sigMicClicked();
}
