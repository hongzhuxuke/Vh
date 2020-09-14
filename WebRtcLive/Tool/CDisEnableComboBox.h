#pragma once

#include <QComboBox> 

class CDisEnableComboBox: public QComboBox
{
    Q_OBJECT

public:
   CDisEnableComboBox(QWidget *parent = Q_NULLPTR);
   void SetDisEnable(const bool& bDisEnable);
protected:
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);

private slots:
   
private:
   QCursor* mpCursor = nullptr;
   bool mbDisEnable = false;
};
