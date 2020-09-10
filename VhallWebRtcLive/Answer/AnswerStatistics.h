#pragma once

#include <QWidget>
#include "ui_AnswerStatistics.h"

class AnswerStatistics : public QWidget
{
    Q_OBJECT

public:
    AnswerStatistics(QWidget *parent = Q_NULLPTR);
    ~AnswerStatistics();

    void SetAnswerPerson(int selectPer, int total);
    void SetAnswerIndex(QString text);
    void SetAnswerCorrect(bool Correct);
protected:
    virtual void paintEvent(QPaintEvent*);

private:
    Ui::AnswerStatistics ui;

    int mSelectPer = 0;
    int mTotal = 0;
    bool mbCorrect = false;
};      
