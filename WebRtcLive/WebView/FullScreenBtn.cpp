#include "FullScreenBtn.h"

FullScreenBtn::FullScreenBtn(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_clickedFullScreen()));


}

FullScreenBtn::~FullScreenBtn()
{
}

void FullScreenBtn::slot_clickedFullScreen() {
    emit sig_fullScreen();
}
