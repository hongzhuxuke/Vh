#include "CDisEnablePushButton.h"
#include <QApplication>
#include <QBitmap> 

CDisEnablePushButton::CDisEnablePushButton(QWidget *parent)
    : QPushButton(parent)
{

   //mpCursor = new QCursor(QBitmap("speak_hover.png"));
}

void CDisEnablePushButton::SetCursorType(eCursorType eCursorType)
{
   mECursortype = eCursorType;
   changeCursor();
   
}

void CDisEnablePushButton::changeCursor()
{
   bool bEnabled = this->isEnabled();
   if (mbCursorIn)
   {
      if (eCursorType_ForbiddenCursor == mECursortype || !bEnabled || mbDisEnable)
      {
         QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
      }
      else if (eCursorType_PointingHandCursor == mECursortype)
      {
         QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
      }
   }
}

void CDisEnablePushButton::SetDisEnable(const bool & bDisEnable)
{
   mbDisEnable = bDisEnable;
   //mECursortype = eCursorType_ForbiddenCursor;
   changeCursor();
}

void CDisEnablePushButton::enterEvent(QEvent * event)
{
   
   mbCursorIn = true;
   changeCursor();
}

void CDisEnablePushButton::leaveEvent(QEvent * event)
{
   mbCursorIn = false;
   QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
   //QApplication::restoreOverrideCursor();//…æ≥˝‘ÿ»ÎµƒÕº
}
