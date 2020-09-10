#include "ApplyJoinWdg.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QJsonObject>
#include "VhallWebRtcLive.h"

#define MAX_TIME_OUT 15

CREATE_WND_FUNCTION(ApplyJoinWdg);
ApplyJoinWdg::ApplyJoinWdg(QWidget *parent)
: CBaseWnd(parent)
{
    ui.setupUi(this);
    mnMaxTimeOut = MAX_TIME_OUT;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(Slot_ShowTimeOut()));
    connect(ui.pushButtonAgree, SIGNAL(clicked()), this, SLOT(Slot_HostAgreeApply()));
    connect(ui.toolButtonRefuse, SIGNAL(clicked()), this, SLOT(Slot_HostRefuseApply()));
    //ui.toolButtonAgree->setStyleSheet(
    //    "QToolButton{border-image: url(\":/interactivity/replyYes\");}"
    //    "QToolButton:hover{border-image: url(\":/interactivity/replyYesHover\");}"
    //    "QToolButton:pressed {border-image: url(\":/interactivity/replyYes\");}"
    //    "QToolButton:disabled {border-image: url(\":/interactivity/replyYes\");}"
    //);

    //ui.toolButtonRefuse->setStyleSheet(
    //    "QToolButton{border-image: url(\":/interactivity/replyNo\");}"
    //    "QToolButton:hover{border-image: url(\":/interactivity/replyNoHover\");}"
    //    "QToolButton:pressed {border-image: url(\":/interactivity/replyNo\");}"
    //    "QToolButton:disabled {border-image: url(\":/interactivity/replyNo\");}"
    //);
   
    mpixmap = QPixmap(":/interactivity/askBackground");

    m_pCloseBtn = new TitleButton();
    m_pCloseBtn->loadPixmap(":/sysButton/close_button");
    connect(m_pCloseBtn,SIGNAL(clicked()),this,SLOT(Slot_GuestRefuseHostInvite()));
    ui.layout_close->addWidget(m_pCloseBtn);
    ui.widget_title->installEventFilter(this);
}

ApplyJoinWdg::~ApplyJoinWdg() {
}

void ApplyJoinWdg::InitTimeOut(int timeout) {
	mnMaxTimeOut = timeout;
}

void ApplyJoinWdg::Notice(const CSTDMapParam &mapParam) {
    std::map<QString, QString>::const_iterator iter = mapParam.find(USER_ID);
    if (iter != mapParam.end()) {
        SetUid(iter->second);
    }
}

void ApplyJoinWdg::Notice(const CSTDMapParamPtr &) {

}


QString ApplyJoinWdg::GetUid()
{
	return mStrUid;
}

void ApplyJoinWdg::SetUid(const QString& strUid)
{
	mStrUid = strUid;
	ui.labApplyInfo->setText(strUid + QStringLiteral("申请上麦"));
}

void ApplyJoinWdg::CenterWindow(QWidget* parent) {
   int x = 0;
   int y = 0;
   if (NULL == parent) {
      const QRect rect = QApplication::desktop()->availableGeometry();
      x = rect.left() + (rect.width()-width())/2;
      y = rect.top() + (rect.height()-height())/2;
   } else {
      QPoint point(0, 0);
      point = parent->mapToGlobal(point);
      x = point.x() + (parent->width()-width())/2;
      y = point.y() + (parent->height()-height())/2;
   }
   move(x, y);
}

void ApplyJoinWdg::Slot_HostAgreeApply() {
    if (m_pTimer) {
        m_pTimer->stop();
    }
    globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Accept user id:%s", __FUNCTION__, mStrUid.toStdString().c_str());
    globalToolManager->GetClassSDK()->AuditPublish(mStrUid.toStdString());
    globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_ASK_FOR_SPEAK + mStrUid));
}

void ApplyJoinWdg::Slot_HostRefuseApply() {
    if (m_pTimer) {
        m_pTimer->stop();
    }
    globalToolManager->GetDataManager()->WriteLog("%s AuditPublish::AuditPublish_Refused user id:%s", __FUNCTION__, mStrUid.toStdString().c_str());
    globalToolManager->GetClassSDK()->AuditPublish(mStrUid.toStdString());
    globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, WND_ID_ASK_FOR_SPEAK + mStrUid));
}

void ApplyJoinWdg::paintEvent(QPaintEvent *){
   QPainter painter(this);
   painter.drawPixmap(rect(),this->mpixmap);
}

bool ApplyJoinWdg::eventFilter(QObject *o, QEvent *e) {
    if(o==ui.widget_title)
    {
       if(e->type()==QEvent::MouseButtonPress) {
           this->pressPoint=this->cursor().pos();
           this->startPoint=this->pos();
       }
       else if(e->type()==QEvent::MouseMove) {
           int dx=this->cursor().pos().x()-this->pressPoint.x();
           int dy=this->cursor().pos().y()-this->pressPoint.y();
           this->move(this->startPoint.x()+dx,this->startPoint.y()+dy);
       }
    }
    return QWidget::eventFilter(o,e);
}

void ApplyJoinWdg::Slot_ShowTimeOut() {
   if (nTimeCount == 0) {
      emit Slot_HostRefuseApply();
      m_pTimer->stop();
   } else {
      QString notice = QStringLiteral("%1s后自动拒绝").arg(nTimeCount);
      ui.label_TimeNotice->setText(notice);
      nTimeCount--;
   }
}

void ApplyJoinWdg::showEvent(QShowEvent *) {
   if (m_pTimer) {
        m_pTimer->stop();
        m_pTimer->start(1000);
   }
   ui.label_TimeNotice->setText(QStringLiteral("%1s后自动拒绝").arg(mnMaxTimeOut));
   nTimeCount = mnMaxTimeOut;
}


