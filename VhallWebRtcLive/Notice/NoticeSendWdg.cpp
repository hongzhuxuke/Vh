#include "NoticeSendWdg.h"
#include "NoticeItem.h"
#include "NoticeShowWdg.h"

extern ToolManager *globalToolManager;

#define CanNotSend  QStringLiteral("QPushButton{font: 12px \"΢���ź�\";color:rgb(255, 255, 255);\
border:0px;border-top-right-radius:4px;border-bottom-right-radius:4px;background-color:rgb(166, 166, 168);}")

#define CanSend  QStringLiteral("QPushButton{font: 12px \"΢���ź�\";color:rgb(255, 255, 255);\
border:0px;border-top-right-radius:4px;border-bottom-right-radius:4px;background-color:rgb(82, 204, 144);}")

CREATE_WND_FUNCTION(NoticeSendWdg);
NoticeSendWdg::NoticeSendWdg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
	m_bIsEnableMove = false;
   setAttribute(Qt::WA_TranslucentBackground);
   setAutoFillBackground(false);
    connect(ui.textEdit, SIGNAL(textChanged()), this, SLOT(slot_textChanged()));
    connect(ui.pushButton_send, SIGNAL(clicked()), this, SLOT(slot_sendMsg()));
	connect(ui.textEdit, SIGNAL(returnPressed()), this, SIGNAL(slot_sendMsg()));
    ui.pushButton_send->setEnabled(false);
	mpNoticeShowWdg = new NoticeShowWdg(false, this);
	ui.vCLayout->insertWidget(0, mpNoticeShowWdg);
	mpNoticeShowWdg->hide();
	ui.textEdit->installEventFilter(this);
}

NoticeSendWdg::~NoticeSendWdg()
{
}

void NoticeSendWdg::showNoticShow(QString strNotic/* = ""*/)
{
	if(mpNoticeShowWdg)
		mpNoticeShowWdg->ShowNoticeMsg(strNotic);
}

bool NoticeSendWdg::isNoticShowWdgVisible()
{
	bool bRef = false;
	if (mpNoticeShowWdg)
		bRef = mpNoticeShowWdg->isVisible();
	return bRef;
}

void NoticeSendWdg::showNotic(bool bShow)
{
	if (mpNoticeShowWdg)
	{
		if (bShow)
			mpNoticeShowWdg->show();
		else
			mpNoticeShowWdg->hide();
	}
}

void NoticeSendWdg::showNoticWdg(bool bShow) {
	if (bShow) {
		ui.widget_send->show();
	}
	else {
		ui.widget_send->hide();
	}
}

void NoticeSendWdg::Notice(const CSTDMapParam &mapParam) {

}

void NoticeSendWdg::Notice(const CSTDMapParamPtr &mapParam) {

}    

void NoticeSendWdg::slot_sendMsg() {
    QString msg = ui.textEdit->toPlainText();
    QString text = QStringLiteral("[���ù���] %1").arg(msg);
    globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new CustomEventType(CustomEvent_ShowNoticeMsg, text));
    globalToolManager->GetClassSDK()->SendNoticeMsg(msg.toStdString());
    AppendMsg(msg);
	ui.textEdit->setText("");
    ui.textEdit->clear();
}

void NoticeSendWdg::AppendMsg(QString msg) {
    NoticeItem *notice = new NoticeItem(ui.listWidget);
    int newHeight = notice->InsertNoticeMsg(msg);
    QListWidgetItem *item = new QListWidgetItem(ui.listWidget);
    ui.listWidget->setItemWidget(item, notice);
    int count = ui.listWidget->count();
    ui.listWidget->insertItem(count,item);
    item->setSizeHint(QSize(320, newHeight));
    ui.listWidget->scrollToBottom();
}

bool NoticeSendWdg::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.textEdit)
	{
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *keyEvent = static_cast <QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				if (!ui.textEdit->toPlainText().isEmpty())
				{
					slot_sendMsg();
					return true;
				}				
			}

		}
	}
	return QObject::eventFilter(obj, event);
}


void NoticeSendWdg::slot_textChanged() {
    QString input_msg = ui.textEdit->toPlainText();
	if (input_msg == '\n')
	{
		ui.textEdit->clear();
		ui.pushButton_send->setEnabled(false);
		ui.pushButton_send->setStyleSheet(CanNotSend);
		return;
	}
    if (input_msg.isEmpty()) {
        ui.pushButton_send->setEnabled(false);
        ui.pushButton_send->setStyleSheet(CanNotSend);
    }
    else {
        ui.pushButton_send->setEnabled(true);
        if (input_msg.length() > 140){
            ui.textEdit->undo();//���Ѿ�������Ҫ����ʱ����ֻҪ��ԭ�Ϳ�������
            return;
        }
        ui.pushButton_send->setStyleSheet(CanSend);
    }
}

void NoticeSendWdg::paintEvent(QPaintEvent *event)
{
	QStyleOption opt; // ��Ҫͷ�ļ�#include <QStyleOption>  
	opt.init(this);
	QPainter p(this); // ��Ҫͷ�ļ�#include <QPainter>  
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}