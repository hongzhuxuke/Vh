#include "VhallLivePlugInUnitDlg.h"
#include "ui_VhallLivePlugInUnitDlg.h"
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QTimer>
#include "VhallLiveMainDlg.h"
#include "ICommonData.h"
#include "Msg_VhallRightExtraWidget.h"
#include "CRPluginDef.h"
#include "MainUIIns.h"
#include "Msg_MainUI.h"
#include "title_button.h"
#include "DebugTrace.h"
#include "ConfigSetting.h"
#include "pathmanager.h"
#include "pub.Const.h"
#include "VhallIALive\VhallIALive.h"

VhallLivePlugInUnitDlg::VhallLivePlugInUnitDlg(QWidget *parent) :
CBaseDlg(parent),
ui(new Ui::VhallLivePlugInUnitDlg) {
   ui->setupUi(this);
   setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
   ui->widget_title->installEventFilter(this);
   ui->left->installEventFilter(this);
   ui->left_top->installEventFilter(this);
   ui->top->installEventFilter(this);
   ui->right_top->installEventFilter(this);
   ui->right->installEventFilter(this);
   ui->right_down->installEventFilter(this);
   ui->down->installEventFilter(this);
   ui->left_down->installEventFilter(this);

   this->installEventFilter(this);
   connect(this, SIGNAL(SigJsCode(QString)), this, SLOT(SlotJsCode(QString)));
   //resize(1000, 610);
   this->setFocusPolicy(Qt::ClickFocus);
   mNormalRect = this->geometry();
   connect(ui->widget_leftTool, &LiveToolWdg::sigClicked, this, &VhallLivePlugInUnitDlg::slotLiveToolChoise);
}

VhallLivePlugInUnitDlg::~VhallLivePlugInUnitDlg() {
    ReleasePluginUrl();
    if (ui) {
        delete ui;
        ui = NULL;
    }
}

void VhallLivePlugInUnitDlg::ReleasePluginUrl() {
    if (m_pWebChannel && m_pWebEngineView) {
        m_pWebChannel->deregisterObject(m_obj);
    }
    if (m_pWebEngineView) {
        ui->verticalLayout_web->removeWidget(m_pWebEngineView);
        delete m_pWebChannel;
        m_pWebChannel = NULL;
        delete m_pWebEngineView;
        m_pWebEngineView = NULL;
    }
}

void VhallLivePlugInUnitDlg::SetWindowTitle(QString title) {
   this->setWindowTitle(title);
   ui->label_tool->setText(title);
}

