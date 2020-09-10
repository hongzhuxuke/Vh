#ifndef VHFADEOUTTIPWIDGET_H
#define VHFADEOUTTIPWIDGET_H

#include "ui_vhfadeouttipwidget.h"
#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QLabel>
#include <QDateTime> 

enum TOST_PAGE_INDEX {
	Tost_Message = 0,
	Tost_Success,
	Tost_Warning,
	Tost_Error,
};


class VHFadeOutTipWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VHFadeOutTipWidget(QWidget *parent = 0);
    ~VHFadeOutTipWidget();
    void Tip(QWidget* mainWdg,QPoint, QString);
    void rePos(QWidget* mainWdg);
    void Stop();
    void SetSize(int ,int);
	void SetStyleSheet(int style);
   void setDispalyMsg(QString str);
   QString DisplayMsg();
   bool isVisible();

public slots:
    void timeout();
    //void Testtimeout();
protected:
   void paintEvent(QPaintEvent *);
   void mousePressEvent(QMouseEvent *event);
private:
   QTimer *m_timer = NULL;
   QDateTime mStartTime;
   //QTimer *m_timerTest = NULL;
   int m_closeCount = 0;
   //QLabel *m_tipLabel = NULL;
   QPoint mPoint;
   QSize m_globalSize;
   QWidget *m_pParentWdg = NULL;
   int mMaxWidth;

private:
	Ui::vhfadeouttipwidget ui;
	QColor mcolor;
};

#endif // VHFADEOUTTIPWIDGET_H
