#include "CDisEnableComboBox.h"
#include <QApplication>
#include <QBitmap> 

CDisEnableComboBox::CDisEnableComboBox(QWidget *parent)
    : QComboBox(parent)
{

   //mpCursor = new QCursor(QBitmap("speak_hover.png"));
}

void CDisEnableComboBox::SetDisEnable(const bool & bDisEnable)
{
   mbDisEnable = bDisEnable;
}

void CDisEnableComboBox::enterEvent(QEvent * event)
{
   bool bEnabled = this->isEnabled();

   if(mbDisEnable || !bEnabled)
   {

      //QApplication::setOverrideCursor(*mpCursor);//�ó�Ա��������ͼ��
      
      QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
   }

}

void CDisEnableComboBox::leaveEvent(QEvent * event)
{
   QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
   //QApplication::restoreOverrideCursor();//ɾ�������ͼ
}
