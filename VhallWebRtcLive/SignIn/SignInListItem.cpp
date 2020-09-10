#include "SignInListItem.h"
#include <QLabel>

SignInListItem::SignInListItem(QWidget *parent) :
	CWidget(parent) {
	ui.setupUi(this);
	//this->setFixedWidth(403);
}

SignInListItem::~SignInListItem() {

}

void SignInListItem::addStudent(const QString& strName)
{
	if (!strName.trimmed().isEmpty())
	{
		ui.labNoCountTip->hide();

		QString	str = GetString(strName, 10);
		QLabel* pLabel = new QLabel(this);
		//pLabel->setObjectName(QStringLiteral("label"));
		pLabel->setGeometry(QRect(80, 130, 30, 10));
		pLabel->setFixedSize(QSize(113, 30));
		pLabel->setAlignment(Qt::AlignCenter);
		pLabel->setText(str);

		int iR = miCount / 3 + 2;
		int iC = miCount % 3;
		ui.gridLayout->addWidget(pLabel, iR, iC, 1, 1);
		//this->setFixedHeight(40 * (iR + 2));
		this->setFixedHeight(40*(iR+1) );
		miCount++;
		showTitle();
		mListLabel.append(pLabel);
	}
	else
	{
	}
	//if (miCount<15)
	//{
	//	addStudent("666");
	//}
}

void SignInListItem::setTitle(const eSignInType& type)
{
	QString strTips = "";
	switch (type)
	{
	case	eSignInType_SignIn:
	{
		mstrTitle = QStringLiteral("签到学员");
		strTips = QStringLiteral("无签到学员");
		ui.labIcon->setStyleSheet("border-image: url(:/idention/img/idention/signInStudent.png);");
	}
	break;
	case	eSignInType_UnSign:
	{
		mstrTitle = QStringLiteral("未签到学员");
		strTips = QStringLiteral("无未签到学员");
		ui.labIcon->setStyleSheet("border-image:url(:/idention/img/idention/unSignStudent.png);");
	}
	break;
	case	eSignInType_NotPresent:
	{
		mstrTitle = QStringLiteral("未出席学员");
		strTips = QStringLiteral("无未出席学员");
		ui.labIcon->setStyleSheet("border-image:url(:/idention/img/idention/notPresentStudent.png);");
	}
	break;

	default:
		break;
	}
	
	if (0 == miCount)
	{
		ui.labNoCountTip->setText(strTips);
		ui.labNoCountTip->show();
		this->setFixedHeight(80);
	}
	else
	{

	}
	showTitle();
}

void SignInListItem::showTitle()
{
	ui.labTitle->setText(mstrTitle + QString("(%1)").arg(miCount) );
}

void SignInListItem::clearData()
{
	QLabel* pLabel;
	while (mListLabel.count() > 0)
	{
		pLabel = mListLabel[0];
		if (pLabel)
		{
			ui.gridLayout->removeWidget(pLabel);
			delete pLabel;
			pLabel = NULL;
			mListLabel.removeAt(0);
		}

	}
	miCount = 0;
	ui.labTitle->setText(mstrTitle + QString("(%1)").arg(miCount));
}

QString SignInListItem::GetString(QString s, int iwPx)
{
	QString temp = s;
	QRegExp rx("[\u4e00-\u9fa5]");
	if (temp.replace(rx, "zz").length() <= iwPx) {
		return s;
	}

	for (int i = s.length(); i >= 0; i--) {
		temp = s.mid(0, i);
		if (temp.replace(rx, "zz").length() <= iwPx) {
			return s.mid(0, i) + "...";
		}
	}
	return QString("");
}