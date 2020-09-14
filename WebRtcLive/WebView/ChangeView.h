#pragma once

#include <QWidget>
#include "ui_ChangeView.h"

class ChangeView : public QWidget
{
    Q_OBJECT

public:
    ChangeView(QWidget *parent = Q_NULLPTR);
    ~ChangeView();

protected:
   virtual void enterEvent(QEvent *event);
   virtual void leaveEvent(QEvent *event);

private slots:
   void slot_changeLayout();

signals:
   void sig_changeLayout();

private:
    Ui::ChangeView ui;
};
