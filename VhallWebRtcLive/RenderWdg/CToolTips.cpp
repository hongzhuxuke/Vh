#include "CToolTips.h"

CToolTips::CToolTips(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CToolTips::~CToolTips()
{
}

void CToolTips::SetText(QString text) {
	ui.label_tips->setText(text);
    int totalShowMsgLen = fontMetrics().width(text);
    setMaximumWidth(totalShowMsgLen + 10);
    setMinimumWidth(totalShowMsgLen + 10);
}

void CToolTips::setSuspension()
{
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
}

void CToolTips::SetOnTop() {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}