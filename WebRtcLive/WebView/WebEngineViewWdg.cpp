#include "WebEngineViewWdg.h"
#include <QWebEngineProfile>
#include <QFileInfo>
#include <QFileDialog>
#include <windows.h>
#include <Shlobj.h>
#include <tchar.h>
#include <Commctrl.h>
#include <QDebug>

#pragma comment(lib, "comctl32.lib")

#define MENU_QSS              "QMenu {\
                     background-color:#ffffff; ; /*��������*/\
                     border: 3px solid #ffffff; ;/*�����˵���Ե*/  \
					 border-radius:8px; \
					}\
					 QMenu::item{ \
						 font-size: 12px; \
						 color: #666666;  /*������ɫ*/\
						 border: 3px solid #ffffff;    /*itemѡ��*/\
						 background-color: #ffffff;\
						 padding:2px 8px 2px 8px; /*���ò˵����������º����ҵ��ڱ߾࣬Ч�����ǲ˵��е���Ŀ�����������˼��*/\
						 margin:2px 2px;/*���ò˵������߾�*/\
					 }\
						 QMenu::item:selected{ \
														 background-color:#EEFAF4;/*ѡ�е���ʽ*/\
							border-color::#EEFAF4;/*ѡ�е���ʽEEFAF4*/ \
							color: #52CC90;  /*������ɫ*/\
					 }\
						 QMenu::item:pressed{/*�˵����Ч��*/\
												   border: 1px solid #ffffff;  \
												   background-color: #ffffff;  \
}"

#define FullScreen_QSS  "QPushButton{border-image:url(:/ctrl/img/ctrl/fullScreen_Normal.png);} \
    QPushButton:hover{border-image:url(:/ctrl/img/ctrl/full_screen_Click.png);}"

extern ToolManager *globalToolManager;


VHQWebEnginePage::VHQWebEnginePage(QObject *parent/* = Q_NULLPTR*/):
    QWebEnginePage(parent){

}

VHQWebEnginePage::~VHQWebEnginePage() {

}

QStringList VHQWebEnginePage::chooseFiles(QWebEnginePage::FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes) {
    qDebug() << oldFiles << "-------" << acceptedMimeTypes;
    if (mode == QWebEnginePage::FileSelectionMode::FileSelectOpen) {
        QStringList fileList;
        OPENFILENAME ofn = { 0 };
        TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
        ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
        ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
        ofn.lpstrFilter = TEXT("*.doc,*.docx,*.xls,*.xlsx,*.ppt,*.pptx,*.pdf,*.jpeg,*.jpg,*.png,*.bmp\0\0");//���ù���
        ofn.nFilterIndex = 1;//����������
        ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
        ofn.nMaxFile = sizeof(strFilename);//����������
        ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
        ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
        if (GetOpenFileName(&ofn))
        {
            QString fileName = QString::fromStdWString(strFilename);
            fileList.push_back(fileName);
        }
        return  fileList;
    }
    else {
        QStringList fileList;
        QString fileExt = acceptedMimeTypes.at(0);
        QString fileName = oldFiles.at(0);
        OPENFILENAME ofn = { 0 };
        TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
        fileName.toWCharArray(strFilename);
        ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
        ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
        ofn.lpstrFilter = fileExt.toStdWString().c_str();//���ù���
        ofn.nFilterIndex = 1;//����������
        ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
        ofn.nMaxFile = sizeof(strFilename);//����������
        ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������
        ofn.lpstrTitle = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���
        ofn.lpstrDefExt = fileExt.toStdWString().c_str();//Ĭ��׷�ӵ���չ��
        if (GetSaveFileName(&ofn))
        {
            fileList.push_back(QString::fromStdWString(strFilename));
        }
        return fileList;
    }
}


