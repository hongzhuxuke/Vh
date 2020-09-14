#pragma once

#include <QCheckBox> 

class CDisEnableCheckBox: public QCheckBox
{
    Q_OBJECT

public:
   CDisEnableCheckBox(QWidget *parent = Q_NULLPTR);
   void SetDisEnable(const bool& bDisEnable);
protected:
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);

private slots:
   
private:
   QCursor* mpCursor = nullptr;
   bool mbDisEnable = false;
};
