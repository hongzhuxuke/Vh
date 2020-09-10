#include "CDisEnableCheckBox.h"
#include <QApplication>
#include <QBitmap> 

CDisEnableCheckBox::CDisEnableCheckBox(QWidget *parent)
    : QCheckBox(parent)
{

   //mpCursor = new QCursor(QBitmap("speak_hover.png"));
}

void CDisEnableCheckBox::SetDisEnable(const bool & bDisEnable)
{
   mbDisEnable = bDisEnable;
}

void CDisEnableCheckBox::enterEvent(QEvent * event)
{
   bool bEnabled = this->isEnabled();

   if(mbDisEnable || !bEnabled)
   {

      //QApplication::setOverrideCursor(*mpCursor);//�ó�Ա��������ͼ��
      
      QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
   }

}

void CDisEnableCheckBox::leaveEvent(QEvent * event)
{
   QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
   //QApplication::restoreOverrideCursor();//ɾ�������ͼ
}
