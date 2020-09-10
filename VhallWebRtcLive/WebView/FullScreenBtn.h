#pragma once

#include <QWidget>
#include "ui_FullScreenBtn.h"

class FullScreenBtn : public QWidget
{
    Q_OBJECT

public:
    FullScreenBtn(QWidget *parent = Q_NULLPTR);
    ~FullScreenBtn();

private slots:
    void slot_clickedFullScreen();

signals:
    void sig_fullScreen();
private:
    Ui::FullScreenBtn ui;
};
