#pragma once

#include <QWidget>
#include "ui_ExitFullScreenWdg.h"
#include "../Unility/Unility.h"
class ExitFullScreenWdg : public QWidget
{
    Q_OBJECT

public:
    ExitFullScreenWdg(QWidget *parent = Q_NULLPTR);
    ~ExitFullScreenWdg();

    void SetUserID(QString id);

public slots:
    void slot_OnClicked();

signals:
    void sig_clicked();
private:
    Ui::ExitFullScreenWdg ui;
    QString mUserId;
};
