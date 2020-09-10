//#include "stdafx.h"
#include <QPainter>
#include "softwareSourceDlg.h"
#include "priConst.h"
#include <QAbstractItemView>

extern ToolManager *globalToolManager;


softwareSourceDlg::softwareSourceDlg(QWidget *parent)
: CBaseWnd(parent){
   ui.setupUi(this);
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Dialog /*| Qt::WindowStaysOnTopHint*/);
   //setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
   //setAttribute(Qt::WA_TranslucentBackground);
   //setAutoFillBackground(false);
   resize(446, 266);
   setFixedSize(446, 266);
   //mPixmap = new QPixmap(":/widget/img/widget/softwareSouce.png");
   connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(OnSave()));
   connect(ui.btnClose, &QPushButton::clicked, this, &QDialog::reject);
   connect(ui.btnFlush, &QPushButton::clicked, this, &softwareSourceDlg::slotReFlush);

   ui.mCmbWindowList->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   ui.mCmbWindowList->view()->window()->setAttribute(Qt::WA_TranslucentBackground);


   ui.ckb_compatible->setVisible(false);

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();
   ui.backWdg->setFixedSize(439, 253);
   setFixedSize(441, 255);
}

softwareSourceDlg::~softwareSourceDlg() {
	//if (nullptr != mPixmap)
	//{
	//	delete  mPixmap;
	//	mPixmap = nullptr;
	//}

}

int softwareSourceDlg::exec()
{
	slotReFlush();
	return QDialog::exec();
}

void softwareSourceDlg::slotReFlush() {
	ui.mCmbWindowList->clear();
	mSource.clear();
	std::vector<DesktopCaptureSource> vertSource = globalToolManager->GetClassSDK()->GetDesktops(eDestopType_Windows);
	int iCout = vertSource.size();
	for (int i = 0; i < iCout; i++){
		mSource[i] = vertSource.at(i);
		ui.mCmbWindowList->addItem(QString::fromStdString(vertSource.at(i).title), i);
#if __WORDSIZE == 64
		globalToolManager->GetDataManager()->WriteLog(" %s soft:%s proId:%ld", __FUNCTION__, vertSource.at(i).title, vertSource.at(i).id);
#else
		globalToolManager->GetDataManager()->WriteLog(" %s i:%d soft:%s ", __FUNCTION__, i, vertSource.at(i).title.c_str()/*, vertSource.at(i).id*/);//proId:%ld
		//globalToolManager->GetDataManager()->WriteLog(" %s  i:%d soft:%s proId:%ld", __FUNCTION__, i, vertSource.at(i).title, mSource[i].id);
#endif
		
	}

}

void softwareSourceDlg::AddDeviceItem(wchar_t* wzDeviceName, QVariant UserData) {
   ui.mCmbWindowList->addItem(QString::fromWCharArray(wzDeviceName), UserData);
}

void softwareSourceDlg::Notice(const CSTDMapParam &)
{

}

bool softwareSourceDlg::GetCurItem(QVariant& UserData) {
   UserData = ui.mCmbWindowList->itemData(ui.mCmbWindowList->currentIndex());
   return true;
}

void softwareSourceDlg::OnSave() {
   QVariant UserData = 	ui.mCmbWindowList->itemData(ui.mCmbWindowList->currentIndex());
   int iID = UserData.toInt();
   globalToolManager->GetDataManager()->WriteLog(" %s iID:%d soft:%s ", /*proId:%ld*/
	   __FUNCTION__, 
	   iID, 
	   ui.mCmbWindowList->currentText().toStdString().c_str()/*, 
	   mSource[iID].id*/);
   mstrCurShareSoftware = QString::fromStdString(mSource[iID].title);
   miSelectResult = globalToolManager->GetClassSDK()->SelectSource(mSource[iID].id);
   accept();
}

void softwareSourceDlg::Clear() {
   ui.mCmbWindowList->clear();
   ui.ckb_compatible->setChecked(false);
}

//void softwareSourceDlg::CenterWindow(QWidget* parent) {
//   int x = 0;
//   int y = 0;
//   if (NULL == parent) {
//      const QRect rect = QApplication::desktop()->availableGeometry();
//      x = rect.left() + (rect.width() - width()) / 2;
//      y = rect.top() + (rect.height() - height()) / 2;
//   } else {
//      QPoint point(0, 0);
//      point = parent->mapToGlobal(point);
//      x = point.x() + (parent->width() - width()) / 2;
//      y = point.y() + (parent->height() - height()) / 2;
//   }
//   move(x, y);
//}
//bool softwareSourceDlg::GetComptible(){
//   return ui.ckb_compatible->isChecked();
//}

//void softwareSourceDlg::paintEvent(QPaintEvent *e) {
//	QPainter painter(this);
//	if (nullptr != mPixmap)
//		painter.drawPixmap(rect(), mPixmap->copy(0, 0, mPixmap->width(), mPixmap->height()));
//	QDialog::paintEvent(e);
//}