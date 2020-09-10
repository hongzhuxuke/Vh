#pragma once

#include <QWidget>
#include "ui_TrophyWdg.h"

class QPropertyAnimation;

class TrophyWdg : public QWidget
{
	Q_OBJECT

public:
	TrophyWdg(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	//TrophyWdg(QWidget *parent);
	~TrophyWdg();
	void initTrophyRewardNum(const int& iNum);
	void setTrophyAdd(const int& iNum);
	void hideLabAdd();
	int iTrophyCount();
	void SetStyleSheet(const QString& strQss);
private slots:
	void SlotAdminFinished();
	void slotHideTrophyAdd();
private:
	Ui::TrophyWdg ui;
	QPropertyAnimation *m_propertyAdmin = NULL;
	int miCurNum = 0;

};