bool VhallLivePlugInUnitDlg::eventFilter(QObject *o, QEvent *e) {
   if (ui->widget_title == o) {
      if (e->type() == QEvent::MouseButtonPress) {
         QMouseEvent *me = dynamic_cast<QMouseEvent *>(e);
         if (me) {
            m_PressPoint = me->globalPos();
            m_StartPoint = this->pos();
         }
      } else if (e->type() == QEvent::MouseMove) {
         QMouseEvent *me = dynamic_cast<QMouseEvent *>(e);
         if (me) {
            if (ui->widget_title == o && !this->isMaximized()) {
               this->move(m_StartPoint + me->globalPos() - m_PressPoint);
            }
         }
      } else if (e->type() == QEvent::MouseButtonDblClick) {
         this->SlotMaxClicked();
      } else if (e->type() == QEvent::Enter) {
         this->setCursor(QCursor(Qt::ArrowCursor));
         qDebug() << "VhallWebViewFree::eventFilter Enter";
      }
   } else if (this == o) {
      if (e->type() == QEvent::FocusIn) {
         qDebug() << "VhallWebViewFree::eventFilter QEvent::FocusIn";
         this->raise();
      } 
   } else {
      if (e->type() == QEvent::MouseButtonPress) {
         this->m_PressPoint = this->cursor().pos();
         this->m_startRect = this->geometry();
      } else if (e->type() == QEvent::MouseMove) {

         QRect geometry = m_startRect;
         QPoint diff = this->cursor().pos() - this->m_PressPoint;

         int left = geometry.x();
         int top = geometry.y();
         int right = geometry.x() + geometry.width();
         int bottom = geometry.y() + geometry.height();

         int new_left = left;
         int new_top = top;
         int new_right = right;
         int new_bottom = bottom;

         int x_diff = diff.x();
         int y_diff = diff.y();

         int min_width = this->minimumWidth();
         int min_height = this->minimumHeight();

         if (ui->left == o) {
            new_left = left + diff.x();
            if (right - new_left < min_width) {
               new_left = right - min_width;
            }
         } else if (ui->left_top == o) {
            new_left = left + diff.x();
            if (right - new_left < min_width) {
               new_left = right - min_width;
            }
            new_top = top + diff.y();
            if (bottom - new_top < min_height) {
               new_top = bottom - min_height;
            }
         } else if (ui->top == o) {
            new_top = top + diff.y();
            if (bottom - new_top < min_height) {
               new_top = bottom - min_height;
            }
         } else if (ui->right_top == o) {
            new_right = right + diff.x();
            if (new_right - left < min_width) {
               new_right = left + min_width;
            }
            new_top = top + diff.y();
            if (bottom - new_top < min_height) {
               new_top = bottom - min_height;
            }
         } else if (ui->right == o) {
            new_right = right + diff.x();
            if (new_right - left < min_width) {
               new_right = left + min_width;
            }
         } else if (ui->right_down == o) {
            new_right = right + diff.x();
            if (new_right - left < min_width) {
               new_right = left + min_width;
            }
            new_bottom = bottom + diff.y();
            if (new_bottom - top < min_height) {
               new_bottom = top + min_height;
            }
         } else if (ui->down == o) {
            new_bottom = bottom + diff.y();
            if (new_bottom - top < min_height) {
               new_bottom = top + min_height;
            }
         } else if (ui->left_down == o) {
            new_left = left + diff.x();
            if (right - new_left < min_width) {
               new_left = right - min_width;
            }
            new_bottom = bottom + diff.y();
            if (new_bottom - top < min_height) {
               new_bottom = top + min_height;
            }
         }

         geometry.setX(new_left);
         geometry.setY(new_top);
         geometry.setWidth(new_right - new_left);
         geometry.setHeight(new_bottom - new_top);

         //if (m_pWebEngineView) {
         //   int width = ui->widget_10->width();
         //   int height = ui->widget_10->height();
         //   m_pWebEngineView->resize(width,height);
         //}
         if (!this->isMaximized()) {
            this->setGeometry(geometry);
         }
      }
   }

   return QWidget::eventFilter(o, e);
}
bool VhallLivePlugInUnitDlg::nativeEvent(const QByteArray &eventType, void *message, long *result) {
   MSG* msg = reinterpret_cast<MSG*>(message);
   if (WM_SETCURSOR == msg->message) {
      if (ui->widget_title->rect().contains(ui->widget_title->mapFromGlobal(this->cursor().pos()))) {
         SetCursor(LoadCursor(NULL, IDC_ARROW));
      }
   }
   return QWidget::nativeEvent(eventType, message, result);
}

bool VhallLivePlugInUnitDlg::Create() {
   m_pBtnRefresh = new TitleButton(this);
   m_pBtnRefresh->loadPixmap(":/sysButton/refresh_button");
   m_pBtnRefresh->setToolTip(QString::fromWCharArray(L"刷新"));
   m_pBtnRefresh->setStyleSheet(TOOLTIPQSS);
   connect(m_pBtnRefresh, SIGNAL(clicked()), this, SLOT(SlotRefresh()));
   ui->layout_tool->addWidget(m_pBtnRefresh);

   m_pBtnMin = new TitleButton(this);
   m_pBtnMin->loadPixmap(":/sysButton/min_button");
   ui->layout_tool->addWidget(m_pBtnMin);
   connect(m_pBtnMin, SIGNAL(clicked()), this, SLOT(SlotMinClicked()));

   m_pBtnMax = new TitleButton(this);
   m_pBtnMax->loadPixmap(":/sysButton/max_button");
   ui->layout_tool->addWidget(m_pBtnMax);
   connect(m_pBtnMax, SIGNAL(clicked()), this, SLOT(SlotMaxClicked()));

   m_pBtnClose = new TitleButton(this);
   m_pBtnClose->loadPixmap(":/sysButton/close_button");
   ui->layout_tool->addWidget(m_pBtnClose);
   connect(m_pBtnClose, SIGNAL(clicked()), this, SLOT(SlotClose()));

   return true;
}
void VhallLivePlugInUnitDlg::Destory() {

}
void VhallLivePlugInUnitDlg::CenterWindow(QWidget* parent) {
   int x = 0;
   int y = 0;
   if (NULL == parent) {
      const QRect rect = QApplication::desktop()->availableGeometry();
      x = rect.left() + (rect.width() - width()) / 2;
      y = rect.top() + (rect.height() - height()) / 2;
   } else {
      QPoint point(0, 0);
      point = parent->mapToGlobal(point);
      x = point.x() + (parent->width() - width()) / 2;
      y = point.y() + (parent->height() - height()) / 2;
   }
   move(x, y);
}

