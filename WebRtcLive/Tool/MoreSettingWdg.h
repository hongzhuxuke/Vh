#pragma once

#include <QDialog>
#include "ui_MoreSettingWdg.h"
#include <QPixmap>

class MoreSettingWdg : public QDialog
{
	Q_OBJECT

public:
	MoreSettingWdg(QWidget *parent = Q_NULLPTR);
	~MoreSettingWdg();
	void SetRoleType(const int& iRolType);
	void SetNickName(const QString& strNickName);
	
	void show(int x, int y);
	void hideMediaSet();
	void AddTrophyCount(const int& iAdd);
	int iTrophyCount();
protected:
	void showEvent(QShowEvent *);
	virtual void focusOutEvent(QFocusEvent *);
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
private slots:
	void on_btnMedia_clicked(bool checked = false);
	void on_btnLaguage_clicked(bool checked = false);
	void on_btnRolQuit_clicked(bool checked = false);
private:
private:
	Ui::MoreSettingWdg ui;
	bool m_bEnter = false;
};
