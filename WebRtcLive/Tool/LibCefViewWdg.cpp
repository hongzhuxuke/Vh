#include "LibCefViewWdg.h"
//#include "VhallUI_define.h"
#include "../Unility/CustomEventType.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen> 
#include "../Unility/Unility.h"
//#include "DebugTrace.h"

extern ToolManager *globalToolManager;

LibCefViewWdg::LibCefViewWdg(MsgCallbackDelegate* delegate,QWidget *parent)
    : QWidget(parent)
   ,mDelegate(delegate)
{
    this->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 255);}");
    //this->setStyleSheet("QWidget{background-color: rgba(255, 0, 0, 255);}");
   //setAttribute(Qt::WA_TranslucentBackground);
   //setAutoFillBackground(true);
   mShowTimer = new QTimer(this);
   connect(mShowTimer,SIGNAL(timeout()),this,SLOT(slot_OnChnageWebView()));
   mShowTimer->setSingleShot(true);
}

LibCefViewWdg::~LibCefViewWdg()
{
   if (mDocCefView) {
      vhall_paas::DestoryVHWebViewObject(mDocCefView);
      mDocCefView = nullptr;
   }
}

void LibCefViewWdg::InitLibCef() {
   if (mDocCefView == nullptr) {
      mDocCefView = vhall_paas::GetVHWebViewObject();
      if (mDocCefView) {
         mDocCefView->InitlibCef();
      }
   }
}

void LibCefViewWdg::Create(QString url) {
   mUrl = url + "&view_type=cef";
   //TRACE6("%s mUrl:%s\n", __FUNCTION__, mUrl.toStdString().c_str());
   globalToolManager->GetDataManager()->WriteLog("%s mUrl:%s\n", __FUNCTION__, mUrl.toStdString().c_str());
}

void LibCefViewWdg::ChangeViewSize(int width, int height) {
   mDocCefView->ChangeWebViewSize(mDocIndex, width, height);
   globalToolManager->GetDataManager()->WriteLog("%s iW %d, iH %d", __FUNCTION__, width, height);
}

void LibCefViewWdg::CloseWeb() {
   if (mDocCefView) {
      mDocCefView->CloseAll();
   }
}

void LibCefViewWdg::LoadUrl(QString url) {
   mUrl = url + "&view_type=cef";
   if (mDocCefView) {
      mDocCefView->LoadUrl(mDocIndex, mUrl.toStdString().c_str());
      int iW = this->width();
      int iH = this->height();
      mDocCefView->ChangeWebViewSize(mDocIndex, iW, iH);
      //TRACE6("%s mUrl:%s  mDocIndex %d\n", __FUNCTION__, mUrl.toStdString().c_str(), mDocIndex);
      globalToolManager->GetDataManager()->WriteLog("%s mUrl:%s  mDocIndex %d iW %d, iH %d\n", __FUNCTION__, mUrl.toStdString().c_str(), mDocIndex, iW, iH);
   }
}

void LibCefViewWdg::AppCallJsFunction(QString function) {
   if (mDocCefView) {
      mDocCefView->AppCallJsFunction(mDocIndex, function.toStdString().c_str());
   }
}

void  LibCefViewWdg::showEvent(QShowEvent *e) {
   if (!mbShown) {
      if (mDocCefView) {
         int w = this->width();
         int h = this->height();
         globalToolManager->GetDataManager()->WriteLog("%s iW %d, iH %d", __FUNCTION__, w, h);
         mDocCefView->CreateWebView(this, (void *)this->winId(), w, h, mUrl.toStdString());
      }
      mbShown = true;
   }
   QWidget::showEvent(e);
}

void  LibCefViewWdg::resizeEvent(QResizeEvent *event) {
   if (mDocCefView) {
      int iW = this->width();
      int iH = this->height();
      mDocCefView->ChangeWebViewSize(mDocIndex, iW, iH);
      //mDocCefView->ChangeWebViewSize(mDocIndex, this->width(), this->height());
      globalToolManager->GetDataManager()->WriteLog("%s iW %d, iH %d", __FUNCTION__, iW, iH);
      
   }
}

