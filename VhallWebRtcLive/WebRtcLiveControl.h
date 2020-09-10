#pragma once

#include <QObject>
#include "../Unility/Unility.h"

class WebRtcLiveControl : public CBaseWnd
{
    Q_OBJECT

public:
    WebRtcLiveControl(QObject *parent = nullptr);
    virtual ~WebRtcLiveControl();
    virtual void Notice(const CSTDMapParam &);

    CREATE_WND();

};
