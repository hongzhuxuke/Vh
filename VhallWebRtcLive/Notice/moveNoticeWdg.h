#ifndef MOVENOTICEWDG_H
#define MOVENOTICEWDG_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

class MoveNoticeWdg: public QWidget
{
    Q_OBJECT

public:
	MoveNoticeWdg(QWidget *parent = Q_NULLPTR);
    ~MoveNoticeWdg();

    void ShowNoticeMsg(QString msg);
	void StopTimer();
protected:
    void paintEvent(QPaintEvent *event);
    void updateIndex();

private slots:
private:
    int m_charWidth;
    int m_curIndex;
    int m_cutStart;
    QString m_showText;
    QTimer mShowMsgTimer;
    bool mbIsPaintStartPos;
    int mPaintStartPos;

private:	
};
#endif// MOVENOTICEWDG_H
