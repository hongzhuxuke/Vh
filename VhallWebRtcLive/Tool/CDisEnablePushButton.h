#pragma once

#include <QPushButton> 

class CDisEnablePushButton : public QPushButton
{
    Q_OBJECT

public:
   enum eCursorType
   {
      eCursorType_Normal = 0,
      eCursorType_ForbiddenCursor, //禁用图标
      eCursorType_PointingHandCursor,       //手指状态
   };

   CDisEnablePushButton(QWidget *parent = Q_NULLPTR);
   void SetCursorType(eCursorType eCursorType);
   void SetDisEnable(const bool& bDisEnable);


   //void   setEnabled(bool);

protected:
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);

private slots:
   
private:
   void changeCursor();
   QCursor* mpCursor = nullptr;
   bool mbDisEnable = false;
   eCursorType mECursortype = eCursorType_Normal;
   bool mbCursorIn = false;
};