void VhallLivePlugInUnitDlg::Load(QString url, QObject *obj) {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        m_obj = obj;
        m_bInit = false;
        m_pluginUrl = url;
        AddWebEngineView();
        if (m_pWebEngineView && m_pWebChannel) {
            m_pWebEngineView->page()->profile()->clearHttpCache(); // 清理缓存
            m_pWebEngineView->page()->profile()->clearAllVisitedLinks(); // 清理浏览记录
            m_pWebEngineView->page()->profile()->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);
            m_pWebEngineView->load(url);
            TRACE6("%s plugin_url:%s\n", __FUNCTION__, url.toStdString().c_str());
        }
        mIsLoadUrlFinished = false;
    }
    else{

    }
}

void VhallLivePlugInUnitDlg::loadFinished(bool ok) {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        TRACE6("VhallWebViewFree::loadFinished %d", ok ? 1 : 0);
        static int iLoadCount = 0;
        if (ok) {
            m_bInit = true;
            mIsLoadUrlFinished = true;
            TRACE6("%s IsLoadUrlFinished\n", __FUNCTION__);
            if (mbStartLive) {
                if (m_pWebEngineView) {
                    m_pWebEngineView->page()->runJavaScript("qtCallJsStartLive(1)");
                    TRACE6("%s qtCallJsStartLive\n", __FUNCTION__);
                }
                mbStartLive = false;
            }
        }
    }
    else {

    }
}


void VhallLivePlugInUnitDlg::show(const QRect& rect)
{
	if (isMinimized())
	{
		int iX = rect.x() + (rect.width() - mNormalRect.width()) / 2;
		int iy = rect.y() + (rect.height() - mNormalRect.height()) / 2;
		//this->setGeometry(mNormalRect);
		QWidget::showNormal();
		this->move(iX, iy);

	}
	else
	{
		int iX = rect.x() + (rect.width() - mNormalRect.width()) / 2;
		int iy = rect.y() + (rect.height() - mNormalRect.height()) / 2;
		QWidget::show();
		this->move(iX, iy);
	}
}

void VhallLivePlugInUnitDlg::executeJSCode(QString method) {
   qDebug() << "executeJSCode " << method;
   emit SigJsCode(method);
}

void VhallLivePlugInUnitDlg::SlotJsCode(QString method) {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        if (m_bInit) {
            if (m_pWebEngineView) {
                m_pWebEngineView->page()->runJavaScript(method);
            }
        }
    }
    else {

    }

}

void VhallLivePlugInUnitDlg::SlotMaxClicked() {
   if (this->isMaximized()) {
      this->showNormal();
      this->setGeometry(m_lastGeometry);
      m_pBtnMax->loadPixmap(":/sysButton/max_button");
   } else {
      m_lastGeometry = this->geometry();
      this->showMaximized();
      m_pBtnMax->loadPixmap(":/sysButton/normal_button");
   }
   this->repaint();
}

void VhallLivePlugInUnitDlg::SlotMinClicked() {
   this->showMinimized();
}

void VhallLivePlugInUnitDlg::SlotRefresh() {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        if (m_pWebChannel && m_pWebEngineView) {
            m_pWebEngineView->page()->setWebChannel(NULL);
            m_pWebChannel->deregisterObject(m_obj);
        }
        if (m_pWebEngineView) {
            ui->verticalLayout_web->removeWidget(m_pWebEngineView);
            delete m_pWebChannel;
            m_pWebChannel = NULL;
            delete m_pWebEngineView;
            m_pWebEngineView = NULL;
        }
        if (m_obj) {
            Load(m_pluginUrl, m_obj);
            m_bInit = false;
        }
    }
    else {
        if (initResp.selectLiveType == eLiveType_Live) {
            if (mpMainDlg) {
                if (m_pH5WebEngineViewTemp) {
                    ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
                    m_pH5WebEngineViewTemp = nullptr;
                }
                mpMainDlg->ReleaseDocWebView();
                mpMainDlg->CreateDocWebView();
                mpMainDlg->RemoveWebViewToPlugin(this);
                ClientApiInitResp startLiveData;
                pCommonData->GetInitApiResp(startLiveData);
                QString docUrl = startLiveData.web_doc_url;
                mpMainDlg->LoadDocUrl(docUrl);
            }
        }
        else if (initResp.selectLiveType == eLiveType_VhallActive && mpVliveDlg) {
            if (m_pH5WebEngineViewTemp) {
                ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
                m_pH5WebEngineViewTemp = nullptr;
            }
            mpVliveDlg->ReleaseDocWebView();
            mpVliveDlg->CreateDocWebView();
            mpVliveDlg->RemoveWebViewToPlugin(this);
        }
    }
}

