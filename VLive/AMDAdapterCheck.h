#pragma once

#include <QWidget>
#include <QDialog>
#include <QKeyEvent>
#include "ui_AMDAdapterCheck.h"

class AMDAdapterCheck : public QDialog
{
    Q_OBJECT

public:
    AMDAdapterCheck(QString name,QWidget *parent = Q_NULLPTR);
    ~AMDAdapterCheck();
protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    Ui::AMDAdapterCheck ui;
};
