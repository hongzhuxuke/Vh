#ifndef NOTICESHOWWDG_H
#define NOTICESHOWWDG_H

#include <QWidget>
#include "ui_NoticeShowWdg.h"
#include <QTimer>
#include <QPainter>

class NoticeShowWdg : public QWidget
{
    Q_OBJECT

public:
    NoticeShowWdg(bool bShowTool , QWidget *parent = Q_NULLPTR);
    ~NoticeShowWdg();

    void ShowNoticeMsg(QString msg);

protected:
    void paintEvent(QPaintEvent *event);
    //void updateIndex();

private slots:
    void slot_close();

private:
    //int m_charWidth;
    //int m_curIndex;
    //int m_cutStart;
    //QString m_showText;
    //QTimer mShowMsgTimer;
    //bool mbIsPaintStartPos;
    //int mPaintStartPos;

private:
    Ui::NoticeShowWdg ui;

	
};
#endif// NOTICESHOWWDG_H