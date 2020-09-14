#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "ui_MultiClassFloatWnd.h"
#include <QTimer>

class ExitFullScreenWdg;
class ESCExitNotice;

class MultiClassFloatWnd : public QWidget
{
    Q_OBJECT

public:
    MultiClassFloatWnd(QWidget *parent = Q_NULLPTR);
    ~MultiClassFloatWnd();

   void AddRenderWidget(QWidget* widget, bool isFull);
   QWidget* RenderWidget(bool bRemove = true);
   void RemoveRenderWidget(QWidget* widget);

   bool IsFullType();
   void SetUserid(QString id);
   QString strUserId() {
      return mstrId;
   }
   void show(bool bShowTitle);
protected:
   virtual void closeEvent(QCloseEvent *event);
   void mousePressEvent(QMouseEvent *);
   void mouseMoveEvent(QMouseEvent *);
   void mouseReleaseEvent(QMouseEvent *);
   virtual void keyPressEvent(QKeyEvent *e);
   virtual void showEvent(QShowEvent *event);
private slots:
   void slot_HideExitFullScreen();
   void slot_ShowExitFullScreen();

private slots:
   void slot_close();

signals:
   void sig_close(QString strId);

private:
   void showESCExitNotice();
   QPoint mPressPoint;

   ExitFullScreenWdg* mExitFullScreen;
   ESCExitNotice* mESCExitNotice;
   QTimer mHideExitFullScreenTimer;

   bool mIsMoved = false;
   bool m_bIsEnableMove = true;

private:
    Ui::MultiClassFloatWnd ui;
   bool mbIsFullType;
   QString mstrId = "";
   QWidget* mpRender = nullptr;
};
