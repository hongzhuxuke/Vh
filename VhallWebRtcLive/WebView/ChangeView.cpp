#include "ChangeView.h"

ChangeView::ChangeView(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
   connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(slot_changeLayout()));
   this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint | Qt::Tool);

}

ChangeView::~ChangeView()
{
}


void ChangeView::enterEvent(QEvent *event) {
   show();
}

void ChangeView::leaveEvent(QEvent *event) {
   hide();
}

void ChangeView::slot_changeLayout() {
   emit sig_changeLayout();
}