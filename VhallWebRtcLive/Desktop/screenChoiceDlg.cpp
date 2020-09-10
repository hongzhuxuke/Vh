#include "screenChoiceDlg.h"
#include <QDesktopWidget> 
#include <QtMath>
#include <QPainter>

#include "../Unility/Unility.h"
#include "priConst.h"
#include "screenChoiceItem.h"
#include <QListWidgetItem> 
#include <QScrollBar> 

////按钮长宽
//#define BTNW 160
//#define BTNH 90
////上下左右变间隙
//#define TOPMARGIN 30
//#define BOTTOMMARGIN 10
//#define LEFTMARGIN 5
//#define RIGHTMARGIN 15
//
//#define HORIMARGIN 10//纵向间隙
//#define VERTIMARGIN 10//横向间隙

extern ToolManager *globalToolManager;

CREATE_WND_FUNCTION(screenChoiceDlg);

screenChoiceDlg::screenChoiceDlg(QWidget *parent)
	: CBaseWnd(parent)
{
	ui.setupUi(this);
	//setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

	connect(ui.checkBox_openDesktopCapture, SIGNAL(clicked(bool)), this, SLOT(slot_OpenDesktopAudio(bool)));

	ui.label_message->installEventFilter(this);

   ui.labBottum->hide();
   ui.labLeft->hide();
   ui.labRight->hide();
   ui.labTop->hide();
   //ui.frame->setFixedSize(556, 352);
   setFixedSize(558, 354);
   ui.listWidget->verticalScrollBar()->hide();
   ui.listWidget->horizontalScrollBar()->hide();
}

void screenChoiceDlg::setFixedSize(int w, int h)
{
   ui.frame->setFixedSize(w - 2, h - 2);
   CBaseWnd::setFixedSize(w, h);
}

bool screenChoiceDlg::GetScreenList(vector<VHD_WindowInfo>& allWindows)
{
   BOOL enum_result = TRUE;

   for (int device_index = 0;; ++device_index) {
      DISPLAY_DEVICE device;
      device.cb = sizeof(device);
      enum_result = EnumDisplayDevices(NULL, device_index, &device, 0);

      // |enum_result| is 0 if we have enumerated all devices.
      if (!enum_result)
         break;

      // We only care about active displays.
      if (!(device.StateFlags & DISPLAY_DEVICE_ACTIVE))
         continue;

      DEVMODE device_mode;
      device_mode.dmSize = sizeof(device_mode);
      device_mode.dmDriverExtra = 0;
      BOOL result = EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);
      VHD_WindowInfo info;
      info.type = VHD_Desktop;
      RECT screenRect;
      screenRect.left = device_mode.dmPosition.x;
      screenRect.top = device_mode.dmPosition.y;
      screenRect.right = device_mode.dmPosition.x + device_mode.dmPelsWidth;
      screenRect.bottom = device_mode.dmPosition.y + device_mode.dmPelsHeight;
      info.rect = screenRect;
      //info.screen_id = device_index;
      allWindows.push_back(info);
   }
   return true;
}

void screenChoiceDlg::initList() {
   clearList();
   //for (int i = 0; i < 2; i++) {

   
   //std::vector<DesktopCaptureSource> vertSource = globalToolManager->GetClassSDK()->GetDesktops(eDestopType_Desktop);
   vector<VHD_WindowInfo> allDesktop;
   GetScreenList(allDesktop);
   miCount = allDesktop.size();

   int iItemW = 560;
   if (miCount == 1) {
      setFixedSize(440, 396);
      iItemW = 412;
      ui.labLeft->setFixedWidth(14);
      ui.labRight->setFixedWidth(14);

      //ui.labTop->setFixedHeight(10);
      //ui.labRight->setFixedHeight(22);
   }
   else {
      setFixedSize(598, 388);
      iItemW = 598;
      ui.labLeft->setFixedWidth(19);
      ui.labRight->setFixedWidth(19);

      //ui.labTop->setFixedHeight(10);
      //ui.labRight->setFixedHeight(22);
   }

   //mindex = miDesktopId = vertSource.at(0).id;
   mindex = miDesktopId = 0;
   for (int iSize = 0; iSize < miCount; iSize++)
   {
      screenChoiceItem* pWdg = new screenChoiceItem(ui.listWidget);
      if (pWdg) {
         connect(pWdg, &screenChoiceItem::sig_Itemclicked, this, &screenChoiceDlg::slotBtnClicked);
         pWdg->SetScreenInfo(screenChoiceItem::eScreenIndex_First, iSize, allDesktop.at(iSize)/*vertSource.at(iSize).id*/);
         if (0 == iSize) {
            pWdg->setItemWdgChecked(iSize);
         }
         pWdg->setFixedWidth(iItemW);

         iSize++;
         if (iSize < miCount) {
            pWdg->SetScreenInfo(screenChoiceItem::eScreenIndex_Second, iSize, allDesktop.at(iSize));

         }
         else if (1==miCount) {
            pWdg->setHide(screenChoiceItem::eScreenIndex_Second, true);
            break;
         }
         else {
            pWdg->setInvisible(screenChoiceItem::eScreenIndex_Second, true);
            break;
         }

         QListWidgetItem *itemWidget = new QListWidgetItem(ui.listWidget);
         itemWidget->setSizeHint(QSize(pWdg->width(), pWdg->height()));
         ui.listWidget->addItem(itemWidget);
         ui.listWidget->setItemWidget(itemWidget, pWdg);
      }
   }

   //}
}
bool screenChoiceDlg::eventFilter(QObject * watched, QEvent * event)
{
    if (watched == ui.label_message)
    {
         if (event->type() == QEvent::MouseButtonPress)
			{
				if (ui.checkBox_openDesktopCapture->isChecked()) {
					ui.checkBox_openDesktopCapture->setChecked(false);
				}
				else {
					ui.checkBox_openDesktopCapture->setChecked(true);
				}
        }
    }
     return QDialog::eventFilter(watched, event);
 }

