#include "LanguageSetDlg.h"
#include "../Unility/Unility.h"


#include <QPainter>
#include <QTranslator>

extern ToolManager *globalToolManager;

LanguageSetDlg::LanguageSetDlg(QWidget *parent /*= Q_NULLPTR*/)
	:QDialog(parent)
{
	ui.setupUi(this);
	this->setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(true);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
	qtrans = new QTranslator(this);
	mPixmap.load("..\\VLive\\img\\widget\\morechouse.png");
	//bool bd = mPixmap.load(":\\widget\\ img\\widget\\morechouse.png");
	//setFixedSize(mPixmap.width(), mPixmap.height());
}

LanguageSetDlg::~LanguageSetDlg()
{
	if (nullptr != qtrans)
	{
		delete qtrans;
		qtrans = nullptr;
	}


}

void LanguageSetDlg::SetRoleType(const int& iRolType) {
	switch (iRolType)
	{
	case JOIN_ROLE_Teacher:// 1 : 老师
		ui.labRol->setText(QStringLiteral("讲师"));
		break;
	case JOIN_ROLE_Student://  2 ; 学员
		ui.labRol->setText(QStringLiteral("学员"));
		break;
	case JOIN_ROLE_Assistant:// 3 : 助教
		ui.labRol->setText(QStringLiteral("助教"));
		break;
	case JOIN_ROLE_Guest://4 : 嘉宾
		ui.labRol->setText(QStringLiteral("嘉宾"));
		break;
	case JOIN_ROLE_Supervise://5 : 监课
		ui.labRol->setText(QStringLiteral("监课"));
		break;
	case JOIN_ROLE_Admin://6 : 管理员
		ui.labRol->setText(QStringLiteral("管理员"));
		break;
	}
}
void LanguageSetDlg::SetNickName(const QString& strNickName) {
	ui.labNick->setText(strNickName);
}

void LanguageSetDlg::show(int x, int y)
{
	QDialog::show();
	move(x/*-222*/, y);
	setFocus();
}

void LanguageSetDlg::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	
	//bool bd = mPixmap.load("E:\\workSpace\\VhallClassRoom\\VLive\\img\\widget\\morechouse.png");

	painter.drawPixmap(rect(), mPixmap.copy(0, 0, mPixmap.width(), mPixmap.height()));
	QDialog::paintEvent(e);
}

//void LanguageSetDlg::showEvent(QShowEvent *e) {
//	setFocus();
//	QDialog::showEvent(e);
//}
//
//void LanguageSetDlg::enterEvent(QEvent *e) {
//	QDialog::enterEvent(e);
//	m_bEnter = true;
//}
//void LanguageSetDlg::leaveEvent(QEvent *e) {
//	QDialog::leaveEvent(e);
//	m_bEnter = false;
//}
//
//void LanguageSetDlg::focusOutEvent(QFocusEvent *e) {
//	if (!m_bEnter) {
//		hide();
//		this->close();
//		return;
//	}
//
//	QWidget *focusWidget = this->focusWidget();
//	bool focusThis = false;
//
//	while (focusWidget) {
//		if (focusWidget == this) {
//			focusThis = true;
//			break;
//		}
//
//		focusWidget = focusWidget->parentWidget();
//	}
//
//	if (!focusThis) {
//		this->close();
//		m_bEnter = false;
//	}
//	else {
//		this->setFocus();
//	}
//}

void LanguageSetDlg::on_btnSimplified_clicked(bool checked /*= false*/) {
	miLanguage = eLanguage_Simplified; 
	languageChanged();
	accept();
}

void LanguageSetDlg::on_btnTraditional_clicked(bool checked /*= false*/) {
	miLanguage = eLanguage_Traditional; 
	languageChanged();
	accept();
}

void LanguageSetDlg::on_btnEnglish_clicked(bool checked /*= false*/) {
	miLanguage = eLanguage_English;
	languageChanged();
	accept();
}



void LanguageSetDlg::translateUi()
{
	//ui.groupBox->setTitle(QObject::tr("language"));
	//ui.radioButton->setText(QObject::tr("english"));
	//ui.radioButton_2->setText(QObject::tr("simplified "));
	//ui.radioButton_3->setText(QObject::tr("traditional"));
	//setWindowTitle(QObject::tr("Mainwindow"));
	//ui.label->setText(QObject::tr("Language is being set up"));
}

void LanguageSetDlg::languageChanged()
{
	switch (miLanguage)
	{
	case eLanguage_Simplified:
		qtrans->load("./chinese.qm"); break;
	case eLanguage_Traditional:
		qtrans->load("./traditional.qm"); break;
	case eLanguage_English:
		qtrans->load("./english.qm"); break;
	default:
		break;
	}
}



void LanguageSetDlg::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange)
		translateUi();
	else
		QDialog::changeEvent(event);
}