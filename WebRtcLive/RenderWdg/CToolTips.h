#pragma once

#include <QWidget>
#include "ui_CToolTips.h"

class CToolTips : public QWidget
{
	Q_OBJECT

public:
	CToolTips(QWidget *parent = Q_NULLPTR);
	~CToolTips();
	void SetText(QString text);
    void setSuspension();
    void SetOnTop();
private:
	Ui::CToolTips ui;
};
