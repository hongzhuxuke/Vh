#include "AnswerWebView.h"
//#include <QMovie>
#include "ShadowBoxDlg.h"
#include <QDesktopWidget>

extern ToolManager *globalToolManager;
CREATE_WND_FUNCTION(AnswerWebView);

AnswerWebView::AnswerWebView(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint 
       | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint /*| Qt::Dialog*/);

    //mpShaowBoxDlg = (ShadowBoxDlg*)parent;
    //setAttribute(Qt::WA_TranslucentBackground);
    //setAutoFillBackground(false);

    //connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_close())); 
    //
    int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
    std::string answer = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.answer;

    QString loadUrl = QString::fromStdString(answer) + QString("&role_type=%1").arg(roleType);
    //QString loadUrl = "http://e.vhall.com/";

    mCefWebView = new LibCefViewWdg(this);

    connect(mCefWebView, &LibCefViewWdg::sig_TitleChanged, this, &AnswerWebView::slot_loadFinished);
    mCefWebView->setObjectName(QString::fromUtf8("AnswerWeb"));

    ui.verticalLayout->addWidget(mCefWebView);
    mCefWebView->InitLibCef();
    mCefWebView->Create(loadUrl);

    setWindowTitle(QStringLiteral("答题"));

	//ui.pushButton_close->setFocusPolicy(Qt::NoFocus);
	//connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_close()));

    //int iw = 674;
    //int ih = 514;
    int iw = 650;
    int ih = 490 ; 
    setFixedSize(iw+2, ih +2);//  674    514
    ui.backframe->setFixedSize(iw, ih);
    mCefWebView->setFixedSize(iw, ih);
    //mpShaowBoxDlg->setLefRightBoerderWidth(23);
//    mpShaowBoxDlg->setFixedSize(696, 522/*iw + 40, ih + 32*/);//714  546
    ClassRoomBaseInfo baseInfo = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo();
    if (baseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {//讲师

       ui.btnClose->show();
       ui.labClose->show();
    }
    else {//学生

       ui.btnClose->hide();
       ui.labClose->hide();
    }

    ui.btnMinimize->hide();
    ui.labMinimize->hide();
    //ui.labBottom->hide();
    //ui.labLeft->hide();
    //ui.labRight->hide();
    //ui.labTop->hide();

    //ui.Titleframe->hide();
}

AnswerWebView::~AnswerWebView()
{
   if (mCefWebView) {

      delete mCefWebView;
      mCefWebView = nullptr;
   }
	//if (nullptr != movie)
	//{
	//	delete movie;
	//	movie = nullptr;
	//}
}

void AnswerWebView::Notice(const CSTDMapParam &mapParam) {

   if (!mbLoadFinished)
   {

      mbLoadFinished = false;
      //mCefWebView->show();
      //ui.stackedWidget->setCurrentIndex(ePage_WEB);
   }

   std::map<QString, QString>::const_iterator iterMsg = mapParam.find(MEDIA_FILE_CTRL_MSG);
   if (iterMsg != mapParam.end()) {
      if (mCefWebView) {
         mCefWebView->AppCallJsFunction(iterMsg->second);
      }
   }
}

void AnswerWebView::show()
{
   //if (mpShaowBoxDlg) {
   //   mpShaowBoxDlg->show();
   //   //mpShaowBoxDlg->hide();
   //}
   CBaseWnd::show();

   if (mCefWebView) {
      mCefWebView->show();
   }
}

void AnswerWebView::hide()
{
   //if (mpShaowBoxDlg) {
   //   mpShaowBoxDlg->hide();
   //}
   if (mCefWebView) {
      mCefWebView->hide();
   }
   CBaseWnd::hide();
}

void AnswerWebView::SetLoadFinished(const bool& bloadfinish)
{
   int iW = mCefWebView->width();
   int iH = mCefWebView->height();
	delete mCefWebView;
   mCefWebView = NULL;
	mbLoadFinished = bloadfinish;
}

//void 	AnswerWebView::setEnabled(bool enabled)
//{
//	CBaseWnd::setEnabled(enabled);
//}

