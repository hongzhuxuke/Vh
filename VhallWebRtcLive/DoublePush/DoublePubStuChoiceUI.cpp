#include "stdafx.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
//#include <QLabel>
//#include <QComboBox>
//#include <QListWidget>
//#include <QToolButton>
//#include <vector>

//#include <QDebug>
//#include <QFileInfo>
//#include <QFileDialog>
//#include "TitleWidget.h"
//#include "push_button.h"
//#include "VedioPlayUI.h"
#include "DoublePubStuChoiceUI.h"
#include "TipsWdg.h"
#include "LeftToolBarWdg.h"
//#include "ToolButton.h"
//#include "Push_button.h"
//#include <QSpacerItem>
//#include <QGraphicsDropShadowEffect> 
//#include "TipsWdg.h"
//#include "VH_ConstDeff.h"


extern ToolManager *globalToolManager;
using namespace std;


CREATE_WND_FUNCTION(DoublePubStuChoiceUI);
DoublePubStuChoiceUI::DoublePubStuChoiceUI(QWidget *parent) :
	CBaseWnd(parent)  {
	ui.setupUi(this);
   mpLeftToolBarWdg = (LeftToolBarWdg*)parent;
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);

   ui.btnClose->setFocusPolicy(Qt::NoFocus);
   connect(ui.ListWidget, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slot_ItemDoubleClicked(const QModelIndex &)));
   ui.ListWidget->setFocusPolicy(Qt::NoFocus);
   
   ui.btnMinimize->hide();
   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();

   ui.backframe->setFixedSize(418, 346);
   setFixedSize(420, 348);
}

DoublePubStuChoiceUI::~DoublePubStuChoiceUI() {
}

void DoublePubStuChoiceUI::initItem(const eChoiceType& type)
{
	Clear();
   if (eChoiceType_Invalid > type) {
      mType = type;
   }
   
	repaint();
}

int DoublePubStuChoiceUI::iRowCount()
{
   return ui.ListWidget->count();
}


void DoublePubStuChoiceUI::Notice(const CSTDMapParam &mapParam)
{
	//std::map<QString, QString>::const_iterator it = mapParam.begin();
	//for (; it != mapParam.end(); ++it)
	//{
	//	QString strKey = it->first;
	//	if (strKey.compare(INIT) == 0)
	//	{
	//		initItem();
	//	}
	//}
}

void DoublePubStuChoiceUI::CloseWdg()
{
	close();
   reject();
}


int DoublePubStuChoiceUI::exec()
{
   if (ui.ListWidget->count()<= 0) {
      return QDialog::Rejected;
   }
   else {
      return CBaseWnd::exec();
   }
}

void DoublePubStuChoiceUI::on_btnClose_clicked(bool checked /*= false*/)
{
	TipsWdg* tips = new TipsWdg(this);
	if (tips) {
		CSTDMapParam param;
		param[TipsMsg] = QStringLiteral("是否确认结束双推?");
      if (mpLeftToolBarWdg && LeftToolBarWdg::eDoubleType_StuScreen == mpLeftToolBarWdg->DoubleType()) {
         param[TipsMsg] = QStringLiteral("是否确认结束学员桌面共享?");
      }
		param[ShowCancelButton] = PARAM_TRUE;
		tips->Notice(param);
	}
	int accept = tips->exec();
	if (accept == QDialog::Accepted) {
		close();
		hide();
	}

}


bool DoublePubStuChoiceUI::Create() {

   return true;
}

void DoublePubStuChoiceUI::Destroy() {

}

void DoublePubStuChoiceUI::AddUser(const CDoubleVrtcUsers& vrtcUser)
{
   //QString strFix = mfi.suffix().trimmed();
	if (vrtcUser.miNoDoubleVrtc == 1 && eChoiceType_Camera == mType) {
		return;
	}
   DoublePubStuChoiceItem *itemWidget = new DoublePubStuChoiceItem();
   if (itemWidget)
   {
      itemWidget->setName(vrtcUser.mstrNickName);
      itemWidget->setId(vrtcUser.mstrId);
      if (eChoiceType_Deskop == mType) {
         itemWidget->setNoDoubleVrtc(DoublePubStuChoiceItem::eEnableVrtc_enable);
      }
      else {
         itemWidget->setNoDoubleVrtc(vrtcUser.miNoDoubleVrtc);
      }
      

      connect(itemWidget, &DoublePubStuChoiceItem::sig_inviteDoubleVrtc, this, &DoublePubStuChoiceUI::slot_inviteDoubleVrtc);
      //itemWidget->setMaximumWidth(506);
      itemWidget->setFixedHeight(55);
   }

   QListWidgetItem *item = new QListWidgetItem(ui.ListWidget);
   ui.ListWidget->addItem(item);
   item->setSizeHint(itemWidget->size());
   item->setFlags(Qt::NoItemFlags);
   item->setFlags(Qt::ItemIsEnabled);
   ui.ListWidget->setItemWidget(item, itemWidget);
}