WebEngineViewWdg::WebEngineViewWdg(QWidget *parent)
    : CWidget(parent)
    , mbIsLoadFinished(false)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Widget /*| Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAutoFillBackground(false);

    rightMenu = new Menu(this);
    if (rightMenu) {
        rightMenu->setStyleSheet(MENU_QSS);
        mActionForbidden = new QAction(QStringLiteral("ˢ��"), rightMenu);
        rightMenu->addAction(mActionForbidden);
        connect(mActionForbidden,SIGNAL(triggered(bool)),this, SLOT(slot_WebFlush())); 
    }

    mpFullScreenBtn = new FullScreenBtn(this);
    if (mpFullScreenBtn) {
        mpFullScreenBtn->hide();
        connect(mpFullScreenBtn, SIGNAL(sig_fullScreen()), this, SLOT(slot_fullScreen()));
    }

}

WebEngineViewWdg::~WebEngineViewWdg()
{
    DestroyWebView();
}

void WebEngineViewWdg::CreateWebView(int type/* = 0*/) {
    if (mWebEngineView == NULL) {
        mWebEngineView = new QWebEngineView();
        mWebEnginePage = new VHQWebEnginePage(mWebEngineView);
        if (mWebEngineView) {
            mWebViewType = type;
            mWebEngineView->setPage(mWebEnginePage);
            QWebEnginePage* page = mWebEngineView->page();
            //mWebEngineView->page()->profile()->clearHttpCache(); // ������
            //mWebEngineView->page()->profile()->clearAllVisitedLinks(); // ���������¼
            QWebEngineSettings* pWebSettings = mWebEngineView->settings();
            if (pWebSettings) {
				pWebSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
				pWebSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
				pWebSettings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
				pWebSettings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
				pWebSettings->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
				pWebSettings->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
				pWebSettings->setAttribute(QWebEngineSettings::AutoLoadImages, true);
				pWebSettings->setAttribute(QWebEngineSettings::WebGLEnabled, true);
            }

            connect(mWebEngineView->page(), SIGNAL(proxyAuthenticationRequired(QUrl, QAuthenticator*, QString)), this, SLOT(slot_proxyAuthenticationRequired(QUrl, QAuthenticator*, QString)));
            connect(mWebEngineView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
            connect(mWebEngineView->page(), SIGNAL(finished(QNetworkReply *)), this, SLOT(loadFinished(QNetworkReply *)));
            connect(mWebEngineView->page(), SIGNAL(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)), SLOT(slot_featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)));
            connect(mWebEngineView->page(), SIGNAL(fullScreenRequested(QWebEngineFullScreenRequest)), SLOT(slot_fullScreenRequested(QWebEngineFullScreenRequest)));


            mWebEngineView->setContextMenuPolicy(Qt::NoContextMenu);
            QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            mWebEngineView->setSizePolicy(sizePolicy);
            //�����������ͻ�������URLʱ������ʾ���°�ɫ������
            mWebEngineView->page()->setBackgroundColor(Qt::transparent);
            ui.verticalLayout->addWidget(mWebEngineView);
            mWebEngineView->installEventFilter(this);
            if (type == WebViewType_LiveStream) {
                InitCtrl();
            }
        }
    }
    if (mWebChannel == NULL && mWebEngineView != NULL) {
        mWebChannel = new QWebChannel(this);
        mWebChannel->blockUpdatesChanged(true);
        if (mWebChannel) {
            mWebChannel->registerObject("bridge", this);
            mWebEngineView->page()->setWebChannel(mWebChannel);
        }
    }
}

bool WebEngineViewWdg::GetWebEngineViewWdg()
{
	if (mWebEngineView)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void WebEngineViewWdg::DestroyWebView() {
    if (mWebChannel && mWebEngineView) {
        //mWebEngineView->page()->setWebChannel(NULL);
        //mWebChannel->deregisterObject(this);
        //mWebChannel->deleteLater();
		delete mWebChannel;
        mWebChannel = nullptr;
    }
    if (mWebEngineView) {
        ui.verticalLayout->removeWidget(mWebEngineView);
        //mWebEngineView->stop();
        //mWebEngineView->hide();
        //mWebEngineView->close();
        //mWebEngineView->removeEventFilter(this);
        delete mWebEngineView;
		mWebEngineView = nullptr;
    }
}

void WebEngineViewWdg::LoadUrl(QString url,int urlType) {
    mUrlType = urlType;
    if (mUrlType == URLType_LiveMixedStream) {
        //this->setAutoFillBackground(false);
        //this->setMouseTracking(true);
        //grabKeyboard();

        this->setAutoFillBackground(false);

        if (mpBtnMoveTimer == nullptr) {
            mpBtnMoveTimer = new QTimer(this);
            connect(mpBtnMoveTimer, SIGNAL(timeout()), this, SLOT(slot_moveTimeOut()));
        }
        if (mpBtnHideTimer == nullptr) {
            mpBtnHideTimer = new QTimer(this);
            connect(mpBtnHideTimer, SIGNAL(timeout()), this, SLOT(slot_hideTimeOut()));
        }
    }

    if (mWebEngineView) {
		mStrUrl = url;
		globalToolManager->GetDataManager()->WriteLog("%s Start load url %s", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString().c_str(), mStrUrl.toStdString().c_str());
        mWebEngineView->page()->load(url);
    }
}

void  WebEngineViewWdg::loadFinished(bool finish) {
	globalToolManager->GetDataManager()->WriteLog("%s Finished load url %s",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString().c_str(), mStrUrl.toStdString().c_str());
    mbIsLoadFinished = finish;
    emit sig_loadFinished(finish);
}

bool WebEngineViewWdg::IsLoadFinished() 
{
	return mbIsLoadFinished; 
}

void WebEngineViewWdg::SetLoadFinished(const bool& bloadfinsh)
{
	mbIsLoadFinished = bloadfinsh;
}

void WebEngineViewWdg::Stop() {
    if (mWebEngineView) {
        mWebEngineView->stop();
    }
}

//void WebEngineViewWdg::SetFullState(bool isFull) {
//    bIsFull = isFull;
//}

void WebEngineViewWdg::SetLayOutPos(int pos) {
    mLayoutPos = pos;
}

int WebEngineViewWdg::GetLayOutPos() {
    return mLayoutPos;
}

void WebEngineViewWdg::runJavaScript(QString fun) {
    if (mWebEngineView) {
        mWebEngineView->page()->runJavaScript(fun);
    }
}

void WebEngineViewWdg::resizeEvent(QResizeEvent *event) {
   QString method = QString::fromStdString("QtCallJsWebviewResize(%1,%2)").arg(this->width()).arg(this->height());
   if (mWebEngineView) {
      mWebEngineView->page()->runJavaScript(method);
   }
}

void WebEngineViewWdg::JsCallQtMsg(QString msg) {
	emit sig_JsCallQt(msg);
}

void WebEngineViewWdg::sendQtCloseSuccess() {
    globalToolManager->GetDataManager()->WriteLog("%s",__FUNCTION__);
    //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
    globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_MEDIA_PLAY_WEBVIEW));
}

