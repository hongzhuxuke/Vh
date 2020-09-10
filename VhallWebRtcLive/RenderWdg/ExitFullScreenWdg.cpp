#include "ExitFullScreenWdg.h"
extern ToolManager *globalToolManager;
ExitFullScreenWdg::ExitFullScreenWdg(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_OnClicked()));
}

ExitFullScreenWdg::~ExitFullScreenWdg()
{
}

void ExitFullScreenWdg::SetUserID(QString id) {
    mUserId = id;
}

void ExitFullScreenWdg::slot_OnClicked() {
    if (!mUserId.isEmpty()) {
        //QString id = WND_ID_FULL_SCREEN_RENDER + mUserId;
        globalToolManager->PostEventToSelectWnd(WND_ID_WEBRTC_MAIN, new QEventRoomEvent(CustomEvent_CloseFloatRender, 0, mUserId));
        //globalToolManager->PostEventToMainThread(new QEventDestoryWnd(CustomEvent_DestoryWnd, id));
        emit sig_clicked();
    }
    else {
        emit sig_clicked();
    }
}
