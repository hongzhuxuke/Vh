#include "CDisEnableLineEdit.h"
#include <QApplication>
#include <QBitmap> 

CDisEnableLineEdit::CDisEnableLineEdit(QWidget *parent)
    : QLineEdit(parent)
{

   //mpCursor = new QCursor(QBitmap("speak_hover.png"));
}

void CDisEnableLineEdit::SetDisEnable(const bool & bDisEnable)
{
   mbDisEnable = bDisEnable;
}

void CDisEnableLineEdit::enterEvent(QEvent * event)
{
   bool bEnabled = this->isEnabled();

   if(mbDisEnable || !bEnabled)
   {
      //QApplication::setOverrideCursor(*mpCursor);//用成员函数设置图标
      
      QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
   }
}

void CDisEnableLineEdit::leaveEvent(QEvent * event)
{
   QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
   //QApplication::restoreOverrideCursor();//删除载入的图
}