void AnswerWebView::on_btnClose_clicked(bool checked /*= false*/) {
   //TipsWdg* pTips = new TipsWdg(QStringLiteral("您即将关闭考试，是否确认?"), this,QStringLiteral("结束"), QStringLiteral("返回考试"));
   //if (pTips && QDialog::Accepted == pTips->exec()) {
   bool b = mCefWebView->isVisible();
   if (mCefWebView && mbLoadFinished) {
      QString funPage = QString("ClientCallCloseAnswer()");//调用关闭操作
      mCefWebView->AppCallJsFunction(funPage);
   }
   else {
      sendQtStopAnswer();
   }

   //}

   //if (pTips) {
   //   delete pTips;
   //   pTips = nullptr;
   //}
}

void AnswerWebView::sendQtStopAnswer()
{
   globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_QUESTION_UI));
   globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_QUESTION_UI));

}


void AnswerWebView::slot_close() {
    int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;

    if (roleType == JOIN_ROLE_Teacher) {
        if (mbLoadFinished /*&& ui.AnserUrlWdg->isVisible()*/) {
           mCefWebView->AppCallJsFunction("closeAnswer()");
			globalToolManager->GetDataManager()->WriteLog("%s 106  slot_close_closeAnswer()\n", __FUNCTION__, mbLoadFinished);
        }
		//if (mWebView)
		//{
		//	delete mWebView;
		//	mWebView = NULL;
		//}
		else
		{
			//killTimer(mShowAnswerWgt);
			hide();
			//movie->stop();
			mbLoadFinished = false;
			globalToolManager->GetDataManager()->WriteLog("%s  slot_close", __FUNCTION__);
			globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_QUESTION_UI));
		}

    }
}

void AnswerWebView::mouseMoveEvent(QMouseEvent * event)
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


void AnswerWebView::SetTitleChanged(const QString& strTitle)
{
   setWindowTitle(strTitle);
   mCefWebView->setWindowTitle(strTitle);
   ui.label->setText( strTitle);

   ui.label->setEnabled(true);
   ui.btnClose->setEnabled(true);
   setEnabled(true);
}

void AnswerWebView::slot_loadFinished(const QString& strTitle/*bool finish*/) {
	if (!this->isVisible())
	{
		return;
	}
   //mbLoadFinished = finish;
   SetTitleChanged(strTitle);
    mbLoadFinished = true;
	globalToolManager->GetDataManager()->WriteLog("%s  slot_loadFinished\n", __FUNCTION__);
    int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
    if (roleType == JOIN_ROLE_Teacher || roleType == JOIN_ROLE_Student) {
        CBaseWnd* pWdg = globalToolManager->GetWndManager()->FindWnd(WND_ID_WEBRTC_MAIN);
        if (pWdg) {
            CSTDMapParam mapParam;
            mapParam[Str_Function] = Str_Function_UserLine;
            pWdg->GetParam(mapParam);
            std::map<QString, QString>::iterator iter = mapParam.find(Str_OnLineNum);
            if (iter != mapParam.end()) {
                QString fun = QString("userOnLineNumber(%1)").arg(iter->second);
                globalToolManager->GetDataManager()->WriteLog("%s %s", __FUNCTION__, fun.toStdString().c_str());
                mCefWebView->AppCallJsFunction(fun);
                
				//mShowAnswerWgt = startTimer(1000);
            }
        }
    }

}

void AnswerWebView::closeEvent(QCloseEvent* event) {
    if (event) {
        event->ignore();
        slot_close();
    }
}

//MsgCallbackDelegate
void AnswerWebView::OnRecvMsg(std::string msg_name, std::string msg)
{
   CefMsgEvent *cefMsg = new CefMsgEvent(CustomEvent_LibCefMsg, QString::fromStdString(msg_name), QString::fromStdString(msg));
   QApplication::postEvent(this, cefMsg);
}

void AnswerWebView::OnWebViewLoadEnd()
{
}

void AnswerWebView::OnWebViewDestoryed()
{

}

void AnswerWebView::customEvent(QEvent* event) {

   if (event) {
      switch (event->type()) {
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

void AnswerWebView::JsCallQtMsg(QString name, QString param)
{
   //if (name.compare("sendQtCloseSuccess") == 0) {
   //   globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
   //   //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
   //   globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));

   //}
   //else 
   if (name.compare("sendQtStopAnswer") == 0) {
      sendQtStopAnswer();
   }

}

void AnswerWebView::moveXY()
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

void AnswerWebView::resizeEvent(QResizeEvent *event)
{
   CBaseWnd::resizeEvent(event);
   moveXY();
}

void AnswerWebView::showEvent(QShowEvent *event)
{
   CBaseWnd::showEvent(event);
   moveXY();
   //mCefTestView->LoadUrl(m_TestUrl);
}