void DoublePubStuChoiceUI::slot_inviteDoubleVrtc(QString strId)
{
   emit sig_inviteDoubleVrtc(strId, mType);
   accept();
}

void DoublePubStuChoiceUI::slot_ItemDoubleClicked(const QModelIndex &index) {
   qDebug()<<"########################DoublePubStuChoiceUI::PlayListDoubleClicked####################";
   if (!ui.ListWidget) {
      return;
   }

   int row = index.row();
   if (row < 0 || row >= ui.ListWidget->count()) {
      qDebug()<<"########################DoublePubStuChoiceUI::PlayListDoubleClicked#################### row < 0 || row >= ui.ListWidget->count()";
      return;
   }

   ui.ListWidget->setCurrentRow(row);

}

void DoublePubStuChoiceUI::DeleteItem(DoublePubStuChoiceItem *itemWidget) {
   if (!ui.ListWidget) {
      return;
   }

   if (!itemWidget) {
      return;
   }

   for (int i = 0; i < ui.ListWidget->count(); i++) {
      QListWidgetItem *item = ui.ListWidget->item(i);
      if (item) {
		  QWidget* w = ui.ListWidget->itemWidget(item);

         if (ui.ListWidget->itemWidget(item) == itemWidget) {

            ui.ListWidget->removeItemWidget(item);
            delete item;
            break;
         }
      }
   }

   ui.ListWidget->setFocus();
}

void DoublePubStuChoiceUI::Clear() {
   if (!ui.ListWidget) {
      return;
   }
   ui.ListWidget->clear();
}



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

DoublePubStuChoiceItem::DoublePubStuChoiceItem(QWidget *parent) :
	CWidget(parent) {
	ui.setupUi(this);
	this->setFixedHeight(40);

	connect(ui.btnChoice, &QPushButton::clicked, this, &DoublePubStuChoiceItem::slot_Choice);
}

DoublePubStuChoiceItem::~DoublePubStuChoiceItem() {
}

void DoublePubStuChoiceItem::slot_Choice()
{
   emit sig_inviteDoubleVrtc(mStrId);
   //globalToolManager->GetClassSDK()->inviteDoubleVrtc(mStrId.toStdString());
}

void DoublePubStuChoiceItem::setName(const QString &text) {
	if (!ui.labFile) {
		return;
	}

	ui.labFile->setText(text);
	this->setToolTip(text);
	mNickName = text;
}

void DoublePubStuChoiceItem::setId(const QString & id)
{
   mStrId = id;
}

void DoublePubStuChoiceItem::setNoDoubleVrtc(const int& iVrtc)
{
   //不支持双路连麦 0:可以双推 1:不可以
   if (eEnableVrtc_enable == iVrtc)
   {
      ui.btnChoice->show();
      ui.labEnableIco->hide();
   }
   else
   {
      ui.btnChoice->hide();
      ui.labEnableIco->show();
   }
}

void DoublePubStuChoiceItem::FlushLabelColor(bool pressed) {
	if (pressed) {
	}
	else {
		if (m_bChecked) {//播放中进入后 鼠标离开出发
			ui.labFile->setStyleSheet(
				"QLabel{color:rgb(82,204,144);font: 14px \"微软雅黑\";}"
			);
		}
		else {
			ui.labFile->setStyleSheet(//未播放鼠标离开
				"QLabel{color:rgb(68,68,68);font: 14px \"微软雅黑\";}"
				"QLabel:hover{color:rgb(102,102,102);}"
			);
		}
	}
}

void DoublePubStuChoiceItem::setChecked(bool ok) {
	m_bChecked = ok;
	//FlushLabelColor(false);
}

void DoublePubStuChoiceItem::leaveEvent(QEvent *e) {
	QWidget::leaveEvent(e);
	//FlushLabelColor(false);
}

void DoublePubStuChoiceItem::mouseReleaseEvent(QMouseEvent *e) {
	QWidget::mouseReleaseEvent(e);
	//FlushLabelColor(false);
}

void DoublePubStuChoiceItem::mousePressEvent(QMouseEvent *e) {
	QWidget::mousePressEvent(e);
	//FlushLabelColor(true);
}

void DoublePubStuChoiceItem::setWidth(int width) {
	this->setFixedWidth(width);
}