void VhallLivePlugInUnitDlg::SlotClose() {
   emit SigClose();
}

void VhallLivePlugInUnitDlg::AddWebEngineView() {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        if (m_pWebEngineView == NULL && m_pWebChannel == NULL) {
            m_pWebEngineView = new QWebEngineView(this);
            m_pWebChannel = new QWebChannel(this);
            if (m_pWebEngineView && m_pWebChannel) {
                m_pWebEngineView->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
                m_pWebEngineView->page()->profile()->clearHttpCache(); // 清理缓存
                m_pWebEngineView->page()->profile()->clearAllVisitedLinks(); // 清理浏览记录
                m_pWebEngineView->page()->profile()->setHttpCacheType(QWebEngineProfile::HttpCacheType::NoCache);
                connect(m_pWebEngineView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
                QWebEngineSettings* pWebSettings = m_pWebEngineView->settings();
                if (pWebSettings) {
                    pWebSettings->setAttribute(QWebEngineSettings::AutoLoadImages, true);
                    pWebSettings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
                    pWebSettings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
                    pWebSettings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
                    pWebSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
                    pWebSettings->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
                }
                //如果不设置这就话，加载URL时会先显示以下白色背景。
                m_pWebEngineView->page()->setBackgroundColor(Qt::transparent);
                ui->verticalLayout_web->addWidget(m_pWebEngineView);
                m_pWebEngineView->setContextMenuPolicy(Qt::NoContextMenu);
                m_pWebChannel->registerObject("bridge", m_obj);
                m_pWebEngineView->page()->setWebChannel(m_pWebChannel);
            }
        }
        else {
            if (m_pWebEngineView) {
                m_pWebEngineView->show();
                ui->verticalLayout_web->addWidget(m_pWebEngineView);
            }
        }
    }
}

void VhallLivePlugInUnitDlg::JsCallQtDebug(QString msg) {
   qDebug() << msg;
}

void VhallLivePlugInUnitDlg::ReloadPluginUrl() {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        if (m_obj && mFirstLoad) {
            Load(m_pluginUrl, m_obj);
            mFirstLoad = false;
        }
    }
    else {
        if (initResp.selectLiveType == eLiveType_Live) {
            if (mpMainDlg) {
                if (m_pH5WebEngineViewTemp) {
                    ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
                    m_pH5WebEngineViewTemp = nullptr;
                }
                mpMainDlg->ReleaseDocWebView();
                mpMainDlg->CreateDocWebView();
                mpMainDlg->RemoveWebViewToPlugin(this);
            }
        }
        else {
            if (initResp.selectLiveType == eLiveType_VhallActive && mpVliveDlg) {
                if (m_pH5WebEngineViewTemp) {
                    ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
                    m_pH5WebEngineViewTemp = nullptr;
                }
                mpVliveDlg->ReleaseDocWebView();
                mpVliveDlg->CreateDocWebView();
                mpVliveDlg->RemoveWebViewToPlugin(this);
            }
        }
    }
}

void VhallLivePlugInUnitDlg::StartLive(bool start) {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        TRACE6("%s StartLive\n", __FUNCTION__);
        mbStartLive = start;
        if (m_pWebEngineView && mIsLoadUrlFinished) {
            m_pWebEngineView->page()->runJavaScript("qtCallJsStartLive(1)");
            TRACE6("%s qtCallJsStartLive\n", __FUNCTION__);
            mbStartLive = false;
        }
    }
}

void VhallLivePlugInUnitDlg::AddWebViewToPlguin(QWebEngineView* webView, VhallLiveMainDlg *mainDlg) {
    if (webView) {
        ui->verticalLayout_web->addWidget(webView);
        m_pH5WebEngineViewTemp = webView;
        if (m_pH5WebEngineViewTemp) {
            QString funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
            m_pH5WebEngineViewTemp->page()->runJavaScript(funPage);
        }
        mpMainDlg = mainDlg;
    }
}

