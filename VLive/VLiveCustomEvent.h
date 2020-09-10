#pragma once

#include <QObject>
#include <QEvent>

class VLiveCustomEvent : public QObject
{
    Q_OBJECT

public:
    VLiveCustomEvent(QObject *parent = nullptr);
    ~VLiveCustomEvent();
protected:
    virtual void customEvent(QEvent *);
private:
    void HandleDestoryWnd(QEvent* event);
};
