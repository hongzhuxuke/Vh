#include "ESCExitNotice.h"

ESCExitNotice::ESCExitNotice(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
}

ESCExitNotice::~ESCExitNotice()
{
}

void ESCExitNotice::show()
{
   QWidget::show();
}
