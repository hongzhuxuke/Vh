
#include "BaiduPaperDlg.h"
#include "../Unility/Unility.h"
//#include <QMessageBox> 
//#include <QDateTime> 
//#include <QTableWidgetItem>
//
//#include <QScrollBar> 
//#include <QDesktopServices> 
//#include <QUrl>
//#include <QLatin1String> 
//#include <QApplication> 
#include <QDesktopWidget> 
//
#include <QDebug>
//#include "ConfigSetting.h"
//
//#include "TipsWdg.h"
////#include "OperationBtnWdg.h"
////#include "TestStateWdg.h"
//#include "pathmanager.h"

extern ToolManager *globalToolManager;

BaiduPaperDlg::BaiduPaperDlg(QWidget *parent) :
	CBaseWnd(parent)
{
    ui.setupUi(this);
	//setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint /*| Qt::Dialog*/);

	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);

	CourseInfo info = globalToolManager->GetClassSDK()->GetCourseInfo();
	ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	
	//m_TestUrl = QString("https:\/\/t-class.e.vhall.com/embed_client.html?webinar_id=%1&join_id=%2&token=%3&no_mic=0&role=%4&course_type=%5")
	//	.arg(info.webinar_id.c_str()).arg(roomInfo.mUserInfo.join_id.c_str()).arg(globalToolManager->GetClassSDK()->GetClassToken().c_str())
	//	.arg(roomInfo.mUserInfo.role_type).arg(info.webinar_course_type);

   m_TestUrl = QString::fromStdString( globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.questionnaire );

   mCefTestView = new LibCefViewWdg(this);


   connect(mCefTestView, &LibCefViewWdg::sig_TitleChanged, this, &BaiduPaperDlg::slot_TitleChanged);
   mCefTestView->setObjectName(QString::fromUtf8("TestView"));
   ui.verticalLayout->addWidget(mCefTestView);

   if (m_TestUrl.trimmed().isEmpty()) {
      
   }
   else {
      mCefTestView->InitLibCef();
      mCefTestView->Create(m_TestUrl);
   }
   
   
   slot_TitleChanged(QStringLiteral("试卷"));
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {//讲师
      ui.backframe->setFixedSize(580, 451);
      mCefTestView->setFixedSize(580, 415);
      setFixedSize(620, 483);
      ui.btnClose->show();
      ui.labClose->show();
   }
   else {//学生
      //ui.backframe->setFixedSize(580, 556);
      //mCefTestView->setFixedSize(580, 520);
      //setFixedSize(620, 588);

      ui.backframe->setFixedSize(580, 586);
      mCefTestView->setFixedSize(580, 550);//580   600
      setFixedSize(620, 618);

      ui.btnClose->hide();
      ui.labClose->hide();
   }
   ui.btnMinimize->hide();
   ui.labMinimize->hide();
   //mDocCefWidget->setObjectName(STRCHAT);
}

void BaiduPaperDlg::slot_TitleChanged(const QString& strTitle)
{
   if (!m_TestUrl.trimmed().isEmpty()) {
      SetTitleChanged(strTitle);
      mbLoadEnd = true;
   }
   
   //if (mpTimer == nullptr) {
   //   mpTimer = new QTimer(this);
   //   connect(mpTimer, &QTimer::timeout, this, &BaiduPaperDlg::slot_timeOut);
   //}
   //
   //mpTimer->start(1000);
}

void BaiduPaperDlg::slot_timeOut()
{
   bool b= ui.label->isEnabled();
   bool bb = ui.btnClose->isEnabled();
   bool bbb = isEnabled();

   qDebug() << b << bb << bbb << endl;
}

CBaseWnd* BaiduPaperDlg::CreateFunction() {
	return new BaiduPaperDlg();
}

void BaiduPaperDlg::OnRecvMsg(std::string funname, std::string argv) {
   CefMsgEvent *cefMsg = new CefMsgEvent(CustomEvent_LibCefMsg, QString::fromStdString(funname), QString::fromStdString(argv));
   QApplication::postEvent(this, cefMsg);
}

void BaiduPaperDlg::OnWebViewLoadEnd() {
   //if (ui.TestView) {
   //   //QString hideDoc = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
   //   //mPluginCefView->AppCallJsFunction(hideDoc);

   mCefTestView->show();
     //QTimer::singleShot(500, this, &BaiduPaperDlg::slot_LoadTest);
   //}
}

void BaiduPaperDlg::OnWebViewDestoryed() {

}

void BaiduPaperDlg::ClientCallCloseExamCallback()
{
   this->close();
   this->hide();
   emit sig_close();
   ToolManager::GetInstance()->PostDestoryTestPaperDlg();
}

void BaiduPaperDlg::Notice(const CSTDMapParam &mapParam) {

}

BaiduPaperDlg::~BaiduPaperDlg()
{
   if (mCefTestView) {

      delete mCefTestView;
      mCefTestView = nullptr;
   }
}

void BaiduPaperDlg::on_btnMin_clicked(bool checked /*= false*/)
{
	showMinimized();
}

void BaiduPaperDlg::on_btnClose_clicked(bool checked /*= false*/){
   //if (mCefTestView && mbLoadEnd) {
   //   QString funPage = QString("ClientCallCloseExam()");//调用关闭操作
   //   mCefTestView->AppCallJsFunction(funPage);
   //}
   //else {
      ClientCallCloseExamCallback();
   //}
}

void BaiduPaperDlg::slot_LoadTest()
{
   //VH::CComPtr<ICommonData> pCommonData;
   //DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
   //ClientApiInitResp initResp;
   //pCommonData->GetInitApiResp(initResp);
   //if (initResp.player == H5_LIVE_CREATE_TYPE && mPluginCefView) {
   //   QString hideDoc = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
   //   mPluginCefView->AppCallJsFunction(hideDoc);
   //}
}

void BaiduPaperDlg::SetTitleChanged(const QString& strTitle)
{
   setWindowTitle(strTitle);
   mCefTestView->setWindowTitle(strTitle);
   ui.label->setText(strTitle);

   ui.label->setEnabled(true);
   ui.btnClose->setEnabled(true);
   setEnabled(true);
}

void BaiduPaperDlg::moveXY()
{
	QRect rect = QApplication::desktop()->availableGeometry();

	int iDW = rect.width();
	int iDH = rect.height();
	int iW = width();
	int iH = height();

	int iX = 0.5 * (iDW - iW);
	int iY = 0.5 * (iDH - iH);

	this->move(iX, iY);
}

void BaiduPaperDlg::resizeEvent(QResizeEvent *event)
{	
	CBaseWnd::resizeEvent(event);
	moveXY();
}

void BaiduPaperDlg::showEvent(QShowEvent *event)
{
	CBaseWnd::showEvent(event);
	moveXY();
   //mCefTestView->LoadUrl(m_TestUrl);
}

void BaiduPaperDlg::customEvent(QEvent* event) {

	if (event) {
		switch (event->type()){	
      case CustomEvent_LibCefMsg: {
         CefMsgEvent* cus_event = dynamic_cast<CefMsgEvent*>(event);
         if (cus_event) {
            JsCallQtMsg(cus_event->mFunName, cus_event->mCefMsgData);
         }
      }
                                  break;
		default: break;
		}
	}
	CBaseWnd::customEvent(event);
}

void BaiduPaperDlg::JsCallQtMsg(QString name, QString param)
{
   if (name.compare("ClientCallCloseExamCallback") == 0) {
      ClientCallCloseExamCallback();
   }
}
