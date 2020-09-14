#pragma once

#include <QWidget>
#include "ui_NumWdg.h"

class NumWdg : public QWidget
{
    Q_OBJECT

public:
    NumWdg(QWidget *parent = Q_NULLPTR);
    ~NumWdg();

    void SetMaxSize(int maxNum);
    void SetNum(int num);
    int GetNum();

    void enableEdit(bool recording);
	void showEdit(const bool& bEnable = true);
private slots:
    void slot_ClickedUp();
    void slot_ClickedDown();
private:
    Ui::NumWdg ui;
    int mNum;
    int mMaxNum;
};
