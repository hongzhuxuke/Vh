#pragma once

#include <QWidget>

class CWidget : public QWidget
{
	Q_OBJECT

public:
	CWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~CWidget();
protected:
	void paintEvent(QPaintEvent *event);
};
