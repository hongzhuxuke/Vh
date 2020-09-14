#pragma once

#include <QDialog>
#include "ui_languageSetDlg.h"
#include <QPixmap>

class QTranslator;

class LanguageSetDlg : public QDialog
{
	Q_OBJECT

public:
	LanguageSetDlg(QWidget *parent = Q_NULLPTR);
	~LanguageSetDlg();
	void SetRoleType(const int& iRolType);
	void SetNickName(const QString& strNickName);
	void show(int x, int y);
	void translateUi();

signals:
	
protected:
	void paintEvent(QPaintEvent *);
	//void showEvent(QShowEvent *);
	//virtual void focusOutEvent(QFocusEvent *);
	//virtual void enterEvent(QEvent *);
	//virtual void leaveEvent(QEvent *);

	void changeEvent(QEvent *);
private slots:
	void on_btnSimplified_clicked(bool checked /*= false*/);
	void on_btnTraditional_clicked(bool checked /*= false*/);
	void on_btnEnglish_clicked(bool checked /*= false*/);
	
private:
	void languageChanged();
	

	enum eLanguage
	{
		eLanguage_Simplified,
		eLanguage_Traditional,
		eLanguage_English,
	};

	Ui::languageSetDlg ui;
	QPixmap mPixmap;
	int miLanguage;
	QTranslator *qtrans = nullptr;
	
};