void screenChoiceDlg::slot_OpenDesktopAudio(bool checked) {
	//emit sig_DesktopCapture(checked);

}

void screenChoiceDlg::on_btnOk_clicked(bool bChecked /*= false*/)
{
	if (ui.checkBox_openDesktopCapture->isChecked()) {
		emit sig_DesktopCapture(true);
	}
	else {
		emit sig_DesktopCapture(false);
	}
}

void screenChoiceDlg::Accept() {

	accept();
}

void screenChoiceDlg::on_btnCancle_clicked(bool bChecked /*= false*/)
{
   reject();
}

screenChoiceDlg::~screenChoiceDlg()
{
   clearList();

}

int screenChoiceDlg::exec()
{
   JOIN_ROLE role = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
	if (miCount < 1 && JOIN_ROLE_Teacher == role){
		//mindex = 0;
		//miDesktopId = mPCureBtn->DesktopIndex();
		return Accepted;
	}
   else {
      ui.btnClose->setEnabled(true);
      ui.checkBox_openDesktopCapture->setEnabled(true);
      ui.btnOk->setEnabled(true);
      ui.btnCancle->setEnabled(true);
      setEnabled(true);

      if (JOIN_ROLE_Teacher == role) {
         //ui.labTip->hide();
         ui.labTip->setText(QStringLiteral("请选择您要共享的内容"));
      }
      else if (JOIN_ROLE_Student == role) {
         //ui.labTip->show();
         ui.labTip->setText(QStringLiteral("讲师邀请您共享桌面，请选择你要共享的内容"));
      }
      //setModal(true);
      //show();
      return QDialog::exec();
   }
}

void screenChoiceDlg::setEnabled(bool b)
{
   CBaseWnd::setEnabled(true);
   ui.btnClose->setEnabled(true);
   ui.checkBox_openDesktopCapture->setEnabled(true);
   ui.btnOk->setEnabled(true);
   ui.btnCancle->setEnabled(true);

   if (!b)
   {
      int i = 0;
   }
}

void screenChoiceDlg::setListWidgetEnabled(bool bEnable /*= true*/)
{
   for (int i = 0; i < ui.listWidget->count(); i++) {
      QListWidgetItem* item = ui.listWidget->item(i);
      if (NULL != item) {
         QWidget* w = ui.listWidget->itemWidget(item);
         if (NULL != item) {
            screenChoiceItem* itemWidget = dynamic_cast<screenChoiceItem*>(w);

            itemWidget->setEnabled(bEnable);
         }
      }
   }
}

void screenChoiceDlg::clearList()
{
   int iCout = ui.listWidget->count();
   for (; iCout > 0; iCout--) {
      QListWidgetItem* item = ui.listWidget->item(0);
      if (NULL != item) {
         QWidget* w = ui.listWidget->itemWidget(item);
         if (NULL != item) {
            screenChoiceItem* itemWidget = dynamic_cast<screenChoiceItem*>(w);

            item = ui.listWidget->takeItem(0);

            delete itemWidget;
            itemWidget = NULL;

            delete item;
            item = NULL;
         }
      }
   }
}



void screenChoiceDlg::SetDesktopCapture(bool bDesktopCaptur) {
	if (bDesktopCaptur) {
		ui.checkBox_openDesktopCapture->setChecked(true);
	}
	else {
		ui.checkBox_openDesktopCapture->setChecked(false);
	}
}

void screenChoiceDlg::Notice(const CSTDMapParam &)
{

}

int screenChoiceDlg::GetIndex()
{
	return mindex;
}

int64_t screenChoiceDlg::GetDesktopIndex()
{
	return miDesktopId;
}

void screenChoiceDlg::on_btnClose_clicked(bool bChecked/* = false*/)
{
	reject();
}

void screenChoiceDlg::slotBtnClicked(int id/*, CSCreenBtnWdg* pdlg*/)
{
	mindex = id;
	miDesktopId = id/* pdlg->DesktopIndex()*/;

   for (int i = 0; i < ui.listWidget->count(); i++) {
      QListWidgetItem* item = ui.listWidget->item(i);
      if (NULL != item) {
         QWidget* w = ui.listWidget->itemWidget(item);
         if (NULL != item) {
            screenChoiceItem* itemWidget = dynamic_cast<screenChoiceItem*>(w);

            itemWidget->setItemWdgChecked(miDesktopId);
         }
      }
   }

	//accept();
}

void screenChoiceDlg::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	if (nullptr != mPixmap)
		painter.drawPixmap(rect(), mPixmap->copy(0, 0, mPixmap->width(), mPixmap->height()));
	QDialog::paintEvent(e);
}