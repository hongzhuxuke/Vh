#include "TipsWdg.h"
#include<QScreen>
CREATE_WND_FUNCTION(TipsWdg);

#define ACCEPT_DEVTESTING QStringLiteral("QPushButton{font-size:14px;font-family : \"微软雅黑\";\
                                                color: rgb(255, 255, 255);\
                                                border:0px;\
                                              	border-radius:20px;\
                                              	background-color: rgb(82, 204, 144);}\
                                                QPushButton:hover{ \
												color: rgb(255, 255, 255); \
												border:0px; \
												border-radius:20px; \
											  	background-color: rgb(52, 184, 119); }\
                                                QPushButton:disable{\
												color: rgb(255, 255, 255); \
												border:0px; \
												border-radius:20px; \
												background - color: rgb(99, 224, 162); }")

TipsWdg::TipsWdg(QWidget *parent)
    : CBaseWnd(parent)
{
    ui.setupUi(this);
	if(Q_NULLPTR ==parent)
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	else
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    this->setWindowTitle(QStringLiteral("提示"));
    connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(reject()));
    ui.checkBox_recordSet->hide();
	ui.label_message->hide();
	ui.labelTips->setAlignment(Qt::AlignCenter);

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();
}

TipsWdg::~TipsWdg()
{

}

TipsWdg::TipsWdg(const QString& strMsg, QWidget *parent /*= Q_NULLPTR*/, const QString& strOk /*= ""*/, const QString strCancle /*= ""*/)
	: CBaseWnd(parent)
{
	ui.setupUi(this);
	if (Q_NULLPTR == parent)
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	else
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAutoFillBackground(false);
	this->setWindowTitle(QStringLiteral("提示"));
	connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(reject()));

	if (strOk.isEmpty())
		ui.pushButton_accept->hide();
	else
	{
		ui.pushButton_accept->setText(strOk);
		ui.pushButton_accept->show();
	}
	ui.labelTips->setText(strMsg);//提示内容消息
	if (strCancle.isEmpty())
	{
		ui.pushButton_reject->hide();
	}
	else
	{
		ui.pushButton_reject->setText(strCancle);
		ui.pushButton_reject->show();
	}
	
	ui.checkBox_recordSet->hide();
	ui.label_message->hide();
	ui.labelTips->setAlignment(Qt::AlignCenter);

   ui.label_4->hide();
   ui.label_5->hide();
   ui.label_6->hide();
   ui.label_7->hide();

   //居中显示
   QScreen *screen = QGuiApplication::primaryScreen();
   QRect mm = screen->availableGeometry();
   move(mm.width() / 2 - this->width() / 2, mm.height() / 2 - this->height() / 2);
}

TipsWdg::TipsWdg(QWidget *parent, const QString& strMsg  , const QString& strCancle)
	: CBaseWnd(parent)
{
	ui.setupUi(this);
	if (Q_NULLPTR == parent)
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	else
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground, true);
	this->setAutoFillBackground(false);
	this->setWindowTitle(QStringLiteral("提示"));
	connect(ui.pushButton_accept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton_reject, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(reject()));
	setFixedSize(450, 223);
	ui.labelTips->setFixedSize(380,20);
	ui.label_2->setAlignment(Qt::AlignLeft);
	ui.horizontalLayout_2->setContentsMargins(24, 7, 0, 0);
	ui.pushButton_accept->setText(QStringLiteral("检测帮助"));
	ui.horizontalLayout->setContentsMargins(300, 0, 0, 20);
	ui.pushButton_accept->setStyleSheet(ACCEPT_DEVTESTING);
	ui.pushButton_accept->setFixedSize(100,40);
	ui.pushButton_accept->show();
	ui.labelTips->setText(strMsg);//提示内容消息

	if (strCancle.isEmpty())
	{
		ui.pushButton_reject->hide();
	}
	else
	{
		ui.pushButton_reject->setText(strCancle);
		ui.pushButton_reject->show();
	}


	ui.checkBox_recordSet->hide();
	ui.label_message->hide();
	ui.labelTips->setAlignment(Qt::AlignCenter);

	ui.label_4->hide();
	ui.label_5->hide();
	ui.label_6->hide();
	ui.label_7->hide();

	//居中显示
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect mm = screen->availableGeometry();
	move(mm.width() / 2 - this->width() / 2, mm.height() / 2 - this->height() / 2);
}

void TipsWdg::Notice(const CSTDMapParam &param) {
    ui.pushButton_accept->show();
    ui.pushButton_reject->show();
    ui.labelTips->clear();
    std::map<QString, QString>::const_iterator iterMsg = param.find(QString::fromStdString(TipsMsg));
    std::map<QString, QString>::const_iterator iterShowCancelButton = param.find(QString::fromStdString(ShowCancelButton));
    if (iterMsg != param.end()) {
        ui.labelTips->setText(iterMsg->second);//提示内容消息
    }
    if (iterShowCancelButton != param.end() && iterShowCancelButton->second.compare(PARAM_TRUE) != 0) {//是否显示取消按钮
        ui.pushButton_reject->hide();
    }

	//居中显示
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect mm = screen->availableGeometry();
    move(mm.width()/2-this->width()/2, mm.height()/2-this->height()/2);


    std::map<QString, QString>::const_iterator iterShowRecordSet = param.find(QString::fromStdString(ShowRecordSet));
    if (iterShowRecordSet != param.end() && iterShowRecordSet->second.compare("1") == 0) {//是否显示复选框
        ui.checkBox_recordSet->show();
		ui.label_message->show();
    }
	else
	{
		ui.checkBox_recordSet->hide();
		ui.label_message->hide();
	}
	std::map<QString, QString>::const_iterator iterSize = param.find(QString::fromStdString(ShowTipsSize));
	if (iterSize != param.end()) {//Tips高度

		ui.labelTips->setFixedHeight(iterSize->second.toInt());
	}
}


void TipsWdg::GetParam(CSTDMapParam& param) {
    std::map<QString, QString>::iterator iterShowRecordSet = param.find(QString::fromStdString(ShowRecordSet));
    if (iterShowRecordSet != param.end()) {
        iterShowRecordSet->second = QString::number(ui.checkBox_recordSet->checkState());
    }
}