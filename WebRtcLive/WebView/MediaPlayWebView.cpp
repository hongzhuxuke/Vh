#include "MediaPlayWebView.h"
#include "ShadowBoxDlg.h"
#include <QDesktopWidget>

//static int ANSWER_LOAD_PAGE = 1;
static int ANSWER_PAGE = 0;

extern ToolManager *globalToolManager;
CREATE_WND_FUNCTION(MediaPlayWebView);
MediaPlayWebView::MediaPlayWebView(QWidget *parent)
    : CBaseWnd(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint /*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAutoFillBackground(false);
	setWindowTitle(QStringLiteral ("²å²¥"));
   //mpShaowBoxDlg = (ShadowBoxDlg*)parent;

   mpCefWebView = new LibCefViewWdg(this);
   mpCefWebView->InitLibCef();
   ui.cefgridLayout->addWidget(mpCefWebView, 0, 0, 1, 1);
   mpCefWebView->setFixedSize(710, 360);
   setFixedSize(712, 402);
   //mpShaowBoxDlg->setLefRightBoerderWidth(25);
   //mpShaowBoxDlg->setFixedSize(760, 432);
	ui.pushButton_close->setFocusPolicy(Qt::NoFocus);
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(slot_close()));  

    ui.label_4->hide();
    ui.label_5->hide();
    ui.label_6->hide();
    ui.label_7->hide();
}

MediaPlayWebView::~MediaPlayWebView()
{
    if (mpCefWebView) {
        delete mpCefWebView;
        mpCefWebView = nullptr;
    }

	if (nullptr != movie)
	{
		delete movie;
		movie = nullptr;
	}
}

void MediaPlayWebView::OnRecvMsg(std::string funname, std::string argv)
{
   CefMsgEvent *cefMsg = new CefMsgEvent(CustomEvent_LibCefMsg, QString::fromStdString(funname), QString::fromStdString(argv));
   QApplication::postEvent(this, cefMsg);
}

void MediaPlayWebView::OnWebViewLoadEnd()
{

}

void MediaPlayWebView::OnWebViewDestoryed()
{

}

void MediaPlayWebView::Notice(const CSTDMapParam &mapParam) {
    //if (ui.WebView == nullptr) {
        //ui.WebView = new WebEngineViewWdg(ui.widget_2);
        if (mpCefWebView /*&&  ui.WebView->GetWebEngineViewWdg()*/) {
			/*disconnect(ui.WebView, SIGNAL(sig_loadFinished(bool)), this, SLOT(slot_loadFinished(bool)));
			ui.WebView->CreateWebView();*/
			//connect(ui.WebView, SIGNAL(sig_loadFinished(bool)), this, SLOT(slot_loadFinished(bool)));
         connect(mpCefWebView, &LibCefViewWdg::sig_TitleChanged, this, &MediaPlayWebView::slot_loadFinished);

            std::string mediaplayer = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().urls.mediaplayer;
            int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
            QString loadUrl = QString::fromStdString(mediaplayer) /*+ QString("&role_type=%1").arg(roleType)*/;
			//mbLoadFinished = false;
            mpCefWebView->Create(loadUrl);
            
            if (roleType != JOIN_ROLE_Teacher) {
                ui.pushButton_close->hide();
            }
            //ui.verticalLayout_view->addWidget(ui.WebView);
			if (movie == nullptr)
				movie = new QMovie(":/widget/img/widget/loading.gif");
			//movie->setScaledSize(QSize(300,300));
			ui.movie_lanel->setMovie(movie);
			movie->start();
			ui.stackedWidget->setCurrentIndex(ANSWER_PAGE);
         }
    //}

    std::map<QString, QString>::const_iterator iterMsg = mapParam.find(MEDIA_FILE_CTRL_MSG);
    if (iterMsg != mapParam.end()) {
        if (mpCefWebView) {
			ui.stackedWidget->setCurrentIndex(ANSWER_PAGE);
         mpCefWebView->AppCallJsFunction(iterMsg->second);
			
        }
    }

	std::map<QString, QString>::const_iterator iterCLOSEMsg = mapParam.find(CLOSE_MEDIA_FILE);
	if (iterCLOSEMsg != mapParam.end()) {
		hide();
		slot_close();
	}
	setEnabled(true);
	//ui.stackedWidget->setCurrentIndex(ANSWER_LOAD_PAGE);
}

