#pragma once

#include <QWidget>
#include "ui_ESCExitNotice.h"

class ESCExitNotice : public QWidget
{
    Q_OBJECT

public:
    ESCExitNotice(QWidget *parent = Q_NULLPTR);
    ~ESCExitNotice();
public slots:
   void show();
private:
    Ui::ESCExitNotice ui;
};