void VhallLivePlugInUnitDlg::AddWebViewToPlguin(QWebEngineView* webView, VhallIALive *vliveDlg) {
    if (webView) {
        ui->verticalLayout_web->addWidget(webView);
        m_pH5WebEngineViewTemp = webView;
        if (m_pH5WebEngineViewTemp) {
            QString funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
            m_pH5WebEngineViewTemp->page()->runJavaScript(funPage);
        }
        mpVliveDlg = vliveDlg;
    }
}

void VhallLivePlugInUnitDlg::RemoveWebViewToMainView(VhallLiveMainDlg *mainDlg) {
    if (mainDlg && m_pH5WebEngineViewTemp != nullptr && ui->verticalLayout_web->count() > 0) {
        ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
        mainDlg->AddWebViewFromPlguinDlg(m_pH5WebEngineViewTemp);
        if (m_pH5WebEngineViewTemp) {
            QString funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Show_Doc);
            m_pH5WebEngineViewTemp->page()->runJavaScript(funPage);
        }
        m_pH5WebEngineViewTemp = nullptr;
        mainDlg = nullptr;
    }
}

void VhallLivePlugInUnitDlg::RemoveWebViewToMainView(VhallIALive *vliveDlg) {
    if (vliveDlg && m_pH5WebEngineViewTemp != nullptr && ui->verticalLayout_web->count() > 0) {
        ui->verticalLayout_web->removeWidget(m_pH5WebEngineViewTemp);
        vliveDlg->AddWebViewFromPlguinDlg(m_pH5WebEngineViewTemp);
        if (m_pH5WebEngineViewTemp) {
            QString funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Show_Doc);
            m_pH5WebEngineViewTemp->page()->runJavaScript(funPage);
        }
        m_pH5WebEngineViewTemp = nullptr;
    }
}

void VhallLivePlugInUnitDlg::JSCallQtResize() {
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        if (m_pWebEngineView) {
            //每次点击文档、白板切换时重置下窗体大小，否则在某些机器上显示异常
            QSize size = this->size();
            if (bResize) {
                this->resize(size + QSize(1, 1));
                bResize = false;
            }
            else {
                this->resize(size - QSize(1, 1));
                bResize = true;
            }
            m_pWebEngineView->repaint();
            repaint();
        }
    }
    else {

    }
}

void VhallLivePlugInUnitDlg::InitPluginUrl(QString url, QObject *obj) {
    m_obj = obj;
    m_bInit = false;
    VH::CComPtr<ICommonData> pCommonData;
    DEF_GET_INTERFACE_PTR(SingletonMainUIIns, PID_ICommonToolKit, IID_ICommonData, pCommonData, return, ASSERT(FALSE));
    ClientApiInitResp initResp;
    pCommonData->GetInitApiResp(initResp);
    if (initResp.player == FLASH_LIVE_CREATE_TYPE) {
        ui->widget_leftTool->hide();
        m_pluginUrl = url;
        mFirstLoad = true;
    }
    else {
        //h5 活动显示左边工具栏
        if (m_pWebEngineView) {
            m_pWebEngineView->hide();
        }
        ui->widget_leftTool->Create();
        ui->widget_leftTool->show();
    }
}

bool VhallLivePlugInUnitDlg::IsLoadUrlFinished() {
    return mIsLoadUrlFinished;
}


void VhallLivePlugInUnitDlg::slotLiveToolChoise(const int& iOpType) {

    QString funPage;
    switch (iOpType)
    {
    case eOpType_Questions://  问答      
    {
        funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Questions);
    }
    break;
    case eOpType_Signin://  签到   
    {
        funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Signin);
    }
    break;
    case eOpType_Luck:// 抽奖    
    {
        funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Luck);
    }
    break;
    case eOpType_questionnaire:// 问卷    
    {
        funPage = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_questionnaire);
    }
    break;
    case eOpType_envelopes:// 红包     
    {

    }
    break;
    default:
        break;
    }

    if (m_pH5WebEngineViewTemp && m_pH5WebEngineViewTemp->page() && !funPage.isEmpty()) {
        QString hideDoc = QString("QtCallFunctionPage(%1)").arg(eDocCtrlType_Hide_Doc);
        m_pH5WebEngineViewTemp->page()->runJavaScript(hideDoc);
        m_pH5WebEngineViewTemp->page()->runJavaScript(funPage);
    }
}