void MediaPlayWebView::slot_close() {
	//emit sig_close();
    int roleType = globalToolManager->GetClassSDK()->GetClassRoomBaseInfo().mUserInfo.role_type;
    if (roleType == JOIN_ROLE_Teacher) {
        if (mpCefWebView && /*ui.WebView->IsLoadFinished() &&*/ mLoadFinsh && ANSWER_PAGE == ui.stackedWidget->currentIndex()) {
           mpCefWebView->AppCallJsFunction("closeMeadiaPlayer()");
			   globalToolManager->GetDataManager()->WriteLog("%s runJavaScript(\"closeMeadiaPlayer()\")", __FUNCTION__);
        }
			//globalToolManager->GetDataManager()->WriteLog("%s failt to runJavaScript(\"closeMeadiaPlayer()\") ui.WebView = %s£¬mbLoadFinished£º%s ", 
			//	__FUNCTION__, ui.WebView == nullptr, ui.WebView->IsLoadFinished());
            //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
			//if (nullptr != mLeftToolBarWdg)
			//{
			//	mLeftToolBarWdg->DestoryMediaPlayWebViewDlg();
			//}
		//hide();
		else
		{
		    movie->stop();
			mLoadFinsh = false;
			//ui.WebView->SetLoadFinished(false);
	    	globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
		}

    }
	else
	{
		globalToolManager->GetDataManager()->WriteLog("%s failt to runJavaScript(\"closeMeadiaPlayer()\") roleType = %d",
			__FUNCTION__, roleType);
	}
}

void MediaPlayWebView::show()
{
   //if (mpShaowBoxDlg) {
   //   mpShaowBoxDlg->show();
   //}
   CBaseWnd::show();
}

void MediaPlayWebView::hide()
{
   //if (mpShaowBoxDlg) {
   //   mpShaowBoxDlg->hide();
   //}
   CBaseWnd::hide();
}

void MediaPlayWebView::slot_loadFinished(const QString& strTitle /*bool finished*/) {
	if (!this->isVisible())
	{
		mLoadFinsh = false;
		return;
	}
	mLoadFinsh = true;
	if (mpCefWebView) {
      mpCefWebView->show();
	}
	movie->stop();
	setEnabled(true);
	ui.stackedWidget->setCurrentIndex(ANSWER_PAGE);
}

void MediaPlayWebView::SetLoadFinished(const bool& bloadfinish)
{
   //mpCefWebView->DestroyWebView();
	mLoadFinsh = bloadfinish;
}

void MediaPlayWebView::move(int x, int y)
{
   //if (mpShaowBoxDlg) {
   //   mpShaowBoxDlg->move(x - mpShaowBoxDlg->GetChildX(), y - mpShaowBoxDlg->GetChildY());
   //   mpShaowBoxDlg->raise();
   //}
   CBaseWnd::move(x, y);
}

void MediaPlayWebView::moveXY()
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

void MediaPlayWebView::resizeEvent(QResizeEvent *event)
{
   CBaseWnd::resizeEvent(event);
   moveXY();
}

void MediaPlayWebView::showEvent(QShowEvent *event)
{
   CBaseWnd::showEvent(event);
   moveXY();
   //mCefTestView->LoadUrl(m_TestUrl);
}

void MediaPlayWebView::closeEvent(QCloseEvent* event) {
    if (event) {
        event->ignore();
        //QWidget::closeEvent(event);
        slot_close();
		
    }
	CBaseWnd::closeEvent(event);
}
//
//void  MediaPlayWebView::showEvent(QShowEvent *event) {
//    if (ui.WebView != nullptr) {
//        ui.WebView->resize(ui.widget_2->width(), ui.widget_2->height());
//    }
//}

void MediaPlayWebView::mouseMoveEvent(QMouseEvent * event)
{
   if (event && (event->buttons() == Qt::LeftButton) && mIsMoved  && m_bIsEnableMove) {
      QPoint  newPos(event->globalPos() + mPressPoint);
      //if (mpShaowBoxDlg) {
      //   mpShaowBoxDlg->move(newPos.x() - mpShaowBoxDlg->GetChildX(), newPos.y() - mpShaowBoxDlg->GetChildY());
      //   mpShaowBoxDlg->raise();
      //}

      this->move(newPos.x(), newPos.y());
      this->raise();
   }
}

void MediaPlayWebView::customEvent(QEvent* event) {

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

void MediaPlayWebView::JsCallQtMsg(QString name, QString param)
{
   if (name.compare("sendQtCloseSuccess") == 0) {
      globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
      //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
      globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
   }


}