/*创建WebView成功时的回调。
*参数：id 对应创建webview的id, url 创建webview时对应加载的url地址。
*用户根据返回的id，可操作指定的webview*/
void LibCefViewWdg::OnCreateWebView(int id, const char* url) {

   globalToolManager->GetDataManager()->WriteLog("%s mUrl:%s  url :%s\n", __FUNCTION__, mUrl.toStdString().c_str(), url);
   QString strHttps = "https:";
   QString strHttp = "http:";
   QString strmUrl;

   QString strTempUrl = QString(url);
   if (mUrl.indexOf(strHttps) == 0) {
      strmUrl = mUrl.mid( strHttps.length() );
   }
   else if (mUrl.indexOf(strHttp) == 0) {
      strmUrl = mUrl.mid(strHttp.length());
   }

   if (strTempUrl.indexOf(strHttps) == 0) {
      strTempUrl = strTempUrl.mid(strHttps.length());
   }
   else if (strTempUrl.indexOf(strHttp) == 0) {
      strTempUrl = strTempUrl.mid(strHttp.length());
   }


   if (url != nullptr && !mUrl.isEmpty() && (strmUrl.contains(strTempUrl) || strTempUrl.contains(strmUrl)) && /*mUrl == QString(url) &&*/ this && mDocCefView) {
      mDocIndex = id;
      //mDocCefView->ChangeWebViewSize(mDocIndex, this->width(), this->height());
      //mShowTimer->start(1500);
      CustomRecvCEFJSMsgEvent* event = new CustomRecvCEFJSMsgEvent(CustomEvent_LibCefLoadFinished);
      QApplication::postEvent(this, event);
   }
}

void LibCefViewWdg::slot_OnChnageWebView() {
   if (mDocCefView) {
      int iW = this->width();
      int iH = this->height();
      mDocCefView->ChangeWebViewSize(mDocIndex, iW, iH);
      globalToolManager->GetDataManager()->WriteLog("%s iW %d, iH %d", __FUNCTION__,iW, iH);
   }
   if (mDelegate) {
      mDelegate->OnWebViewLoadEnd();
   }
}

/*webview销毁事件通知*/
void LibCefViewWdg::OnWebViewDestoryed(int id) {
   if (id == mDocIndex && mDelegate) {
      mDelegate->OnWebViewDestoryed();
   }
}

void LibCefViewWdg::OnRecvMsg(const int id, std::string fun_name, std::string msg) {
   if (id == mDocIndex && mDelegate) {
      mDelegate->OnRecvMsg(fun_name, msg);
   }
}

void LibCefViewWdg::OnTitleChanged(const int id, std::string title_name)
{
	if (id == mDocIndex /*&& mDelegate*/) {
		CustomEventType* event = new CustomEventType(CustomEvent_TitleChanged, QString::fromStdString(title_name.c_str()));
		QApplication::postEvent(this, event);
	}
}

void LibCefViewWdg::customEvent(QEvent* event) {
   if (event) {
      int typeValue = event->type();
      switch (typeValue) {
      case CustomEvent_LibCefLoadFinished: {
         mShowTimer->start(500);
         //slot_OnChnageWebView(         break;
      }
	  case CustomEvent_TitleChanged: {
		  CustomEventType* msgevent = static_cast<CustomEventType*>(event);
		  emit sig_TitleChanged(msgevent->msg);
		  break;
	  }
      default:
         break;
      }
   }
}

void LibCefViewWdg::enterEvent(QEvent *event) {
   if (event) {
      int a = 0;
      a = 0;
   }
}

void LibCefViewWdg::keyPressEvent(QKeyEvent *event) {
   switch (event->key())
   {
   case Qt::Key_Escape:
      emit sig_PressKeyEsc();
      break;
   default:
      QWidget::keyPressEvent(event);
   }
}

void LibCefViewWdg::keyReleaseEvent(QKeyEvent *event) {
   switch (event->key())
   {
   case Qt::Key_Escape:
      emit sig_PressKeyEsc();
      break;

   default:
      QWidget::keyPressEvent(event);
   }
}

