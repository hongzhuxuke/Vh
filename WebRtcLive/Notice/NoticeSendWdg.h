#pragma once

#include <QWidget>
#include "ui_NoticeSendWdg.h"
#include "../Unility/Unility.h"
#include <QEvent>
#include <QCloseEvent>

class NoticeShowWdg;

class NoticeSendWdg : public CBaseWnd
{
    Q_OBJECT

public:
    NoticeSendWdg(QWidget *parent = Q_NULLPTR);
    ~NoticeSendWdg();
    virtual void Notice(const CSTDMapParam &);
    virtual void Notice(const CSTDMapParamPtr &);
    CREATE_WND();
	void showNoticShow(QString strNotic = "");
	void showNotic(bool bShow);
	bool isNoticShowWdgVisible();
	void showNoticWdg(bool bShow);
	void AppendMsg(QString msg);
protected:
	void paintEvent(QPaintEvent *event);
private:
	bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void slot_sendMsg();
    void slot_textChanged();

private:
    Ui::NoticeSendWdg ui;
	NoticeShowWdg*  mpNoticeShowWdg = nullptr;
};
