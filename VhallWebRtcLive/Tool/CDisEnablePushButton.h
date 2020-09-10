#pragma once

#include <QPushButton> 

class CDisEnablePushButton : public QPushButton
{
    Q_OBJECT

public:
   enum eCursorType
   {
      eCursorType_Normal = 0,
      eCursorType_ForbiddenCursor, //����ͼ��
      eCursorType_PointingHandCursor,       //��ָ״̬
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
