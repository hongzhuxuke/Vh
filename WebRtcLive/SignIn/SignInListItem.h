#pragma once
#include "CWidget.h"
#include "ui_SignInListItem.h"

class SignInListItem : public CWidget
{
	Q_OBJECT
public:
	enum eSignInType
	{
		eSignInType_SignIn = 0,
		eSignInType_UnSign,
		eSignInType_NotPresent,
	};

	explicit SignInListItem(QWidget *parent = NULL);
	~SignInListItem();
	void addStudent(const QString& strName);
	void setTitle(const eSignInType& type);
	static QString GetString(QString s, int iwPx);
	void clearData();
protected:

public slots:
	
signals:

private slots:

private:
	void showTitle();

	Ui::SignInListItem ui;
	int miCount = 0;
	QString mstrTitle = "";
	QList<QLabel*> mListLabel;
};