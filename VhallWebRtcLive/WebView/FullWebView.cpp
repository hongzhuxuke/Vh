#include "FullWebView.h"
#include <QDesktopWidget>
#include "ExitFullScreenWdg.h"
#include "ESCExitNotice.h"

#define HIDE_EXIT_BUTTON    3000

FullWebView::FullWebView(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();
    int screenWidth = screen.width();
    int screenHeight = screen.height();
    QSize screenSize(screenWidth, screenHeight);
    setFixedSize(screenSize);

    mExitFullScreen = new ExitFullScreenWdg(this);
    if (mExitFullScreen) {
        mExitFullScreen->hide();
        connect(mExitFullScreen, SIGNAL(sig_clicked()), this, SLOT(slot_exitFullScreen())); 
    }
    mESCExitNotice = new ESCExitNotice(this);
    if (mESCExitNotice) {
        mESCExitNotice->hide();
    }
    connect(&mHideExitFullScreenTimer, SIGNAL(timeout()), this, SLOT(slot_HideExitFullScreen()));
}

FullWebView::~FullWebView()
{
}

void FullWebView::AddWebView(QWidget* widget) {
    if (widget == nullptr) {
        return;
    }
    ui.verticalLayout->addWidget(widget);
    widget->setFocusPolicy(Qt::FocusPolicy::WheelFocus);
    widget->setFocus();
}

void FullWebView::RemoveWebView() {

}

void FullWebView::slot_HideExitFullScreen() {
    if (mExitFullScreen) {
        mExitFullScreen->hide();
    }
    if (mESCExitNotice) {
        mESCExitNotice->hide();
    }
}

void FullWebView::slot_exitFullScreen() {
    emit sig_exitFull();
}

void FullWebView::slot_ShowExitFullScreen() {
    if (mExitFullScreen) {
        mHideExitFullScreenTimer.start(HIDE_EXIT_BUTTON);
        mExitFullScreen->show();
        QDesktopWidget *desktop = QApplication::desktop();
        QRect screen = desktop->screenGeometry();
        mExitFullScreen->move(screen.width() / 2 - mExitFullScreen->width() / 2, screen.height() - mExitFullScreen->height() - 40);
    }
    if (mESCExitNotice) {
        mHideExitFullScreenTimer.start(HIDE_EXIT_BUTTON);
        mESCExitNotice->show();
        QDesktopWidget *desktop = QApplication::desktop();
        QRect screen = desktop->screenGeometry();
        mESCExitNotice->move(screen.width() / 2 - mESCExitNotice->width() / 2, 100);
    }
}

void FullWebView::mousePressEvent(QMouseEvent *e) {
    slot_ShowExitFullScreen();
    QWidget::mousePressEvent(e);
}

void FullWebView::mouseMoveEvent(QMouseEvent *e) {
    slot_ShowExitFullScreen();
    QWidget::mouseMoveEvent(e);
}

void FullWebView::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);
}

void FullWebView::keyPressEvent(QKeyEvent *event) {
    switch (event->key())
    {
    case Qt::Key_Escape:
        if (mExitFullScreen) {
            emit sig_exitFull();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}