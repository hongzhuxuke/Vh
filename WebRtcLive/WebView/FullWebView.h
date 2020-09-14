#pragma once

#include <QWidget>
//#include <WebEngineViewWdg.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include "ui_FullWebView.h"

class ExitFullScreenWdg;
class ESCExitNotice;
class FullWebView : public QWidget
{
    Q_OBJECT

public:
    FullWebView(QWidget *parent = Q_NULLPTR);
    ~FullWebView();

    void AddWebView(QWidget* widget);
    void RemoveWebView();

signals:
    void sig_exitFull();

public slots:
    void slot_HideExitFullScreen();
    void slot_ShowExitFullScreen();
    void slot_exitFullScreen();
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *e);
private:
    Ui::FullWebView ui;
    QTimer mHideExitFullScreenTimer;
    ExitFullScreenWdg* mExitFullScreen;
    ESCExitNotice* mESCExitNotice;
};