void WebEngineViewWdg::sendQtStopAnswer() {
    globalToolManager->GetDataManager()->WriteLog("%s", __FUNCTION__);
    //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_QUESTION_UI));
    globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN,new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_QUESTION_UI));
}

void WebEngineViewWdg::slot_OnStuClickedFloat() {

}

void WebEngineViewWdg::slot_OnStuClickedFullWnd() {

}

void WebEngineViewWdg::slot_featurePermissionRequested(const QUrl& url, QWebEnginePage::Feature feature) {
    if (mWebEngineView && mWebEngineView->page()) {
        mWebEngineView->page()->setFeaturePermission(url, feature, QWebEnginePage::PermissionGrantedByUser);
    }
}

void WebEngineViewWdg::slot_fullScreenRequested(QWebEngineFullScreenRequest request) {

}

void WebEngineViewWdg::slot_moveTimeOut() {
    if (mWebViewType == WebViewType_LiveStream && mUrlType == URLType_LiveMixedStream) {
        if (/*!bIsFull &&*/ !mpFullScreenBtn->isHidden()) {
            QPoint pos = this->mapToGlobal(QPoint(0, 0));
            qDebug() << pos;
            mpFullScreenBtn->move(pos + QPoint(this->width() - mpFullScreenBtn->width(), 10));
        }
    }
}

void WebEngineViewWdg::slot_hideTimeOut() {
    if (mWebViewType == WebViewType_LiveStream && mUrlType == URLType_LiveMixedStream) {
        mpFullScreenBtn->hide();
    }
}

void WebEngineViewWdg::slot_WebFlush() {
    if (mWebEngineView) {
        mWebEngineView->load(mStrUrl);
    }
}

void WebEngineViewWdg::slot_fullScreen() {
    //bIsFull = true;
    mpFullScreenBtn->hide();
    mpBtnMoveTimer->stop();
    mpBtnHideTimer->stop();
    emit sig_fullScreen();
}

bool WebEngineViewWdg::eventFilter(QObject * receiver, QEvent * event) {
    if (mWebEngineView && mWebEngineView == receiver) {
        if (event->type() == QEvent::ContextMenu) {
            if (rightMenu) {
                QPoint pos = QCursor::pos();
                rightMenu->show();
                rightMenu->move(pos);
            }
        }
    }
    return CWidget::eventFilter(receiver, event);
}

void WebEngineViewWdg::enterEvent(QEvent *event) {
   if (mUrlType == URLType_LiveMixedStream) {
      emit sig_Enter();
   }
}

void WebEngineViewWdg::leaveEvent(QEvent *event) {

}

void WebEngineViewWdg::InitCtrl() {
    if (mNormalCtrl == nullptr) {
        mNormalCtrl = new TeacherCtrlWdg(this);
        if (mNormalCtrl) { 
            mNormalCtrl->installEventFilter(this);
            mNormalCtrl->hide();
            mNormalCtrl->InitByWebView();
            connect(mNormalCtrl, SIGNAL(sig_OnStuClickedFloat()), this, SLOT(slot_OnStuClickedFloat()));
            connect(mNormalCtrl, SIGNAL(sig_OnStuClickedFullWnd()), this, SLOT(slot_OnStuClickedFullWnd()));
        }
    }
}
