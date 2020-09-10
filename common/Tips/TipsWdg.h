#pragma once

#include <QWidget>
#include "ui_TipsWdg.h"
#include "../Unility/Unility.h"

class TipsWdg : public CBaseWnd
{
    Q_OBJECT

public:
    TipsWdg(QWidget *parent = Q_NULLPTR);
	TipsWdg(const QString& strMsg, QWidget *parent = Q_NULLPTR, const QString& strOk = "", const QString strCancle = "");
	TipsWdg(QWidget *parent, const QString& strMsg, const QString& strCancle);
    ~TipsWdg();
    virtual void Notice(const CSTDMapParam &);
    virtual void GetParam(CSTDMapParam&);
    CREATE_WND();
private:
    Ui::TipsWdg ui;
};
