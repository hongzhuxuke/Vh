#pragma once

#include <QLineEdit> 

class CDisEnableLineEdit: public QLineEdit
{
    Q_OBJECT

public:
   CDisEnableLineEdit(QWidget *parent = Q_NULLPTR);
   void SetDisEnable(const bool& bDisEnable);
protected:
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);

private slots:
   
private:
   QCursor* mpCursor = nullptr;
   bool mbDisEnable = false;
};
