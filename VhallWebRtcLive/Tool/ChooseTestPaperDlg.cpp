
#include "ChooseTestPaperDlg.h"
#include "../Unility/Unility.h"
#include "ShadowBoxDlg.h"
#include "TipsWdg.h"
#include <QDesktopWidget> 
#include <QDebug>

extern ToolManager *globalToolManager;

ChooseTestPaperDlg::ChooseTestPaperDlg(QWidget *parent) :
	CBaseWnd(parent)
{
    ui.setupUi(this);
	//setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint 
      | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
      | Qt::WindowMaximizeButtonHint /*| Qt::Dialog*/);

   //mpShaowBoxDlg = (ShadowBoxDlg*)parent;
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);

	CourseInfo info = globalToolManager->GetClassSDK()->GetCourseInfo();
	ClassRoomBaseInfo roomInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
	
	//m_TestUrl = QString("https:\/\/t-class.e.vhall.com/embed_client.html?webinar_id=%1&join_id=%2&token=%3&no_mic=0&role=%4&course_type=%5")
	//	.arg(info.webinar_id.c_str()).arg(roomInfo.mUserInfo.join_id.c_str()).arg(globalToolManager->GetClassSDK()->GetClassToken().c_str())
	//	.arg(roomInfo.mUserInfo.role_type).arg(info.webinar_course_type);

   m_TestUrl = QString::fromStdString( globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.questionnaire );

   mCefTestView = new LibCefViewWdg(this);


   connect(mCefTestView, &LibCefViewWdg::sig_TitleChanged, this, &ChooseTestPaperDlg::slot_TitleChanged);
   mCefTestView->setObjectName(QString::fromUtf8("TestView"));
   ui.verticalLayout->addWidget(mCefTestView);

   mCefTestView->InitLibCef();
   mCefTestView->Create(m_TestUrl);
   //mpShaowBoxDlg->setLefRightBoerderWidth(20);
   slot_TitleChanged(QStringLiteral("试卷"));
   ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
   if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {//讲师
      ui.backframe->setFixedSize(580, 451);
      mCefTestView->setFixedSize(580, 415);
      setFixedSize(582, 453);

      ui.btnClose->show();
      ui.labClose->show();
   }
   else {//学生
      //ui.backframe->setFixedSize(580, 556);
      //mCefTestView->setFixedSize(580, 520);
      //setFixedSize(620, 588);

      ui.backframe->setFixedSize(580, 586);
      mCefTestView->setFixedSize(580, 550);//580   600
      setFixedSize(582, 588);

      ui.btnClose->hide();
      ui.labClose->hide();
   }
   ui.btnMinimize->hide();
   ui.labMinimize->hide();
   //mDocCefWidget->setObjectName(STRCHAT);

   ui.labBottom->hide();
   ui.labLeft->hide();
   ui.labTop->hide();
   ui.labRight->hide();
}

void ChooseTestPaperDlg::slot_TitleChanged(const QString& strTitle)
{
   if (m_TestUrl.trimmed().isEmpty()) {
      SetTitleChanged(QStringLiteral("考试链接为空"));
      ui.btnClose->show();
      ui.labClose->show();
   }
   else {
      SetTitleChanged(strTitle);
      mbLoadEnd = true;
   }
   //if (mpTimer == nullptr) {
   //   mpTimer = new QTimer(this);
   //   connect(mpTimer, &QTimer::timeout, this, &ChooseTestPaperDlg::slot_timeOut);
   //}
   //
   //mpTimer->start(1000);
}

void ChooseTestPaperDlg::slot_timeOut()
{
   //bool b= ui.label->isEnabled();
   //bool bb = ui.btnClose->isEnabled();
   //bool bbb = isEnabled();

   //qDebug() << b << bb << bbb << endl;
}

CBaseWnd* ChooseTestPaperDlg::CreateFunction() {
	return new ChooseTestPaperDlg();
}

void ChooseTestPaperDlg::OnRecvMsg(std::string funname, std::string argv) {
   CefMsgEvent *cefMsg = new CefMsgEvent(CustomEvent_LibCefMsg, QString::fromStdString(funname), QString::fromStdString(argv));
   QApplication::postEvent(this, cefMsg);
}

