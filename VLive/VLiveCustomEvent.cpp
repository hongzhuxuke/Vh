#include "VLiveCustomEvent.h"
#include "CustomEventType.h"
#include "../Unility/Unility.h"

VLiveCustomEvent::VLiveCustomEvent(QObject *parent)
    : QObject(parent)
{
}

VLiveCustomEvent::~VLiveCustomEvent()
{
}


void VLiveCustomEvent::customEvent(QEvent *event) {
    if (event == nullptr) {
        return;
    }
    int type = event->type();
    switch (type)
    {
        case CustomEvent_DestoryWnd: {
            HandleDestoryWnd(event);
            break;
        }
        case CustomEvent_SafeToExitApp: {
            ToolManager::GetInstance()->AppQuit();
            break;
        }
        case CustomEvent_TurnToLogin: {
            CBaseWnd* wnd1 = ToolManager::GetInstance()->GetWndManager()->FindWnd(WND_ID_LOGIN);
            wnd1->show();
            break;
        }
        default:
            break;
    }
}


void VLiveCustomEvent::HandleDestoryWnd(QEvent* event) {
    if (event == nullptr) {
        return;
    }
    QEventDestoryWnd* destoryEvent = dynamic_cast<QEventDestoryWnd*>(event);
    if (destoryEvent) {
        ToolManager::GetInstance()->GetWndManager()->DestoryWnd(destoryEvent->mWndId);
    }
}