void ChooseTestPaperDlg::OnWebViewLoadEnd() {
   //if (ui.TestView) {
   //   //QString hideDoc = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
   //   //mPluginCefView->AppCallJsFunction(hideDoc);

   mCefTestView->show();
     //QTimer::singleShot(500, this, &ChooseTestPaperDlg::slot_LoadTest);
   //}
}

void ChooseTestPaperDlg::OnWebViewDestoryed() {

}

void ChooseTestPaperDlg::ClientCallCloseExamCallback()
{
   //mpShaowBoxDlg->close();
   //mpShaowBoxDlg->hide();
   this->close();
   this->hide();
   emit sig_close();
   ToolManager::GetInstance()->PostDestoryTestPaperDlg();
}

void ChooseTestPaperDlg::Notice(const CSTDMapParam &mapParam) {

}

ChooseTestPaperDlg::~ChooseTestPaperDlg()
{
   if (mCefTestView) {

      delete mCefTestView;
      mCefTestView = nullptr;
   }
}

void ChooseTestPaperDlg::on_btnMin_clicked(bool checked /*= false*/)
{
	showMinimized();
}

void ChooseTestPaperDlg::on_btnClose_clicked(bool checked /*= false*/){
   //TipsWdg* pTips = new TipsWdg(QStringLiteral("您即将关闭考试，是否确认?"), this,QStringLiteral("结束"), QStringLiteral("返回考试"));
   //if (pTips && QDialog::Accepted == pTips->exec()) {

      if (mCefTestView && mbLoadEnd) {
         QString funPage = QString("ClientCallCloseExam()");//调用关闭操作
         mCefTestView->AppCallJsFunction(funPage);
      }
      else {
         ClientCallCloseExamCallback();
      }

   //}
   
   //if (pTips) {
   //   delete pTips;
   //   pTips = nullptr;
   //}
}

void ChooseTestPaperDlg::slot_LoadTest()
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

void ChooseTestPaperDlg::SetTitleChanged(const QString& strTitle)
{
   setWindowTitle(strTitle);
   mCefTestView->setWindowTitle(strTitle);
   ui.label->setText( strTitle);

   ui.label->setEnabled(true);
   ui.btnClose->setEnabled(true);
   setEnabled(true);
}

void ChooseTestPaperDlg::moveXY()
{
   //if (mpShaowBoxDlg) {
   //   this->move(mpShaowBoxDlg->GetChildGlobalX(), mpShaowBoxDlg->GetChildGlobalY());
   //}
   QRect rect = QApplication::desktop()->availableGeometry();

   int iDW = rect.width();
   int iDH = rect.height();
   int iW = width();
   int iH = height();

   int iX = 0.5 * (iDW - iW);
   int iY = 0.5 * (iDH - iH);

   this->move(iX, iY);
}

void ChooseTestPaperDlg::resizeEvent(QResizeEvent *event)
{	
	CBaseWnd::resizeEvent(event);
	moveXY();
}

void ChooseTestPaperDlg::showEvent(QShowEvent *event)
{
	CBaseWnd::showEvent(event);
	moveXY();
   //mCefTestView->LoadUrl(m_TestUrl);
}

void ChooseTestPaperDlg::mouseMoveEvent(QMouseEvent * event)
{
   if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
      QPoint  newPos(event->globalPos() + mPressPoint);
      //if (mpShaowBoxDlg) {
      //   mpShaowBoxDlg->move(newPos.x() - mpShaowBoxDlg->GetChildX(), newPos.y() - mpShaowBoxDlg->GetChildY());
      //   mpShaowBoxDlg->raise();
      //}

      this->move(newPos);
      this->raise();
   }
}

void ChooseTestPaperDlg::customEvent(QEvent* event) {

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

void ChooseTestPaperDlg::JsCallQtMsg(QString name, QString param)
{
   if (name.compare("ClientCallCloseExamCallback") == 0) {
      ClientCallCloseExamCallback();
   }
}
